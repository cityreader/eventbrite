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
  // $form['name'] = array(
    // '#type' => 'textfield',
    // '#title' => t('Venue Name'),
    // '#default_value' => $entity->name,
    // '#required' => 1,
    // '#weight' => 0,
  // );
// 
  // // TODO: Add Organizer.  Select box if there are more than one, otherwise set the default as a value
// 
  // $form['address'] = array(
    // '#type' => 'textfield',
    // '#title' => t('Address Line 1'),
    // '#default_value' => $entity->address,
    // '#weight' => 1,
  // );
// 
  // $form['address_2'] = array(
    // '#type' => 'textfield',
    // '#title' => t('Address Line 2'),
    // '#default_value' => $entity->address_2,
    // '#weight' => 2,
  // );
// 
  // $form['city'] = array(
    // '#type' => 'textfield',
    // '#title' => t('City'),
    // '#default_value' => $entity->city,
    // '#weight' => 3,
  // );
// 
  // $form['region'] = array(
    // '#type' => 'textfield',
    // '#title' => t('Region'),
    // '#default_value' => $entity->region,
    // '#weight' => 4,
  // );
// 
  // $form['postal_code'] = array(
    // '#type' => 'textfield',
    // '#title' => t('Postal Code'),
    // '#default_value' => $entity->postal_code,
    // '#weight' => 5,
  // );
// 
  // $form['country_code'] = array(
    // '#type' => 'select',
    // '#title' => t('Country'),
    // '#options' => country_get_list(),
    // '#default_value' => $entity->country_code ? $entity->country_code : variable_get('eventbrite_country_code', 'US'),
    // '#weight' => 6,
  // );
// 
// //  field_attach_form('eventbrite_venue', $entity, $form, $form_state);
// 
  // if ($op == 'add') {
    // // Organizer field required for new venues
    // module_load_include('inc', 'eventbrite', 'eventbrite.organizer');
    // $form['organizer_id'] = array(
      // '#type' => 'select',
      // '#title' => 'Organizer',
      // '#options' => eventbrite_organizer_get_enabled(),
      // '#default_value' => variable_get('eventbrite_organizer_default', ''),
      // '#required' => 1,
      // '#weight' => 9,
    // );
  // }
// 
  $submit_value = ($op == 'add') ? t('Create this event') : t('Update this event');


  field_attach_form('eventbrite_event', $entity, $form, $form_state);

  $form['actions'] = array('#type' => 'actions');
	$form['actions']['submit'] = array(
	  '#type' => 'submit',
	  '#value' => $submit_value,
	  '#weight' => 10,
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

