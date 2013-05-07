<?php

/**
 * @file
 * Defines venue handling functions
 */

/**
 * UI controller.
 */
class EventbriteVenueUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    $items[$this->path]['type'] = MENU_LOCAL_TASK;
    return $items;
  }

  /**
   * Overrides EntityDefaultUIController::overviewForm() defaults.
   */
  public function overviewForm($form, &$form_state) {
    // Synchronize organizer data first if cache is expired
    eventbrite_sync('user_list_venues');

    $form = parent::overviewForm($form, $form_state);
    return $form;
  }

  /**
   * Overrides EntityDefaultUIController::overviewTableHeaders().
   */
  protected function overviewTableHeaders($conditions, $rows, $additional_header = array()) {
    $header = parent::overviewTableHeaders($conditions, $rows, $additional_header);

    // Change table column name from "Label" to "Organizer Name"
    $header[0] = t('Venue Name');
    $extra = array(
      t('Address'),
      t('Status'),
    );
    array_splice($header, 1, 0, $extra);
    return $header;
  }

  /**
   * Overrides EntityDefaultUIController::overviewTableRow().
   */
  protected function overviewTableRow($conditions, $id, $entity, $additional_cols = array()) {
    $row = parent::overviewTableRow($conditions, $id, $entity, $additional_cols);

    $address = $entity->address;
    if (!empty($entity->address_2)) {
      $address .= ', '. $entity->address_2;
    }
    if (!empty($entity->city)) {
      $address .= ', '. $entity->city;
    }
    if (!empty($entity->region)) {
      $address .= ', '. $entity->region;
    }
    if (!empty($entity->country_code)) {
      include_once DRUPAL_ROOT . '/includes/locale.inc';
      $country_list = country_get_list();

      $address .= ', '. $country_list[$entity->country_code];
    }
    if (!empty($entity->postal_code)) {
      $address .= ', '. $entity->postal_code;
    }
    $extra[] = array(
      'data'=> $address,
    );
    $extra[] = array(
      'data'=> $entity->status ? t('Active') : t('Inactive'),
    );
    array_splice($row, 1, 0, $extra);
    
    return $row;
  }
}

/**
 * Generates the Eventbrite venue editing form.
 *
 * @see eventbrite_venue_form_validate()
 * @see eventbrite_venue_form_submit()
 */
function eventbrite_venue_form($form, &$form_state, $entity, $op = 'edit') {
  include_once DRUPAL_ROOT . '/includes/locale.inc';

  $form = array();

  $form['venue_id'] = array(
    '#type' => 'value',
    '#value' => $entity->venue_id,
  );

  // Name is the only required field
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Venue Name'),
    '#default_value' => $entity->name,
    '#required' => 1,
    '#weight' => 0,
  );

  // TODO: Add Organizer.  Select box if there are more than one, otherwise set the default as a value

  $form['address'] = array(
    '#type' => 'textfield',
    '#title' => t('Address Line 1'),
    '#default_value' => $entity->address,
    '#weight' => 1,
  );

  $form['address_2'] = array(
    '#type' => 'textfield',
    '#title' => t('Address Line 2'),
    '#default_value' => $entity->address_2,
    '#weight' => 2,
  );

  $form['city'] = array(
    '#type' => 'textfield',
    '#title' => t('City'),
    '#default_value' => $entity->city,
    '#weight' => 3,
  );

  $form['region'] = array(
    '#type' => 'textfield',
    '#title' => t('Region'),
    '#default_value' => $entity->region,
    '#weight' => 4,
  );

  $form['postal_code'] = array(
    '#type' => 'textfield',
    '#title' => t('Postal Code'),
    '#default_value' => $entity->postal_code,
    '#weight' => 5,
  );

  $form['country_code'] = array(
    '#type' => 'select',
    '#title' => t('Country'),
    '#options' => country_get_list(),
    '#default_value' => $entity->country_code ? $entity->country_code : variable_get('eventbrite_country_code', 'US'),
    '#weight' => 6,
  );

  $form['status'] = array(
    '#type' => 'checkbox',
    '#title' => t('Status'),
    '#default_value' => $entity->status,
    '#weight' => 7,
  );
  
//  field_attach_form('eventbrite_venue', $entity, $form, $form_state);

  if ($op == 'add') {
    // Organizer field required for new venues
    module_load_include('inc', 'eventbrite', 'eventbrite.organizer');
    $form['organizer_id'] = array(
      '#type' => 'select',
      '#title' => 'Organizer',
      '#options' => eventbrite_organizer_get_enabled(),
      '#default_value' => variable_get('eventbrite_organizer_default', ''),
      '#required' => 1,
      '#weight' => 9,
    );
  }

  $submit_value = ($op == 'add') ? t('Create this venue') : t('Update this venue');

  $form['actions'] = array('#type' => 'actions');
	$form['actions']['submit'] = array(
	  '#type' => 'submit',
	  '#value' => $submit_value,
	  '#weight' => 10,
	);

  return $form;
}

