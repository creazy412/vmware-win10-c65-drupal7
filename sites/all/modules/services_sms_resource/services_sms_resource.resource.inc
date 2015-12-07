<?php

/**
 * @file
 * Service's resources callbacks.
 */

function random() {
   srand((double)microtime()*1000000); 
   //生成四位随机整数验证码 
   while(($authnum=rand()%10000)<1000);
   return $authnum;
}

/**
 * send mobile a verify code by smsframework
 *
 * @param $mobile
 *  the number of mobile.
 *
 * @return boolean
 *   success return true, error return False, $age, $count.
 */
function services_sms_resource_sendsms($mobile) {
	if (empty($mobile) || (strlen($mobile)!=11)) {
		return services_error(t('empty mobile number or wrong lenth with @mno.', array('@mno' => $mobile)), 404);
	}
	// begin   add by maklee  2014-08-23 
	// $account = user_load_by_name($mobile);
	// if ($account) {
    //  return services_error(t('There is exist user with ID @uid.', array('@uid' => $mobile)), 406);
    //}
   //end
  if (!isset($_SESSION['mobile_verify_code'])) {
    $_SESSION['mobile_verify_code'] = array();
    $_SESSION['mobile_verify_code']['timestamp'] = time();
    $_SESSION['mobile_verify_code']['sendcount'] = 0;
  }   
  $age= time()-$_SESSION['mobile_verify_code']['timestamp'];
  $count = $_SESSION['mobile_verify_code']['sendcount'];
  
  if ($count>0 && $age<120) {
  	return array(False, $age, $count);
  }
  
  if ($count>5) { 
  	if ($age>3600) {
  		//reset the count after one hour
     	$_SESSION['mobile_verify_code']['sendcount'] = 0;
    }
  	return array(False, $age, $count);
  }

  $code = random();

  $_SESSION['mobile_verify_code']['timestamp'] = time();
  $_SESSION['mobile_verify_code']['sendcount'] = $count+1;
  $_SESSION['mobile_verify_code']['code'] = $code;
  $_SESSION['mobile_verify_code']['number'] = $mobile;

  $message = services_sms_resource_confirm_message($code);
  watchdog('services_sms_resource',$message);
	if (sms_send($mobile, $message, '')) {
	    drupal_set_message(t('The message "@message" sent to @number.', array('@message' => $message, '@number' => $mobile)));
	  }
  
  return True;
}
/**
 * Create a confirmation message sent to a user, this contains a code which is
 * used to confirm that the number is actually correct.
 *
 * @param $code
 *   Random code to send to user.
 *
 * @return $text
 *   Created message, ready to send to user.
 */
function services_sms_resource_confirm_message($code) {
  $text_format = variable_get('mobile_registration_confirm_message', '[site:name]confirmation code: @code') ;
  $text = token_replace($text_format, array());
  $text = t($text,  array('@code'=>$code));
  return $text;
}
/**
 *
 * Verified code entered by user.
 *
 * @param $code
 *   An string of code, entered by user
 *
 * @param $mobile
 *   A mobile number of user 
 *
 * @return Boolean
 *   Result of Verification.
 */
function services_sms_resource_verified($code, $mobile) {

  if (isset($_SESSION['mobile_verify_code'])) {
		$verify_code = $_SESSION['mobile_verify_code']['code'];
		$verify_mobile=$_SESSION['mobile_verify_code']['number'];
		
    if ($verify_mobile==$mobile & $verify_code==$code){
    	 unset($_SESSION['mobile_verify_code']);
  	   return True;
    }
  }   
  return False;
}
