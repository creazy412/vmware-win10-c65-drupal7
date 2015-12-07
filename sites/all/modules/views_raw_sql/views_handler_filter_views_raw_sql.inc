<?php

/**
 * A handler to provide a filter that is completely custom SQL by the administrator.
 *
 * @ingroup views_filter_handlers
 */
class views_handler_filter_views_raw_sql extends views_handler_filter {

  function query() {
    $raw_sql = $this->options['raw_sql'];
    // Get replacement tokens like @see views_handler_filter::get_replacement_tokens()
    $tokens = array();
    $count = 0;
    foreach ($this->view->display_handler->get_handlers('argument') as $arg => $handler) {
      ++$count;
      $tokens['!' . $count] = isset($this->view->args[$count - 1]) ? strip_tags(decode_entities($this->view->args[$count - 1])) : '';
    }
    // And replace.
    $raw_sql = str_replace(array_keys($tokens), $tokens, $raw_sql);

    $this->ensure_my_table();
    $this->query->add_where_expression($this->options['group'], $raw_sql);
  } // query

  function option_definition() {

    $options = parent::option_definition();

    // Override the alter text option to never alter the text.
    $options['alter']['contains']['alter_text'] = array('default' => FALSE);
    $options['raw_sql'] = array('default' => '');

    return $options;

  } // option_definition

  function options_form(&$form, &$form_state) {

    parent::options_form($form, $form_state);

    unset($form['alter']);

    $form['raw_sql'] = array(
      '#type' => 'textarea',
      '#title' => 'SQL',
      '#description' => t('This SQL will be added to the query as a filter expression. Use placeholders like !1 for arguments.'),
      '#default_value' => $this->options['raw_sql'],
      '#required' => TRUE,
    );

    if (!user_access('edit views raw sql')) {
      $form['raw_sql']['#disabled'] = TRUE;
      $form['raw_sql']['#description'] = t('You lack %permission permission to edit this field.', array('%permission' => 'edit views raw sql'));
    } // if

  } // options_form

  /**
   * Simple submit handler
   */
  function options_submit(&$form, &$form_state) {

    $this->options['raw_sql'] = $form_state['values']['options']['raw_sql'];

  } // options_submit

} // views_handler_filter_views_raw_sql
