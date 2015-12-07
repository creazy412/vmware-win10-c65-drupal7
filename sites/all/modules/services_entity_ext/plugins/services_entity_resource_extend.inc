<?php
/**
 * This class is designed to create a extend API that integrates with
 * the services and entity modules. We want to strip all "drupalisms" out
 * of the API. For example, there should be no [LANGUAGE_NONE][0][value] or
 * field_ in the API.
 *
 * It should be possible to create an API that is easily replicated on another
 * system.
 *
 * Much of this code is borrowed from restws module.
 */
class ServicesEntityResourceControllerExtend extends ServicesEntityResourceController {

  /**
   * Implements ServicesResourceControllerInterface::create().
   */
  public function create($entity_type, array $values) {
		if (!isset($values['fields'])){
			$fields='*';
		}
		else {
			$fields = $values['fields'];
			unset($values['fields']);
		}
    $this->checkTextFormatAccess($values);
		//print_r($entity_type);
		if ($entity_type=='comment' && module_exists('user_profile_comments')){
			if (empty($values['nid'])){
			  if (!empty($values['field_comment_user']) ){
				  $comment_user_uid=$values['field_comment_user'][LANGUAGE_NONE][0]['target_id'];
				  $node=user_profile_comments_load_entity($comment_user_uid);
					if (!empty($node))
					  $values['nid'] = $node->nid;
				}
			}
		}
    $entity = entity_create($entity_type, $values);
    entity_save($entity_type, $entity);
    list($id, ) = entity_extract_ids($entity_type, $entity);

    // Check we got an ID back for the new entity.
    if (!isset($id)) {
      services_error('Error saving entity.', 406);
    }
		if ($entity_type=='comment' && module_exists('user_profile_comments')){
			if (!empty($values['field_fivestar'])){
				$fivestar = $values['field_fivestar'][LANGUAGE_NONE][0]['rating'];
				$uid = $values['uid'];
				if (!empty($values['field_comment_user'])){
					$comment_user_uid=$values['field_comment_user'][LANGUAGE_NONE][0]['target_id'];
					_fivestar_cast_vote('user',$comment_user_uid, $fivestar, NULL, $uid);
				}
				_fivestar_cast_vote($entity_type,$id, $fivestar,NULL,$uid);
			}
		}

    $entity = entity_load_single($entity_type, $id);
    return $this->process_results($entity_type, $entity, $fields,0);
  }
  
  public function retrieve($entity_type, $entity_id, $fields, $revision=NULL) {
    $entity = parent::retrieve($entity_type, $entity_id, '*', $revision);
   
    if (!empty($entity)) {
    	$entity = $this->process_results($entity_type, $entity, $fields);
    }
    return $entity;  		
  }
  
	public function index($entity_type, $fields, $parameters, $page, $pagesize, $sort, $direction) {
	  if ($entity_type=='commerce_order'){
			if (empty($parameters['uid'])){
				global $user;
				$parameters['uid'] = !empty($user->uid) ? $user->uid : 0;
			}				
		}
		if ($entity_type=='message'){
			if (isset($parameters['type']))
				return $this->retrieve_messages($entity_type, $fields, $parameters, $page, $pagesize, $sort, $direction);
		}
    // Make sure the pagesize is not too large.
    $max_pagesize = variable_get('services_entity_max_pagesize', 100);
    $pagesize = ($max_pagesize < $pagesize) ? $max_pagesize : $pagesize;

    // Build an EFQ based on the arguments.
    $query = new EntityFieldQuery();
    $query
        ->entityCondition('entity_type', $entity_type)
        ->range($page * $pagesize, $pagesize);

    if (!empty($parameters)) {
      foreach ($parameters as $field => $value) {
        $this->propertyQueryOperation($entity_type, $query, 'Condition', $field, $value);
      }
    }
    if ($sort != '') {
      $direction = ($direction == 'DESC') ? 'DESC' : 'ASC'; // Ensure a valid direction
      $this->propertyQueryOperation($entity_type, $query, 'OrderBy', $sort, $direction);
    }

    $result = $query->execute();

	  if ($entity_type=='commerce_order') {
			if (empty($result)&& isset($parameters['status']) && $parameters['status']=='cart') {
			  // Create the new order with the customer's uid and the cart order status.
			  // Instead of using commerce_cart_order_new() directly, its contents have been
			  // copied here to allow for setting field and property data before saving so
			  // any errors in the data prevent saving a new order.
				$uid= $parameters['uid'];
			  $order = commerce_cart_order_new($uid);
				if (isset($parameters['field_display'])){
					$this->set_field_values($entity_type, $order, 
							array("field_display"=>array('target_id'=>$parameters['field_display'])));					
				}
				commerce_order_save($order);
				$return[]=$this->process_results($entity_type, $order, $fields);
				return $return;
			}
		}
    if (empty($result)) {
				return services_error(t('No entities found.'), 404);
    }
    // Convert to actual entities.
    $entities = entity_load($entity_type, array_keys($result[$entity_type]));

    foreach ($entities as $id => $entity) {
      if ($entity_type=='group_membership' || entity_access('view', $entity_type, $entity)) {
        // Users get special treatment to remove sensitive data.
        if ($entity_type == 'user') {
          // Use the helper that Services module already has.
          services_remove_user_data($entity);
        }
        $return[] = $this->process_results($entity_type, $entity, $fields);
      }
    }

    // The access check may have resulted in there being no entities left.
    if (empty($return)) {
      return services_error(t('No entities found.'), 404);
    }

    return $return;		
  }

