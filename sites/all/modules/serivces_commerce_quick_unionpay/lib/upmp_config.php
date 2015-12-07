<?php

/**
 * 类名：配置类
 * 功能：配置类
 * 类属性：公共类
 * */

class upmp_config
{
	
    static $timezone        		= "Asia/Shanghai"; //时区    
    static $version     			= "1.0.0"; // 版本号
    static $charset    		 		= "UTF-8"; // 字符编码
    static $sign_method 			= "MD5"; // 签名方法，目前仅支持MD5    
   // static $mer_id     				= "880000000002530"; // 商户号
   //static $security_key    		= "H6ETds2rlEqJAZgsI4NSM4yGbApeyslg"; // 商户密钥
	//static $upmp_trade_url   	 	= "http://202.101.25.178:8080/gateway/merchant/trade";
    //static $upmp_query_url    	 	= "http://202.101.25.178:8080/gateway/merchant/query"; 
	
	static $mer_id     				= "104440148990142"; // 上线商户号
	static $security_key    		= "2QyRbkPT6JkTUKP0pozcNcXuOyv9dzAy"; // 上线商户密钥
    static $upmp_trade_url   	 	= "https://mgate.unionpay.com/gateway/merchant/trade";
    static $upmp_query_url    	 	= "https://mgate.unionpay.com/gateway/merchant/query"; 
	
   // static $mer_back_end_url     	= "http://app.rruby.cn/commerce_mobile_unionpay/backendurl"; // 后台通知地址
   // static $mer_front_end_url     	= "http://app.rruby.cn/commerce_mobile_unionpay/frontendurl"; // 前台通知地址
	
    static $mer_back_end_url     	= "http://deal.rruby.cn/app/services_commerce_unionpay/backendurl"; // 后台通知地址
    static $mer_front_end_url     	= "http://deal.rruby.cn/app/services_commerce_unionpay/frontendurl"; // 前台通知地址	
	

    const VERIFY_HTTPS_CERT 		= false;    
    const RESPONSE_CODE_SUCCESS 	= "00"; // 成功应答码
	const SIGNATURE 				= "signature"; // 签名
	const SIGN_METHOD 				= "signMethod"; // 签名方法
	const RESPONSE_CODE 			= "respCode"; // 应答码
	const RESPONSE_MSG				= "respMsg"; // 应答信息   
    const QSTRING_SPLIT				= "&"; // &
    const QSTRING_EQUAL 			= "="; // =   
}

?>
