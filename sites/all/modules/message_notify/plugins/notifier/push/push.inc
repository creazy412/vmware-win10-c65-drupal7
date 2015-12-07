<?php
if (module_exists('push_notifications')) {
  $plugin = array(
    'title' => t('PUSH Notify'),
    'description' => t('Send Message using the push_notifications module.'),
    'class' => 'MessageNotifierPush',
    'view_modes' => array('message_notify_push_body' 
					=> array('label' => t('Notify - Push Body')),
    ),
  );
}