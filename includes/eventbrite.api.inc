<?php
/**
 * @file
 * Eventbrite API functions
 */

/**
 * Allows a get-oriented operation to called via the Eventbrite API
 *
 * @param $op
 *   A string of Eventbrite API method name.
 * @param $params
 *   An array of Eventbrite API parameters.
 * @param $reset
 *   An boolean indication that the interanl cache should be reset.
 * 
 * @return
 *   An array of object result requested from Eventbrite website.  
 * 
 * @see
 *   eventbrite_save()
 */
function eventbrite_get($op, $params = NULL, $reset = FALSE) {
  $result = array();

  $eb_client = eventbrite_get_object();

  if (!$eb_client) {
    return FALSE;
  }

  $valid_callbacks = array(
    // Events
    'event_get',
    // Organizers
    'organizer_get',
    // Users
    'user_list_events',
    'user_list_venues',
    'user_list_organizers',
    'user_get',
  );

  if (!in_array($op, $valid_callbacks)) {
    drupal_set_message('Eventbrite get operation is not valid', 'error');
    watchdog('eventbrite', t('Eventbrite get operation is not valid'), NULL, WATCHDOG_ERROR);
    return;
  }


  $user_key = variable_get('eventbrite_user_key', '');
  $cid = $op . ':' . $user_key;
  $cache = FALSE;

  if ($reset) {
    clear_cache_all($cid, 'cache_eventbrite');
  }
  else {
    $cache = cache_get($cid, 'cache_eventbrite');
  }

  if ($cache !== FALSE && REQUEST_TIME <= $cache->expire) {
    $result = $cache->data;
  }
  else {
    try{
      $result = $eb_client->$op($params);

      eventbrite_debug_message($result);

    } catch (Exception $e) {
      drupal_set_message('Eventbrite API call failed. Error message:' . "\n" . $e->getMessage(), 'error');
      watchdog('eventbrite', 'Eventbrite API call failed. Error message:' . "\n" . $e->getMessage(), NULL, WATCHDOG_ERROR);
      return;
    }

    if (strpos($op, 'user_list') !== FALSE) {
      $plural_label = substr($op, 10);
      $label = substr($op, 10, -1);
      $data = array();

      foreach ($result->$plural_label as $object) {
        $entry = $object->$label;
        $id_key = $label . '_id';

        $entry->$id_key = (string) $entry->id;

        unset($entry->id);

        $data[$entry->$id_key] = $entry;
      }

      $result = $data;

      module_invoke_all('eventbrite_' . $op, $result);

      $expire = 60 * variable_get('eventbrite_cache_minimum', 5);

      cache_set($cid, $data, 'cache_eventbrite', REQUEST_TIME + $expire);
    }
  }

  return $result;
}

/**
 * Allows a save-oriented operation to called via the Eventbrite API
 *
 * @param $op
 *   A string of Eventbrite API method name.
 * @param $params
 *   An array of Eventbrite API parameters.
 * @param $reset
 *   An boolean indication that the interanl cache should be reset.
 * 
 * @return
 *   An array of object result requested from Eventbrite website.  
 * 
 * @see
 *   eventbrite_get()
 */
function eventbrite_save($op, $params) {
  $result = array();

  $eb_client = eventbrite_get_object();

  if (!$eb_client) {
    return FALSE;
  }

  $valid_callbacks = array(
    // Events
    'event_new',
    'event_update',
    // Tickets
    'ticket_new',
    'ticket_update',
    // Venues
    'venue_new',
    'venue_update',
    // Organizer Profiles
    'organizer_new',
    'organizer_update',
    // Users
    'user_new',
    // Payments
    'payment_update',
  );

  if (!in_array($op, $valid_callbacks)) {
    drupal_set_message('Eventbrite save operation is not valid', 'error');
    watchdog('eventbrite', t('Eventbrite save operation is not valid'), NULL, WATCHDOG_ERROR);
    return;
  }

  try{
    $result = $eb_client->$op($params);

    eventbrite_debug_message($result);
  } catch (Exception $e) {
    drupal_set_message('Eventbrite API call failed. Error message:' . "\n" . $e->getMessage(), 'error');
    watchdog('eventbrite', 'Eventbrite API call failed. Error message:' . "\n" . $e->getMessage(), NULL, WATCHDOG_ERROR);
    return;
  }

//  module_invoke_all('eventbrite_' . $op, $result);

  return $result;
}

/**
 * Synchronize local data with remote Eventbrite data
 */
function eventbrite_sync($op) {
  eventbrite_get($op);
}

/**
 * Get $eb_client instance
 */
function eventbrite_get_object($app_key = '', $user_key = '') {
  static $eb_client;

  if (empty($eb_client) && eventbrite_is_valid_auth()) {
        
    libraries_load('eventbrite.php');

    $authentication_tokens = array(
      'app_key'  => $app_key ? $app_key : variable_get('eventbrite_app_key', ''),
      'user_key' => $user_key ? $user_key : variable_get('eventbrite_user_key', ''),
    );

    $eb_client = new Eventbrite($authentication_tokens); 
  }

  return $eb_client;
}

