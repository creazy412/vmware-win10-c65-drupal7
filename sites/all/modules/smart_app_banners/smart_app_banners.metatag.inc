<?php

/**
 * @file
 * Metatag module include.
 */

/**
 * Implements hook_metatag_config_default_alter().
 */
function smart_app_banners_metatag_config_default_alter(array &$configs) {
  foreach ($configs as &$config) {
    switch ($config->instance) {
      case 'global':
        $config->config += array(
          'apple-itunes-app' => array('value' => ''),
        );
        break;
    }
  }
}

/**
 * Implements hook_metatag_info().
 */
function smart_app_banners_metatag_info() {
  $info['groups']['mobile'] = array(
    'label' => t('Mobile'),
  );

  $info['tags']['apple-itunes-app'] = array(
    'label' => t('Smart App Banner app-id'),
    'description' => t('Site wide Smart App Banner app-id'),
    'class' => 'DrupalTextMetaTag',
    'form' => array(
      '#title' => t('Smart App Banner') . ' app-id',
      '#description' => t('Apple provides numeric %app_id; visit !itunes_linkmaker_url to get any app id.', array('%app_id' => 'app-id', '!itunes_linkmaker_url' => l(ITUNES_LINKMAKER_URL, ITUNES_LINKMAKER_URL))),
      '#maxlength' => 10,
    ),
    'group' => 'mobile',
    'element' => array(
    '#theme' => 'metatag_smart_app_banner',
    ),
  );

  return $info;
}
