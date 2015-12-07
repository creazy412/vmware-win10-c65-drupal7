<?php

/**
 * Implementation of hook_services_resources().
 */
function services_search_api_services_resources() {
  return array(
    'search_api' => array(
      'retrieve' => array(
        'file' => array('type' => 'inc', 'module' => 'services_search_api', 'name' => 'services_search_api.resources'),
        'callback' => '_services_search_api_resource_retrieve',
        'args' => array(
          array(
            'name' => 'index',
            'type' => 'string',
            'optional' => FALSE,
            'source' => array('path' => '0'),
            'description' => t('Search API index machine name to use.'),
          ),
          array(
            'name' => 'keys',
            'type' => 'string',
            'optional' => TRUE,
            'source' => array('param' => 'keys'),
            'default value' => '',
            'description' => t('Search terms.'),
          ),
          array(
            'name' => 'filter',
            'type' => 'array',
            'optional' => TRUE,
            'source' => array('param' => 'filter'),
            'default value' => '',
            'description' => t('String of filters to apply to the search, in the form \'filter[~and][title]=title\'. Filters can be nested.'),
          ),
          array(
            'name' => 'sort',
            'type' => 'array',
            'optional' => TRUE,
            'source' => array('param' => 'sort'),
            'default value' => '',
            'description' => t('String containing the sort elements for the search in the form \'sort[nid]=DESC\'. Multiple sort terms can be used.'),
          ),
          array(
            'name' => 'limit',
            'type' => 'int',
            'optional' => TRUE,
            'source' => array('param' => 'limit'),
            'description' => t('Number of results to retrieve.'),
          ),
          array(
            'name' => 'offset',
            'type' => 'int',
            'optional' => TRUE,
            'source' => array('param' => 'offset'),
            'default value' => 0,
            'description' => t('The position of the first returned search result.'),
          ),
          array(
            'name' => 'fields',
            'optional' => TRUE,
            'type' => 'string',
            'description' => 'The fields to get, if fields are from a related entity, use field_relationship:field_name for getting a given field or field_relationship_* for getting the whole set of data',
            'default value' => '*',
            'source' => array('param' => 'fields'),
          ),
        ),
        'access arguments' => array('search content'),
        'access arguments append' => FALSE,
      ),
    ),
    'search_api_location' => array(
      'retrieve' => array(
	    'help'=>'Retrieve posted blogs',
        'file' => array('type' => 'inc', 'module' => 'services_search_api', 'name' => 'services_search_api.resources'),
        'callback' => '_services_search_api_location_retrieve',
        'args' => array(
          array(
            'name' => 'index',
            'type' => 'string',
            'optional' => FALSE,
            'source' => array('path' => '0'),
            'description' => t('Search API index machine name to use.'),
          ),
		  array(
            'name' => 'keys',
            'type' => 'string',
            'optional' => TRUE,
            'source' => array('param' => 'keys'),
            'default value' => '',
            'description' => t('Search terms.'),
          ),
          array(
            'name' => 'location',
            'type' => 'string',
            'optional' => TRUE,
            'source' => array('param' => 'location'),
            'default value' => '',
            'description' => t('Field name of indexed location.'),
          ),
          array(
            'name' => 'lat',
            'type' => 'string',
            'optional' => TRUE,
            'source' => array('param' => 'lat'),
            'default value' => '',
            'description' => t('The latitude of the point on which this location parameter is centered.'),
          ),
          array(
            'name' => 'lon',
            'type' => 'string',
            'optional' => TRUE,
            'source' => array('param' => 'lon'),
            'default value' => '',
            'description' => t('The longitude of that point.'),
          ),
          
          array(
            'name' => 'radius',
            'type' => 'string',
            'optional' => TRUE,
            'source' => array('param' => 'radius'),
            'default value' => '',
            'description' => t('the maximum distance at which a point should be included (in kilometers).'),
          ),
          array(
            'name' => 'distance',
            'type' => 'string',
            'optional' => TRUE,
            'source' => array('param' => 'distance'),
            'default value' => 'FALSE',
            'description' => t('If set to TRUE, the distance to the given point is returned in an additional field in the result. Defaults to FALSE.'),
          ),
          
          array(
            'name' => 'filter',
            'type' => 'array',
            'optional' => TRUE,
            'source' => array('param' => 'filter'),
            'default value' => '',
            'description' => t('String of filters to apply to the search, in the form \'filter[~and][title]=title\'. Filters can be nested.'),
          ),
          array(
            'name' => 'sort',
            'type' => 'array',
            'optional' => TRUE,
            'source' => array('param' => 'sort'),
            'default value' => '',
            'description' => t('String containing the sort elements for the search in the form \'sort[nid]=DESC\'. Multiple sort terms can be used.'),
          ),
          array(
            'name' => 'limit',
            'type' => 'int',
            'optional' => TRUE,
            'source' => array('param' => 'limit'),
            'description' => t('Number of results to retrieve.'),
          ),
          array(
            'name' => 'offset',
            'type' => 'int',
            'optional' => TRUE,
            'source' => array('param' => 'offset'),
            'default value' => 0,
            'description' => t('The position of the first returned search result.'),
          ),
          array(
            'name' => 'fields',
            'optional' => TRUE,
            'type' => 'string',
            'description' => 'The fields to get, if fields are from a related entity, use field_relationship:field_name for getting a given field or field_relationship_* for getting the whole set of data',
            'default value' => '*',
            'source' => array('param' => 'fields'),
          ),
        ),
        'access arguments' => array('search content'),
        'access arguments append' => FALSE,
      ),
    ),
  );
}
