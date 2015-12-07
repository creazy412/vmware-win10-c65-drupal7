<?php
/**
 * @file
 * commerce_funds.views.inc
 */

/**
 * Impliments hook_views_data().
 */
function commerce_funds_views_data() {
  $data = array();

  // Expose the transaction amount.
  $data['commerce_funds_account']['balance'] = array(
    'title' => t('Balance'),
    'help' => t('The balance of the account.'),
    'field' => array(
      'handler' => 'commerce_payment_handler_field_amount',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  $data['commerce_funds_account']['account_id'] = array(
    'title' => t('Funds Account'),
    'help' => t('The Funds Account ID.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Expose the transaction amount.
  $data['commerce_funds_transaction']['amount'] = array(
    'title' => t('Amount'),
    'help' => t('The amount of the transaction.'),
    'field' => array(
      'handler' => 'commerce_payment_handler_field_amount',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );


  $data['commerce_funds_transaction']['type'] = array(
    'title' => t('Transaction Type'),
    'help' => t('The transaction type.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'commerce_funds_type_views_handler_filter_in_operator',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  return $data;
}