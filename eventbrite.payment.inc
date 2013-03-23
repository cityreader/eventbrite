<?php

/**
 * @file
 * Defines payment handling functions
 */

/**
 * Default Payment Settings Form
 */
function eventbrite_payment_admin_settings($form, &$form_state) {
  $default_values = variable_get('eventbrite_default_payment_settings', array());

  $form = eventbrite_payment_settings_form($default_values, TRUE);
  $form['#validate'][''] = 'eventbrite_payment_settings_form_validate';
  $form['#submit'][''] = 'eventbrite_payment_settings_form_submit';

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Update default settings',
  );

  return $form;
}

/**
 * Eventbrite payment setting form
 *
 * @see eventbrite_payment_settings_form_validate()
 * @see eventbrite_payment_settings_form_submit()
 */
function eventbrite_payment_settings_form($default_values = array(), $show_control = FALSE) {
  if ($show_control == TRUE) {
  
    $form['workflow'] = array(
      '#type' => 'fieldset',
      '#title' => t('Payment workflow'),
    );

    $form['workflow']['allow_payment_override'] = array(
      '#type' => 'checkbox',
      '#title' => t('Allow event creators/editors to override these settings'),
      '#default_value' => isset($default_values['allow_payment_override']) ? $default_values['allow_payment_override'] : 0,
    );

    $form['workflow']['autocreate_payment'] = array(
      '#type' => 'checkbox',
      '#title' => t('Automatically submit payment settings on creation of every event'),
      '#default_value' => isset($default_values['autocreate_payment']) ? $default_values['autocreate_payment'] : 0,
    );
  
  }
  else {
  
    if (isset($default_values['allow_payment_override'])) {
      $form['allow_payment_override'] = array(
        '#type' => 'value',
        '#value' => $default_values['allow_payment_override'],
      );
    }

    if (isset($default_values['autocreate_payment'])) {
      $form['autocreate_payment'] = array(
        '#type' => 'value',
        '#value' => $default_values['autocreate_payment'],
      );
    }
  }

  $form['method'] = array(
    '#type' => 'fieldset',
    '#title' => t('Payment methods'),
  );

  $form['method']['accept_paypal'] = array(
    '#type' => 'checkbox',
    '#title' => t('Accept PayPal payments'),
  );

  $form['method']['paypal_email'] = array(
    '#type' => 'textfield',
    '#title' => t('PayPal Email'),
    '#states' => array(
      'visible' => array(
        ':input[name="accept_paypal"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['method']['accept_google'] = array(
    '#type' => 'checkbox',
    '#title' => t('Accept Google Checkout payments'),
  );

  $form['method']['google_merchant_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Google Checkout Merchant ID'),
    '#states' => array(
      'visible' => array(
        ':input[name="accept_google"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['method']['google_merchant_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Google Checkout Merchant Key'),
    '#states' => array(
      'visible' => array(
        ':input[name="accept_google"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['method']['accept_check'] = array(
    '#type' => 'checkbox',
    '#title' => t('Accept "Pay by Check" payments'),
  );

  $form['method']['instructions_check'] = array(
    '#type' => 'textfield',
    '#title' => 'Instructions to attendees who want to pay by check',
    '#states' => array(
      'visible' => array(
        ':input[name="accept_check"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['method']['accept_cash'] = array(
    '#type' => 'checkbox',
    '#title' => t('Accept "Pay with Cash" payments'),
  );

  $form['method']['instructions_cash'] = array(
    '#type' => 'textfield',
    '#title' => 'Instructions to attendees who want to pay with cash',
    '#states' => array(
      'visible' => array(
        ':input[name="accept_cash"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['method']['accept_invoice'] = array(
    '#type' => 'checkbox',
    '#title' => t('Accept "Send an Invoice" payments'),
  );

  $form['method']['instructions_invoice'] = array(
    '#type' => 'textfield',
    '#title' => 'Instructions to attendees who need to be sent an invoice',
    '#states' => array(
      'visible' => array(
        ':input[name="accept_invoice"]' => array('checked' => TRUE),
      ),
    ),
  );

  foreach ($form['method'] as $field_name=>$field) {
    if (element_child($field_name) && isset($default_values[$field_name])) {
      $form['method'][$field_name]['#default_value'] = $default_values[$field_name];
    }
  }

  // If show controls = FALSE & allow_payment_override = FALSE,
  // add readonly to each element
  if (!$show_control && !$default_values['allow_payment_override']) {

    if ($form['method']['accept_paypal']['#default_value']) {
      $form['method']['accept_paypal']['#disabled'] = TRUE;
      $form['method']['paypal_email']['#attributes']['readonly'] = 'readonly';
    }
    else {
      unset($form['method']['accept_paypal']);
      unset($form['method']['paypal_email']);
    }

    if ($form['method']['accept_google']['#default_value']) {
      $form['method']['accept_google']['#disabled'] = TRUE;
      $form['method']['google_merchant_id']['#disabled'] = TRUE;
      $form['method']['google_merchant_key']['#disabled'] = TRUE;
    }
    else {
      unset($form['method']['accept_google']);
      unset($form['method']['google_merchant_id']);
      unset($form['method']['google_merchant_key']);
    }

    if ($form['method']['accept_check']['#default_value']) {
      $form['method']['accept_check']['#disabled'] = TRUE;
      $form['method']['instructions_check']['#disabled'] = TRUE;
    }
    else {
      unset($form['method']['accept_check']);
      unset($form['method']['instructions_check']);
    }

    if ($form['method']['accept_cash']['#default_value']) {
      $form['method']['accept_cash']['#disabled'] = TRUE;
      $form['method']['instructions_cash']['#disabled'] = TRUE;
    }
    else {
      unset($form['method']['accept_cash']);
      unset($form['method']['instructions_cash']);
    }

    if ($form['method']['accept_invoice']['#default_value']) {
      $form['method']['accept_invoice']['#disabled'] = TRUE;
      $form['method']['instructions_invoice']['#disabled'] = TRUE;
    }
    else {
      unset($form['method']['accept_invoice']);
      unset($form['method']['instructions_invoice']);
    }
  }

  return $form;
}

/**
 * Submit callback for Eventbrite payment setting form
 *
 * @see eventbrite_payment_settings_form()
 * @see eventbrite_payment_settings_form_submit()
 */
function eventbrite_payment_settings_form_validate($form, &$form_state) {
  $values = $form_state['values'];

  if ($values['accept_paypal']) {
    // If using PayPay check that paypal email is entered
    if (!valid_email_address($values['paypal_email'])) {
      // TODO: Does this need to actually verified that there is a PayPal account registered to this user
      form_set_error('paypal_email', t('Please provide a valid Paypal email.'));
    }
  }

  if ($values['accept_google']) {
    // If using Google Checkout check that Merchant ID is a 10 or 15 digit number
    if (!preg_match('/^([0-9]{10}|[0-9]{15})$/', $values['eventbrite_google_merchant_id'])) {
      form_set_error('google_merchant_id', t('To use Google Checkout you need to enter a 10 or 15 digit Google Checkout Merchant ID.'));
    }
    // If using Google Checkout check that Merchant Key is a 22 character string
    if (!preg_match('/^(.{22})$/', $values['google_merchant_key'])) {
      form_set_error('google_merchant_key', t('To use Google Checkout you need to enter a 22 character Google Checkout Merchant Key.'));
    }
  }
}

/**
 * Submit callback for Eventbrite payment setting form
 *
 * @see eventbrite_payment_settings_form()
 * @see eventbrite_payment_settings_form_validate()
 */
function eventbrite_payment_settings_form_submit($form, &$form_state) {
  variable_set('eventbrite_default_payment_settings', eventbrite_payment_get_settings($form_state['values']));
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function eventbrite_payment_get_settings($values) {
  $valid_params = array(
    'allow_payment_override',
    'autocreate_payment',
    'accept_paypal',
    'paypal_email',
    'accept_google',
    'google_merchant_id',
    'google_merchant_key',
    'accept_check',
    'instructions_check',
    'accept_cash',
    'instructions_cash',
    'accept_invoice',
    'instructions_invoice',
  );

  return array_intersect_key($values, array_flip($valid_params));
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function eventbrite_payment_get_default_settings() {
  $default_payment_settings = module_invoke_all('eventbrite_default_payment_settings');

  if (empty($default_payment_settings)) {
    $default_payment_settings = variable_get('eventbrite_default_payment_settings', array());
  }

  return $default_payment_settings;
}