	function retrieve_messages($entity_type, $fields, $parameters, $page, $pagesize, $sort, $direction){
		global $user;
		if (isset($parameters['type'])) {
			if (strpos($parameters['type'],",")!==false) {
					$types = explode(',',$parameters['type']);
			}
			else $types = array(0=>$parameters['type']);
		  unset($parameters['type']);
		}
		
		if (!isset($parameters['uid']) && (in_array('task_created', $types) || in_array('task_updated', $types))){
			if (!isset($parameters['field_group']) && module_exists('group')) {
			  foreach (group_membership_load_by_user($user->uid) as $group_membership) {
			    $parameters['field_group'][] = $group_membership->gid;
			  }			
			}		
			if (!isset($parameters['field_career']) && module_exists('profile2')) {
				$profile2 = profile2_load_by_user($user,'employee');
				if ($profile2) {
				  $parameters['field_career'] = $profile2->{'field_career'}[LANGUAGE_NONE];
				}	
			}	
		}	
		// Make sure the pagesize is not too large.
    $max_pagesize = variable_get('services_entity_max_pagesize', 100);
    $pagesize = ($max_pagesize < $pagesize) ? $max_pagesize : $pagesize;

   	foreach ($types as $type) { 	
	    // Build an EFQ based on the arguments.
	    $query = new EntityFieldQuery();
	    $query
	        ->entityCondition('entity_type', $entity_type)
	        ->range($page * $pagesize, $pagesize)
					->propertyCondition('type',$type);
	    if (!empty($parameters)) {
	      foreach ($parameters as $field => $value) {
					if ($field=='timestamp'){
						$query->propertyCondition($field,$value,'>');
					}
					elseif ($field=='field_group') {
					 	if (in_array($type, array('task_created','task_updated')))
							$query->fieldCondition($field,'target_id',$value,'IN');
					}
					elseif ($field=='field_career'){
					 	if (in_array($type, array('task_created','task_updated')))
							$query->fieldCondition($field,'tid',$value,'IN');
					}
					else
	          $this->propertyQueryOperation($entity_type, $query, 'Condition', $field, $value);
	      }
	    }
	    if ($sort != '') {
	      $direction = ($direction == 'DESC') ? 'DESC' : 'ASC'; // Ensure a valid direction
	      $this->propertyQueryOperation($entity_type, $query, 'OrderBy', $sort, $direction);
	    }
	
	    $result = $query->execute();
			
	    if (!empty($result)) {
		    // Convert to actual entities.
		    $entities = entity_load($entity_type, array_keys($result[$entity_type]));
			
		    foreach ($entities as $id => $entity) {
		      if (entity_access('view', $entity_type, $entity)) {
		        // Users get special treatment to remove sensitive data.
		        if ($entity_type == 'user') {
		          // Use the helper that Services module already has.
		          services_remove_user_data($entity);
		        }
		        $return[] = $this->process_results($entity_type, $entity, $fields,0);
		      }
		    }
			}
		}
    // The access check may have resulted in there being no entities left.
    if (empty($return)) {
      return services_error(t('No entities found.'), 404);
    }

    return $return;		
	}
	/**
	 * Updates an entity.
	 *
	 * @param $entity_type
	 *   The machine-name of the type of entity to update.
	 * @param $entity_id
	 *   The ID of the entity to update.
	 * @param $data
	 *   The entity data to update.
	 * @return
	 *   The updated entity processed as specified to include additional read-only
	 *   properties or flattened field value arrays.
	 */  
	public function update($entity_type, $entity_id, array $values) {
		if (!isset($values['fields'])){
			$fields='*';
			if ($entity_type=='commerce_order'){
				$fields='nid';
			}
		}
		else {
			$fields = $values['fields'];
			unset($values['fields']);
		}
    if ($entity_type=='commerce_order' && isset($values['line_items'])){
			return $this->commerce_order_line_item_update($entity_type, $entity_id, $values, $fields);		 
		}
		
		if ($entity_type=='commerce_product'){
		  if (isset($values['product_display'])){
				$product_display_id = $values['product_display'];
				unset($values['product_display']);
			}
		}
		
		//$this->checkTextFormatAccess($values);
	  // Load the entity.
	  $entity = entity_load_single($entity_type, $entity_id);
		if ($entity_type=='commerce_product'){		 
		  $product = entity_metadata_wrapper($entity_type, $entity);
			if ($product->{'field_result'}->value()==2) {
				return services_error(t('Cannot changed, You have made bid successful.'), 400);
			}
		}	  
	  
		// Set field values using the data provided.
	  $this->set_field_values($entity_type, $entity, $values, $flatten_fields = 'true');
	  
	  // Save the updates to the entity.
	  entity_save($entity_type, $entity);

		if ($entity_type=='commerce_product'){		 
		  if (isset($product_display_id)){
			  $product_display = entity_load_single('node', $product_display_id);
			  entity_save('node',$product_display);
			}
		}
					
 /*   if ($entity_type=='commerce_order' && $entity->status=='completed'){
    	$order_wrapper = entity_metadata_wrapper($entity_type, $entity);
    	foreach($order_wrapper->commerce_line_items as $key => $lineitem) {
    		if ($lineitem->type->value()=='product') {
	      	$lineitem->{'field_status'}->set(1);
	      	$lineitem->save();
        }
		  }			
		}		
	*/			
    return $this->retrieve($entity_type, $entity_id, $fields);
  }
  
	function commerce_order_line_item_update($entity_type, $entity_id, array $values, $fields){
		if (!isset($values['line_items'])|| !is_array($values['line_items'])){
			return services_error(t('You must specify a valid line item values'), 400);
		}
		
		$order = commerce_order_load($entity_id);
		$status_array =  array("canceled",
				"pending",
				"processing",
				"completed",
				"checkout_complete");
		if (in_array($order->status,$status_array)){
			return services_error(t('The order cannot be changed!'), 400);
		}
		
		//empty order
		$order = commerce_cart_order_empty($order);
		$order_wrapper = entity_metadata_wrapper($entity_type, $order);

		
		$line_items = $values['line_items'];
		foreach ($line_items as $key => $value){				
			$line_item = $this->commerce_order_new_line_item($entity_id, $value);
	    $order_wrapper->commerce_line_items[] = $line_item;
		}
		//update order status to cart
		commerce_order_status_update($order, 'cart', TRUE);
		//refresh order to invoke rules and reflect changes
		commerce_cart_order_refresh($order);
		//save order
		commerce_order_save($order);
		//retrive order to get new data
		return $this->retrieve($entity_type, $entity_id, $fields, NULL);
  }

	/**
	* Attaches or overwrites line-items(s) to an existing order.
	*
	* @param $order_id
	*   Node ID of the node the product(s) is being attached to.
	* @param $field_name
	*   Machine name of the field that is attached to the node.
	* @return
	*   An array of products that were attached in the form:
	*   array(
	*     array(
	*       fid => N,
	*       uri => http://site.com/endpoint/file/N
	*     ),
	*     ...
	*   )
	*/
  function commerce_order_new_line_item($order_id, array $values){
	  // Ensure the create request specifies a valid line item type.
		if (!isset($values['type'])){
			$values['type']='product';
		}
	  if (!in_array($values['type'], array_keys(commerce_line_item_types()))) {
	    return services_error(t('You must specify a valid line item type'), 400);
	  }
	
	  // Default the quantity to 1.
	  if (!isset($values['quantity'])) {
	    $values['quantity'] = 1;
	  }

    if (isset($values['commerce_product'])) {
    	$product = commerce_product_load($values['commerce_product']);
	    // Bail now if the product wasn't found.
	    if (empty($product)) {
	      return services_error(t('You must specify a valid commerce_product value'), 400);
	    }
			if (!$product->status){
				//return services_error(t('invalid product'), 400);
			}
		}
	  // Create the new line item.
	  $line_item = commerce_line_item_new($values['type'], $order_id);

	  // Remove the type and order_id from the data array since they've been set.
	  unset($values['type']);
	
    // Populate the line item.
    commerce_product_line_item_populate($line_item, $product);
	  // Set the field and property data and save the line item.
	  $this->set_field_values('commerce_line_item', $line_item, $values, $flatten_fields = 'true');

	  commerce_line_item_save($line_item);
		return $line_item;
  }

