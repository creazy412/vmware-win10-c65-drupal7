<?php

/**
 * Callback function for the index service call.
 */
function _services_search_api_resource_retrieve($index, $keys = '', $filter, $sort, $limit, $offset, $fields) {
  $options = array(
  'parse mode' => 'terms',
  'limit' => $limit,
  'offset' => $offset,
  );

  // Initialize the query object.
  $query = search_api_query($index, $options);
  if (!empty($keys)) {
    $query->keys($keys);
  }
  // Set the filters, transforming the query array in a SearchAPIQueryFilter
  // object.
	global $user;
	if ($user->uid==0 || $user->uid==112) {
		if (!empty($filter)){
			unset($filter);
 	    //watchdog('debug', '<pre>@value</pre>', array('@value' => print_r($filter,  TRUE))); 
		}		
	}		
  if (!empty($filter) && is_array($filter)) {
    $query_filter = new SearchApiQueryFilter();
    _services_search_api_add_filter($filter, $query_filter);
    $query->filter($query_filter);
  }

  // Also set sorts.
  if (!empty($sort) && is_array($sort)) {
    foreach ($sort as $field => $mode) {
      $query->sort($field, $mode);
    }
  }

  $result = $query->execute();
  
  return _services_search_api_process_results($result, $query, $fields);
}

function _services_search_api_process_results($result, $query, $fields) {

  if (empty($result['results']) || !is_array($result['results'])) {
    return FALSE;
  }
	
  $entity_type = $query->getIndex()->item_type;
  $entities = entity_load($entity_type, array_keys($result['results']));
  
  foreach ($entities as $entity_id => $entity) {
    try {		
      list($id, $vid,) = entity_extract_ids($entity_type, $entity);   
      $resource = _services_entity_get_controller($entity_type);
      $return[] = $resource->retrieve($entity_type, $id, $fields, $vid);
    }
    catch (Exception $e) {
      watchdog_exception('services_search_api', $e);
      return;
    }
  }

  return $return;
}





/**
 * Prepare the query filters for the Search API query object.
 */
function _services_search_api_add_filter(array $filter, &$query_filter) {
  // Allow to use simpler queries such as filter[title]=toto and assume a ~and
  // conjuction fot those.
  if (empty($filter['~and']) && empty($filter['~or'])) {
    $filter = array('~and' => $filter);
  }

  foreach ($filter as $key => $value) {
    if (is_array($value) && !empty($value)) {
      $conjunction = 'AND';
      if ($key == '~or') {
        $conjunction = 'OR';
      }
			//print_r($value);print_r($conjunction);
      $process_filter = new SearchApiQueryFilter($conjunction);
      foreach ($value as $k => $v) {
        if ($k <> '~and' && $k <> '~or') {
          $field_name = $k;
/*					if (strpos($v, ',')>0){
						$v=explode(',',$v);
	          if (is_array($v) && !empty($v)) {
						  $process_filter1 = new SearchApiQueryFilter('OR');
						  foreach($v as $km =>$kv) {								
  	            $process_filter1->condition($field_name, $kv);
							}
	          }
						
					}
*/
          if (is_array($v) && !empty($v)) {
            $process_filter->condition($field_name, reset($v), _services_search_api_operator(key($v)));
          }
          else {
            $process_filter->condition($field_name, $v);
          }
        }
        else {
          _services_search_api_add_filter(array($k => $v), $process_filter);
        }
      }

      if (!empty($query_filter) && $query_filter instanceof SearchApiQueryFilter) {
        $query_filter->filter($process_filter);
      }
      else {
        $query_filter = $process_filter;
      }
    }
  }
}

/**
 * Transform the operator into Search API supported ones.
 */
function _services_search_api_operator($operator) {
  $operators = array(
    '~gt' => '>',
    '~lt' => '<',
    '~gte' => '>=',
    '~lte' => '<=',
    '~ne' => '<>',
  );
  return isset($operators[$operator]) ? $operators[$operator] : '=';
}

