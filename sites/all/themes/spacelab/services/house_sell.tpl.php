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
	<script type="text/javascript" src="http://app.rruby.cn/sites/all/themes/zircon/js/jquery-1.7.1.min.js"></script>
    <script type="text/javascript" src="http://app.rruby.cn/sites/all/themes/zircon/js/fullscript.js"></script>
    <script type="text/javascript" src="http://app.rruby.cn/sites/all/themes/zircon/js/jquery.touchSwipe.min.js"></script>
    <script type="text/javascript" src="http://app.rruby.cn/sites/all/themes/zircon/js/jquery.versatileTouchSlider.min.js"></script>
    <script type="text/javascript" src="http://app.rruby.cn/sites/all/themes/zircon/js/jquery.easing.1.3.js"></script>
    <link href="http://app.rruby.cn/sites/all/themes/zircon/css/sti_style.css" rel="stylesheet" type="text/css"></link>

 <style>
		body{font-family:"Microsoft YaHei",微软雅黑,"Microsoft JhengHei","Arial","sans-serif";font-size:16px;background-color:#fff;color:#3e3e3e;}
		.rich_media{width:100%;margin-left:auto;margin-right:auto}
		.rich_media_inner{padding-left:10px;padding-right:10px;background-color:#fff;border-left:1px solid #d9dadc;border-right:1px solid #d9dadc;border-top-width:0}		
</style>
</head> 
<body style="padding:0px;margin:0px;">

 <div class="rich_media">
    <div class="rich_media_inner">
	   <div class="sti_container" id="banner" style="margin-top:10px;">
                        <!-- slider -->
                        <div class="sti_slider" style="background-color:#F6F6F6;">
                            <div class="sti_items">
<?php
       $arr =array();
	   if(is_array($node->field_picture)){ 
	    foreach($node->field_picture as $key=>$var){
          if(is_array($var)){
	         foreach($var as $key1=>$var1){
					foreach($var1 as $k=>$v){
				    if($k=="uri")
					  array_push($arr,$v);
					}
			 }
		  }
	    }
       }   
?>


	  <?php foreach ($arr as $column_number => $item): ?>
       	 <?php if (!empty($item)):?>
                       <div class="sti_slide">
                         <img class="image" src="http://app.rruby.cn/sites/default/files/<?php print str_replace("public://","",$item); ?>" alt="">
                      </div>
       	 <?php endif;?>
      <?php endforeach; ?>
      	
       <?php if (empty($arr)):?>   
              <div class="sti_slide">
                     <img title="" alt="" src="http://app.rruby.cn/sites/default/files/styles/galleryformatter_slide/public/default_images/610x450.png">
              </div>	 
       <?php endif;?> 
								 
                            </div>
                            <!-- sti_items -->
                        </div>
                        <!-- sti_slider -->
                        <!-- 控制区 -->
                        </br>
                        </br>
                        <div class="sti_paginate pagenate01">
                            <div class="sti_page">
                            </div>
                        </div>
                    </div><!--sti_container--> 
					
					
	</div>
 </div>
<div class="rich_media">
    <div class="rich_media_inner" style="padding-bottom:10px;">
      <span style="font-size:18px;color:#414141"><strong>   <?php print ($node->title) ?> </strong></span>
    </div>
</div>

<div style="border:1px solid #e7e7eb"></div>

<div class="rich_media">
    <div class="rich_media_inner">
      <div style="font-size:17px;padding-bottom:10px;padding-top:10px"><strong><span style="color:#ff4444;">
	  <?php
     //押金类型
	   if(is_array($node->field_deposit)){ 
	    foreach($node->field_deposit as $key=>$var){
		$deposit = get_taxonomy_namy_by_tid($var[0]["tid"]);
	    }
      }
	  
	  if(is_array($node->field_prices)){ 
	    foreach($node->field_prices as $key=>$var){
          if(is_array($var)){
	         foreach($var as $key1=>$var1){
					foreach($var1 as $k=>$v){
				    if($k=="value")
					$price = $v; 
					}
			 }
		  }
	    }
       }
	   
	 ?>
	 <?php print $price; ?>万元 </span><!--（<?php print $deposit; ?>）-->
	 </strong></div>
	  
	  <div>
<?php

   //出租方式
		if(is_array($node->field_rent)){ 
	    foreach($node->field_rent as $key=>$var){
          if(is_array($var)){
	         foreach($var as $key1=>$var1){
					foreach($var1 as $k=>$v){
				    if($k=="tid")
					$rent = $v;
					}
			 }
		  }
	    }
      }
	  		
        //户型
		if(is_array($node->field_apart)){ 
	    foreach($node->field_apart as $key=>$var){
          if(is_array($var)){
	         foreach($var as $key1=>$var1){
					foreach($var1 as $k=>$v){
				    if($k=="tid")
					$apart = get_taxonomy_namy_by_tid($v);
					//$v; 
					}
			 }
		  }
	    }
      }
	  //面积
	  if(is_array($node->field_area)){ 
	    foreach($node->field_area as $key=>$var){
          if(is_array($var)){
	         foreach($var as $key1=>$var1){
					foreach($var1 as $k=>$v){
				    if($k=="value")
					$area = $v; 
					}
			 }
		  }
	    }
      }
	  //装修情况
	  if(is_array($node->field_decorate)){ 
	    foreach($node->field_decorate as $key=>$var){
          if(is_array($var)){
	         foreach($var as $key1=>$var1){
					foreach($var1 as $k=>$v){
				    if($k=="tid")
					//$decorate = $v; 
					$decorate = get_taxonomy_namy_by_tid($v);
					}
			 }
		  }
	    }
      }
	   //房屋类型
	  if(is_array($node->field_kind)){ 
	    foreach($node->field_kind as $key=>$var){
          if(is_array($var)){
	         foreach($var as $key1=>$var1){
					foreach($var1 as $k=>$v){
				    if($k=="tid")
					
					$kind = get_taxonomy_namy_by_tid($v);
					}
			 }
		  }
	    }
      }
	   //楼层
	   if(is_array($node->field_floor)){ 
	    foreach($node->field_floor as $key=>$var){
          if(is_array($var)){
	         foreach($var as $key1=>$var1){
					foreach($var1 as $k=>$v){
				    if($k=="value")
					$floor = $v; 
					}
			 }
		  }
	    }
      }
	   //总楼层
	   if(is_array($node->field_floors)){ 
	    foreach($node->field_floors as $key=>$var){
          if(is_array($var)){
	         foreach($var as $key1=>$var1){
					foreach($var1 as $k=>$v){
				    if($k=="value")
					$floors = $v; 
					}
			 }
		  }
	    }
      }
	  //房屋朝向
	   if(is_array($node->field_face)){ 
	    foreach($node->field_face as $key=>$var){
          if(is_array($var)){
	         foreach($var as $key1=>$var1){
					foreach($var1 as $k=>$v){
				    if($k=="tid")
					$face = get_taxonomy_namy_by_tid($v);
					}
			 }
		  }
	    }
      }
	//描述
	   if(is_array($node->field_description)){ 
	    foreach($node->field_description as $key=>$var){
          if(is_array($var)){
	         foreach($var as $key1=>$var1){
					foreach($var1 as $k=>$v){
				    if($k=="value")
					$description = $v; 
					}
			 }
		  }
	    }
      }
	 
	  
	    //房屋配置
		$equip="";
	   if(is_array($node->field_h_equipment)){ 
	    foreach($node->field_h_equipment as $key=>$var){
		
          if(is_array($var)){
	         foreach($var as $key1=>$var1){
					foreach($var1 as $k=>$v){
					
					$equip .= get_taxonomy_namy_by_tid($v).'  , ';
				    //if($k=="value")
					//$deposit = $v; 
					}
			 }
		  }
	    }
      }
	
	  $equip=substr($equip,0,-2);
	  
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
	     if(is_array($node->field_sjhm)){ 
	    foreach($node->field_sjhm as $key=>$var){
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
	  //https://api.drupal.org/api/drupal/modules%21taxonomy%21taxonomy.module/function/taxonomy_term_load/7
	 function get_taxonomy_namy_by_tid($tid){
	 	 $taxonomy_term = taxonomy_term_load($tid);
	     if(is_object($taxonomy_term)) {       
		  // $key = $taxonomy_term->tid;
           return $taxonomy_term->name;
        }
	 }
	 
      //地址
	  $address="";
	  if(is_array($node->field_address)){ 
	    foreach($node->field_address as $key=>$var){
          if(is_array($var)){
		  //var_dump($var);
		 $address= $var[0]["administrative_area"].$var[0]["locality"].$var[0]["thoroughfare"].$var[0]["premise"];
	
		  }
	    }
      }		
?>
	
	  <table width="100%" style="line-height:1.6em;font-size:18px;">
	    <tr>
	    <td width="50%" align="left">户型：<?php print $apart; ?></td>  <td width="50%"  align="left">面积：<?php print $area; ?>m²</td> 
	  </tr>
	  
	  <tr>
	    <td width="50%" align="left">类型：<?php print $kind; ?></td> <td width="50%"  align="left">装修：<?php print $decorate; ?></td>
	  </tr> 
	  
	  
	    <tr>
	    <td width="50%" align="left">楼层：<?php print $floor; ?>/<?php print $floors; ?></td> <td width="50%"  align="left">朝向：<?php print $face; ?></td>
	  </tr> 
<!--	  <tr>
	  <td width="100%" align="left" colspan="2">房置：<?php print $equip; ?></td>
	  </tr>-->
	  </table>
	   
	  </div>
</div>  
</div>  



<div style="border:1px solid #e7e7eb;margin-top:10px;margin-bottom:10px;"></div>

<div class="rich_media">
    <div class="rich_media_inner">
			   <div style="font-size:18px;line-height:1.6em;">
			 <span style="font-size:18px">  小区地址：</span><p><?php print $address ?></p>
			   </div>
		<!--	   <div style="height:200px;width:100%;border:1px solid #e7e7eb;text-align: center;">地图</div>-->
			   
</div>
 </div> 
 
 
 <div style="border:1px solid #e7e7eb;margin-top:10px;"></div>
 
 <div class="rich_media">
    <div class="rich_media_inner">
	  <div style="font-size:18px;margin-bottom:70px;margin-top:10px;line-height:1.6em;">
	 <span style="font-size:18px">房屋描述：</span><?php print $description; ?>
	 </div>
    </div>
 </div> 	 
 <div style="width:100%;border-top:2px solid #ff0000;height:62px;position:fixed;_position:absolute;bottom:0px;;_margin-top:expression(this.style.pixelHeight+document.documentElement.scrollTop)">
 </div>	 
 <div style="background:#ffffff;filter:alpha(opacity=95);-moz-opacity:0.95;opacity:0.95;height:61px;width:100%;position:fixed;_position:absolute;bottom:0px;_margin-top:expression(this.style.pixelHeight+document.documentElement.scrollTop)">
 <div style="padding:10px;">
 <table border="0" width="100%">
    <tr>
	  <td align="left"><?php print $contacts; ?></td>
	  
	 
	  
	  
        <td rowspan="2" align="right"> <a href="tel:<?php print $phone; ?>" mce_href="tel:<?php print $phone; ?>"><img src="http://app.rruby.cn/sites/default/files/icon19.png" border="0" width="38" height="38"/></a></td>
	</tr>
	<tr>
	  <td align="left"><?php print $phone; ?></td>
	
	</tr>
 </table>

 </div>

</div> 
      
</body>

<script type="text/javascript">
        var myS6;
$(document).ready(function(){
		var dd=$.versatileTouchSlider('#banner', {
				slideWidth: 620, 
				slideHeight: 572, 
				showPreviousNext: false,
				currentSlide: 0,
				scrollSpeed: 600,
				autoSlide: false,
				autoSlideDelay: 3000,
				showPlayPause: true,
				showPagination: true,
				alignPagination: 'center',
				alignMenu: 'left',
				swipeDrag: true,
				sliderType: 'image_banner', 
				pageStyle: 'bullets', 
				ajaxEvent: function() {}
			}
		);
});
</script>

</html>