	/**
	* Attaches or overwrites product(s) to an existing node.
	*
	* @param $nid
	*   Node ID of the node the product(s) is being attached to.
	* @param $field_name
	*   Machine name of the field that is attached to the node.
	* @param $attach
	*   Optional. Defaults to true. This means that products will be attached to the
	*   node, alongside existing products. If the maximum number of products have already
	*   been uploaded to this node an error is given.
	*   If false, it removes the products, and attaches the new products uploaded.
	* @return
	*   An array of products that were attached in the form:
	*   array(
	*     array(
	*       fid => N,
	*       uri => http://site.com/endpoint/file/N
	*     ),
	*     ...
	*   )
	*/
  public function attach_product($entity_type, $nid, $field_name, $attach, $field_values){
	  $field = field_info_field($field_name);
	  if ($field['type']<>'commerce_product_reference') {
	     return services_error(t('Field type is not commerce_product_reference type'), 406);
	  }

	  $node = entity_load_single($entity_type, $nid);
		if ($node->status==0){
			services_error(t('Task have been canceled.'), 400);
		}

		if ($node->promote==0){
			services_error(t('Bidding of the task have been ended.'), 400);
		}

	  $node_type=$node->type;
	  if (empty($node->{$field_name}[LANGUAGE_NONE] )) {
	    $node->{$field_name}[LANGUAGE_NONE] = array();
	  }
	
	  // Validate whether field instance exists and this node type can be edited.
	  $this->validate_node_type_field_name('update', array($node_type, $field_name));
		//check for already bid for the task
		$node_wrapper = entity_metadata_wrapper($entity_type, $node);
	  if ($attach) {
			$field_bidder = $field_values['field_bidder'][LANGUAGE_NONE][0]['target_id'];
			foreach($node_wrapper->{$field_name} as $delta => $product){
				if ($product->{'field_bidder'}->raw()==$field_bidder) {
					services_error(t('Already bid for the task.'), 400);
				}
			}
	  }
			
		//check user valid datetime	
    $starttime =$node_wrapper->{'field_start_date'}->value();
		$endtime = $node_wrapper->{'field_end_date'}->value();
		//watchdog('debug', '<pre>@value</pre>', array('@value' => print_r($starttime,  TRUE))); 		
		//watchdog('debug', '<pre>@value</pre>', array('@value' => print_r($endtime,  TRUE))); 
		if (module_exists('banshibao')){
			global $user;
			//watchdog('debug', '<pre>@value</pre>', array('@value' => print_r(validate_user_time($user->uid, $starttime, $endtime),  TRUE))); 		
			 if (validate_user_time($user->uid, $starttime, $endtime)==FALSE){
			 	 services_error(t('User working time is not allowed.'), 400);
			 }
		}
			
	  //create product
	  $product_entity_type = 'commerce_product';
	  $product = entity_create($product_entity_type, $field_values);
    entity_save($product_entity_type, $product);
    list($id, ) = entity_extract_ids($product_entity_type, $product);
    // Check we got an ID back for the new entity.
    if (!isset($id)) {
      services_error('Error saving commerce product.', 406);
    }
    //reload data
    $product = entity_load_single($product_entity_type, $id);

    //reload data
    $node = entity_load_single($entity_type, $nid);
	  $node_wrapper = entity_metadata_wrapper($entity_type, $node);
	  if ($attach) {
			$node_wrapper->{$field_name}[] = $product;
	  }
	  else {
		  $node_wrapper->{$field_name} = array();
			$node_wrapper->{$field_name}[] = $product;
	  }	  
	  entity_save($entity_type, $node);
	  
		$entity = entity_load_single($entity_type, $nid);
		if (!empty($entity)) {
    	$entity = $this->process_results($entity_type, $entity, 'nid',0);
    }
    return $entity; 
  }
  
	/**
	* Attaches or overwrites line-items(s) to an existing order.
	*
	* @param $order_id
	*   Node ID of the node the product(s) is being attached to.
	* @param $field_name
	*   Machine name of the field that is attached to the node.
	* @param $attach
	*   Optional. Defaults to true. This means that products will be attached to the
	*   node, alongside existing products. If the maximum number of products have already
	*   been uploaded to this node an error is given.
	*   If false, it removes the products, and attaches the new products uploaded.
	* @return
	*   An array of products that were attached in the form:
	*   array(
	*     array(
	*       fid => N,
	*       uri => http://site.com/endpoint/file/N
	*     ),
	*     ...
	*   )
	*/
  public function add_line_items($entity_type, $order_id, $attach, $values, $fields='*'){
	  // Ensure the create request specifies a valid line item type.
	  if (empty($values['type']) || !in_array($values['type'], array_keys(commerce_line_item_types()))) {
	    return services_error(t('You must specify a valid line item type'), 400);
	  }
	
	  // Default the quantity to 1.
	  if (empty($values['quantity'])) {
	    $values['quantity'] = 1;
	  }

    if (!empty($values['commerce_product'])) {
    	$product = commerce_product_load($values['commerce_product']);
	    // Bail now if the product wasn't found.
	    if (empty($product)) {
	      return services_error(t('You must specify a valid commerce_product value'), 400);
	    }
			if (!$product->status){
				return services_error(t('invalid product'), 400);
			}
		}
	  $order = commerce_order_load($order_id);
		if (!$attach) {
			$order = commerce_cart_order_empty($order);
		}
		//check line-item if exist
		$commerce_line_items = field_get_items('commerce_order',$order,'commerce_line_items');
		
		if (!empty($commerce_line_items)){
			foreach($commerce_line_items as $key => $item){
			  $id = reset($item);
				// Load the line item.
	  		$line_item = commerce_line_item_load($id);
	      if ($line_item->type=='product') {
					$prod_id = $line_item->commerce_product[LANGUAGE_NONE][0]['product_id'];
					if ($values['commerce_product']==$prod_id){
					  //update line-item quantity
						$quantity = $line_item->quantity;
						$quantity += $values['quantity'];
						$line_item = $this->update('commerce_line_item',$id, array('quantity'=>$quantity));
					  // If the line item quantity or unit price was updated, load and save the
					  // order to recalculate the order total.
						commerce_order_status_update($order, 'cart', TRUE);
						commerce_cart_order_refresh($order);
			      commerce_order_save($order);
						return $this->retrieve('commerce_order', $order_id, $fields,NULL);    
					}		
				}
			}
		}	

	  // Create the new line item.
	  $line_item = commerce_line_item_new($values['type'], $order_id);
	
	  // Remove the type and order_id from the data array since they've been set.
	  unset($values['type']);
	
    // Populate the line item.
    commerce_product_line_item_populate($line_item, $product);
	  // Set the field and property data and save the line item.
	  $this->set_field_values('commerce_line_item', $line_item, $values, $flatten_fields = 'true');

	  commerce_line_item_save($line_item);
	  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
	  $order_wrapper->commerce_line_items[] = $line_item;
		commerce_order_status_update($order, 'cart', TRUE);
		commerce_cart_order_refresh($order);
		commerce_order_save($order);
	  
	  return $this->retrieve('commerce_order', $order_id, $fields, NULL);    
  }
		
