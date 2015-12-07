<?php

/**
 * @file
 * Page callback file for the commerce_unionpay module.
 */

/**
 * Implements hook_permission().
 */
function services_commerce_quick_unionpay_permission() {
  return TRUE;
}

/**
 * Menu callback function to process Unionpay's feedback notifications.
 */
function services_commerce_quick_unionpay_notify() {

  //	watchdog('back POST wang', 'back POST wang:<pre>@POST</pre>', array('@POST' => print_r($_POST, TRUE)));
	
  if (empty($_POST)) {
    return FALSE;
  }
  require_once("lib/upmp_service.php");
  
  $order = commerce_order_load(trim($_POST['order_id']));
	global $user;
	if ($user->uid == 0 && $order) {
		  $user = user_load($order->uid);
  }
  	watchdog('back order', 'back order:<pre>@order</pre>', array('@order' => print_r($order, TRUE)));
    $total=0;
     $total=$order->commerce_order_total['und'][0]['amount'];

  
  //if($total !=''||$total !=0){
	//$total =$total*100;
  //}
watchdog('back total', 'back total:<pre>@total</pre>', array('@total' => print_r($total, TRUE)));
  $req['version']     		= upmp_config::$version; // 版本号
  $req['charset']     		= upmp_config::$charset; // 字符编码
  $req['transType']   		= "01"; // 交易类型
  $req['merId']       		= upmp_config::$mer_id; // 商户代码
  $req['backEndUrl']      	= upmp_config::$mer_back_end_url; // 通知URL
  $req['frontEndUrl']     	= upmp_config::$mer_front_end_url; // 前台通知URL(可选)
  $req['orderDescription']	= "商品购买";// 订单描述(可选)
  $req['orderTime']   		= date("YmdHis"); // 交易开始日期时间yyyyMMddHHmmss

  $req['orderTimeout']   		= ""; // 订单超时时间yyyyMMddHHmmss(可选)
  $req['orderNumber'] 		=date("YmdHiss").'Z'.$order->order_number; //订单号(商户根据自己需要生成订单号)
  $req['orderAmount'] 		= $total; // 订单金额
  $req['orderCurrency'] 		= "156"; // 交易币种(可选)
  $req['reqReserved'] 		= "深圳市红石科技有限公司"; // 请求方保留域(可选，用于透传商户信息)
  // 保留域填充方法
  $merReserved['test']   		= "test";
  $req['merReserved']   		= UpmpService::buildReserved($merReserved); // 商户保留域(可选)
  $resp = array ();
  $validResp = UpmpService::trade($req, $resp);
  
  
   // 商户的业务逻辑
  if ($validResp){
  
	// 服务器应答签名验证成功
    if(""!=$resp['respCode'] && "00"==$resp['respCode']){	
   watchdog('resp success', 'resp success:<pre>@resp</pre>', array('@resp' => print_r($resp, TRUE)));
	  echo  '{"tn":"'.$resp["tn"].'"'.',"ordernumber":"'.$req['orderNumber'].'"'.',"ordertime":"'.$req['orderTime'].'"}';
	  die();
	}
  }else {
	// 服务器应答签名验证失败
    echo  '{"tn":"fail"}';
	die();
  }
}

function services_commerce_quick_unionpay_backendurl() {
  if (empty($_POST)) {
    return FALSE;
  }
//watchdog('trade post', 'trade post:<pre>@post</pre>', array('@post' => print_r($_POST, TRUE)));
  require_once("lib/upmp_service.php");

  if (UpmpService::verifySignature($_POST)){// 服务器签名验证成功
	//请在这里加上商户的业务逻辑程序代码
	//获取通知返回参数，可参考接口文档中通知参数列表(以下仅供参考)
	//$transStatus = $_POST['transStatus'];// 交易状态
	//if (""!=$transStatus && "00"==$transStatus){
	
	  $order_no = $_POST['orderNumber'];
    $arr_str=explode("Z",$order_no);
	  if(empty($arr_str)){    
	     return FALSE;   
	  }
		$order_no1 = $arr_str[1];
		$order = commerce_order_load($order_no1);
		global $user;
		if ($user->uid == 0 && $order) {
				  $user = user_load($order->uid);
		}
	  $payment_method = commerce_payment_method_instance_load("unionpay|commerce_payment_unionpay");
	  commerce_mobile_unionpay_notify_submit1($order, $payment_method, $_POST);
  }
  else{
   echo "fail";
   die();
  }
}

