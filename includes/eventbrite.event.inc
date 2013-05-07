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

    // Change table column name from "Label" to "Event Name"
    $header[0] = t('Event Name');
    $extra = array(
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

    $extra = array(
      'data'=> $entity->status ? t('Live') : t('Draft'),
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
    '#format' => $entity->description_format,
    '#rows' => 15,
    '#weight' => -19,
  );

  module_load_include('inc', 'date_api', 'date_api_elements');
  // $timezone = date_get_timezone($field['settings']['tz_handling'], isset($items[0]['timezone']) ? $items[0]['timezone'] : date_default_timezone());
  $timezone = date_default_timezone();

  $form['date'] = array(
    '#title' => t('Date'),
    '#type' => 'fieldset',
    '#validator' => array('eventbrite_date_combo_validate'),
    '#weight' => -18,
    '#element_validate' => array('eventbrite_date_combo_validate'),
  );

  $form['date']['start_date'] = array(
    '#title' => t('Start'),
    '#description' => t('The date and time when event start.'),
    '#type' => 'date_popup',
    '#date_type' => DATE_DATETIME,
    '#date_timezone' => $timezone,
    '#date_format' => 'd/m/Y - H:i',
    '#date_increment' => 1,
    '#date_year_range' => '0:+1',
    '#default_value' => $entity->start_date ? $entity->start_date : eventbrite_default_date(array('month' => 1)),
    '#required' => TRUE,
  );

  $form['date']['end_date'] = array(
    '#title' => t('End'),
    '#description' => t('The date and time when event end.'),
    '#type' => 'date_popup',
    '#date_type' => DATE_DATETIME,
    '#date_timezone' => $timezone,
    '#date_format' => 'd/m/Y - H:i',
    '#date_increment' => 1,
    '#date_year_range' => '0:+1',
    '#default_value' => $entity->end_date ? $entity->end_date : eventbrite_default_date(array('month' => 1, 'hour' => 2)),
    '#required' => TRUE,
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
    '#default_value' => $entity->capacity ? $entity->capacity : '',
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
  if ($form_state['triggering_element']['#value'] == $form['actions']['draft']['#value']) {
    $form_state['values']['status'] = 0;
  }
  elseif ($form_state['triggering_element']['#value'] == $form['actions']['submit']['#value']) {
    $form_state['values']['status'] = 1;
  }

}

/**
 * Form API submit callback for the Eventbrite event edit form.
 *
 * @see eventbrite_event_form()
 * @see eventbrite_event_form_validate()
 */
function eventbrite_event_form_submit($form, &$form_state) {
  $eventbrite_event = entity_ui_form_submit_build_entity($form, $form_state);
 
  if (TRUE) {

  }
  
  $eventbrite_event->save();
  $form_state['redirect'] = 'admin/config/services/eventbrite/events';
  return;
}



