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
		body{line-height:1.6em;font-family:"Microsoft YaHei",微软雅黑,"Microsoft JhengHei","Arial","sans-serif";font-size:17px}
		body,h1,h2,h3,h4,h5,p,ul,ol,dl,dd,fieldset,textarea{margin:0}
		ul,ol{padding-left:0;list-style-type:none;list-style-position:inside}
		a img,fieldset{border:0}
		a{text-decoration:none}
		a{color:#607fa6}
		.rich_media_inner{padding:15px}
		.rich_media_title{line-height:24px;font-weight:700;font-size:20px;color:#414141;font-weight:normal;word-wrap:break-word;-webkit-hyphens:auto;-ms-hyphens:auto;hyphens:auto}
		.rich_media_title .rich_media_meta{vertical-align:middle;position:relative}
		.rich_media_meta{display:inline-block;vertical-align:middle;font-weight:400;font-style:normal;margin-right:.5em;font-size:12px;width:auto;overflow:hidden;text-overflow:ellipsis;white-space:nowrap;word-wrap:normal;max-width:none}
		.rich_media_meta.text{color:#8c8c8c}
		.rich_media_content{margin-top:16px;color:#3e3e3e;word-wrap:break-word;-webkit-hyphens:auto;-ms-hyphens:auto;hyphens:auto}
		.rich_media_content p{*zoom:1;min-height:1em;word-wrap:normal;white-space:pre-wrap;margin-top:1em;margin-bottom:1em}
		.rich_media_content p:after{content:"\200B";display:block;height:0;clear:both}
		.rich_media_content *{max-width:100%!important;word-wrap:break-word!important;-webkit-box-sizing:border-box!important;box-sizing:border-box!important}.rich_media_tool{*zoom:1;padding:18px 0;font-size:14px}
			.rich_media_meta_list{padding-bottom:6px;margin-bottom:5px;border-bottom:1px solid #e7e7eb}
		body{background-color:#fff;-webkit-touch-callout:none}
		h1,h2,h3,h4,h5,h6{font-weight:400;font-style:normal;font-size:100%}
		.rich_media_meta.nickname{max-width:10em}
		.rich_media_content{font-size:17px}
		.rich_media_content p{margin-top:0;margin-bottom:0}
		img{vertical-align:middle;}
		.rich_media_content p{margin-top: 0px; margin-bottom: 0px; padding: 0px; line-height: 1.6em; width: 700px; min-height: 1.5em; word-wrap: break-word;}  
		</style>
</head> 

<body id="activity-detail">
        <div class="rich_media">
        <div class="rich_media_inner">
            <h2 class="rich_media_title" id="activity-name">
                <?php print $node->title; ?> 
            </h2>
            <div class="rich_media_meta_list" style="">
                <img height="12px" width="12px" src="http://app.rruby.cn/sites/default/files/icon6.png" border="0"></img>&nbsp;<em id="post-date" class="rich_media_meta text">
		
				
			<?php 
$bj_time =strtotime($node->field_infor_time['und'][0]['value']);
$bj_time = $bj_time+28800;
$bj_time = date('Y-m-d H:i:s', $bj_time);
?>
				<?php print $bj_time; ?>	
						
				</em>
                <a class="rich_media_meta link nickname" id="post-user"></a>
            </div>
            <div id="page-content">
                <div id="img-content">                
                     <div class="rich_media_content" id="js_content">				
<?php 
$about_content="";
if(is_array($node->field_body)){ 
	    foreach($node->field_body as $key=>$var){
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
if($about_content==''||empty($about_content)){
$about_content="暂无相关内容";
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

