<?php

/**
 * @file
 * SMS notifier.
 */

class MessageNotifierPush extends MessageNotifierBase {

  public function deliver(array $output = array()) {
  	$recipients =array();
		$message = $this->message;
		
		if (in_array($message->type, array('private_message','response','confirm','payremindexec'))){
		  $wrapper = entity_metadata_wrapper('message', $message);
      $users = $wrapper->field_message_user_ref->value();
    	if (is_array($users)) {
      	foreach ($users as $user) {
				  $recipients[]=$user->uid;
				}
			}
    }
		elseif ($message->type=='task_created') {
		  $wrapper = entity_metadata_wrapper('message', $message);
			$group = $wrapper->field_group->raw();
			$career = $wrapper->field_career->raw();
			$members = group_membership_load_by_group($group);
			foreach($members as $mid => $member) {
			  $subscribers[]= $member->uid;				
			}
			//query for subscriber
			// Build an EFQ based on the arguments.
	    $query = new EntityFieldQuery();
	    $query
	        ->entityCondition('entity_type','profile2')
					->propertyCondition('type','employee')
					->propertyCondition('uid',$subscribers,"IN");
			$query->fieldCondition('field_career','tid', $career);	
	    $result = $query->execute();
			if (!empty($result)){
				 $profiles=entity_load('profile2', array_keys($result['profile2']));
				 foreach($profiles as $pid => $profile){
				 	  $recipients[]=$profile->uid;
				 }
			}
		  //watchdog('debug', 'debug info:<pre>@A1</pre>', array('@A1' => print_r($subscribers, TRUE)));		
			//watchdog('debug', 'debug info:<pre>@A1</pre>', array('@A1' => print_r($recipients, TRUE)));		
		}
		elseif ($message->type=='task_updated') {
			$recipients[]=$message->field_display_author[LANGUAGE_NONE][0]['target_id'];
		}
		else {
			$recipients[]=$message->uid;			
		}

		/*
		$clone_message = clone($message);
		unset($clone_message->language);
		unset($clone_message->arguments);
		unset($clone_message->data);
		unset($clone_message->field_message_body);
		unset($clone_message->field_message_subject);		
		$this->flatten_fields('message',$clone_message);
		$clone_message->
		*/
		$msg=str_replace(array("\r", "\n", "  "), ' ',strip_tags($output['message_notify_push_body']));
    $payload = $msg;//drupal_json_encode($clone_message);
		
    watchdog('payload', 'payload:<pre>@A1</pre>', array('@A1' => print_r($payload, TRUE)));		
    watchdog('output', 'output:<pre>@A2</pre>', array('@A2' => print_r($recipients, TRUE)));		
    $result= push_notifications_send_message($recipients, $payload);
		if ($result){
		  watchdog('output', 'output:<pre>@A2</pre>', array('@A2' => print_r($result['message'], TRUE)));		
			//$dsm_type = ($result['success']) ? 'status' : 'error';
      //drupal_set_message($result['message'], $dsm_type);		
		}
		return $result;
}

	/**
	 * Flattens field value arrays on the given entity.
	 *
	 * Field flattening in Commerce Services involves reducing their value arrays to
	 * just the current language of the entity and reducing fields with single
	 * column schemas to simple scalar values or arrays of scalar values.
	 *
	 * Note that because this function irreparably alters an entity's structure, it
	 * should only be called using a clone of the entity whose field value arrays
	 * should be flattened. Otherwise the flattening will affect the entity as
	 * stored in the entity cache, causing potential errors should that entity be
	 * loaded and manipulated later in the same request.
	 *
	 * @param $entity_type
	 *   The machine-name entity type of the given entity.
	 * @param $cloned_entity
	 *   A clone of the entity whose field value arrays should be flattened.
	 */
	protected function flatten_fields($entity_type, $cloned_entity) {
	  list(, , $bundle) = entity_extract_ids($entity_type, $cloned_entity);
	  $clone_wrapper = entity_metadata_wrapper($entity_type, $cloned_entity);
	
	  // Loop over every field instance on the given entity.
	  foreach (field_info_instances($entity_type, $bundle) as $field_name => $instance) {
	    // Set the field property to the raw wrapper value, which applies the
	    // desired flattening of the value array.	
					$cloned_entity->{$field_name} = $clone_wrapper->{$field_name}->raw();		
	  }
	}
	
}