/**
 * Callback function for the index service call.
 */
 //added by marklee 2015-01-23 搜索地图
function _services_search_api_location_retrieve($index, $keys, $location, $lat, $lon, $radius, $distance, $filter, $sort, $limit, $offset, $fields) {

  $options = array(
    'parse mode' => 'terms',
	//'parse mode' => 'direct',
    'limit' => $limit,
    'offset' => $offset,
  );  
  
  $spatials[] = array(
	    'field' => $location,
	    'lat' => $lat,
	    'lon' => $lon,
	    'radius' => $radius,
	  //  'distance' => $distance
	);
  // Initialize the query object.
  $query = search_api_query($index, $options);
  if (!empty($keys)) {
    $query->keys($keys);
  }
  if(!empty($location)&&!empty($lat)&&!empty($lon)){
  	 $query->setOption('search_api_location', $spatials); 
  }
 
  // Set the filters, transforming the query array in a SearchAPIQueryFilter object.
  $search_falg=$filter['search_falg'];
   	 
  unset($filter['search_falg']);//注销不存在的条件，
      //begin-- added by marklee 2015-02-02 阳光租售一起过滤，如果以后有类似的需求，可考虑写一个方法过滤。	 
  if($search_falg=="house_rent_sell"){	
	  $or = $query->createFilter('OR');
      $or->condition('type','house_rent','=');
      $or->condition('type','house_sell','=');
      $query->filter($or);   	 
  }
  if($filter['type']=='house_rent_sell'){
      unset($filter['type']);//注销不存在的条件，
   	  $or = $query->createFilter('OR');
      $or->condition('type','house_rent','=');
      $or->condition('type','house_sell','=');
      $query->filter($or);  
   }  
   //--end
   //var_dump($filter);exit("123");
   //begin added by marklee 2015 02 05 跳蚤市场
   if($search_falg=='classified'){
   	$classified_category=$filter['classified_category'];
	$field_classified=$filter['field_classified'];
	$field_secondary_price_range=$filter['field_secondary_price_range'];
   	$filter=array('type'=>'community');
	//filter[classified_category]=11702&filter[field_classified]=11694&filter[field_secondary_price_range]=12923
   }
   //end
  if (!empty($filter) && is_array($filter)) { 
    $query_filter = new SearchApiQueryFilter();
    _services_search_api_add_filter($filter, $query_filter);
    $query->filter($query_filter);
  }
  
  
  // Also set sorts.
  if (!empty($sort) && is_array($sort)) {
    foreach ($sort as $field => $mode) {
      $query->sort($field, $mode);
    }
  }
  $result = $query->execute();
  //var_dump($result);exit("123");
  if (empty($result['results']) || !is_array($result['results'])) {
    return FALSE;
  }
  module_load_include('inc', 'services_entity', 'services_entity.resources');
  $entity_type = $query->getIndex()->item_type;
  $entities = entity_load($entity_type, array_keys($result['results']));
  
//print_r($entities);exit("123");
  // Allow other modules to alter the entities retrieved.
  drupal_alter('services_search_api_postprocess', $entities, $entity_type);
  // ServicesEntityResourceController is at the moment broken, therefore use
  // the class directly and not _services_entity_resource_retrieve().
  $resourceclass = variable_get('services_entity_resource_class', 'ServicesEntityResourceControllerClean');
  $nodes=array();
  if (!empty($entities)) {
   
    $nodes = node_load_multiple(array_keys($entities));
	  //begin-- added by marklee 2015-02-02  地图标点
	$falg="";
	if($search_falg=='house_rent'||$search_falg=='house_sell'||$search_falg=='recruitment'||$search_falg=='app_convenience'||$search_falg=='app_carpooling'){	
	  if($search_falg=='house_rent'||$search_falg=='house_sell'){
	 	$falg="套";
	  }
	 //人才招聘，便民服务，拼车，跳蚤市场
	  if($search_falg=='recruitment'||$search_falg=='app_convenience'||$search_falg=='app_carpooling'){
	 	$falg="条";
	  }
	  $node_and_community_ids=array();
	 
	   $community_ids=array();
	  foreach($nodes as $nidd=>&$nodee){
	       if(!empty($nodee->field_gcc_audience['und'][0]['entity_id'])){
		   	  array_push($community_ids,$nodee->field_gcc_audience['und'][0]['entity_id']);
			  $c_n_ids=$nodee->field_gcc_audience['und'][0]['entity_id'].'|'.$nodee->nid;
			  array_push($node_and_community_ids,$c_n_ids); 
		   }	     
	  }  
	  
	  $community_count=array_count_values($community_ids);
	  foreach($community_count as $c_key=>$c_value){
	         $nids=array();
	         foreach($node_and_community_ids as $node_com_key=>$node_com_val){		
		   	 $node_var=explode("|",$node_com_val);	
			 if($node_var[0]==$c_key){
				array_push($nids,$node_var[1]);	
			 }
		   }
	       $node_nodes = node_load_multiple($nids);
		   foreach($node_nodes as $nid => &$node){	
                  commerce_services_decorate_entity('node', $node);
                  $expand_entities=1;
	              $flatten_fields='true';	  
                  if($expand_entities > 0){
                     commerce_services_expand_entities('node', $node, $expand_entities, $flatten_fields == 'true');
                  }
                  if($flatten_fields == 'true'){
                     $node = clone($node);
                  }
                  if(!in_array('rdf_mapping', $fields)) {
                     unset($node->rdf_mapping);
                  }
		   }
	   	   $nodesss=_node_resource_retrieve($c_key);
		   $arr_community_count[$c_key]=array('community_id'=>$c_key,'title'=>$nodesss->title,'count'=>$c_value.$falg,'lat'=>$nodesss->field_geo['und'][0]['lat'],'lon'=>$nodesss->field_geo['und'][0]['lon'],'nodes'=>$node_nodes);
	  }	   
	  //var_dump($arr_community_count);exit("123");
	   return $arr_community_count;
	}
	//end
	//var_dump($search_falg);exit("123");
   //begin added by marklee 2015 02 05 地图 拼车标点，先查小区，再通过小区ID去找拼车信息  
    if($search_falg=='classified'){
     
      foreach($nodes as $niddd=>&$nodeee){
	 // ,e.field_gcc_audience_entity_id
  	         $sql="SELECT a.nid FROM node AS a INNER JOIN field_data_field_classified AS b ON a.nid=b.entity_id INNER JOIN field_data_classified_category AS c ON b.entity_id=c.entity_id INNER JOIN field_data_field_secondary_price_range AS d ON c.entity_id=d.entity_id INNER JOIN field_data_field_gcc_audience AS e ON d.entity_id=e.entity_id WHERE a.type='classified' AND a.status=1 AND b.entity_type='node' AND b.bundle='classified' AND c.entity_type='node' AND c.bundle='classified' AND d.entity_type='node' AND d.bundle='classified' AND e.entity_type='node' AND e.bundle='classified' AND e.field_gcc_audience_entity_id=".$nodeee->nid."order by a.changed desc";
			 
            if(!empty($field_classified)){
	          $sql.=" AND b.field_classified_tid=".$field_classified;
	        }
            if(!empty($classified_category)){
	          $sql.=" AND c.classified_category_tid=".$classified_category; 
            }
            if(!empty($field_secondary_price_range)){
	          $sql.=" AND d.field_secondary_price_range_tid=".$field_secondary_price_range; 
            }
            $result = db_query($sql)->fetchAll();
		    $count='';
            if(!empty($result)){
             
			   $classifield_nids=array();
              foreach($result as $keyy=>$varr){
	                  array_push($classifield_nids,$varr->nid);
              }
			  $classifield_nids=array_unique($classifield_nids);
			  $count=count($classifield_nids).'条';
			
              $node_nodess = node_load_multiple($classifield_nids);
		      foreach($node_nodess as $nid => &$node) {	
                     commerce_services_decorate_entity('node', $node);
                     $expand_entities=1;
	                 $flatten_fields='true';	  
                     if($expand_entities > 0){
                     commerce_services_expand_entities('node', $node, $expand_entities, $flatten_fields == 'true');
                     }
                     if($flatten_fields == 'true'){
                       $node = clone($node);
                     }
                     if(!in_array('rdf_mapping', $fields)){
                       unset($node->rdf_mapping);
                     }
					 if($node->type=='classified'){
	  	             $community_info=_node_resource_retrieve($node->field_gcc_audience['und'][0]['entity_id']);
                     $node->province=$community_info->field_address['und'][0]['administrative_area'];
                     $node->city=$community_info->field_address['und'][0]['locality'];
                     $node->area=$community_info->field_address['und'][0]['thoroughfare'];
	                 $node->address=$community_info->field_address['und'][0]['premise'];
	                } 
					 
					 
		      }
 	          $arr_community_count[$nodeee->nid]=array('community_id'=>$nodeee->nid,'title'=>$nodeee->title,'count'=>$count,'lat'=>$nodeee->field_geo['und'][0]['lat'],'lon'=>$nodeee->field_geo['und'][0]['lon'],'nodes'=>$node_nodess);
            }
      }

      if(empty($arr_community_count)){
         return FALSE;
      }
	  return $arr_community_count; 
    }
   //end

    foreach ($nodes as $nid => &$node) {	
      // Add simplified fields to the entity object for certain field types.
      commerce_services_decorate_entity('node', $node);
      $expand_entities=1;
	  $flatten_fields='true';
      // Expand referenced entities into the full entity to the specified depth.	  
      if ($expand_entities > 0) {
        commerce_services_expand_entities('node', $node, $expand_entities, $flatten_fields == 'true');
      }
      // Flatten field value arrays if specified. This must be the last operation
      // performed as it breaks the standard field data model. An entity whose
      // fields have thus been flattened is no longer wrappable.
      if ($flatten_fields == 'true') {
        $node = clone($node);
        //commerce_services_flatten_fields('node', $node);
      }
      // Unset the rdf_mapping key from any loaded node unless explicitly included
      // in the $fields array.
      if (!in_array('rdf_mapping', $fields)) {
        unset($node->rdf_mapping);
      }
    
	  $start['lat']=(double)$lat;//搜索参数
	  $start['lon']=(double)$lon;
	  $destination['lat']=(double)$node->field_geo['und'][0]["lat"];//目标参数
	  $destination['lon']=(double)$node->field_geo['und'][0]["lon"];
	  if(!empty($start)||!empty($destination)){
	 		 $ret = search_api_location_calculate_distance($start,$destination);	 
	         $ret = sprintf("%.3f", $ret)*1000; 
	         $node->field_distance=$ret.'m';
	  }
	   // Filter out the node properties based on the field list if given; always
      // remove rdf_mapping unless explicitly requested in the field list.
      if (!empty($fields)) {
	    foreach ((array) $node as $key => $value) {  
          if (!in_array($key, explode(',', $fields[0]))) { 
              unset($node->$key);
          }
        }
      }
	  //begin added by marklee 2015-02-06二手市场 查询小区，取小区的地址显示
	  if($node->type=='classified'){
	  	$community_info=_node_resource_retrieve($node->field_gcc_audience['und'][0]['entity_id']);

        $node->province=$community_info->field_address['und'][0]['administrative_area'];
        $node->city=$community_info->field_address['und'][0]['locality'];
        $node->area=$community_info->field_address['und'][0]['thoroughfare'];
	    $node->address=$community_info->field_address['und'][0]['premise'];
	  }
	 
	  //end 
	 $nodess[]=$node;
    }	
  }
 //print_r($nodess);exit("123");
  return $nodess; 
}

