<?php

/**
 * @file
 * Contains the resource callbacks for product display nodes.
 */


/**
 * Returns a collection of product display nodes.
 *
 * @param $fields
 *   An array of field and property names to include in the returned product
 *   display node data. If empty, nodes will be returned as loaded with the
 *   exception of rdf_mapping data, which will only ever be included in an index
 *   if it is explicitly requested.
 * @param $expand_entities
 *   An integer depth to which entity reference field values should be expanded
 *   into full entities in a corresponding property on returned entities;
 *   defaults to 1.
 * @param $flatten_fields
 *   A string indicating whether or not field value arrays should be flattened;
 *   accepts true or false, defaults to true.
 * @param $filter
 *   An associative array of property names, single column field names, or
 *   multi-column field column names with their values to use to filter the
 *   result set of the index request.
 * @param $filter_op
 *   An associative array of field and property names with the operators to use
 *   when applying their filter conditions to the index request query.
 * @param $sort_by
 *   An array of database fields to sort the query by, with sort fields being
 *   valid properties, single column field names, or multi-column field column
 *   names for the matching entity type.
 * @param $sort_order
 *   The corresponding sort orders for the fields specified in the $sort_by
 *   array; one of either 'DESC' or 'ASC'.
 * @param $limit
 *   The number of product displays to retrieve.
 * @param $offset
 *   The zero-based index from which to start retrieving product displays.
 *
 * @return
 *   An array of product display nodes.
 */
 /*
function commerce_services_product_display_index($fields, $expand_entities, $flatten_fields, $filter, $filter_op, $sort_by, $sort_order, $limit, $offset) {

  // Build a query to load all accessible nodes that contain product reference
  // fields, whether those fields have data or not.
  $query = new EntityFieldQuery();
  $query
    ->addTag('node_access')
    ->addMetaData('op', 'view')
    ->addMetaData('base_table', 'node')
    ->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', array_keys(commerce_product_reference_node_types()))
    ->range($offset, $limit);

  // Add the specified property and field condtions to the query.
  if (!empty($filter)) {
      foreach ($filter as $filter_field => $filter_value) {
				$operator = '=';  	
		    if (!empty($filter_op[$filter_field])) {
		      $operator = $filter_op[$filter_field];
		    }
        commerce_services_product_display_index_query_filter('node', $query, $filter_field, $filter_value, $operator);
      }
    }

  // Add the specified sort conditions to the query.
  commerce_services_index_query_sort($query, 'node', $sort_by, $sort_order);

  // Execute the query and load the nodes it returns if any.
  $result = $query->execute();

  $nodes = array();

  if (!empty($result['node'])) {
  

  
    $nodes = node_load_multiple(array_keys($result['node']));

    foreach ($nodes as $nid => &$node) {
      // Add simplified fields to the entity object for certain field types.
	 
      commerce_services_decorate_entity('node', $node);
     //added by marklee 2014-11-08 filter product invalid status  begin--
   	    commerce_services_product_status_filter($node);
     //--end	  	  
      // Expand referenced entities into the full entity to the specified depth.
      if ($expand_entities > 0) {
        commerce_services_expand_entities('node', $node, $expand_entities, $flatten_fields == 'true');
      }

      // Flatten field value arrays if specified. This must be the last operation
      // performed as it breaks the standard field data model. An entity whose
      // fields have thus been flattened is no longer wrappable.
      if ($flatten_fields == 'true') {
        $node = clone($node);
        commerce_services_flatten_fields('node', $node);
      }

      // Unset the rdf_mapping key from any loaded node unless explicitly included
      // in the $fields array.
      if (!in_array('rdf_mapping', $fields)) {
        unset($node->rdf_mapping);
      }

      // Filter out the node properties based on the field list if given; always
      // remove rdf_mapping unless explicitly requested in the field list.
      if (!empty($fields)) {
        commerce_services_object_filter($node, $fields);
      }
    }

  }

  return $nodes;
}*/