//APP收到银联返回成功后回调后台，向银联

function services_commerce_quick_unionpay_appbackendurl() {
  if (empty($_POST)) {
    return FALSE;
  }
  //sleep(5);
   sleep(3840);
  $currenttime=time();
  $order_no = $_POST['orderNumber'];
  $arr_str=explode("Z",$order_no);
  if(empty($arr_str)){    
     return FALSE;   
  }
	$order_no1 = $arr_str[1];
  $order = commerce_order_load($order_no1);
	global $user;
 	if ($user->uid == 0 && $order) {
		  $user = user_load($order->uid);
	}
  if($order->status=='checkout_complete'||$order->status=='completed'){
		return;
  }else{		
  //checkount_compelete后所有的状态都不可以再更新
  require_once("lib/upmp_service.php");  
  //需要填入的部分
  $req['version']     	= upmp_config::$version; // 版本号
  $req['charset']     	= upmp_config::$charset; // 字符编码
  $req['transType']   	= "01"; // 交易类型
  $req['merId']       	= upmp_config::$mer_id; // 商户代码
  $req['orderTime']   	= $_POST['ordertime']; // 交易开始日期时间yyyyMMddHHmmss或yyyyMMdd
  $req['orderNumber'] 	= $_POST['ordernumber']; // 订单号
  // 保留域填充方法
  $merReserved['test']   	= "test";
  $req['merReserved']   	= UpmpService::buildReserved($merReserved); // 商户保留域(可选)
  watchdog('req success', 'req success:<pre>@req</pre>', array('@req' => print_r($req, TRUE)));
  $resp = array ();
  $validResp = UpmpService::query($req, $resp);
 // 商户的业务逻辑
  if ($validResp){
	// 服务器应答签名验证成功
     if(""!=$resp['transStatus'] && "00"==$resp['transStatus']){
	 
	   $payment_method = commerce_payment_method_instance_load("unionpay|commerce_payment_unionpay");
	   commerce_mobile_unionpay_notify_submit1($order, $payment_method, $resp);
 	    print_r($resp);
		die();
     }
  }
  else {
	print_r($resp);
	die();
  }
}

}



function services_commerce_quick_unionpay_frontendurl() {

  if (empty($_POST)) {
    return FALSE;
  }
}



function commerce_mobile_unionpay_notify_submit1($order, $payment_method, $notify) {
    // Attempt to load prior authorization capture IPN created previously.
  $transactions = commerce_payment_transaction_load_multiple(array(), array('remote_id' => $notify['qn']));	
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
  $transaction->amount = commerce_currency_decimal_to_amount($notify['settleAmount'], 'CNY');
  $transaction->currency_code = 'CNY';
   $transaction->remote_id = $notify["qn"];
   $transaction->data['notify'] = $notify;
  if(""!=$notify['transStatus'] && $notify['transStatus']=='00'){
  	  $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
      $transaction->message = 'The payment has completed.';
	  if($order->status=='checkout_complete'||$order->status=='completed'){
	   	echo 'The bill is finished.';
	  }else{
	  	
		 $flag=commerce_order_status_update($order, 'checkout_complete');
		 if($flag&&commerce_payment_transaction_save($transaction)){
            watchdog('success', 'success:<pre>@success</pre>', array('@success' => print_r('success', TRUE)));
  	        echo 'success';
			die();
         } 
		 else{
  	        echo 'fail'; 
			die();
         }	  
	  }
  }
  else{
  	echo 'fail';
	die();
  }
 }
  