/**
 * Form API submit callback for the Eventbrite venue edit form.
 *
 * @see eventbrite_venue_form()
 * @see eventbrite_venue_form_submit()
 */
function eventbrite_venue_form_validate($form, &$form_state) {
  // @todo Check that there isn't already a venue with this name for this user key
  /*
   if ($values['accept_paypal']) {
   // If using PayPay check that paypal email is entered
   if (!valid_email_address($values['paypal_email'])) {
   // TODO: Does this need to actually verified that there is a PayPal account registered to this user
   $form_errors['paypal_email'] = t('Please provide a valid Paypal email.');
   }
   }
   */
}

/**
 * Form API submit callback for the Eventbrite venue edit form.
 *
 * @see eventbrite_venue_form()
 * @see eventbrite_venue_form_validate()
 */
function eventbrite_venue_form_submit($form, &$form_state) {
  $eventbrite_venue = entity_ui_form_submit_build_entity($form, $form_state);

  $eventbrite_venue->save();
  $form_state['redirect'] = 'admin/config/services/eventbrite/venues';
  return;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function eventbrite_venue_options($required = FALSE) {
  $result = db_query('SELECT * FROM {eventbrite_venue}');
  $venue_options = array();
  if (!$required) {
    $venue_options[''] = '--';
  }
  foreach ($result AS $row) {
    $venue_options[$row->venue_id] = $row->name;
  }
  return $venue_options;
}

/*
 function eventbrite_venue_list() {
 // TODO: Get venues from Eventbrite server
 $venues = eventbrite_get('user_list_venues');

 if ($venues) {
 $header = array('Venue ID', 'Edit link', 'Venue Name', 'Address' / * TODO: Add venue editing, 'Edit Link' * /);
 $rows = array();
 foreach ($venues as $venue) {
 $address = $venue->address;
 if (!empty($venue->address2)) {
 $address .= ', '. $venue->address2;
 }
 if (!empty($venue->city)) {
 $address .= ', '. $venue->city;
 }
 if (!empty($venue->region)) {
 $address .= ', '. $venue->region;
 }
 if (!empty($venue->country)) {
 $address .= ', '. $venue->country;
 }
 if (!empty($venue->postal_code)) {
 $address .= ', '. $venue->postal_code;
 }
 $rows[] = array(
 $venue->venue_id,
 l('edit', 'admin/config/eventbrite/venues/'.$venue->venue_id),
 //        l($venue->name, '/admin/settings/eventbrite/view-venue/'. $venue->venue_id),
 $venue->name,
 $address,
 // TODO: Add venue editing back in
 //        l('edit', 'admin/config/eventbrite/list-venues/'. $venue->venue_id),
 );
 }
 return theme('table', $header, $rows);
 }

 return t('There are currently no venues conifigured for this user account.');
 }
 */
///**
// * @todo Please document this function.
// * @see http://drupal.org/node/1354
// */
//function eventbrite_venue_save($venue) {
////  Check to see if this venue already exists on Eventbrite server by venue_id
//  if (isset($venue->venue_id) && !empty($venue->venue_id)) {
//    // This venue already has a venue_id, so venue_update is the API call
//    if ($saved_result = eventbrite_save('venue_update', $venue)) {
//      drupal_set_message(t('This venue has been updated on the Eventbrite server.'));
//    }
//    else {
//      drupal_set_message(t('This venue could not be updated on the Eventbrite server'), 'error');
//      return;
//    }
//  }
//  else {
//    // This venue does not exist, so venue_new is the API call
//    if ($saved_result = eventbrite_save('venue_new', $venue)) {
//      drupal_set_message(t('This venue has been created on the Eventbrite server.'));
//      $venue->venue_id = $saved_result;
//
//      // Call a hook so that submodules know that a new venue got saved
//      module_invoke_all('eventbrite_venue_new', $venue);
//    }
//    else {
//      drupal_set_message(t('This venue could not be created on the Eventbrite server'), 'error');
//      return;
//    }
//  }
//  if ($saved_result) {
//    eventbrite_venue_save_local($venue);
//    return $saved_result;
//  }
//}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function eventbrite_venue_save_local($venue) {
  // Only save locally if there is already an Eventbrite Venue ID
  if (isset($venue->venue_id) && !empty($venue->venue_id)) {
    $result = db_query('SELECT * FROM {eventbrite_venue} WHERE venue_id = :venue_id', array(':venue_id' => $venue->venue_id));
    if ($result) {
      foreach ($result AS $row) {
        // This event already exists in the local DB, so update it
        return drupal_write_record('eventbrite_venue', $venue, array('venue_id'));
      }
    }
    else {
      // This venue does not exist in the local DB, so insert it
      // Set status here
      if (empty($venue->name) || $venue->name == 'TBD' || empty($venue->city)) {
        $venue->status = 0;
      }
      else {
        $venue->status = 1;
      }
      return drupal_write_record('eventbrite_venue', $venue);
    }
  }
}


