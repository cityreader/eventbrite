<?php

/**
 * @file
 * Defines event handling functions
 */

/**
 * Generates the Eventbrite event editing form.
 *
 * @see eventbrite_event_form_validate()
 * @see eventbrite_event_form_submit()
 */
function eventbrite_event_form($form, &$form_state, $entity, $op = 'edit') {
  include_once DRUPAL_ROOT . '/includes/locale.inc';

  $form = array();

  $form_state['eventbrite_event'] = $entity;
  // // Name is the only required field
  // $form['title'] = array(
    // '#type' => 'textfield',
    // '#title' => t('Event title'),
    // '#default_value' => $entity->title,
    // '#required' => 1,
    // '#weight' => -20,
  // );
// 
  // $form['description'] = array(
    // '#type' => 'text_format',
    // '#title' => t('Description'),
    // '#default_value' => $entity->description,
    // '#format' => $entity->format,
    // '#rows' => 15,
    // '#weight' => -19,
  // );

  // $form['privacy'] = array(
    // '#type' => 'checkbox',
    // '#title' => t('Privacy'),
    // '#default_value' => $entity->privacy,
    // '#weight' => -17,
  // );
// 
  // $form['personalized_url'] = array(
    // '#type' => 'textfield',
    // '#title' => t('Personalized URL'),
    // '#default_value' => $entity->personalized_url,
    // '#weight' => -16,
  // );
//   
  // $venue_options = eventbrite_get_options('venue');
// 
  // $form['venue_id'] = array(
    // '#type' => 'select',
    // '#title' => t('Select a venue'),
    // '#options' => $venue_options,
    // '#default_value' => $entity->venue_id,
    // '#weight' => -15,
  // );
// 
  // $organizer_options = eventbrite_get_options('organizer');
//   
  // $form['organizer_id'] = array(
    // '#type' => 'select',
    // '#title' => t('Select an organizer'),
    // '#options' => $organizer_options,
    // '#default_value' => $entity->organizer_id,
    // '#weight' => -14,
  // );
// 
  // $form['capacity'] = array(
    // '#type' => 'textfield',
    // '#title' => t('Capacity'),
    // '#default_value' => $entity->capacity ? $entity->capacity : '',
    // '#description' => t('The maximum number of people who can attend the event.'),
    // '#weight' => -13,
  // );
  
  
  field_attach_form('eventbrite_event', $entity, $form, $form_state);

  $submit_value = ($op == 'add') ? t('Create this event') : t('Update this event');

  $form['actions'] = array('#type' => 'actions');
	$form['actions']['submit'] = array(
	  '#type' => 'submit',
	  '#value' => $submit_value,
	  '#weight' => 10,
	);

  // $form['actions']['draft'] = array(
    // '#type' => 'submit',
    // '#value' => t('Save as draft'),
    // '#weight' => 11,
  // );

//  $form['#validate'][] = 'eventbrite_sale_end_date_validate';
  
  return $form;
}

/**
 * Form API submit callback for the Eventbrite event edit form.
 *
 * @see eventbrite_event_form()
 * @see eventbrite_event_form_submit()
 */
function eventbrite_event_form_validate($form, &$form_state) {
  // if ($form_state['triggering_element']['#value'] == $form['actions']['draft']['#value']) {
    // $form_state['values']['status'] = 0;
  // }
  // elseif ($form_state['triggering_element']['#value'] == $form['actions']['submit']['#value']) {
    // $form_state['values']['status'] = 1;
  // }

}

/**
 * Form API submit callback for the Eventbrite event edit form.
 *
 * @see eventbrite_event_form()
 * @see eventbrite_event_form_validate()
 */
function eventbrite_event_form_submit($form, &$form_state) {
  // $event = entity_ui_form_submit_build_entity($form, $form_state);
  $event = $form_state['eventbrite_event'];
  entity_form_submit_build_entity('eventbrite_event', $event, $form, $form_state);
  $event->save();

  if (empty($form_state['redirect'])) {
    $form_state['redirect'] = eventbrite_event_uri() . '/' . $event->event_id;
  }
}