  /**
   * Implements ServicesResourceControllerInterface::delete().
   */
  public function delete($entity_type, $entity_id) {
    if ($entity_type=='message'){
			global $user;
 			$flag = flag_get_flag('message_delete_flag');
  		if (!$flag) {// Flag does not exist.
    		return services_error(t('There is no file with flag with the name message_delete_flag'), 406);
  		}
  		return $flag->flag('flag', $entity_id, $user);
		}
		else
	 	 parent::delete($entity_type, $entity_id);
  }
			
	public function message_mark_read($entity_type, $entity_id){
    if ($entity_type=='message'){
			global $user;
 			$flag = flag_get_flag('message_read_flag');
  		if (!$flag) {// Flag does not exist.
    		return services_error(t('There is no file with flag with the name message_read_flag'), 406);
  		}
  		return $flag->flag('flag', $entity_id, $user);
		}
	}
  /**
	 * Helper function to validate data.
	 *
	 * @param $op
	 *   Array representing the attributes a node edit form would submit.
	 * @param $args
	 *   Resource arguments passed through from the original request (node_type,
	 *   field_name).
	 *
	 * @return bool
	 *   TRUE/FALSE based on access.
	 */
	function validate_node_type_field_name($op = 'create', $args = array()) {
	  $node_type = $args[0];
	  $field_name = $args[1];
	
	  $temp_node= array('type' => $node_type);
	
	  // An invalid node type throws an exception, and stops before the return below.
	  $this->validate_type($temp_node);
	
	  if (!field_info_instance('node', $field_name, $node_type)) {
	    return services_error(t('Field name \'@field_name\' not found on node type \'@node_type\'', array('@field_name' => $field_name, '@node_type' => $node_type)), 406);
	  }
	
	  return TRUE;
	}

	/*
	 * Helper function to validate node type information.
	 *
	 * @param $node
	 *   Array representing the attributes a node edit form would submit.
	 */
	function validate_type($node) {
	  if (!isset($node['type'])) {
	    return services_error(t('Missing node type'), 406);
	  }
	  // Wanted to return a graceful error instead of a blank nid, this should
	  // allow for that.
	  $types = node_type_get_types();
	  $node_type = $node['type'];
	  if (!isset($types[$node_type])) {
	    return services_error(t('Node type @type does not exist.', array('@type' => $node_type)), 406);
	  }
	  $allowed_node_types = variable_get('services_allowed_create_content_types', $types);
	  if (!isset($allowed_node_types[$node_type])) {
	    return services_error(t("This node type @type can't be processed via services", array('@type' => $node_type)), 406);
	  }
	}



  protected function process_results($entity_type, $entity, $fields, $depth=2){
	  if ($entity_type == 'commerce_order') $depth=3;
    /*if ($entity_type == 'commerce_product'){
      //unset($entity->field_geo);
    }*/

    // Add simplified fields to the entity object for certain field types.
    $this->decorate_entity($entity_type, $entity);
    

   // Expand referenced entities into the full entity to the specified depth.
     if ($depth > 1) {
     	 $this->expand_entities($entity_type, $entity, $depth, $flatten_fields = 'true');
     }
    
	  // Flatten field value arrays if specified. This must be the last operation
	  // performed as it breaks the standard field data model. An entity whose
	  // fields have thus been flattened is no longer wrappable or writable.
    $entity = clone($entity);
    $this->flatten_fields($entity_type, $entity);
	  
    // Users get special treatment to remove sensitive data.
    if ($entity_type == 'user') {
      // Use the helper that Services module already has.
      services_remove_user_data($entity);
    }
       	  
    if (empty($fields)){
      $fields='*';    
    }
    // Filter out the properties based on the field list if given.    
    return $this->limit_fields($entity, $fields);  		
  }

