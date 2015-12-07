<?php

/**
 * @file
 * Contains the resource callbacks for orders.
 */


/**
 * Returns a collection of orders.
 *
 * @see commerce_services_index_entities()
 */
function commerce_services_order_index($fields, $expand_entities, $flatten_fields, $filter, $filter_op, $sort_by, $sort_order, $limit, $offset) {
  return commerce_services_index_entities('commerce_order', $fields, $expand_entities, $flatten_fields, $filter, $filter_op, $sort_by, $sort_order, $limit, $offset);
}

/**
 * Determines an API user's access to index orders.
 */
function commerce_services_order_index_access() {
  // Because the entity access control system will filter any result sets,
  // there's no reason to limit a particular user's access.
  return TRUE;
}

/**
 * Returns a single order.
 *
 * @see commerce_services_retrieve_entity()
 */
function commerce_services_order_retrieve($order_id, $expand_entities, $flatten_fields) {
  return commerce_services_retrieve_entity('commerce_order', $order_id, $expand_entities, $flatten_fields);
}

/**
 * Determines an API user's access to retrieve a given order.
 *
 * @param $order_id
 *   The ID of the order to be retrieved.
 *
 * @return
 *   Boolean indicating the user's access to retrieve the order.
 */
function commerce_services_order_retrieve_access($order_id) {
  // Attempt to load the order.
  if ($order = commerce_order_load($order_id)) {
    // And perform the view access check.
    if (commerce_order_access('view', $order)) {
      return TRUE;
    }
    else {
      return services_error(t('Access to this operation not granted'), 401);
    }
  }
  else {
    return services_error(t('Order not found'), 404);
  }
}

/**
 * Updates an order.
 *
 * @see commerce_services_update_entity()
 */
function commerce_services_order_update($order_id, $data, $flatten_fields) {
    //--begin added by marklee 2015-01-09
  $order = commerce_order_load($order_id);
	if($order->type='commerce_marketplace_order'){		
	   $query1 = new EntityFieldQuery();
       $query1->entityCondition('entity_type', 'commerce_order')
        ->entityCondition('bundle', 'commerce_store_order')
        ->fieldCondition('cmp_order', 'target_id', $order->order_id)
		->propertyOrderBy('changed', 'DESC')
        ->range(0, 1);
       $result1 = $query1->execute();
	   $store_order = NULL;
	    if (isset($result1['commerce_order'])) {
        $res = array_pop($result1['commerce_order']);		
		 commerce_services_update_entity('commerce_order', $res->order_id, $data, $flatten_fields);		
      }	  
	 }
	 
	//--end  
	 	 

	//modified by Richard 2014-11-03
	$order = commerce_services_update_entity('commerce_order', $order_id, $data, $flatten_fields);
	$order = commerce_order_load($order->order_id);
	if ($order->status=='checkout_complete') {
		  rules_invoke_all('commerce_checkout_complete', $order);
	}
	if ($order->status=='canceled') {
		  rules_invoke_all('commerce_canceled', $order);
	}
	//$order->status=='checkout_checkout' || 
	if ($order->status=='checkout_checkout'||$order->status=='checkout_shipping') {

	      //commerce_shipping_collect_rates($order);
		  //commerce_shipping_service_rate_order('standard_shipping',$order);
		  
		// rules_invoke_all('commerce_shipping_collect_rates', $order);
		  $total=$order->commerce_order_total['und'][0]['amount'];
	      $shipping_service_name='';
	      if($total>=9900){
		    $shipping_service_name='free_shipping';
	      }else{
		    $shipping_service_name='standard_shipping';
	      }
	
	       commerce_test($order,$shipping_service_name);              
     
	}
	 $order = commerce_services_retrieve_entity('commerce_order', $order_id, TRUE, $flatten_fields);
	return $order;
}



/**
 * Determines an API user's access to update a given order.
 *
 * @param $order_id
 *   The ID of the order to be updated.
 *
 * @return
 *   Boolean indicating the user's access to update the order.
 */
function commerce_services_order_update_access($order_id) {
  // Attempt to load the order.
  if ($order = commerce_order_load($order_id)) {
    // If the user has access to perform the operation...
    if (commerce_order_access('update', $order)) {
      return TRUE;
    }
    else {
      return services_error(t('Access to this operation not granted'), 401);
    }
  }
  else {
    return services_error(t('Order not found'), 404);
  }
}

/**
 * Deletes an order.
 *
 * @param $order_id
 *   The ID of the order to be deleted.
 */
function commerce_services_order_delete($order_id) {
  commerce_order_delete($order_id);
}

/**
 * Determines an API user's access to delete a given order.
 *
 * @param $order_id
 *   The ID of the order to be deleted.
 *
 * @return
 *   Boolean indicating the user's access to delete the order.
 */
function commerce_services_order_delete_access($order_id) {
  // Attempt to load the order.
  if ($order = commerce_order_load($order_id)) {
    // If the user has access to perform the operation...
    if (commerce_order_access('delete', $order)) {
      return TRUE;
    }
    else {
      return services_error(t('Access to this operation not granted'), 401);
    }
  }
  else {
    return services_error(t('Order not found'), 404);
  }
}

/**
 * Returns a collection of line items on an order.
 *
 * @see commerce_services_index_entities()
 */
function commerce_services_order_line_item_index($order_id, $fields, $expand_entities, $flatten_fields, $filter, $filter_op, $sort_by, $sort_order, $limit, $offset) {
  // Add the order ID to the filter list.
  $filter['order_id'] = $order_id;
  $filter_op['order_id'] = '=';

  return commerce_services_index_entities('commerce_line_item', $fields, $expand_entities, $flatten_fields, $filter, $filter_op, $sort_by, $sort_order, $limit, $offset);
}

/**
 * Determines an API user's access to index line items on an order.
 */
function commerce_services_order_line_item_index_access($order_id) {
  // Attempt to load the order.
  if ($order = commerce_order_load($order_id)) {
    // And perform the view access check.
    if (commerce_order_access('view', $order)) {
      return TRUE;
    }
    else {
      return services_error(t('Access to this operation not granted'), 401);
    }
  }
  else {
    return services_error(t('Order not found'), 404);
  }
}