///*
// * This function pulls venue from the form results and builds a venue object for base Eventbrite module
// */
//function _eventbrite_populate_venue_from_form($item) {
//  // TODO: All this function does is converts array to object
//  $venue = new stdClass();
//
//  if (!empty($item['venue_id'])) {
//    $venue->venue_id = $item['venue_id'];
//  }
//  $venue->name = $item['name'];
//  if (!empty($item['organizer'])) {
//    $venue->organizer = $item['organizer'];
//  }
//  $venue->address = $item['address'];
//  $venue->address2 = $item['address2'];
//  $venue->city = $item['city'];
//  $venue->region = $item['region'];
//  $venue->postal_code = $item['postal_code'];
//  $venue->country = $item['country'];
//  $venue->country_code = $item['country'];
//
//  return $venue;
//}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function eventbrite_venue_get_enabled() {
  // If Eventbrite OG is enabled and there is a group context
  // TODO: This filtering of venues by sub modules should really happen via a hook
  if (module_exists('eventbrite_og') && $group_node = og_get_group_context()) {
    $filter_venues = eventbrite_og_get_enabled_venues($group_node);
  }
  else {
    // If Eventbrite OG is not overriding this field, use default user key for filtering
    $default_venues = eventbrite_get('user_list_venues');
    $filter_venues = array_keys($default_venues);
  }

  $enabled = array();
  $result = db_query('SELECT venue_id, name, address, address_2, city, region FROM {eventbrite_venue} WHERE status = :status ORDER BY name', array(':status' => 1));
  foreach ($result AS $row) {
    if (in_array($row->venue_id, $filter_venues)) {
      $enabled[$row->venue_id] = $row->name;
      if (!empty($row->address)) {
        $enabled[$row->venue_id] .= ', ' . $row->address;
      }
      if (!empty($row->address_2)) {
        $enabled[$row->venue_id] .= ', ' . $row->address_2;
      }
      if (!empty($row->city)) {
        $enabled[$row->venue_id] .= ', ' . $row->city;
      }
      if (!empty($row->region)) {
        $enabled[$row->venue_id] .= ', ' . $row->region;
      }
    }
  }
  return $enabled;
}

/* Enabling venues by city not currently supported
 function eventbrite_venue_get_enabled_bycity($list) {
 $enabled = array();
 $result = db_query('SELECT venue_id, city, region FROM {eventbrite_venue} WHERE status = 1 ORDER by city, region');
 while ($row = db_fetch_object($result)) {
 $enabled[$row->city .', '. $row->region][] = $row->venue_id;
 }
 $enabled_bycity = array();
 foreach ($enabled as $city => $venue_ids) {
 $enabled_bycity[serialize($venue_ids)] = $city;
 }
 return $enabled_bycity;
 }
 */

