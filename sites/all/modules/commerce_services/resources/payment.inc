<?php

/*
//add by marklee 2014-09-02 
function commerce_services_payment_index(){

 $arr1=array("payment_title"=>"Alipay","payment_id"=>"commerce_payment_alipay");
 $arr2=array("payment_title"=>"示例支付","payment_id"=>"commerce_payment_commerce_payment_example");
 return array($arr1,$arr2);

}
*/
/**
 * @file
 * Contains the resource callbacks for payments.
 */

/**
 * Returns a collection of payment transactions.
 *
 * @see commerce_services_index_entities()
 */
function commerce_services_payment_index($fields, $expand_entities, $flatten_fields, $filter, $filter_op, $sort_by, $sort_order, $limit, $offset) {
  return commerce_services_index_entities('commerce_payment_transaction', $fields, $expand_entities, $flatten_fields, $filter, $filter_op, $sort_by, $sort_order, $limit, $offset);
}

/**
 * Determines an API user's access to index payment transactions.
 */
function commerce_services_payment_index_access() {
  return TRUE;
}

/**
 * Returns a single payment transaction.
 *
 * @see commerce_services_retrieve_entity()
 */
function commerce_services_payment_retrieve($transaction_id, $expand_entities, $flatten_fields) {
  return commerce_services_retrieve_entity('commerce_payment_transaction', $transaction_id, $expand_entities, $flatten_fields);
}

/**
 * Determines an API user's access to retrieve a given payment transaction.
 *
 * @param $transaction_id
 *   The ID of the transaction to be retrieved.
 *
 * @return
 *   Boolean indicating the user's access to retrieve the payment.
 */
function commerce_services_payment_retrieve_access($transaction_id) {
  // Attempt to load the payment transaction.
  if ($payment = commerce_payment_transaction_load($transaction_id)) {
    // And perform the view access check.
    if (commerce_payment_transaction_access('view', $payment)) {
      return TRUE;
    }
    else {
      return services_error(t('Access to this operation not granted'), 401);
    }
  }
  else {
    return services_error(t('Payment not found'), 404);
  }
}

/**
 * Creates a new payment transaction on an order.
 */
function commerce_services_payment_create($data, $flatten_fields) {
  // Ensure the create request specifies a valid payment method type.
  if (empty($data['payment_method'])) {
    return services_error(t('You must specify a valid payment method type'), 400);
  }

  // Create the new payment transaction
  $payment = commerce_payment_transaction_new($data['method_id'], $data['order_id']);

  // Remove the method_id and order_id from the data array since they've been set.
  unset($data['method_id'], $data['order_id']);
return services_error(t('TestMe'), 400);
  // Set the field and property data and save the payment.
  commerce_services_set_field_values('commerce_payment_transaction', $payment, $data, $flatten_fields);
	return services_error(t('TestMe'), 400);
  commerce_payment_transaction_save($payment);

  // Add simplified fields to the payment object for certain field types.
  commerce_services_decorate_entity('commerce_payment_transaction', $payment);

  // Flatten field value arrays if specified. This must be the last operation
  // performed as it breaks the standard field data model. An entity whose
  // fields have thus been flattened is no longer wrappable or writable.
  if ($flatten_fields == 'true') {
    $payment = clone($payment);
    commerce_services_flatten_fields('commerce_payment_transaction', $payment);
  }

  return $payment;
}

/**
 * Determines an API user's access to create new pamyent transactions.
 */
function commerce_services_payment_create_access($data) {
  // Load the order from the data array to determine access to create payments
  // on it.
  $order = NULL;

  if (!empty($data['order_id'])) {
    $order = commerce_order_load($data['order_id']);
  }

  if (empty($order)) {
    return services_error(t('You must specify a valid order ID'), 400);
  }

  // If the user has access to update the order...
  if (commerce_order_access('update', $order)) {
    return TRUE;
  }
  else {
    return services_error(t('Access to this operation not granted'), 401);
  }
}

/**
 * Updates a payment.
 *
 * @see commerce_services_update_entity()
 */
function commerce_services_payment_update($transaction_id, $data, $flatten_fields) {
  return commerce_services_update_entity('commerce_payment_transaction', $transaction_id, $data, $flatten_fields);
}

/**
 * Determines an API user's access to update a given payment transaction
 *
 * @param $transaction_id
 *   The ID of the payment transaction to be updated.
 *
 * @return
 *   Boolean indicating the user's access to update the payment transaction.
 */
function commerce_services_payment_update_access($transaction_id) {
  // Attempt to load the payment transaction.
  if ($payment = commerce_payment_transaction_load($transaction_id)) {
    // If the user has access to perform the operation...
    if (commerce_payment_transaction_access('update', $payment)) {
      return TRUE;
    }
    else {
      return services_error(t('Access to this operation not granted'), 401);
    }
  }
  else {
    return services_error(t('Payment transaction not found'), 404);
  }
}

/**
 * Deletes a payment transaction
 *
 * @param $transaction_id
 *   The ID of the payment transaction to be updated.
 */
function commerce_services_payment_delete($transaction_id) {
  commerce_payment_transaction_delete($transaction_id);
}

/**
 * Determines an API user's access to delete a given payment transaction.
 *
 * @param $transaction_id
 *   The ID of the payment transaction to be deleted.
 *
 * @return
 *   Boolean indicating the user's access to delete the payment transaction.
 */
function commerce_services_payment_delete_access($transaction_id) {
  // Attempt to load the payment transaction
  if ($payment = commerce_payment_transaction_load($transaction_id)) {
    // If the user has access to perform the operation...
    if (commerce_payment_transaction_access('delete', $payment)) {
      return TRUE;
    }
    else {
      return services_error(t('Access to this operation not granted'), 401);
    }
  }
  else {
    return services_error(t('Payment transaction not found'), 404);
  }
}