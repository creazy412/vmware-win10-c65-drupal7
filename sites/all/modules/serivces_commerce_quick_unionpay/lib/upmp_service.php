<?php
/**
 * 类名：接口处理核心类
 * 功能：组转报文请求，发送报文，解析应答报文
 */

require_once("upmp_core.php");

if (function_exists("date_default_timezone_set")) {
	date_default_timezone_set(upmp_config::$timezone);
}

class UpmpService {
    
    /**
     * 交易接口处理
     * @param req 请求要素
     * @param resp 应答要素
     * @return 是否成功
     */
    static function trade($req, &$resp) {
    	$nvp = self::buildReq($req);
		 watchdog('trade', 'trade:<pre>@nvp</pre>', array('@nvp' => print_r($nvp, TRUE)));
    	$respString = post(upmp_config::$upmp_trade_url, $nvp);
    	return self::verifyResponse($respString, $resp);
    }
    
	/**
	 * 交易查询处理
	 * @param req 请求要素
	 * @param resp 应答要素
	 * @return 是否成功
	 */
    static function query($req, &$resp) {
    	$nvp = self::buildReq($req);
      watchdog('nvp', 'nvp:<pre>@nvp</pre>', array('@nvp' => print_r($nvp, TRUE)));	
    	$respString = post(upmp_config::$upmp_query_url, $nvp);
    	return self::verifyResponse($respString, $resp);
    }
    
    /**
     * 拼接请求字符串
     * @param req 请求要素
     * @return 请求字符串
     */
    static function buildReq($req) {
	
    	//除去待签名参数数组中的空值和签名参数
    	$filteredReq = paraFilter($req);
    	// 生成签名结果
    	$signature = buildSignature($filteredReq);
    	
    	// 签名结果与签名方式加入请求
    	$filteredReq[upmp_config::SIGNATURE] = $signature;
    	$filteredReq[upmp_config::SIGN_METHOD] = upmp_config::$sign_method;
    	
    	return createLinkstring($filteredReq, false, true);
    }
    
    /**
     * 拼接保留域
     * @param req 请求要素
     * @return 保留域
     */
    static function buildReserved($req) {
    	$prestr = "{".createLinkstring($req, true, true)."}";
    	return $prestr;
    }
    
    /**
     * 应答解析
     * @param respString 应答报文
     * @param resp 应答要素
     * @return 应答是否成功
     */
    static function verifyResponse($respString, &$resp) {
    	if  ($respString != ""){
    		parse_str($respString, $para);
    		watchdog('respString', 'respString:<pre>@respString</pre>', array('@respString' => print_r($respString, TRUE)));	
    		$signIsValid = self::verifySignature($para);
    		
    		$resp = $para;
    		if ($signIsValid) {
    			return true;
    		}else {
    			return false;
    		}
    	}
    	
    	
    }
    
    /**
     * 异步通知消息验证
     * @param para 异步通知消息
     * @return 验证结果
     */
    static function verifySignature($para) {
    	$respSignature = $para[upmp_config::SIGNATURE];
    	// 除去数组中的空值和签名参数
	
    	$filteredReq = paraFilter($para);
	
    	$signature = buildSignature($filteredReq);
    	if ("" != $respSignature && $respSignature==$signature) {
    		return true;
    	}else {
    		return false;
    	}
    }
	
}
?>