function commerce_services_product_display_index($fields, $expand_entities, $flatten_fields, $filter, $filter_op, $sort_by, $sort_order, $limit, $offset) {

$communityid='';
if(array_key_exists('communityid',$filter)){
	$communityid=$filter['communityid'];
	unset($filter['communityid']);
}
  // Build a query to load all accessible nodes that contain product reference
  // fields, whether those fields have data or not.
  $query = new EntityFieldQuery();
  $query
    ->addTag('node_access')
    ->addMetaData('op', 'view')
    ->addMetaData('base_table', 'node')
    ->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', array_keys(commerce_product_reference_node_types()))
    ->range($offset, $limit);

  // Add the specified property and field condtions to the query.
  if (!empty($filter)) {
      foreach ($filter as $filter_field => $filter_value) {
				$operator = '=';  	
		    if (!empty($filter_op[$filter_field])) {
		      $operator = $filter_op[$filter_field];
		    }
        commerce_services_product_display_index_query_filter('node', $query, $filter_field, $filter_value, $operator);
      }
    }

  // Add the specified sort conditions to the query.
  commerce_services_index_query_sort($query, 'node', $sort_by, $sort_order);

  // Execute the query and load the nodes it returns if any.
  $result = $query->execute();
 
  $nodes = array();

  if (!empty($result['node'])) {
    $nodes = node_load_multiple(array_keys($result['node']));
    $arr_nodes=array();
    foreach ($nodes as $nid => &$node) {
      // Add simplified fields to the entity object for certain field types.
    commerce_services_decorate_entity('node', $node);
  
  //begin-- added by marklee 2014 12 16 过滤配送区域
    commerce_services_product_status_filter($communityid,$node);
  //--end

  
  
  
  
      // Expand referenced entities into the full entity to the specified depth.
      if ($expand_entities > 0) {
        commerce_services_expand_entities('node', $node, $expand_entities, $flatten_fields == 'true');
      }

      // Flatten field value arrays if specified. This must be the last operation
      // performed as it breaks the standard field data model. An entity whose
      // fields have thus been flattened is no longer wrappable.
      if ($flatten_fields == 'true') {
        $node = clone($node);
        commerce_services_flatten_fields('node', $node);
      }

      // Unset the rdf_mapping key from any loaded node unless explicitly included
      // in the $fields array.
      if (!in_array('rdf_mapping', $fields)) {
        unset($node->rdf_mapping);
      }

      // Filter out the node properties based on the field list if given; always
      // remove rdf_mapping unless explicitly requested in the field list.
      if (!empty($fields)) {
        commerce_services_object_filter($node, $fields);
      }
	    //begin added by marklee 2014-12-16过滤掉为空的product对应的NODE
	    if(empty($node->field_product)){
		unset($nodes[$node->nid]);
	    }
		//-end
		
	 
	 //--end
	 
	  //begin-- added by marklee 2015-01-13增加商店字段
	
	$arrs=array();
	foreach($node->field_product_entities as $key =>$val){	
     $query2 = db_select('eck_commerce_store', 'a');
     $query2->addTag('node_access');
     $query2->condition('a.id', $val->cmp_store);
     $query2->addField('a', 'title');
     $commerce_store_title = $query2->execute()->fetchCol();
	 $arrs[$val->product_id]=$commerce_store_title[0];
	  }
	 $node->cmp_store_title=$arrs;
	 //--end
	 
	 
	  
    }
 
	
  }
     return $nodes;	

}









/**
 * Helper function for adding a property to an EntityFieldQuery.
 *
 * This takes care of distinguishing between fields and entity properties when
 * adding a condition or ordering to an EntityFieldQuery. It executes the
 * right EntityFieldQuery method to add the property to the query.
 *
 * @param string $entity_type
 *   The entity type for the query.
 * @param $query
 *   The EntityFieldQuery object.
 * @param string $property
 *   The name of the raw property or field which is to be added to the query.
 * @param string|array $value
 *   The value for the function.
 * @param string $operator
 *   An operator to use  when applying their filter conditions to the index request query.
 */
function commerce_services_product_display_index_query_filter($entity_type, $query, $property, $value, $operator) {

  // First pass: check the entity's table schema.
  // Get the database schema for the entity's table.
  $entity_info = entity_get_info($entity_type);
  $schema = drupal_get_schema($entity_info['base table']);
  if (isset($schema['fields'][$property])) {
    // If the property is defined in the schema, use the schema property.
    // The EFQ method is either 'propertyCondition' or 'OrderByCondition'.
    $query->propertyCondition($property, $value, $operator);
    return;
  }

  // Second pass: check fields.
  // Get the metadata property info for the entity type, including properties
  // for all bundles.
  $properties = entity_get_all_property_info($entity_type);
  if (isset($properties[$property]) && !empty($properties[$property]['field'])) {
    // For fields we need the field info to get the right column for the
    // query.
    $field_info = field_info_field($property);
    if (is_array($value)) {
      // Specific column filters are given, so add a query condition for each
      // one of them.
      foreach ($value as $column => $val) {
        $query->fieldCondition($field_info, $column, $val, $operator);
      }
    }
    else {
      // Just pick the first field column for the operation.
      $columns = array_keys($field_info['columns']);
      $column = $columns[0];
      $query->fieldCondition($field_info, $column, $value, $operator);
    }

    return;
  }

  // Still here if no matching property was found.
  services_error(t('Parameter @prop does not exist', array('@prop' => $property)), 406);
}