	/**
	 * Expands entity reference field values into full entities in a corresponding
	 * property on the given entity to a specified depth of child entities.
	 *
	 * @param $entity_type
	 *   The machine-name entity type of the given entity.
	 * @param $entity
	 *   The entity whose reference field values should be expanded.
	 * @param $depth
	 *   The depth to which referenced entities should be expanded; a depth of 1
	 *   means only the reference field values on the entity passed to the function
	 *   should be expanded, 2 means those and the entities they reference, etc.
	 * @param $flatten_fields
	 *   Boolean indicating whether or not field value arrays should be flattened.
	 */
	protected function expand_entities($entity_type, $entity, $depth, $flatten_fields) {
	  // Extract the bundle name.
	  list(, , $bundle) = entity_extract_ids($entity_type, $entity);
			  
	  // Look for each type of Commerce reference field on the given entity.
	  foreach (array('commerce_product_reference', 'commerce_line_item_reference', 'commerce_customer_profile_reference', 'taxonomy_term_reference', 'entityreference') as $field_type) {
	    // Look for reference field instances of each type on the entity.
	    foreach (field_info_instances($entity_type, $bundle) as $field_name => $instance) {
	      $entities_field_name = $field_name . '_entities';
	      $field_info = field_info_field($field_name);

	      if ($field_info['type'] == $field_type) {
	        // Extract the type of referenced entity from the property info.
	        $property_info = entity_get_property_info($entity_type);
	 
	        if (!empty($property_info['bundles'][$bundle]['properties'][$field_name]['type'])) {
	          $referenced_entity_type = $property_info['bundles'][$bundle]['properties'][$field_name]['type'];
	          $entity_wrapper = entity_metadata_wrapper($entity_type, $entity);
 	        	
	          // If the field is multi-value, extract the entity type from within
	          // the list markup and expand the whole list.
	          if (substr($referenced_entity_type, 0, 5) == 'list<') {
	            $referenced_entity_type = substr($referenced_entity_type, 5, strlen($referenced_entity_type) - 6);

	            foreach ($entity_wrapper->{$field_name} as $delta => $referenced_entity_wrapper) {
	              // Extract the entity from the wrapper.
	              $referenced_entity_id = $referenced_entity_wrapper->raw();

	              if (!empty($referenced_entity_id)) {
		              $referenced_entity = entity_load_single($referenced_entity_type,$referenced_entity_id);
	                if (empty($referenced_entity)){
									   //may be deleted
	  								 return;
									}  
	                // Decorate the entity.
	                $this->decorate_entity($referenced_entity_type, $referenced_entity);
	
	                // If our entity traversal depth has not reduced to 1, then we
	                // need to recurse to expand any entity reference fields present
	                // on the referenced entity.
	                if ($depth > 1) {
	                  $this->expand_entities($referenced_entity_type, $referenced_entity, $depth - 1, $flatten_fields);
	                }	
		              if ($referenced_entity_type=='user'){
										services_remove_user_data($referenced_entity);
									}
								  // Flatten field value arrays on the referenced entity if directed.
	                if ($flatten_fields) {
									  $referenced_entity = clone($referenced_entity);
	                  $this->flatten_fields($referenced_entity_type, $referenced_entity);
	                }
	                	
	                // And finally set the referenced entity to be the new value at
	                // the current delta in the reference field value array.
	                $entity->{$entities_field_name}[$referenced_entity_id] = $referenced_entity;
	              }
	            }
	          }
	          else {
	            // Extract the entity and its ID from the wrapper.
	            $referenced_entity_id = $entity_wrapper->{$field_name}->raw();
	            if (!empty($referenced_entity_id)) {
	              $referenced_entity = entity_load_single($referenced_entity_type,$referenced_entity_id);
                if (empty($referenced_entity)){
								   //may be deleted
  								 return;
								} 
	              // Decorate the entity.
	              $this->decorate_entity($referenced_entity_type, $referenced_entity);
	
	              // If our entity traversal depth has not reduced to 1, then we
	              // need to recurse to expand any entity reference fields present
	              // on the referenced entity.
	              if ($depth > 1) {
	               $this->expand_entities($referenced_entity_type, $referenced_entity, $depth - 1, $flatten_fields);
	              }
	              if ($referenced_entity_type=='user'){
									services_remove_user_data($referenced_entity);
								}

	              // Flatten field value arrays on the referenced entity if directed.
	              if ($flatten_fields) {	
								  $referenced_entity= clone($referenced_entity);							
	                $this->flatten_fields($referenced_entity_type, $referenced_entity);
	              }
	
	              // And finally set the referenced entity to be the new value at the
	              // current delta in the reference field value array.
	              $entity->{$entities_field_name}[$referenced_entity_id] = $referenced_entity;
	            }
	          }
	          
	        }
	      }
	    }
	  }
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
	  list($id, , $bundle) = entity_extract_ids($entity_type, $cloned_entity);
	  $clone_wrapper = entity_metadata_wrapper($entity_type, $cloned_entity);
	
	  // Loop over every field instance on the given entity.
	  foreach (field_info_instances($entity_type, $bundle) as $field_name => $instance) {
	    // Set the field property to the raw wrapper value, which applies the
	    // desired flattening of the value array.	
			$cloned_entity->{$field_name} = $clone_wrapper->{$field_name}->raw();		
			if ($field_name=='field_fivestar'){
			  $criteria = array(
			    'entity_type' => $entity_type,
			    'entity_id' => $id,
			    'value_type' => 'percent',
			    'tag' => 'vote',
			  );
				$results = votingapi_select_results($criteria);				
			  foreach ($results as $result) {
			    if ($result['function'] == 'average') {	
						$cloned_entity->{$field_name}=$result['value'];
						break;
			    }
			  }
			}			
	  }
	}
	
	
	/**
	 * Decorate the given entity with additional helper properties based on its type.
	 *
	 * Product entities will be given an attribute_fields property whose value is an
	 * array of product attribute fields on the product.
	 *
	 * Any instance of a price field will be given a corresponding property named
	 * [field_name]_formatted whose value is the currency formatted price.
	 *
	 * Any instance of a file or image field will be given a corresponding property
	 * named [field_name]_url whose value is the URL to the file or image.
	 *
	 * @param $entity_type
	 *   The machine-name entity type of the given entity.
	 * @param $entity
	 *   The entity whose field value arrays should be flattened.
	 */
	protected function decorate_entity($entity_type, $entity) {
	  list($id, , $bundle) = entity_extract_ids($entity_type, $entity);
	  
	  // If the entity is a line item, add its title to it.
	  if ($entity_type == 'commerce_line_item') {
		  $entity->line_item_title = commerce_line_item_title($entity);
	  }
	
	  if ($entity_type == 'commerce_order') {
				$order_wrapper = entity_metadata_wrapper($entity_type, $entity);
				$display_node = $order_wrapper->field_display->value();
				if (!empty($display_node)){
						$entity->display_title = check_plain($display_node->title);
				}
				else 
					$entity->display_title =NULL;
	  }

	  if ($entity_type == 'user') {
			list($id, , $bundle) = entity_extract_ids($entity_type, $entity);
			$comment_user_uid=$id;
			$node=user_profile_comments_load_entity($comment_user_uid);
			if (!empty($node)){
				$entity->comment_count = $node->comment_count;
			}
	  }

	  // If the entity is a product, add a list of its attribute fields to it.
	  if ($entity_type == 'commerce_product') {
	    $entity->attribute_fields = array();
	  }
	  			
	  // Loop over every field instance on the given entity.
	  foreach (field_info_instances($entity_type, $bundle) as $field_name => $instance) {
	    $field_info = field_info_field($field_name);
	    // If the current field instance functions as a product attribute field, add
	    // it to the array.
	    if (module_exists('commerce_cart')) {
	      if (commerce_cart_field_attribute_eligible($field_info) && commerce_cart_field_instance_is_attribute($instance)) {
	        $entity->attribute_fields[] = $field_name;
	      }
	    }
				
	    // Add formatted price fields to correspond with every price field instance.
	    if ($field_info['type'] == 'commerce_price' && $entity_type == 'commerce_product') {
	      $entity_wrapper = entity_metadata_wrapper($entity_type, $entity);
	      $formatted_field_name = $field_name . '_formatted';
	
	      // Set the formatted price for a single value field.
	      if ($field_info['cardinality'] == 1) {	
					try {
		        $entity->{$formatted_field_name} = commerce_currency_format($entity_wrapper->{$field_name}->amount->value(), $entity_wrapper->{$field_name}->currency_code->value(), $entity);
					}	
		      catch (EntityMetadataWrapperException $e) {
		        // A property causes problems - ignore that.
		      }
	      }
	      else {
	        // Otherwise loop over the field and format each value.
	        foreach ($entity_wrapper->{$field_name} as $delta => $price_wrapper) {
	          $entity->{$formatted_field_name}[$delta] = commerce_currency_format($price_wrapper->amount->value(), $price_wrapper->currency_code->value(), $entity);
	        }
	      }
	    }
	
	    // Add a direct URL to correspond with every file field instance.
	    if (in_array($field_info['type'], array('file', 'image'))) {
	      $entity_wrapper =  entity_metadata_wrapper($entity_type, $entity);
	      $url_field_name = $field_name . '_url';
	
	      // Set the formatted price for a single value field.
	      if ($field_info['cardinality'] == 1) {
	        $field_value = $entity_wrapper->{$field_name}->raw();
	        $entity->{$url_field_name} = '';
	        $url = NULL;
	
	        // If the field value contains a URI...
	        if (!empty($field_value['uri'])) {
	          // And we can generate a URL to the file at that URI...
	          $url = file_create_url($field_value['uri']);
	
	          if (!empty($url)) {
	            // Add it to the entity using the URL field name.
	            $entity->{$url_field_name} = $url;
	          }
	        }
	      }
	      else {
	        // Otherwise loop over the field and generate each URL.
	        $entity->{$url_field_name} = array();
	
	        foreach ($entity_wrapper->{$field_name} as $delta => $field_wrapper) {
	          $field_value = $field_wrapper->raw();
	          $url = NULL;
	
	          // If the field value contains a URI...
	          if (!empty($field_value['uri'])) {
	            // And we can generate a URL to the file at that URI...
	            $url = file_create_url($field_value['uri']);
	
	            if (!empty($url)) {
	              // Add it to the entity using the URL field name.
	              $entity->{$url_field_name}[$delta] = $url;
	            }
	          }
	
	          // If the field value did not have a URI or the URL to the file could not
	          // be determined, add an empty URL string to the entity.
	          if (empty($url)) {
	            $entity->{$url_field_name}[$delta] = '';
	          }
	        }
	      }
	    }
	  }
	}

