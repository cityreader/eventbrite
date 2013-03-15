<?php

/**
 * @file
 * Manages and verifies the authentication params and settings
 *   used for communication with Eventbrite server.
 */

module_load_include('inc', 'eventbrite', 'eventbrite.common');

/*
 * Admin settings form for the module
 */
/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function eventbrite_admin_settings($form, &$form_state) {
  $form = array();

  $form['eventbrite_app_key'] = array(
    '#type' => 'textfield',
    '#title' => 'Application Key (API Key)',
    '#required' => TRUE,
    '#default_value' => variable_get('eventbrite_app_key', ''),
    '#description' => t('You must have an Eventbrite API key to use this module. You can obtain a key by <a href="http://www.eventbrite.com/api/key/" target="_blank">clicking here</a>.'),
  );
  $form['eventbrite_user_key'] = array(
    '#type' => 'textfield',
    '#title' => 'User Key',
    '#required' => TRUE,
    '#default_value' => variable_get('eventbrite_user_key', ''),
    '#description' => t('Your user key for secure authentication. You can find it by <a href="http://www.eventbrite.com/userkeyapi" target="_blank">clicking here</a>'),
  );

  // TODO: Think more about how min cache is being used with the Eventbrite API
  // TODO: Provide better description min cache based on implementation
  $form['eventbrite_cache_minimum'] = array(
    '#type' => 'textfield',
    '#title' => 'Minimum Cache Lifetime (unit: minutes)',
    '#required' => TRUE,
    '#default_value' => variable_get('eventbrite_cache_minimum', 5),
    '#description' => t('The minimum amount of time, in minutes, to cache results from the Eventbrite server.'),
  );

  $form['eventbrite_debug'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display DEBUG messages'),
    '#description' => t('This should be unchecked on production sites.'),
    '#default_value' => variable_get('eventbrite_debug', 0),
  );

  // Set the Authentication flag to valid on form submit
  $form['eventbrite_is_valid_auth'] = array(
    '#type' => 'value',
    '#value' => TRUE,
  );

  $form['#validate'][] =  'eventbrite_admin_settings_validate';
  $form['#submit'][] = 'eventbrite_admin_settings_submit';

  return system_settings_form($form);
}

/*
 * Validate admin settings form
 */
/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function eventbrite_admin_settings_validate($form, &$form_state) {
  $eventbrite_app_key = $form_state['values']['eventbrite_app_key'];
  $eventbrite_user_key = $form_state['values']['eventbrite_user_key'];

  // Validate the keys
  if (_eventbrite_admin_authenticate_keys($eventbrite_app_key, $eventbrite_user_key)) {
    // Keys were successfully authenticated by the Eventbrite server
    return TRUE;
  }

  return FALSE;
}

/*
 * Function to test keys entered by the user
 *
 * This function builds its own requests, using the arguments as keys, rather than
 *   the module's variables.  This function DOES use the module's result processing
 *   and takes in keys rather than using the module's variables to authenticate
 *
 * Returns the user that was authenticated
 */
function _eventbrite_admin_authenticate_keys($app_key, $user_key) {
  // Return false if any keys are empty
  if (empty($app_key) || empty($user_key)) {
    return FALSE;
  }

  eventbrite_get_instance($app_key, $user_key);

  $user = eventbrite_get('user_get');

  return $user;
}

/* if we validated correctly, import organizers and venues from eventbrite */
function eventbrite_admin_settings_submit($form, &$form_state) {
  // $venues = eventbrite_get('user_list_venues');
  // $organizers = eventbrite_get('user_list_organizers');
}