///**
// * @todo Please document this function.
// * @see http://drupal.org/node/1354
// */
//function eventbrite_venue_list_form($form, $form_state, $user_key = '') {
//  // Set default user key to site wide Eventbrite user key
//  if (empty($user_key)) {
//    $user_key = variable_get('eventbrite_user_key', '');
//    if (empty($user_key)) {
//      return t('No Eventbrite user key has been configured for this account.');
//    }
//  }
//
//  $form = array();
//
//  // This refreshes organizers for the site-wide account
//  if ($venues = eventbrite_get('user_list_venues')) {
//    $countries = country_get_list();
//    $header = array(
//      array(),
//      array(
//        'data' => t('Venue Name'),
//        'field' => 'name',
//      ),
//      array(
//        'data' => t('Address'),
//        'field' => 'address',
//      ),
//      array(
//        'data' => t('City'),
//        'field' => 'city',
//      ),
//      array(
//        'data' => t('Region / Postal Code'),
//        'field' => 'region',
//      ),
//      array(
//        'data' => t('Country'),
//        'field' => 'country',
//      ),
//    );
//
//    $results = db_select('eventbrite_venue', 'ev')
//                ->condition('user_key', $user_key, '=')
//                ->fields('ev', array('venue_id', 'name', 'address', 'address2', 'city', 'region', 'postal_code', 'country'))
//                ->extend('TableSort')->orderByHeader($header)
//                ->extend('PagerDefault')->limit(50)
//                ->execute();
//
//    $status = array(t('blocked'), t('active'));
//    $roles = user_roles(TRUE);
//    $venue_array = array();
//    foreach ($results AS $venue) {
//      $venue_array[$venue->venue_id] = '';
//
//      $form['name'][$venue->venue_id] = array(
//        '#value' => l(
//          $venue->name,
//          'admin/config/eventbrite/venues/' . $venue->venue_id,
//          array('query' => $destination)
//        ),
//      );
//      $form['address'][$venue->venue_id] = array('#value' => $venue->address);
//      if (!empty($venue->address2)) {
//        $form['address'][$venue->venue_id]['#value'] .= '<br/>' . $venue->address2;
//      }
//      $form['city'][$venue->venue_id] = array('#value' => $venue->city);
//      $form['region'][$venue->venue_id] = array('#value' => $venue->region);
//      if (!empty($venue->postal_code)) {
//        $form['region'][$venue->venue_id]['#value'] .= '<br/>' . $venue->postal_code;
//      }
//      $form['country'][$venue->venue_id] = array('#value' => $countries[$venue->country]);
//    }
//
//    $enabled_venues = eventbrite_venue_get_enabled();
//    $form['enabled_venues'] = array(
//      '#type' => 'checkboxes',
//      '#options' => $venue_array,
//      '#default_value' => array_keys($enabled_venues),
//    );
//
//    $form['submit'] = array(
//      '#type' => 'submit',
//      '#value' => t('Save enabled venues'),
//      '#weight' => 100,
//    );
//    $form['pager'] = array('#value' => theme('pager', array('tags' => NULL, 'element' => 0)));
//  }
//
//  return $form;
//}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function eventbrite_venue_list_form_submit($form, &$form_state) {
  $enabled_venues = $form_state['values']['enabled_venues'];

  // Get current status for all venues in DB
  $db_venue_status = array();
  // Make sure DB matches enabled venues list
  // TODO Please convert this statement to the D7 database API syntax.
  $result = db_query('SELECT venue_id, status FROM {eventbrite_venue} ev WHERE 1');
  foreach ($result AS $venue) {
    $db_venues[$venue->venue_id] = $venue;
  }

  // Scroll through venues returned by form.  Since this was a pager query we don't
//   want to affect any venues that weren't listed on this pager page
  foreach ($enabled_venues as $venue_id => $form_venue_status) {
    $form_venue_status = ($form_venue_status) ? 1 : 0;
    // For each venue, if status has changed update the DB
    $db_venue = $db_venues[$venue_id];
    if ($form_venue_status != $db_venue->status) {
      $db_venue->status = $form_venue_status;
      drupal_write_record('eventbrite_venue', $db_venue, array('venue_id'));
      // TODO: Log to watchdog if this doesn't write
    }
  }
}

///**
// * Theme venue list form.
// *
// * @ingroup themeable
// */
//function theme_eventbrite_venue_list_form($variables) {
//  $form = $variables['form'];
//  // TODO Please change this theme call to use an associative array for the $variables parameter.
//  $header = array(
//    theme('table_select_header_cell'),
//    array(
//      'data' => t('Venue Name'),
//      'field' => 'ev.name',
//    ),
//    array(
//      'data' => t('Address'),
//      'field' => 'ev.address',
//    ),
//    array(
//      'data' => t('City'),
//      'field' => 'ev.city',
//    ),
//    array(
//      'data' => t('Region / Postal Code'),
//      'field' => 'ev.region',
//    ),
//    array(
//      'data' => t('Country'),
//      'field' => 'ev.country',
//    ),
//  );
//
//  $output = drupal_render($form['options']);
//  if (isset($form['name']) && is_array($form['name'])) {
//    foreach (element_children($form['name']) as $key) {
//      $rows[] = array(
//        drupal_render($form['enabled_venues'][$key]),
//        drupal_render($form['name'][$key]),
//        drupal_render($form['address'][$key]),
//        drupal_render($form['city'][$key]),
//        drupal_render($form['region'][$key]),
//        drupal_render($form['country'][$key]),
//      );
//    }
//  }
//  else {
//    $rows[] = array(array(
//        'data' => t('No venues were found in this Eventbrite account.'),
//        'colspan' => '7',
//      ));
//  }
//
//  $output .= '<div class="form-item"><label>' . t('Enabled Venues:') . '</label></div>';
//  $output .= theme('table', array('header' => $header, 'rows' => $rows));
//  if ($form['pager']['#value']) {
//    $output .= drupal_render($form['pager']);
//  }
//
//  $output .= drupal_render_children($form);
//
//  return $output;
//}