	/**
	 * Updates the field and property values on an entity with the data provided.
	 *
	 * @param $entity_type
	 *   The machine-name of the type of entity to update.
	 * @param $entity
	 *   The entity to update.
	 * @param $data
	 *   The entity data to update.
	 * @param $flatten_fields
	 *   A string indicating whether or not field value arrays in the data are
	 *   flattened as in GET requests; accepts true or false, defaults to true.
	 */
	protected function set_field_values($entity_type, $entity, $data, $flatten_fields='true') {
	  // Build an array of protected properties, preventing update requests from
	  // changing IDs and other immutable properties of the entity.
	  $entity_info = entity_get_info($entity_type);
	  $entity_keys = array_diff_key($entity_info['entity keys'], array('label' => ''));
	
	  $protected_properties = array_values($entity_keys);
	
	  // Loop over the data submitted with the request.
	  foreach ($data as $key => $value) {
	    // If the key is a property name...
	    if (in_array($key, $this->entity_type_properties($entity_type))) {
	      // If it attempts to change a protected property, return the error.
	      if (in_array($key, $protected_properties) && $entity->{$key} != $value) {
	        return services_error(t('Property @property cannot be changed', array('@property' => $key)), 400);
	      }
	
	      // If valid, set it directly to the entity.
	      if ($this->validate_property_value($entity_type, $entity, $key, $value)) {
	        $entity->{$key} = $value;
	
	        // If the key was a log message, ensure a new revision is triggered.
	        $entity->revision = TRUE;
	      }
	      else {
	        return services_error(t('Invalid @property value given', array('@property' => $key)), 400);
	      }
	    }
	    else {
	      // Look for the key as a field name in the entity type's field list.
	      $known_field = FALSE;
	
	      foreach ($this->entity_type_fields($entity_type) as $field_name => $field_type) {
	        // If the key begins with a field name, then either the key is the field
	        // name or is a column of the field.	        
	        if (strpos($key, $field_name) === 0) {
	          $field_info = field_info_field($field_name);
	          // Because the entity metadata wrapper treats text fields differently
	          // based on whether or not they enable format selection, we also
	          // accommodate the inconsistency here. If a text field does not have
	          // text formatting enabled, we unset that column from the field schema
	          // so it can be set independently as if it were the only column in the
	          // field value.
	          if (in_array($field_info['type'], array('text', 'text_long'))) {
	            $instance_info = field_info_instance($entity_type, $field_name, $entity->type);
	
	            if (empty($instance_info['settings']['text_processing'])) {
	              unset($field_info['columns']['format']);
	            }
	          }
	
	          if ($field_name == $key) {
	            // If the update request sent fields in a flattened format...
	            if ($flatten_fields == 'true') {
	              // Update the field value without specifying a language code.	          
	              if ($field_info['cardinality'] == 1) {
	                $this->update_field_value($entity_type, $entity, $field_info, $value);
	                
	              }
	              else {
	                // For multi-value fields, loop over the value array and pass
	                // the delta value to the update function.
									$entity->{$field_name}[LANGUAGE_NONE] = array();
	                foreach ($value as $delta => $item) {
	                  $this->update_field_value($entity_type, $entity, $field_info, $item, NULL, $delta);
	                }
	              }
	            }
	            else {
	              // Otherwise, loop over the languages given and attempt to update
	              // the field value individually for each language code. Since
	              // field values are not flattened, the value array must include a
	              // language code and delta values even for single-value fields.
	              foreach ($value as $langcode => $items) {
	                // Return an error if the $langcode is unknown or if $items is
	                // not an array.
	                if ($langcode != LANGUAGE_NONE && !in_array($langcode, array_keys(language_list()))) {
	                  return services_error(t('Unknown language @langcode in @field value', array('@langcode' => $langcode, '@field' => $field_name)), 400);
	                }
	                elseif (!is_array($items)) {
	                  return services_error(t('Invalid @field value given', array('@field' => $field_name)), 400);
	                }
									$entity->{$field_name}[$langcode] = array();
	                foreach ($items as $delta => $item) {
	                  // If $item is empty, simply unset it at the delta value.
	                  if (empty($item)) {
	                    unset($entity->{$field_name}[$langcode][$delta]);
	                  }
	                  else {
	                    // Otherwise update the field value for the delta value.
	                    $this->update_field_value($entity_type, $entity, $field_info, $item, $langcode, $delta);
	                  }
	                }
	              }
	            }
	
	            $known_field = TRUE;
	            break;
	          }
	          else {
	            // Otherwise if the key contains a valid column specification for
	            // the field type, create a value array to set the column directly.
	            $column = substr($key, strlen($field_name) + 1);
	
	            if (in_array($column, array_keys($field_info['columns']))) {
	              if ($flatten_fields == 'true') {
	                // Directly set single value fields.
	                if ($field_info['cardinality'] == 1) {
	                  $this->update_field_value($entity_type, $entity, $field_info, array($column => $value));
	                }
	                else {
	                  // For multi-value fields, loop over the value array and pass
	                  // the delta value to the update function.
	                  foreach ($value as $delta => $item) {
	                    $this->update_field_value($entity_type, $entity, $field_info, array($column => $item), NULL, $delta);
	                  }
	                }
	              }
	              else {
	                // If field values are not flattened, they must include both a
	                // language code and delta values even for single-value fields.
	                foreach ($value as $langcode => $items) {
	                  foreach ($items as $delta => $item) {
	                    $this->update_field_value($entity_type, $entity, $field_info, array($column => $item), $langcode, $delta);
	                  }
	                }
	              }
	
	              $known_field = TRUE;
	              break;
	            }
	          }
	        }
	      }
	
	      // If no known field matched, return a 400 error.
	      if (!$known_field) {
	        return services_error(t('Unknown property @name', array('@name' => $key)), 400);
	      }
	    }
	  }
	
	  // @todo Post-process textfields by invoking text_field_load() on their values
	  // to get the updated summary and safe_value values.
	}
	
