<?php

/**
 * @file
 * Page callback file for the commerce_unionpay module.
 */

/**
 * Menu callback function to process Unionpay's feedback notifications.
 */
function commerce_unionpay_process_notify() {
  if (empty($_POST)) {
    return FALSE;
  }
  $order = commerce_order_load(substr($_POST['orderNumber'], 10));
  $payment_method = commerce_payment_method_instance_load($order->data['payment_method']);
  // Validate the received notification from Unionpay.
  if (commerce_unionpay_notify_validate($order, $payment_method, $_POST)) {
    commerce_unionpay_notify_submit($order, $payment_method, $_POST);
  }
}

/**
 * Validation of Unionpay's notifications.
 */
function commerce_unionpay_notify_validate($order, $payment_method, $notify) {
  if (empty($notify)) {
    return FALSE;
  }
  // Log an entry of the notification received for a transaction.
  watchdog('commerce_unionpay', 'Customer returned from Unionpay with the following data:<pre>@notify</pre>', array('@notify' => print_r($notify, TRUE)));

  // TODO

  return TRUE;
}

/**
 * Submit and process a notification feedback from Unionpay for a transaction.
 *
 * This function creates or updates payment transaction records for
 * corresponding orders depending on the status of the notifications received.
 */
function commerce_unionpay_notify_submit($order, $payment_method, $notify) {
  // Create a new payment transaction for the order.
  $transaction = commerce_payment_transaction_new($payment_method['method_id'], $order->order_id);
  $transaction->instance_id = $payment_method['instance_id'];

  // Currently supports only transactions in CNY.
  $transaction->amount = commerce_currency_decimal_to_amount($notify['settleAmount'], 'CNY');
  $transaction->currency_code = 'CNY';

  $transaction->remote_id = $notify['qid'];
  $transaction->data['notify'] = $notify;

  switch ($notify['respCode']) {
    case COMMERCE_UNIONPAY_RESP_SUCCCESS:
      $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
      $transaction->message = $notify['respMsg'];
      break;
  }

  // Save the payment transaction and redirect to next page.
  commerce_payment_transaction_save($transaction);
  commerce_payment_redirect_pane_next_page($order);
  // Record an entry of the transaction with the order and trade numbers.
  watchdog('commerce_unionpay', 'Notify processed for Order @order_number.', array('@order_number' => $order->order_number), WATCHDOG_INFO);
}
