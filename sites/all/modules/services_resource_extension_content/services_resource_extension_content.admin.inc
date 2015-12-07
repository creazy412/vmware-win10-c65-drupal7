<?php
/**
 * @file
 * Admin forms for content resource settings.
 */

/**
 * Build form for cache settings.
 *
 * @see services_resource_extension_content_cache_settings_validate()
 * @see services_resource_extension_content_cache_settings_submit()
 */
function services_resource_extension_content_cache_settings($form, &$state) {
  $form = array();
  // Help text.
  $form['help'] = array(
    '#type' => 'fieldset',
    '#title' => t('Help'),
    '#description' => t('Cache setting for content requests in seconds. E.G. 600 = 5mins or 3600 = 1 hour.<br />By default a cache of 3600 (1 hour) is used for all requests.'),
  );
  // Page layout cache settings.
  $form['layout'] = array(
    '#type' => 'fieldset',
    '#title' => t('Layout request cache settings'),
  );
  $form['layout']['page_request'] = array(
    '#type' => 'textfield',
    '#title' => t('Page layout request'),
    '#size' => 10,
    '#default_value' => variable_get('srec_page_request', 3600),
  );
  $form['layout']['blank_request'] = array(
    '#type' => 'textfield',
    '#title' => t('Blank page request'),
    '#size' => 10,
    '#default_value' => variable_get('srec_blank_request', 3600),
  );
  $form['layout']['drupal_block_request'] = array(
    '#type' => 'textfield',
    '#title' => t('Drupal block request'),
    '#size' => 10,
    '#default_value' => variable_get('srec_drupal_block_request', 3600),
    '#description' => t('If drupal block cache setting is set to BLOCK_NO_CACHE, the cache will be set to 0 for that block. Otherwise this value will be used.'),
  );
  // Custom block cache settings.
  $form['block'] = array(
    '#type' => 'fieldset',
    '#title' => t('Custom block cache settings'),
    '#description' => t('Define the cache settings for CUSTOM BLOCKS.<br />The block name needs to be the same as the theme function that delivers the content.<br />You can overwrite a drupal block by putting the block module-delta as the block name. E.G. block-1'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['block']['custom_blocks'] = array(
    '#prefix' => '<div id="custom-blocks">',
    '#suffix' => '</div>',
    '#theme' => 'services_resource_extension_content_custom_blocks',
    '#tree' => TRUE,
  );
  // Work out number of blocks to show.
  $custom_settings = variable_get('srec_custom_blocks', array());
  $num_blocks = count($custom_settings) + 1;
  if (!empty($state['values']['custom_blocks'])) {
    $num_blocks = count($state['values']['custom_blocks']) + 1;
  }
  for ($i = 0; $i < $num_blocks; $i++) {
    // Get default values.
    $name = (!empty($custom_settings[$i]['block_name'])) ? $custom_settings[$i]['block_name'] : '';
    $name = (!empty($state['values']['custom_blocks'][$i]['block_name'])) ? $state['values']['custom_blocks'][$i]['block_name'] : $name;
    $time = (isset($custom_settings[$i]['cache_time'])) ? $custom_settings[$i]['cache_time'] : '';
    $time = (isset($state['values']['custom_blocks'][$i]['cache_time'])) ? $state['values']['custom_blocks'][$i]['cache_time'] : $time;
    // Define fields.
    $form['block']['custom_blocks'][] = array(
      'block_name' => array(
        '#type' => 'textfield',
        '#title' => '',
        '#size' => 40,
        '#default_value' => $name,
      ),
      'cache_time' => array(
        '#type' => 'textfield',
        '#title' => '',
        '#size' => 10,
        '#default_value' => $time,
      ),
    );
  }
  $form['block']['add_more'] = array(
    '#type' => 'submit',
    '#value' => 'Add another block',
    '#ajax' => array(
      'callback' => 'services_resource_extension_content_cache_settings_add_block',
      'wrapper' => 'custom-blocks',
    ),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save settings'),
  );
  return $form;
}

/**
 * Validate cache settings form submission.
 *
 * @see services_resource_extension_content_cache_settings()
 */
function services_resource_extension_content_cache_settings_validate($form, &$state) {
  // Ensure validation does not run on add more.
  if ($state['clicked_button']['#value'] != 'Add another block') {
    if (!is_numeric($state['values']['page_request'])) {
      form_set_error('page_request', 'Enter page request cache time, must be a numeric value.');
    }
    if (!is_numeric($state['values']['blank_request'])) {
      form_set_error('blank_request', 'Enter blank request cache time, must be a numeric value.');
    }
    if (!is_numeric($state['values']['drupal_block_request'])) {
      form_set_error('drupal_block_request', 'Enter drupal block request cache time, must be a numeric value.');
    }
    foreach ($state['values']['custom_blocks'] as $ptr => $custom) {
      if (!empty($custom['block_name'])) {
        if (empty($custom['block_name'])) {
          form_set_error('custom_blocks][' . $ptr . '][block_name', 'Enter block name.');
        }
        if (!is_numeric($custom['cache_time'])) {
          form_set_error('custom_blocks][' . $ptr . '][cache_time', 'Enter block cache time, must be a numeric value.');
        }
      }
    }
  }
}

/**
 * Process cache settings form submission.
 *
 * @see services_resource_extension_content_cache_settings()
 */
function services_resource_extension_content_cache_settings_submit($form, &$state) {
  // Process add more button click.
  if ($state['clicked_button']['#value'] == 'Add another block') {
    $state['rebuild'] = TRUE;
  }
  // Process form submission.
  else {
    // Save settings.
    variable_set('srec_page_request', (int) $state['values']['page_request']);
    variable_set('srec_blank_request', (int) $state['values']['blank_request']);
    variable_set('srec_drupal_block_request', (int) $state['values']['drupal_block_request']);
    // Save custom block settings.
    $custom_blocks = array();
    foreach ($state['values']['custom_blocks'] as $ptr => $custom) {
      if (!empty($custom['block_name'])) {
        $custom['cache_time'] = (int) $custom['cache_time'];
        $custom_blocks[] = $custom;
      }
    }
    variable_set('srec_custom_blocks', $custom_blocks);
  }
}

/**
 * AJAX handler for adding a custom block.
 */
function services_resource_extension_content_cache_settings_add_block($form, &$state) {
  return $form['block']['custom_blocks'];
}

/**
 * Theme custom blocks table in settings form.
 *
 * @param $element array
 *   Form field definition array.
 *
 * @return string
 *   HTML for table.
 */
function theme_services_resource_extension_content_custom_blocks($args) {
  $rows = array();
  $element = $args['element'];
  $header = array('Block name', 'Cache settings');
  foreach (element_children($element) as $key) {
    if (isset($element[$key]['block_name'])) {
      $field = &$element[$key];
      $row = array();
      $row[] = drupal_render($field['block_name']);
      $row[] = drupal_render($field['cache_time']);
      $rows[] = $row;
    }
  }
  return theme('table', array('header' => $header, 'rows' => $rows));
}
?>