	/**
	 * Returns a list of properties for the specified entity type.
	 *
	 * For the purpose of the Commerce Services module, the properties returned are
	 * those that correspond to a database column as determined by the Entity API.
	 * These may be used to filter and sort index queries.
	 *
	 * @param $entity_type
	 *   Machine-name of the entity type whose properties should be returned.
	 *
	 * @return
	 *   An associative array of properties for the specified entity type with the
	 *   key being the property name and the value being the corresponding schema
	 *   field on the entity type's base table.
	 */
	protected function entity_type_properties($entity_type) {
	  $properties = drupal_static(__FUNCTION__);
	
	  if (!isset($properties[$entity_type])) {
	    $entity_info = entity_get_info($entity_type);
	    $info = entity_get_property_info($entity_type);
	    $properties[$entity_type] = array();
	
	    // Loop over only the properties of the entity type.
	    foreach ($info['properties'] as $key => $value) {
	      // If the value specifies a schema field...
	      if (!empty($value['schema field'])) {
	        $properties[$entity_type][$key] = $value['schema field'];
	      }
	    }
	
	    // If the entity type supports revisions, add revision and log to the array
	    // of acceptable properties.
	    if (!empty($entity_info['revision table'])) {
	      $properties[$entity_type] += array('revision', 'log');
	    }
	  }
	
	  return $properties[$entity_type];
	}

	/**
	 * Returns a list of fields for the specified entity type.
	 *
	 * @param $entity_type
	 *   Machine-name of the entity type whose properties should be returned.
	 * @param $bundle
	 *   Optional bundle name to limit the returned fields to.
	 *
	 * @return
	 *   An associative array of fields for the specified entity type with the key
	 *   being the field name and the value being the Entity API property type.
	 */
	protected function entity_type_fields($entity_type, $bundle = NULL) {
	  $fields = drupal_static(__FUNCTION__);
	
	  if (!isset($fields[$entity_type])) {
	    $info = entity_get_property_info($entity_type);
	    $fields = array();
	
	    // Loop over the bundles info to inspect their fields.
	    foreach ($info['bundles'] as $bundle_name => $bundle_info) {
	      // Loop over the properties on the bundle to find field information.
	      foreach ($bundle_info['properties'] as $key => $value) {
	        if (!empty($value['field'])) {
	          $fields[$entity_type][$bundle_name][$key] = $value['type'];
	        }
	      }
	    }
	  }
	
	  // If a specific bundle's fields was requested, return just those.
	  if (!empty($bundle)) {
	    return $fields[$entity_type][$bundle];
	  }
	  else {
	    // Otherwise combine all the fields for various bundles of the entity type
	    // into a single return value.
	    $combined_fields = array();
	
	    foreach ($fields[$entity_type] as $bundle_name => $bundle_fields) {
	      $combined_fields += $bundle_fields;
	    }
	
	    return $combined_fields;
	  }
	}

	/**
	 * Validates a value to be assigned to a property on an entity.
	 *
	 * @param $entity_type
	 *   The type of entity requiring validation.
	 * @param $entity
	 *   The actual entity.
	 * @param $property
	 *   The name of the property.
	 * @param $value
	 *   The value to be validated for the property.
	 *
	 * @return
	 *   Boolean indicating whether or not the given value is valid for the
	 *   specified property; TRUE if validity simply cannot be determined.
	 */
	protected function validate_property_value($entity_type, $entity, $property, $value) {
	  // Do not allow array values to be set to properties.
	  if (is_array($value)) {
	    return FALSE;
	  }
	
	  $properties = entity_get_property_info($entity_type);
	
	  $wrapper = entity_metadata_wrapper($entity_type, $entity);
	
	  // If the property has an options list callback...
	  if (!empty($wrapper->{$property}) && $options = $wrapper->{$property}->optionsList('edit')) {
	    // Ensure the proposed value is an available option, supporting single depth
	    // options arrays and arrays that use options groups.
	    $match = FALSE;
	
	    foreach ($options as $option_key => $option_value) {
	      if (is_array($option_value)) {
	        if (in_array($value, array_keys($option_value))) {
	          $match = TRUE;
	          break;
	        }
	      }
	      elseif ($option_key == $value) {
	        $match = TRUE;
	        break;
	      }
	    }
	
	    if (!$match) {
	      return FALSE;
	    }
	  }
	
	  // If the property is an integer or a date, ensure an integer was given.
	  if (!empty($properties['properties'][$property]) &&
	    in_array($properties['properties'][$property]['type'], array('integer', 'date')) &&
	    !is_int($value)) {
	    return FALSE;
	  }
	
	  return TRUE;
	}

	/**
	 * Validates a value to be assigned to a field on an entity.
	 *
	 * @param $entity_type
	 *   The type of entity requiring validation.
	 * @param $entity
	 *   The actual entity.
	 * @param $field_name
	 *   The name of the field.
	 * @param $value
	 *   The value to be validated for the field.
	 *
	 * @return
	 *   Boolean indicating whether or not the given value is valid for the
	 *   specified field; TRUE if validity simply cannot be determined.
	 */
	protected function validate_field_value($entity_type, $entity, $field_name, $value) {
	  $field_info = field_info_field($field_name);
	
	  // Do not allow array values to be set to single column fields.
	  if (is_array($value) && count($field_info['columns']) == 1) {
	    return FALSE;
	  }
	
	  // Convert $value to an array to simplify further validation.
	  if (!is_array($value)) {
	    $value = array($value);
	  }
	
	  $wrapper = entity_metadata_wrapper($entity_type, $entity);
	
	  // If the field has an options list callback...
	  if ($options = $wrapper->{$field_name}->optionsList('edit')) {
	    // Ensure each proposed value is an available option.
	    foreach ($value as $field_value) {
	      if (!in_array($field_value, array_keys($options))) {
	        return FALSE;
	      }
	    }
	  }
	
	  return TRUE;
	}
	