/**
 * Determines access to perform product display operations.
 *
 * @param $op
 *   The operation being performed. One of 'view', 'update', 'create' or
 *   'delete'.
 * @param $args
 *   Resource arguments passed through from the original request.
 *
 * @return
 *   Boolean indicating whether or not the requested operation should be allowed.
 */
function commerce_services_product_display_access($op, $args) {
  // First load the node ID from the arguments if available.
  if ($op != 'create' && !empty($args[0])) {
    $node = node_load($args[0]);

    if (empty($node)) {
      return services_error(t('Product display not found'), 404);
    }
  }
  else {
    $node = NULL;
  }

  // If the request operation is a node create...
  if ($op == 'create') {
    // And the POSTed node object has a valid product display node type...
    if (!empty($args[0]->type) && in_array($args[0]->type, commerce_product_reference_node_types())) {
      // Return the user's access to create nodes of that type.
      if (!node_access($op, $args[0]->type)) {
        return services_error(t('Access to this operation not granted'), 401);
      }
      else {
        return TRUE;
      }
    }
    else {
      // Otherwise throw an error with an appropriate error message.
      return services_error(t('Valid product display node type required'), 400);
    }
  }

  // If the node could be loaded, return the node_access() value for the operation.
  if (!empty($node->nid)) {
    if (!node_access($op, $node)) {
      return services_error(t('Access to this operation not granted'), 401);
    }
    else {
      return TRUE;
    }
  }
  else {
    return services_error(t('Product display not found'), 404);
  }
}

function commerce_services_product_display_lottery_access($op, $args) {
 /* if ($op != 'create' && !empty($args[0])) {
  // $uid=explode("-",$args[0]);
   $account = user_load($args[0]);
    if (empty($account)) {
      return services_error(t('Product display not found'), 404);
    }
  }
  else {
     return TRUE;
  }*/
 return TRUE;
}
/**
 * Returns a single product display node.
 *
 * @param $nid
 *   The node ID of the product display to retrieve.
 * @param $expand_entities
 *   An integer depth to which entity reference field values should be expanded
 *   into full entities in a corresponding property on returned entities;
 *   defaults to 1.
 * @param $flatten_fields
 *   A string indicating whether or not field value arrays should be flattened;
 *   accepts true or false, defaults to true.
 *
 * @return
 *   The requested product display node or FALSE if not found.
 */
