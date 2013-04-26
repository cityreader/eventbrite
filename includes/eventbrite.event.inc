<?php

/**
 * @file
 * Defines event handling functions
 */

/**
 * UI controller.
 */
class EventbriteEventUIController extends EntityDefaultUIController {

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
//    eventbrite_sync('user_list_venues');

    $form = parent::overviewForm($form, $form_state);
    return $form;
  }

  /**
   * Overrides EntityDefaultUIController::overviewTableHeaders().
   */
  protected function overviewTableHeaders($conditions, $rows, $additional_header = array()) {
    $header = parent::overviewTableHeaders($conditions, $rows, $additional_header);

    // Change table column name from "Label" to "Organizer Name"
    $header[0] = t('Event Name');
    $extra = array(
      t('Address'),
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
    array_splice($row, 1, 0, $extra);
    
    return $row;
  }
}

/**
 * Generates the Eventbrite event editing form.
 *
 * @see eventbrite_event_form_validate()
 * @see eventbrite_event_form_submit()
 */
function eventbrite_event_form($form, &$form_state, $entity, $op = 'edit') {
  include_once DRUPAL_ROOT . '/includes/locale.inc';

  $form = array();

  // $form['venue_id'] = array(
    // '#type' => 'value',
    // '#value' => $entity->venue_id,
  // );
// 
  // // Name is the only required field
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Event title'),
    '#default_value' => $entity->title,
    '#required' => 1,
    '#weight' => -20,
  );
  
  $form['description'] = array(
    '#type' => 'text_format',
    '#title' => t('Description'),
    '#default_value' => $entity->description,
    '#rows' => 15,
    '#weight' => -19,
  );

  module_load_include('inc', 'date_api', 'date_api_elements');
  // $timezone = date_get_timezone($field['settings']['tz_handling'], isset($items[0]['timezone']) ? $items[0]['timezone'] : date_default_timezone());
  $timezone = date_default_timezone();

  $form['date'] = array(
    '#title' => t('Date'),
    '#type' => 'fieldset',
    '#validator' => array('eventbrite_date_combo_validat'),
    '#weight' => -18,
  );

  $form['date']['start_date'] = array(
    '#title' => t('Start'),
    '#description' => t('The date and time when event start.'),
    '#type' => 'date_popup',
    '#date_type' => DATE_DATETIME,
    '#date_timezone' => $timezone,
    '#date_format' => 'm/d/Y - H:i',
    '#date_increment' => 1,
    '#date_year_range' => '0:+1',
    '#default_value' => $entity->start_date,
  );

  $form['date']['end_date'] = array(
    '#title' => t('End'),
    '#description' => t('The date and time when event end.'),
    '#type' => 'date_popup',
    '#date_type' => DATE_DATETIME,
    '#date_timezone' => $timezone,
    '#date_format' => 'm/d/Y - H:i',
    '#date_increment' => 1,
    '#date_year_range' => '0:+1',
    '#default_value' => $entity->end_date,
  );

  $form['privacy'] = array(
    '#type' => 'checkbox',
    '#title' => t('Privacy'),
    '#default_value' => $entity->privacy,
    '#weight' => -17,
  );

  $form['personalized_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Personalized URL'),
    '#default_value' => $entity->personalized_url,
    '#weight' => -16,
  );
  
  $venue_options = eventbrite_get_options('venue');

  $form['venue_id'] = array(
    '#type' => 'select',
    '#title' => t('Select a venue'),
    '#options' => $venue_options,
    '#default_value' => $entity->venue_id,
    '#weight' => -15,
  );

  $organizer_options = eventbrite_get_options('organizer');
  
  $form['organizer_id'] = array(
    '#type' => 'select',
    '#title' => t('Select an organizer'),
    '#options' => $organizer_options,
    '#default_value' => $entity->organizer_id,
    '#weight' => -14,
  );

  $form['capacity'] = array(
    '#type' => 'textfield',
    '#title' => t('Capacity'),
    '#default_value' => $entity->capacity,
    '#description' => t('The maximum number of people who can attend the event.'),
    '#weight' => -13,
  );
  
  
  field_attach_form('eventbrite_event', $entity, $form, $form_state);

  $submit_value = ($op == 'add') ? t('Create this event') : t('Update this event');

  $form['actions'] = array('#type' => 'actions');
	$form['actions']['submit'] = array(
	  '#type' => 'submit',
	  '#value' => $submit_value,
	  '#weight' => 10,
	);

  $form['actions']['draft'] = array(
    '#type' => 'submit',
    '#value' => t('Save as draft'),
    '#weight' => 11,
  );

  return $form;
}

/**
 * Form API submit callback for the Eventbrite event edit form.
 *
 * @see eventbrite_event_form()
 * @see eventbrite_event_form_submit()
 */
function eventbrite_event_form_validate($form, &$form_state) {
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
 * Form API submit callback for the Eventbrite event edit form.
 *
 * @see eventbrite_event_form()
 * @see eventbrite_event_form_validate()
 */
function eventbrite_event_form_submit($form, &$form_state) {
  // $eventbrite_venue = entity_ui_form_submit_build_entity($form, $form_state);
// 
  // $eventbrite_venue->save();
  // $form_state['redirect'] = 'admin/config/services/eventbrite/venues';
  return;
}



