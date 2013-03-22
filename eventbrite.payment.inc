<?php

/**
 * @file
 * Defines payment handling functions
 */

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
/*
 * Default Payment Settings Form
 */
/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
//function eventbrite_payment_default_settings($form, &$form_state) {
//  $default_values = variable_get('eventbrite_default_payment_settings', array());
//
//  $form = eventbrite_payment_settings_form($default_values, 1);
//
//  $form['submit'] = array(
//    '#type' => 'submit',
//    '#value' => 'Update default settings',
//  );
//
//  return $form;
//}

//function eventbrite_payment_settings_form($default_values = array(), $show_controls = 0) {
function eventbrite_payment_settings_form($form, &$form_state, $show_controls = FALSE) {
  $default_values = variable_get('eventbrite_default_payment_settings', array());

  drupal_add_js(drupal_get_path('module', 'eventbrite')  . '/eventbrite.payment.js');

  if (is_object($default_values)) {
    $default_values = (array) $default_values;
  }

  if ($show_controls) {
    $form['workflow'] = array(
      '#type' => 'fieldset',
      '#title' => t('Payment workflow'),
    );
    $form['workflow']['eventbrite_allow_payment_override'] = array(
      '#type' => 'checkbox',
      '#title' => t('Allow event creators/editors to override these settings'),
      '#default_value' => isset($default_values['allow_payment_override']) ? $default_values['allow_payment_override'] : 1,
    );

    $form['workflow']['eventbrite_autocreate_payment'] = array(
      '#type' => 'checkbox',
      '#title' => t('Automatically submit payment settings on creation of every event'),
      '#default_value' => isset($default_values['autocreate_payment']) ? $default_values['autocreate_payment'] : 0,
    );
  }
  else {
    if (isset($default_values['allow_payment_override'])) {
      $form['eventbrite_allow_payment_override'] = array(
        '#type' => 'value',
        '#value' => $default_values['allow_payment_override'],
      );
    }

    if (isset($default_values['autocreate_payment'])) {
      $form['eventbrite_autocreate_payment'] = array(
        '#type' => 'value',
        '#value' => $default_values['autocreate_payment'],
      );
    }
  }

  $form['method'] = array(
    '#type' => 'fieldset',
    '#title' => t('Payment methods'),
  );

  $form['method']['eventbrite_accept_paypal'] = array(
    '#type' => 'checkbox',
    '#title' => t('Accept PayPal payments'),
    '#default_value' => isset($default_values['accept_paypal']) ? $default_values['accept_paypal'] : 0,
  );

  $form['method']['eventbrite_paypal_email'] = array(
    '#type' => 'textfield',
    '#title' => t('PayPal Email'),
    '#default_value' => isset($default_values['paypal_email']) ? $default_values['paypal_email'] : '',
    '#states' => array(
      'visible' => array(
        ':input[name="eventbrite_accept_paypal"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['method']['eventbrite_accept_google'] = array(
    '#type' => 'checkbox',
    '#title' => t('Accept Google Checkout payments'),
    '#default_value' => isset($default_values['accept_google']) ? $default_values['accept_google'] : 0,
  );

  $form['method']['eventbrite_google_merchant_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Google Checkout Merchant ID'),
    '#default_value' => isset($default_values['google_merchant_id']) ? $default_values['google_merchant_id'] : '',
    '#states' => array(
      'visible' => array(
        ':input[name="eventbrite_accept_google"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['method']['eventbrite_google_merchant_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Google Checkout Merchant Key'),
    '#default_value' => isset($default_values['google_merchant_key']) ? $default_values['google_merchant_key'] : '',
    '#states' => array(
      'visible' => array(
        ':input[name="eventbrite_accept_google"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['method']['eventbrite_accept_check'] = array(
    '#type' => 'checkbox',
    '#title' => t('Accept "Pay by Check" payments'),
    '#default_value' => isset($default_values['accept_check']) ? $default_values['accept_check'] : 0,
  );

  $form['method']['eventbrite_instructions_check'] = array(
    '#type' => 'textfield',
    '#title' => 'Instructions to attendees who want to pay by check',
    '#default_value' => isset($default_values['instructions_check']) ? $default_values['instructions_check'] : '',
    '#states' => array(
      'visible' => array(
        ':input[name="eventbrite_accept_check"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['method']['eventbrite_accept_cash'] = array(
    '#type' => 'checkbox',
    '#title' => t('Accept "Pay with Cash" payments'),
    '#default_value' => isset($default_values['accept_cash']) ? $default_values['accept_cash'] : 0,
  );

  $form['method']['eventbrite_instructions_cash'] = array(
    '#type' => 'textfield',
    '#title' => 'Instructions to attendees who want to pay with cash',
    '#default_value' => isset($default_values['instructions_cash']) ? $default_values['instructions_cash'] : '',
    '#states' => array(
      'visible' => array(
        ':input[name="eventbrite_accept_cash"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['method']['eventbrite_accept_invoice'] = array(
    '#type' => 'checkbox',
    '#title' => t('Accept "Send an Invoice" payments'),
    '#default_value' => isset($default_values['accept_invoice']) ? $default_values['accept_invoice'] : 0,
  );

  $form['method']['eventbrite_instructions_invoice'] = array(
    '#type' => 'textfield',
    '#title' => 'Instructions to attendees who need to be sent an invoice',
    '#default_value' => isset($default_values['instructions_invoice']) ? $default_values['instructions_invoice'] : '',
    '#states' => array(
      'visible' => array(
        ':input[name="eventbrite_accept_invoice"]' => array('checked' => TRUE),
      ),
    ),
  );

  // If show controls = FALSE & allow_payment_override = FALSE, add readonly to each element
  if (!$show_controls && !$default_values['allow_payment_override']) {

    if ($form['method']['eventbrite_accept_paypal']['#default_value']) {
      $form['method']['eventbrite_accept_paypal']['#disabled'] = 1;
      $form['method']['eventbrite_paypal_email']['#attributes']['readonly'] = 1;
    }
    else {
      unset($form['method']['eventbrite_accept_paypal']);
      unset($form['method']['eventbrite_paypal_email']);
    }

    if ($form['method']['eventbrite_accept_google']['#default_value']) {
      $form['method']['eventbrite_accept_google']['#disabled'] = 1;
      $form['method']['eventbrite_google_merchant_id']['#attributes']['readonly'] = 1;
      $form['method']['eventbrite_google_merchant_key']['#attributes']['readonly'] = 1;
    }
    else {
      unset($form['method']['eventbrite_accept_google']);
      unset($form['method']['eventbrite_google_merchant_id']);
      unset($form['method']['eventbrite_google_merchant_key']);
    }

    if ($form['method']['eventbrite_accept_check']['#default_value']) {
      $form['method']['eventbrite_accept_check']['#disabled'] = 1;
      $form['method']['eventbrite_instructions_check']['#attributes']['readonly'] = 1;
    }
    else {
      unset($form['method']['eventbrite_accept_check']);
      unset($form['method']['eventbrite_instructions_check']);
    }

    if ($form['method']['eventbrite_accept_cash']['#default_value']) {
      $form['method']['eventbrite_accept_cash']['#disabled'] = 1;
      $form['method']['eventbrite_instructions_cash']['#attributes']['readonly'] = 1;
    }
    else {
      unset($form['method']['eventbrite_accept_cash']);
      unset($form['method']['eventbrite_instructions_cash']);
    }

    if ($form['method']['eventbrite_accept_invoice']['#default_value']) {
      $form['method']['eventbrite_accept_invoice']['#disabled'] = 1;
      $form['method']['eventbrite_instructions_invoice']['#attributes']['readonly'] = 1;
    }
    else {
      unset($form['method']['eventbrite_accept_invoice']);
      unset($form['method']['eventbrite_instructions_invoice']);
    }
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Update default settings',
  );

  return $form;
}

// Temporary function
// TODO $form_errors wil have to turn into form_set_errors()
/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function eventbrite_payment_settings_errors($values) {
  $form_errors = array();

  if ($values['eventbrite_accept_paypal']) {
    // If using PayPay check that paypal email is entered
    if (!valid_email_address($values['eventbrite_paypal_email'])) {
      // TODO: Does this need to actually verified that there is a PayPal account registered to this user
      $form_errors['eventbrite_paypal_email'] = t('Please provide a valid Paypal email.');
    }
  }

  if ($values['eventbrite_accept_google']) {
    // If using Google Checkout check that Merchant ID is a 10 or 15 digit number
    if (!preg_match('/^([0-9]{10}|[0-9]{15})$/', $values['eventbrite_google_merchant_id'])) {
      $form_errors['eventbrite_google_merchant_id'] = t('To use Google Checkout you need to enter a 10 or 15 digit Google Checkout Merchant ID.');
    }
    // If using Google Checkout check that Merchant Key is a 22 character string
    if (!preg_match('/^(.{22})$/', $values['google_merchant_key'])) {
      $form_errors['eventbrite_google_merchant_key'] = t('To use Google Checkout you need to enter a 22 character Google Checkout Merchant Key.');
    }
  }
  return $form_errors;
}



/*
 * Default Payment Settings Form Validate
 */
/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function eventbrite_payment_default_settings_validate($form, &$form_state) {
  $form_values = $form_state['values'];

  $form_errors = eventbrite_payment_settings_errors($form_values);
  foreach ($form_errors as $form_element => $error_msg) {
    form_set_error($form_element, $error_msg);
  }
}

/*
 * Default Payment Settings Form Submit
 */
/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function eventbrite_payment_default_settings_submit($form, &$form_state) {
  variable_set('eventbrite_default_payment_settings', eventbrite_payment_get_settings($form_state['values']));
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function eventbrite_payment_get_settings($values) {
  if (is_object($values)) {
    $values = (array) $values;
  }
  $return_values = array();

  $return_values['allow_payment_override'] = $values['eventbrite_allow_payment_override'];
  $return_values['autocreate_payment'] = $values['eventbrite_autocreate_payment'];
  $return_values['accept_paypal'] = $values['eventbrite_accept_paypal'];
  $return_values['paypal_email'] = $values['eventbrite_paypal_email'];
  $return_values['accept_google'] = $values['eventbrite_accept_google'];
  $return_values['google_merchant_id'] = $values['eventbrite_google_merchant_id'];
  $return_values['google_merchant_key'] = $values['eventbrite_google_merchant_key'];
  $return_values['accept_check'] = $values['eventbrite_accept_check'];
  $return_values['instructions_check'] = $values['eventbrite_instructions_check'];
  $return_values['accept_cash'] = $values['eventbrite_accept_cash'];
  $return_values['instructions_cash'] = $values['eventbrite_instructions_cash'];
  $return_values['accept_invoice'] = $values['eventbrite_accept_invoice'];
  $return_values['instructions_invoice'] = $values['eventbrite_instructions_invoice'];

  return $return_values;
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
