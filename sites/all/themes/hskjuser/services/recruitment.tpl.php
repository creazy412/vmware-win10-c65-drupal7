<!DOCTYPE html>
<html> 
<head> 
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8">
    <title></title>
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width,initial-scale=1.0,maximum-scale=1.0,user-scalable=0" />
    <meta name="apple-mobile-web-app-capable" content="yes">
    <meta name="apple-mobile-web-app-status-bar-style" content="black">
    <meta name="format-detection" content="telephone=no">
	<style type="text/css" media="all">@import url("http://app.rruby.cn/sites/all/modules/ckeditor/css/ckeditor.css?n9h58l");</style>
	<style type="text/css" media="all">@import url("http://app.rruby.cn/sites/all/modules/ckeditor/css/ckeditor.editor.css?n9h58l");</style>
	<style type="text/css" media="all">@import url("http://app.rruby.cn/sites/all/modules/ckeditor_link/ckeditor_link.css?n9h58l");</style>

 <style>
		body{font-family:"Microsoft YaHei",微软雅黑,"Microsoft JhengHei","Arial","sans-serif";font-size:16px;background-color:#fff;color:#3e3e3e;}
		.rich_media{width:100%;margin-left:auto;margin-right:auto}
		.rich_media_inner{padding-left:10px;padding-right:10px;background-color:#fff;border-left:1px solid #d9dadc;border-right:1px solid #d9dadc;border-top-width:0}		
</style>
</head> 
<body style="padding:0px;margin:0px;">


<div class="rich_media">
    <div class="rich_media_inner" style="padding-bottom:10px;padding-top:15px;">
	
      <span style="font-size:18px;"><strong><?php print ($node->title) ?> </strong></span>
	        
	  	   <div class="rich_media_meta_list" style="">
                <img height="12px" width="12px" src="http://app.rruby.cn/sites/default/files/icon6.png" border="0"></img>&nbsp;<em id="post-date" style="color:#8c8c8c;font-size:12px;">2014-08-07</em>
            </div>
    </div>

			
			
</div>


<?php

   //薪酬范围
		if(is_array($node->field_salary)){ 
	    foreach($node->field_salary as $key=>$var){
          if(is_array($var)){
	         foreach($var as $key1=>$var1){
					foreach($var1 as $k=>$v){
				    if($k=="value")
					$salary = $v;
					}
			 }
		  }
	    }
      }
	  //详情
	  if(is_array($node->body)){ 
	    foreach($node->body as $key=>$var){
          if(is_array($var)){
	         foreach($var as $key1=>$var1){
					foreach($var1 as $k=>$v){
				    if($k=="value")
					$content = $v; 
					}
			 }
		  }
	    }
     }
	   
	   
	     //小区ID
	  if(is_array($node->field_gcc_audience)){ 
	    foreach($node->field_gcc_audience as $key=>$var){
          if(is_array($var)){
	         foreach($var as $key1=>$var1){
					foreach($var1 as $k=>$v){
				    if($k=="entity_id")
					$entity_id = $v; 
					}
			 }
		  }
	    }
     }
	 
	 
	  //联系人field_contacts
	   if(is_array($node->field_contacts)){ 
	    foreach($node->field_contacts as $key=>$var){
          if(is_array($var)){
	         foreach($var as $key1=>$var1){
					foreach($var1 as $k=>$v){
				    if($k=="value")
					$contacts = $v; 
					}
			 }
		  }
	    }
      }
	  
	  //联系电话
	     if(is_array($node->field_phone)){ 
	    foreach($node->field_phone as $key=>$var){
          if(is_array($var)){
	         foreach($var as $key1=>$var1){
					foreach($var1 as $k=>$v){
				    if($k=="value")
					$phone = $v; 
					}
			 }
		  }
	    }
      }
	  
	  
	 
?>

 <div style="border:1px solid #e7e7eb;margin-top:10px;"></div>
 
 <div class="rich_media">
    <div class="rich_media_inner">
	  <div style="font-size:16px;margin-bottom:10px;margin-top:10px;">
	 <span style="font-size:17px">  <strong>薪酬范围：</strong></span>fght554<?php print $salary; ?>
	 <br/>
	 <span style="font-size:17px">  <strong>公司名称：</strong></span>fght554<?php print $entity_id; ?>
	 </div>
    </div>
 </div> 	

 
 
 <div style="border:1px solid #e7e7eb;margin-top:10px;"></div>
 
 <div class="rich_media">
    <div class="rich_media_inner">
	  <div style="font-size:16px;margin-bottom:10px;margin-top:10px;">
	 <span style="font-size:17px">  <strong>招聘详情：</strong></span><br/><?php print $content; ?>
	 </div>
    </div>
 </div> 	
 
  <div style="border:1px solid #e7e7eb;margin-top:10px;"></div>
 
 <div class="rich_media">
    <div class="rich_media_inner">
	  <div style="font-size:16px;margin-bottom:10px;margin-top:10px;">
	 <span style="font-size:17px"><strong>其他岗位：</strong></span>

	 </div>
    </div>
 </div> 
 
<div style="border:1px solid #e7e7eb"></div>

<div class="rich_media">
    <div class="rich_media_inner">
      <div style="font-size:17px;padding-bottom:70px;padding-top:10px"><strong><span style="color:#cc0000;">
	 
	
	  <table width="100%" style="color:#3e3e3e;">
	    <tr>
	    <td width="33%" align="center">hdfgh</td> <td width="33%" align="center">rtgyh</td>  <td width="33%" align="center">rtgyh</td> 
	  </tr>
	  

	  </table>
	   
	  </div>
</div>  
</div>  
 
  
 <div style="width:100%;border-top:2px solid #ff0000;height:62px;position:fixed;_position:absolute;bottom:0px;;_margin-top:expression(this.style.pixelHeight+document.documentElement.scrollTop)">
 </div>	 
 <div style="background:#ffffff;height:61px;width:100%;position:fixed;_position:absolute;bottom:0px;_margin-top:expression(this.style.pixelHeight+document.documentElement.scrollTop)">
 <div style="padding:10px;">
 <table border="0" width="100%" style="line-height:18px;">
    <tr><td><?php print $contacts; ?></td><td rowspan="2" align="right" width="80">我要应聘</td><td width="40" rowspan="2" align="right"><a href="tel:<?php print $phone; ?>" mce_href="tel:<?php print $phone; ?>"><img src="http://app.rruby.cn/sites/default/files/icon19.png" border="0" width="38" height="38"/></a></td></tr>
	<tr><td align="left"><?php print $phone; ?></td></tr>
	
  
 </table>

 </div>

</div> 
      
</body>


</html>

