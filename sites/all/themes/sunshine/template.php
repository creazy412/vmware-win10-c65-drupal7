<?php
/**
 * @file
 * Theme functions
 */

/**
 * Implements hook_css_alter().
 */
function sunshine_css_alter(&$css) {
  $radix_path = drupal_get_path('theme', 'radix');

  // Radix now includes compiled stylesheets for demo purposes.
  // We remove these from our subtheme since they are already included 
  // in compass_radix.
  unset($css[$radix_path . '/assets/stylesheets/radix-style.css']);
  unset($css[$radix_path . '/assets/stylesheets/radix-print.css']);
}


/**
 * Preprocess variables for page template.
 */
function sunshine_preprocess_page(&$variables) {
	/**
	 * insert variables into page template.
	 */
	$variables['no_panels'] = False;
	$variables['always_show_page_title'] = True;

  // Add copyright to theme.
  if ($copyright = theme_get_setting('copyright')) {
    $variables['copyright'] = check_markup($copyright['value'], $copyright['format']);
  }
}

function sunshine_mmenu_alter(&$mmenus) {
  global $theme;
  if ($theme == 'sunshine') {
    $mmenus['mmenu_left']['enabled'] = false;
  }
  else {
    $mmenus['mmenu_left']['enabled'] = True;
  }
}