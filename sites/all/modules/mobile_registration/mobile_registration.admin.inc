<?php

/**
 * @file
 * mobile_registration callback
 */ 

function mobile_registration_config_form() {
  $form['mobile_registration_nickname_field'] = array(
    '#type' => 'textfield',
    '#title' => t('Machine name of nickname field that used as user name'),
    '#default_value' => variable_get('mobile_registration_nickname_field', 'field_nickname'),
    '#maxlength' => 255,
    '#required' => TRUE,
  );  
  
  $form['mobile_registration_confirm_message'] = array(
    '#type' => 'textfield',
    '#title' => t('mobile registration confirm message format'),
    '#default_value' => variable_get('mobile_registration_confirm_message', '[site:name]confirmation code: '),
    '#maxlength' => 255,
    '#required' => TRUE,
  );  
  return system_settings_form($form, TRUE);
}