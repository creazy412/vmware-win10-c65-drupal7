<?php
	
function sent_push($message,$token)	
{

$deviceToken= "b4a061a84b9d774dbc3d0b965c99423e0dc1fbe941b8d19e217fffee41733ab3"; //没有空格
$body = array("aps" => array("alert" =>'test',"badge" =>2,"sound"=>'default'));  //推送方式，包含内容和声音
$ctx = stream_context_create();
//如果在Windows的服务器上，寻找pem路径会有问题，路径修改成这样的方法：
$pem = dirname(__FILE__) . '/' . 'PC.pem';
//$pem="http://app.rruby.cn/sites/default/files/PC.pem";
stream_context_set_option($ctx,"ssl","local_cert",$pem);
//linux 的服务器直接写pem的路径即可
//stream_context_set_option($ctx,"ssl","local_cert","PC.pem");
$pass = "pushchatios";
stream_context_set_option($ctx, 'ssl', 'passphrase', $pass);
//stream_context_set_option($ctx, 'ssl', 'allow_self_signed', true);
//stream_context_set_option($ctx, 'ssl', 'verify_peer', false);



//此处有两个服务器需要选择，如果是开发测试用，选择第二名sandbox的服务器并使用Dev的pem证书，如果是正是发布，使用Product的pem并选用正式的服务器
//$fp = stream_socket_client("ssl://gateway.push.apple.com:2195", $err, $errstr, 60, STREAM_CLIENT_CONNECT, $ctx);
$fp = stream_socket_client("ssl://gateway.sandbox.push.apple.com:2195", $err, $errstr, 60, STREAM_CLIENT_CONNECT, $ctx);
if (!$fp) {
echo "Failed to connect $err $errstrn";
return;
}
//print "Connection OK\n";
$payload = json_encode($body);
$msg = chr(0) . pack("n",32) . pack("H*", str_replace(' ', '', $deviceToken)) . pack("n",strlen($payload)) . $payload;
echo "sending message :" . $payload ."\n";
fwrite($fp, $msg);
fclose($fp);


}

?>
		
	

