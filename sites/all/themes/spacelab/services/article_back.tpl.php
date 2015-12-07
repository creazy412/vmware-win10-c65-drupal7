<!DOCTYPE html>
<html> 
<head> 
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8">
    <title><?php if ($node->title) ?></title>
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width,initial-scale=1.0,maximum-scale=1.0,user-scalable=0" />
    <meta name="apple-mobile-web-app-capable" content="yes">
    <meta name="apple-mobile-web-app-status-bar-style" content="black">
    <meta name="format-detection" content="telephone=no">
	<style type="text/css" media="all">@import url("http://app.rruby.cn/sites/all/modules/ckeditor/css/ckeditor.css?n9h58l");</style>
	<style type="text/css" media="all">@import url("http://app.rruby.cn/sites/all/modules/ckeditor/css/ckeditor.editor.css?n9h58l");</style>
	<style type="text/css" media="all">@import url("http://app.rruby.cn/sites/all/modules/ckeditor_link/ckeditor_link.css?n9h58l");</style>
        <style>
		html{-ms-text-size-adjust:100%;-webkit-text-size-adjust:100%}
		body{line-height:1.6em;font-family:"Microsoft YaHei",微软雅黑,"Microsoft JhengHei","Arial","sans-serif";font-size:16px}
		body,h1,h2,h3,h4,h5,p,ul,ol,dl,dd,fieldset,textarea{margin:0}
		ul,ol{padding-left:0;list-style-type:none;list-style-position:inside}
		a img,fieldset{border:0}
		a{text-decoration:none}
		a{color:#607fa6}
		.rich_media_inner{padding:15px}
		.rich_media_title{line-height:24px;font-weight:700;font-size:20px;word-wrap:break-word;-webkit-hyphens:auto;-ms-hyphens:auto;hyphens:auto}
		.rich_media_title .rich_media_meta{vertical-align:middle;position:relative}
		.rich_media_meta{display:inline-block;vertical-align:middle;font-weight:400;font-style:normal;margin-right:.5em;font-size:12px;width:auto;overflow:hidden;text-overflow:ellipsis;white-space:nowrap;word-wrap:normal;max-width:none}
		.rich_media_meta.text{color:#8c8c8c}
		.rich_media_content{margin-top:12px;color:#3e3e3e;word-wrap:break-word;-webkit-hyphens:auto;-ms-hyphens:auto;hyphens:auto}
		.rich_media_content p{*zoom:1;min-height:1em;word-wrap:normal;white-space:pre-wrap;margin-top:1em;margin-bottom:1em}
		.rich_media_content p:after{content:"\200B";display:block;height:0;clear:both}
		.rich_media_content *{max-width:100%!important;word-wrap:break-word!important;-webkit-box-sizing:border-box!important;box-sizing:border-box!important}.rich_media_tool{*zoom:1;padding:18px 0;font-size:14px}
		@media screen and (min-width:1023px){
		.rich_media{width:740px;margin-left:auto;margin-right:auto}
		.rich_media_inner{padding:20px;background-color:#fff;border:1px solid #d9dadc;border-top-width:0}
		.rich_media_meta{max-width:none}
		.rich_media_content{min-height:350px}
		/*.rich_media_title{padding-bottom:10px;margin-bottom:5px;border-bottom:1px solid #e7e7eb}*/
		.rich_media_meta_list{padding-bottom:10px;margin-bottom:5px;border-bottom:1px solid #e7e7eb}
		}
		body{background-color:#f8f7f5;-webkit-touch-callout:none}
		h1,h2,h3,h4,h5,h6{font-weight:400;font-style:normal;font-size:100%}
		.rich_media_meta.nickname{max-width:10em}
		.rich_media_content{font-size:16px}
		.rich_media_content p{margin-top:0;margin-bottom:0}
		img{vertical-align:middle;}
		.rich_media_content p{margin-top: 0px; margin-bottom: 0px; padding: 0px; line-height: 1.6em; width: 700px; min-height: 1.5em; word-wrap: break-word;}  
		</style>
</head> 
<body id="activity-detail">
        <div class="rich_media">
        <div class="rich_media_inner">
            <h2 class="rich_media_title" id="activity-name">
                <?php if ($node->title) ?> 
            </h2>
            <div id="page-content">
                <div id="img-content">                
                     <div class="rich_media_content" id="js_content">
		
<?php 
$about_content="";
if(is_array($node->body)){ 
	    foreach($node->body as $key=>$var){
          if(is_array($var)){
	         foreach($var as $key1=>$var1){
					foreach($var1 as $k=>$v){
				    if($k=="value")
					$about_content = $v; 
					}
			 }
		  }
	    }
}
?>							
				
	                <?php print $about_content; ?>
										
				 </div> 
               </div>
           </div>
      </div>
      </div>                
</body>
</html>