function commerce_services_product_display_retrieve($nid, $expand_entities, $flatten_fields) {
  $node = node_load($nid);

  // If the node exists, determine its URI and add it to the node.
  if ($node) {
    $uri = entity_uri('node', $node);
    $node->path = url($uri['path'], array('absolute' => TRUE));

    // Unset the uri property if it's on the node to prevent recursion.
    unset($node->uri);

    // Add simplified fields to the entity object for certain field types.
    commerce_services_decorate_entity('node', $node);

    // Expand referenced entities into the full entity to the specified depth.
    if ($expand_entities > 0) {
      commerce_services_expand_entities('node', $node, $expand_entities, $flatten_fields == 'true');
    }

    // Flatten field value arrays if specified. This must be the last operation
    // performed as it breaks the standard field data model. An entity whose
    // fields have thus been flattened is no longer wrappable.
    if ($flatten_fields == 'true') {
      $node = clone($node);
      commerce_services_flatten_fields('node', $node);
    }
  }

  return $node;
}
//add by marklee 2014-10-24   begin 
//抽奖商品API，通过用户ID，随机数，小区ID，请求是否第一次来取商品
function commerce_services_product_display_lottery($uid_rnum, $expand_entities, $flatten_fields){

 $uid_rnum=base64_decode($uid_rnum);
 $uid_rnum=explode("-",$uid_rnum);
  if($uid_rnum[0]==''){
  	return -1001;
  }
   if($uid_rnum[1]==''){
  	return -1002;
  }
   if($uid_rnum[2]==''){
  	return -1003;
  }
  $account = user_load($uid_rnum[0]);
  $name = $account->name;
  if(empty($account)){
   	return -1004;
  } 
  $node1=_node_resource_retrieve($uid_rnum[2]);

  //如果没有活动返回0
   if(!$node1||$node1->status==0)	{
   	return 0;
   }
	
  $promotion_title=$node1->title;
   //取可以抽奖的次数
  $node2=$node1->field_limits;
  $promotion_limits= 3;//$node2["und"][0]['value'];
 
    //是否针对新用户
  $newuser=$node1->field_is_newuser;
  $promotion_is_newuser=$newuser["und"][0]['value'];


  $registerdate=date('Y-m-d',$account->created);  
  $today=date("Y-m-d");

  if(trim($uid_rnum[3])==1||trim($uid_rnum[3])=='1'){
       $today_count=0;
	  //  $parameters=array("uid"=>$uid_rnum[0],"status"=>1);
	     $parameters=array("uid"=>$uid_rnum[0],"status"=>1,"type"=>"promotion_users");
	    
		$result = _node_resource_index(0,'nid,changed',$parameters,200);

		foreach($result as $key=>$var){
		 $today_result=date('Y-m-d',$var->changed); 
		 if($today_result==$today){
		 	$today_count++;
			
		 }		  
		}
         
	   $counta= $promotion_limits-$today_count;
     
		if($counta<0){
		 $counta=0;
		}
	    
         $arr=array("lorrery_count"=>$counta,"lorrery_product"=>"");
         return $arr;
  }else{
  	
	    $today_count=0;
	    $parameters=array("uid"=>$uid_rnum[0],"status"=>1,"type"=>"promotion_users");
		$result = _node_resource_index(0,'nid,changed',$parameters,20);
		
		foreach($result as $key=>$var){
		 $today_result=date('Y-m-d',$var->changed); 
		
		 if($today_result==$today){
		 	$today_count++;
		 }
		  
		}
		
	 	$countb=$promotion_limits-$today_count;
	
	    if($countb<=0)
	      {
		    $countb=0;
	  	    $arr=array("lorrery_count"=>$countb,"lorrery_product"=>"");
            return $arr;
	      }
	        $title=$promotion_title.'-'.$name;
	  	  //  $node1=array("status"=>1,"type"=>"promotion_users","title"=>$title,"uid"=>$uid_rnum[0]);
			
	      //  _node_resource_create($node1);
			
		   $e = entity_create('node', array('type' => 'promotion_users'));
          // Specify the author
           $e->uid = $uid_rnum[0];
           // Create a Entity Wrapper of that new Entity
           $entity = entity_metadata_wrapper('node',$e);
           $entity->title = $title;
           $entity->status = 1;
           $entity->save();
			
			
			
	        $countb=$countb-1;
		
			$proArr =array('1'=>1,'2'=>5,'3'=>24,'4'=>70);
			if($uid_rnum[1]>0&&$uid_rnum[1]<=1){
			 $numb = get_rand($proArr);
			 if($numb==1){
			 	$rnum=1;
			 }else{
			 	$rnum=0;
			 }
	          
            }
            elseif($uid_rnum[1]>1&&$uid_rnum[1]<=5){
			
			 $numb = get_rand($proArr);
	          if($numb==2){
			 	$rnum=2;
			 }else{
			 	$rnum=0;
			 }
            }
            elseif($uid_rnum[1]>5&&$uid_rnum[1]<=30){
			 $numb = get_rand($proArr);
	           if($numb==3){
			 	$rnum=3;
			 }else{
			 	$rnum=0;
			 }
            }
            else{	    
	          $arr=array("lorrery_count"=>$countb,"lorrery_product"=>"");
              return $arr;
            }
			$newuser_only=0;
	        if($registerdate==$today){
				$newuser_only=1;
			} 
		   $filter=array("field_community_limit"=>$uid_rnum[2],"field_newuser_only"=>$newuser_only,"field_lottery"=>$rnum,"status"=>1);
	       $node =  commerce_services_product_display_index('',1,TRUE,$filter,'=','created','DESC',20,0);
		  
		   foreach($node as $key=>$val){
		     $aar= $val->field_product_entities;
	         foreach($aar as $key1=>$var1){
			  if($var1->commerce_stock=='0.00'||$var1->commerce_stock=='0'||$var1->commerce_stock==''){
			  
				unset($node[$key]);
			  }
			 }
	       }	   
	       $randkey=array_rand($node,1);
	       $node3 = $node[$randkey];			   
	       if($node3){
	           $arr=array("lorrery_count"=>$countb,"lorrery_product"=>$node3);
                return $arr; 
	       }else{
		   	 $arr=array("lorrery_count"=>$countb,"lorrery_product"=>"");
              return $arr;
		   }
          
	
  }


}