	/**
	 * Updates the field value array on the given entity.
	 *
	 * @param $entity_type
	 *   The type of entity being updated.
	 * @param $entity
	 *   The actual entity.
	 * @param $field_info
	 *   The field info array for the field being updated.
	 * @param $value
	 *   The single or multi-value field value array to update the field to either
	 *   through a wrapper or the given language.
	 * @param $langcode
	 *   The language to set the field value to if given.
	 */
	protected function update_field_value($entity_type, $entity, $field_info, $value, $langcode = NULL, $delta = NULL) {
	  $field_name = $field_info['field_name'];
	
	  // If the value given is an array...
	  if (is_array($value)) {
	    // Ensure the keys given for the value array all match columns in the field
	    // type's schema.
	    if (count(array_diff_key($value, $field_info['columns'])) > 0) {
	      return services_error(t('Invalid @field value given', array('@field' => $field_name)), 400);
	    }
	  }
	  elseif (count($field_info['columns']) == 1) {
	    // Otherwise, if a scalar was given for a single column field, rewrite the
	    // value to be an array keyed by the column name.
	    $value = array(key($field_info['columns']) => $value);
	  }
	  else {
	    // Return an error if an array was given for a single column field instead
	    // of a scalar.
	    return services_error(t('Invalid @field value given', array('@field' => $field_name)), 400);
	  }
	
	  // Default to the entity language if $langcode is not set.
	  if ($langcode === NULL) {
	    $langcode = entity_language($entity_type, $entity);
	
	    if (empty($langcode)) {
	      $langcode = LANGUAGE_NONE;
	    }
	  }
	
	  // Return an error if a delta value over 0 is given for a single value field.
	  if ($field_info['cardinality'] == 1 && !empty($delta)) {
	    return services_error(t('Invalid delta value @delta given for single-value field @field', array('@delta' => $delta, '@field' => $field_name)), 400);
	  }
	  elseif ($field_info['cardinality'] != 1 && $delta === NULL) {
	    // Return an error if a delta value was not specified for a multi-value field.
	    return services_error(t('Delta value required for multi-value field @field', array('@field' => $field_name)), 400);
	  }
	
	  // If updating a single value field, ensure the delta value matches the
	  // current single delta value. There may be instances where for some reason it
	  // was not zero, so we ensure this is the case by changing the delta key of
	  // the field value array.
	  if ($field_info['cardinality'] == 1) {
	    	$current_delta = key($entity->{$field_name}[$langcode]);
	    if ($current_delta != 0) {
	      $entity->{$field_name}[$langcode][0] = $entity->{$field_name}[$langcode][$current_delta];
	      unset($entity->{$field_name}[$langcode][$current_delta]);
	    }
	
	    // Also, ensure the $delta value is changed from NULL to 0 if necessary.
	    $delta = 0;
	  }
	
	  // Update only the columns that were specified in the update request.
	  // @todo Add individual column validation.
	  foreach ($value as $column => $column_value) {
	    $entity->{$field_name}[$langcode][$delta][$column] = $column_value;
	  }
	}	

  /**
   * Implemented to define basic CRUD/I operations.
   *
   * May be extended to add arguments to these operations, or to
   * add additional actions or relationships.
   *
   * @see ServicesResourceControllerInterface::resourceInfo()
   */
  public function resourceInfo($entity_type) {
    $info = parent::resourceInfo($entity_type);
    if ($entity_type=='node'){
			if (module_exists('commerce_product_reference') && module_exists('commerce_product')){
				$info['targeted_actions']=array();
				
    		$info['targeted_actions']['attach_product'] = 
    				$this->getInfoElement($entity_type, 'attach_product', 'update', FALSE, FALSE);
    		$info['targeted_actions']['attach_product']['help']=
    		    'Upload and attach product to a product-display(node). POST json-data to node/123/add_product';
        $info['targeted_actions']['attach_product']['args'][] = array(
              'name' => 'nid',
              'optional' => FALSE,
              'source' => array('path' => 0),
              'type' => 'int',
              'description' => 'The nid of the node to attach a file to',
            );
            
        $info['targeted_actions']['attach_product']['args'][] = array(
              'name' => 'field_name',
              'optional' => FALSE,
              'source' => array('data' => 'field_name'),
              'description' => 'The file field name',
              'type' => 'string',
            );
        $info['targeted_actions']['attach_product']['args'][] = array(
              'name' => 'attach',
              'optional' => TRUE,
              'source' => array('data' => 'attach'),
              'description' => 'Attach the file(s) to the node. If FALSE, this clears ALL files attached, and attaches the files',
              'type' => 'int',
              'default value' => TRUE,
            );
        $info['targeted_actions']['attach_product']['args'][] = array(
              'name' => 'field_values',
              'optional' => TRUE,
              'source' => array('data' => 'field_values'),
              'description' => 'The extra field values',
              'type' => 'array',
              'default value' => array(),
            );				
		  }		  
		}

    if ($entity_type=='commerce_order'){
			if (module_exists('commerce_line_item') && module_exists('commerce_product')){
				$info['targeted_actions']=array();
				
    		$info['targeted_actions']['add_line_items'] = 
    				$this->getInfoElement($entity_type, 'add_line_items', 'update', FALSE, FALSE);
    		$info['targeted_actions']['add_line_items']['help']=
    		    'Upload snd attach line item to a order. POST json-data to commerce_order/123/add_line_items';
        $info['targeted_actions']['add_line_items']['args'][] = array(
              'name' => 'order_id',
              'optional' => FALSE,
              'source' => array('path' => 0),
              'type' => 'int',
              'description' => 'The id of the order to attach a line-item to',
            );
        $info['targeted_actions']['add_line_items']['args'][] = array(
              'name' => 'attach',
              'optional' => TRUE,
              'source' => array('data' => 'attach'),
              'description' => 'Attach the file(s) to the node. If FALSE, this clears ALL files attached, and attaches the files',
              'type' => 'int',
              'default value' => TRUE,
            );
        $info['targeted_actions']['add_line_items']['args'][] = array(
              'name' => 'values',
              'optional' => TRUE,
              'source' => array('data' => 'values'),
              'description' => 'The extra field values',
              'type' => 'array',
              'default value' => array(),
            );				
				$info['targeted_actions']['add_line_items']['args'][] = array(
              'name' => 'fields',
              'type' => 'string',
              'description' => 'A comma separated list of field or property names from product display nodes to include in the result set. Will be converted to an array before being passed to the operation callback.',
              'optional' => TRUE,
              'source' => array('data' => 'fields'),
              'default value' => '*',
            );
					}
		}
		
    if ($entity_type=='message'){
			if (module_exists('message')){
				$info['targeted_actions']=array();
				
    		$info['targeted_actions']['markread'] = 
    				$this->getInfoElement($entity_type, 'message_mark_read', 'update', FALSE, FALSE);
    		$info['targeted_actions']['markread']['help']=
    		    'flag message as read';
        $info['targeted_actions']['markread']['args'][] = array(
              'name' => 'entity_id',
              'optional' => FALSE,
              'source' => array('path' => 0),
              'type' => 'int',
              'description' => 'The id of the message to mark as read',
            );
					}
		}
		
    return $info;
  }
}
