<?php

/**
 * Retrieves the thread.
 */
function _thread_resource_retrieve($thread_id, $load_full = TRUE) {

global $user;
	// $account = user_load($user);
	//return privatemsg_message_change_delete($pmid, TRUE, $user);
//	return privatemsg_message_change_status($thread_id, 1, $account);
	
	
    /* $ss = privatemsg_thread_load($thread_id);
	  if(is_array($ss)){ 
	    foreach($ss as $key=>$var){
           if(is_array($var)){
	
	         foreach($var as $key1=>$var1){
					foreach($var1 as $k=>$v){
					
                   if($k=='pass'){
                     unset($k);
					 unset($v);
					 unset($ss[$key][$key1][$k]);
                   // $var[$key1]=$var1;
					//$var1[$k]=$v;
					//$var["userinfo"]=$var2[$k];
                    }
					
					// var_dump($var1);
					// var_dump($var1["uid"]);
					//$var1[$k]=$v;
                    // var_dump($var1); 
					 /// $var1[$k]=$v;
                    }
					//var_dump($var1);
					//$var[$key1]=$var2;
					
				  // $var[$key1]=$var1;
				  // $var["userinfo"]=$var2[$k];
	         }
			 //var_dump($var);
	       }
	  // $ss[$key]=$var;
       }
     }
  
	return $ss;*/
	
//	return privatemsg_thread_load($thread_id);
	//global $user;
	return privatemsg_thread_change_app_status($thread_id,0,$user);
}

/**
 * Retrieves the message.
 */
 
function _msg_resource_retrieve($pmids) {
//edit by marklee 2014-07-22
if($pmids==""){
	
	return false;
}
	if (!privatemsg_user_access('read all private messages')) {
		module_load_include('inc', 'services', 'services.runtime');

		if (!privatemsg_user_access('read privatemsg')) {
			// Make sure that the user has the permission to read privatemsg.
			return services_error(t('Access to message denied.'), 401);
		}

		global $user;
		// Find the corresponding threads.
		$accessible_pmids = db_select('pm_index', 'pi')
			->condition('pi.mid', $pmids, 'IN')
			->condition('pi.recipient', $user->uid)
			->fields('pi', array('mid'))
			->execute()
			->fetchCol();

		$inaccessible_pmids = array_diff($pmids, $accessible_pmids);

		if (sizeof($inaccessible_pmids)) {
			return services_error(t('Access to message denied.'), 401);
		}
	}
	return privatemsg_message_load_multiple($pmids);
}

/**
 * Deletes the message.
 */
function _msg_resource_delete($pmid) {
	global $user;
/**
*change by marklee for app delete 2014-07-24 begin
*/
	//return privatemsg_message_change_app_delete($pmid, TRUE, $user);
	
	return privatemsg_thread_change_app_delete($pmid, TRUE, $user);

	//return privatemsg_message_change_delete($pmid, TRUE, $user);
}
/*
function _msg_resource_update($pmid) {
	global $user;
	 $account = user_load($user);
	//return privatemsg_message_change_delete($pmid, TRUE, $user);
	return privatemsg_message_change_status($pmid, 1, $account);
}*/

/**
 * Retrieves all the messages for a user.
 */
function _usermsg_resource_retrieve($uid, $type = 'inbox') {
    
	 
	$account = user_load($uid);

	if ($type != 'sent') {
		$type = 'inbox';
	}

	$query = _privatemsg_assemble_query('list', $account, $type);
	
	$msgs = array();
	foreach ($query->execute() as $row) {
	
		$msgs[$row->thread_id] = $row;
	}
	
    $unread_count  =  privatemsg_unread_count($account);
     $msgs = array_values($msgs);
//	var_dump($msgs);
	//die();
	
    $message_info = array("unread_count"=>$unread_count,"message_info"=>$msgs);

	return $message_info;
}

/**
 * Creates a new message.
 */
function _msg_resource_create($subject, $body, $body_format = 'plain_text', $thread_id = NULL, $recipient = '') {
	global $user;

	$form_state = array();
	$form_state['values'] = array(
		'author' => $user,
		'recipient' => $recipient,
		'subject' => $subject,
		'body' => array(
			'value' => $body,
			'format' => $body_format,
		),
		'op' => 'Send message',
	);
	if (!is_null($thread_id) && is_numeric($thread_id)) {
		$form_state['values']['thread_id'] = $thread_id;
	}
	$form_state['triggering_element'] = array(
		'name' => 'op',
		'#value' => 'Send message',
	);

	module_load_include('inc', 'privatemsg', 'privatemsg.pages');
	drupal_form_submit('privatemsg_new', $form_state, $recipient, $subject, $thread_id);

	if ($errors = form_get_errors()) {
		return services_error(implode(" ", $errors), 406, array('form_errors' => $errors));
	}

	return $form_state['validate_built_message'];
}


/**
*    caculate unread message
* */
/*
function _unreadmsg_resource_retrieve($uid){
   
     $account = user_load($uid);
	return privatemsg_unread_count($account);
}*/

function _updatemsg_resource_retrieve($thread_id) {

	global $user;
 
	//die();
	//$account = user_load($user);
	
	//return privatemsg_message_change_status($thread_id,0,$user);
	return privatemsg_thread_change_status($thread_id,0,$user);
	//return privatemsg_status($pmid,'0',$user);
}
