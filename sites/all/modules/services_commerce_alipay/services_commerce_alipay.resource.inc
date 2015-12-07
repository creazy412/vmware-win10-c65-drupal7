<?php
/**
 * Implements hook_permission().
 */
function services_commerce_alipay_permission() {
  return TRUE;
}

function services_commerce_alipay_notify(){
   require_once("alipay.config.php");
   require_once("lib/alipay_notify.class.php");
   $alipayNotify = new AlipayNotify($alipay_config);
   $verify_result = $alipayNotify->verifyNotify();

   if($verify_result) {
			// watchdog('alipay success', 'alipay success:<pre>@success</pre>', array('@notify' => print_r($verify_result, TRUE)));
	   	$order_no = $_POST['out_trade_no'];
	   	$arr_str=explode("Z",$order_no);
	 
	   	if(!empty($arr_str)){    
	      $order_no1 = $arr_str[1];
			  $order = commerce_order_load($order_no1);
				global $user;
				if ($user->uid == 0) {
				  $user = user_load($order->uid);
			  }
				
				$payment_method = commerce_payment_method_instance_load("alipay|commerce_payment_alipay");
				commerce_quick_alipay_notify_submit1($order, $payment_method, $_POST);
	   	}
		 	else {
		 	//error, no order id process
		 	}
	  //  watchdog('order_no1', 'order_no1:<pre>@order_no1</pre>', array('@order_no1' => print_r($order_no1, TRUE)));
   }
   else {
    //验证失败
    echo "fail";
		die();
   }
}



function commerce_quick_alipay_notify_submit1($order, $payment_method, $notify) {
	watchdog('successback', 'successback:<pre>@successback</pre>', array('@successback' => print_r($notify, TRUE)));
    // Attempt to load prior authorization capture IPN created previously.
  $transactions = commerce_payment_transaction_load_multiple(array(), array('remote_id' => $notify["trade_no"]));	
  if (!empty($transactions)) {
    // @TODO: Load the prior transaction ID and update with the capture values.
    // There is probably some special handling necessary in this case.
    $transaction = reset($transactions);
  }  
  else {
    // Create a new payment transaction for the order.
    $transaction = commerce_payment_transaction_new($payment_method['method_id'], $order->order_id);
    $transaction->instance_id = $payment_method['instance_id'];
  } 
  // Currently supports only transactions in CNY.
  $transaction->amount = commerce_currency_decimal_to_amount($notify['total_fee'], 'CNY');
  $transaction->currency_code = 'CNY';

  $transaction->remote_id = $notify["trade_no"];
  // Set the transaction's statuses based on notify's trade_status.
  $transaction->remote_status = $notify['trade_status'];
  $transaction->data['notify'] = $notify;	

  $flag='';
   // Handle refund types of cases.
  if (isset($notify['refund_status'])) {
    switch ($notify['refund_status']) {
      // Pending seller accept refund request.
      case 'WAIT_SELLER_AGREE':
        $transaction->status = COMMERCE_PAYMENT_STATUS_PENDING;
        $transaction->message = commerce_alipay_status_message1($notify['refund_status']);
        commerce_order_status_update($order, 'wait_seller_agree');
        break;

      // Success refund transaction.
      case 'REFUND_SUCCESS':
        $transaction->status = COMMERCE_PAYMENT_STATUS_PENDING;
        $transaction->message = commerce_alipay_status_message1($notify['refund_status']);
        commerce_order_status_update($order, 'refund_success');
        break;
    }
  }
  else {
   if($order->status=='checkout_complete'||$order->status=='completed'){
	   	echo 'The bill is finished.';
	 }
    // Handle trade types of cases.
    switch ($notify['trade_status']) {
      // Operation canceled.
      case 'TRADE_CLOSED':
        $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
        $transaction->message = commerce_alipay_status_message1($notify['trade_status']);
        commerce_order_status_update($order, 'canceled');
        break;

      // Transaction complete.
      case 'TRADE_FINISHED':
	  
        $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
        $transaction->message = commerce_alipay_status_message1($notify['trade_status']);
        // No need to change order status when Instant Payment is successful.
        //if ($notify['exterface']!= "create_direct_pay_by_user") {
        if (!isset($notify['exterface']) || $notify['exterface']!= "create_direct_pay_by_user") {
		
          $flag=commerce_order_status_update($order, 'pending');
		  
        }
        break;

      // Transaction successful.
      case 'TRADE_SUCCESS':
        $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
        $transaction->message = commerce_alipay_status_message1($notify['trade_status']);
        // No need to change order status when Instant Payment is successful.
        if (!isset($notify['exterface']) || $notify['exterface']!= "create_direct_pay_by_user") {
          $flag=commerce_order_status_update($order, 'pending');
        }
        break;
    }
  } 
  if($flag&&commerce_payment_transaction_save($transaction)){	
    watchdog('success', 'success:<pre>@success</pre>', array('@success' => print_r('success', TRUE)));
		commerce_checkout_complete($order);
  	echo 'success';
	  die();
  } else{
  	 echo 'fail'; 
	   die();
  }	    
}




function commerce_alipay_status_message1($status) {
  switch ($status) {
    // Messages used for refund types of cases.
    case 'WAIT_SELLER_AGREE':
      return t('The refund request is waiting for seller to agree.');

    case 'REFUND_SUCCESS':
      return t('Seller agrees on the refund request.');

    // Messages used for trade types of cases.
    case 'TRADE_CLOSED':
      return t('The payment has failed. This happens only if the payment was made from your customer’s bank account.');

    case 'WAIT_SELLER_SEND_GOODS':
      return t('The payment is waiting for the seller to send goods.');

    case 'WAIT_BUYER_CONFIRM_GOODS':
      return t('Seller has already sent the goods, pending buyer to confirm.');

    case 'WAIT_BUYER_PAY':
      return t('The payment is waiting for the buyer to pay.');

    case 'TRADE_FINISHED':
      return t('The payment has completed.');

    case 'TRADE_SUCCESS':
      return t('The payment has completed.');
  }
}