function get_rand($proArr) { 
    $result = ''; 
    //概率数组的总概率精度 
    $proSum = array_sum($proArr); 
    //概率数组循环 
    foreach ($proArr as $key => $proCur) { 
        $randNum = mt_rand(1, $proSum);             //抽取随机数
        if ($randNum <= $proCur) { 
            $result = $key;                         //得出结果
            break; 
        } else { 
            $proSum -= $proCur;                     
        } 
    } 
    unset ($proArr); 
    return $result; 
}


function promotion_lottery($promotion_is_newuser){

	
	
	if($uid_rnum[3]==1){
	$parameters=array("uid"=>$uid_rnum[0],"status"=>1);
	$count1 = count(_node_resource_index(0,'nid',$parameters,100)) ;
	
	if($count1<=0){
		$count1=$promotion_limits;
	}
	else{
		
		$count1=$promotion_limits-$count1;
	}
    $arr=array("lorrery_count"=>$count1,"lorrery_product"=>"");
    return $arr;
    }
	elseif($uid_rnum[3]==2){	
	$node1=_node_resource_retrieve(10017);
	$promotion_title=$node1->title;
	$node2=$node1->field_limits;
	$promotion_limits=$node2["und"][0]['value'];
	$newuser=$node1->field_is_newuser;
	$promotion_is_newuser=$newuser["und"][0]['value'];
	return $promotion_is_newuser;
	//field_is_newuser	 
		 //$parameters=array("type"=>"promotion","status"=>1);
		//return _node_resource_index(0,'nid',$parameters,1);
	//增加摇中的记录
	//$parameters=array("uid"=>51);
	//return count(_node_resource_index(0,'nid',$parameters,100)) ;   
	//$node1=array("status"=>1,"type"=>"promotion_users","title"=>914,"uid"=>914);
	//return _node_resource_create($node1);
	}
	else{
	  $parameters=array("uid"=>$uid_rnum[0],"status"=>1);//$uid_rnum[0]
	  $count1 = count(_node_resource_index(0,'nid',$parameters,100)) ;
	 if($count1>=$promotion_limits)
	  {
	  	return -1006;
	  }else{
	  $title=$promotion_title.'-'.$name;
	  	$node1=array("status"=>1,"type"=>"promotion_users","title"=>$title,"uid"=>$uid_rnum[0]);
	    _node_resource_create($node1);
	  }
	  if($count1<=0){
	  	$count1=$promotion_limits;
	  }else{
	  	  $count1 = $promotion_limits-$count1;
	  }
      if($count1<0){
  	    return -1007;
      }
       if($uid_rnum[1]>0&&$uid_rnum[1]<=1){
	  $rnum=1;
       }
       elseif($uid_rnum[1]>1&&$uid_rnum[1]<=5){
	  $rnum=2;
       }
       elseif($uid_rnum[1]>5&&$uid_rnum[1]<=30){
	  $rnum=3;
       }
       else{	    
	  $arr=array("lorrery_count"=>$count1,"lorrery_product"=>"");
          return $arr;
       }
	  if($promotion_is_newuser==1) {
	  	 $filter=array("field_community_limit"=>$uid_rnum[2],"field_newuser_only"=>"1","field_lottery"=>$uid_rnum[1],"status"=>1);
	  }
	  else{
	  	$filter=array("field_community_limit"=>$uid_rnum[2],"field_lottery"=>$uid_rnum[1],"status"=>1);
	  }
	  $node =  commerce_services_product_display_index('',1,TRUE,$filter,'=','created','DESC',1,0);
	  if(!$node){
	      $arr=array("lorrery_count"=>$count1,"lorrery_product"=>"");
              return $arr;
	  }
         $arr=array("lorrery_count"=>$count1,"lorrery_product"=>$node);
         return $arr; 
    }
	
}

//2014-10-24   end
