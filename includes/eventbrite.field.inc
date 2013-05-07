<?php

/**
 * @file
 * Defines a Eventbrite ticket field type.
 */

/**
 * Implements hook_field_info().
 */
function eventbrite_field_info() {
  return array(
    'eventbrite_ticket' => array(
      'label' => t('Eventbrite ticket'),
      'description' => t('Eventbrite ticket field'),
      // 'settings' => array(
      //   'max_length' => 10
      // ),
      'default_widget' => 'eventbrite_ticket_widget',
      'default_formatter' => 'eventbrite_ticket_formatter',
    ),
  );
}

/**
 * Implements hook_field_widget_info().
 */
function eventbrite_field_widget_info() {
  return array(
    'eventbrite_ticket_widget' => array(
      'label' => t('Eventbrite ticket'),
      'field types' => array('eventbrite_ticket'),
      // 'settings' => array('size' => 60),
      // 'behaviors' => array(
      //   'multiple values' => FIELD_BEHAVIOR_DEFAULT,
      //   'default value' => FIELD_BEHAVIOR_DEFAULT,
      // ),
    ),
  );
}

/**
 * Implements hook_field_widget_form().
 */
function eventbrite_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  // $element += array(
  //   '#type' => $instance['widget']['type'],
  //   '#default_value' => isset($items[$delta]) ? $items[$delta] : '',
  // );
  $item =& $items[$delta];

  // $element['type'] = array(
    // '#title' => t('Ticket type'),
    // '#type' => 'checkboxes',
    // '#default_value' => isset($item['type']) ? $item['type'] : NULL,
    // '#options' => array(
      // 0 => t('Free'),
      // 1 => t('Paid'),
      // 2 => t('Donation'),
    // ),
  // );

  $element['ticket_id'] = array(
    '#type' => 'hidden',
    '#default_value' => isset($item['ticket_id']) ? $item['ticket_id'] : 0,
  );

  $element['is_donation'] = array(
    '#title' => t('Is donation'),
    '#description' => t('0 for fixed-price tickets, 1 for donations. 0 will be used by default if not provided.'),
    '#type' => 'radios',
    '#options' => array(
      0 => 'No',
      1 => 'Yes',
    ),
    '#default_value' => isset($item['is_donation']) ? $item['is_donation'] : 0,
  );

  $element['name'] = array(
    '#title' => t('Ticket name'),
    '#type' => 'textfield',
    '#default_value' => isset($item['name']) ? $item['name'] : '',
  );

  $element['description'] = array(
    '#title' => t('Ticket description'),
    '#type' => 'textarea',
    '#default_value' => isset($item['description']) ? $item['description'] : '',
  );

  $element['price'] = array(
    '#title' => t('Ticket Price'),
    '#type' => 'textfield',
    '#default_value' => isset($item['price']) ? $item['price'] : '',
    '#attributes' => array(
      'placeholder' => '0.00',
    ),
  );

  $element['quantity_available'] = array(
    '#title' => t('Ticket Quantity'),
    '#description' => t('The number of tickets available. Not required for donations.'),
    '#type' => 'textfield',
    '#default_value' => isset($item['quantity_available']) ? $item['quantity_available'] : '',
    '#attributes' => array(
      'placeholder' => '100',
    ),
  );

  module_load_include('inc', 'date_api', 'date_api_elements');
  // $timezone = date_get_timezone($field['settings']['tz_handling'], isset($items[0]['timezone']) ? $items[0]['timezone'] : date_default_timezone());
  $timezone = date_default_timezone();

  $element['date'] = array(
    '#title' => t('Date & Time'),
    '#type' => 'fieldset',
    '#parents' => array($field['field_name'], $langcode, $delta),
    '#element_validate' => array('eventbrite_date_combo_validate'),
  );
  
  $element['date']['start_date'] = array(
    '#title' => t('Sale Starts'),
    '#type' => 'date_popup',
    '#date_type' => DATE_DATETIME,
    '#date_timezone' => $timezone,
    '#date_format' => 'd/m/Y - H:i',
    '#date_increment' => 1,
    '#date_year_range' => '0:+1',
    '#default_value' => isset($item['start_date']) ? $item['start_date'] : eventbrite_default_date(array('month' => 1)),
  );

  $element['date']['end_date'] = array(
    '#title' => t('Sale Ends'),
    '#type' => 'date_popup',
    '#date_type' => DATE_DATETIME,
    '#date_timezone' => $timezone,
    '#date_format' => 'd/m/Y - H:i',
    '#date_increment' => 1,
    '#date_year_range' => '0:+1',
    '#default_value' => isset($item['end_date']) ? $item['end_date'] : eventbrite_default_date(array('month' => 1, 'hour' => 2)),
  );
  
  $element['include_fee'] = array(
    '#title' => t('Including service fees'),
    '#type' => 'radios',
    '#options' => array(
      0 => t('Add the Eventbrite service fee on top of ticket price'),
      1 => t('Include it in the ticket price'),
    ),
    '#default_value' => isset($item['include_fee']) ? $item['include_fee'] : 0,
  );

  $element['tickets_per_order'] = array(
    '#title' => t('Tickets permitted per order'),
    '#type' => 'fieldset',
    '#parents' => array($field['field_name'], $langcode, $delta),
  );

  $element['tickets_per_order']['min'] = array(
    '#title' => t('Minimum'),
    '#description' => t('The minimum number of tickets per order.'),
    '#type' => 'textfield',
    '#default_value' => isset($item['min']) ? $item['min'] : 1,
  );

  $element['tickets_per_order']['max'] = array(
    '#title' => t('Maximum'),
    '#description' => t('The maximum number of tickets per order.'),
    '#type' => 'textfield',
    '#default_value' => isset($item['max']) ? $item['max'] : NULL,
  );

  $element['hide'] = array(
    '#title' => t('Auto-hide ticket'),
    '#prefix' => '<label>' . t('Ticket Visibility') . '</label>',
    '#type' => 'checkbox',
    '#default_value' => isset($item['hide']) ? $item['hide'] : 0,
  );
  
  // $element['remove_button'] = array(
    // '#name' => implode('_', array($field['field_name'], $langcode, $delta)) . '_remove_button',
    // '#type' => 'submit',
    // '#value' => t('Remove'),
    // '#validate' => array(),
    // '#submit' => array('eventbrite_field_ticket_submit'),
    // '#limit_validation_errors' => array(array($field['field_name'], $langcode)),
    // '#ajax' => array(
      // 'path' => 'file/ajax/' . implode('/', array($field['field_name'], $langcode)) . '/' . $form['form_build_id']['#value'],
      // 'wrapper' => $element['#id'] . '-ajax-wrapper',
    // ),
  // );
  
  drupal_add_js(drupal_get_path('module', 'eventbrite') . '/js/eventbrite.ticket.js');
  
  $element['remove_button'] = array(
    '#type' => 'markup',
    '#markup' => '<a href="#" class="form-submit form-submit-remove eventbrite_ticket_remove">Remove</a>',
    // '#attached' => array(
      // 'js' => drupal_get_path('module', 'eventbrite') . '/js/eventbrite.ticket.js',
    // ),
  );
  
  return $element;
}

/**
 * Implements hook_field_formatter_info().
 */
function hook_field_formatter_info() {
  return array(
    'eventbrite_ticket_formatter' => array(
      'label' => t('Default'),
      'field types' => array('eventbrite_ticket'),
    ),
  );
}




/**
 * Implements hook_form_FORM_ID_alter().
 */
// function eventbrite_form_field_ui_field_edit_form_alter(&$form, &$form_state, $form_id) {
  // //we dont want users change the field size from 10.
  // $form['instance']['widget']['settings']['size']['#type'] = 'value';
  // $form['instance']['widget']['settings']['size']['#value'] = '10';
// }

/**
 * Implements hook_field_settings_form().
 */
function eventbrite_field_settings_form($field, $instance, $has_data) {
  //don't do anything for now
  /*
  $form['max_length'] = array(
    '#title' => t('Time zones to be excluded from the option list'),
    '#type' => 'select',
    '#options' => eventbrite_identifiers_list(),
    '#default_value' => $field['settings']['exclude'],
    '#multiple' => TRUE,
    '#required' => FALSE,
    '#description' => t('Any time zones selected here will be excluded from the allowed values.'),
  );
  return $form;
  */
}

/**
 * Implements hook_field_validate().
 * 
 * Validate
 *  - start_date
 *  - end_date
 *  - max
 *  - quantity_available
 */
function eventbrite_field_validate($entity_type, $entity, $field, $instance, $langcode, $items, &$errors) {
  //no validation for now. We need to add a validator to check to see if the event exists or not yet.

  foreach ($items as $delta => $item) {
    if (!empty($item['quantity_available'])) {
        $errors[$field['quantity_available']][$langcode][$delta][] = array(
          'error' => 'eventbrite_field_quantity_available_required',
          'message' => t('%name is required.', array('%name' => $instance['label'])),
        );
    }
  }
  return $items;
}

/**
 * Implements hook_field_is_empty().
 */
function eventbrite_field_is_empty($item, $field) {
  return empty($item['name']);
}

/**
 * Implements hook_field_presave()
 * 
 * Sanitize field data and remove empty values
 */
function eventbrite_field_presave($entity_type, $entity, $field, $instance, $langcode, &$items) {
  $array = array(
    'price',
    'min',
    'max',
    'quantity_available',
  );
  
  foreach ($items as $delta=>$item) {
    foreach ($array as $attribute) {
      if (!is_numeric($item[$attribute]) && empty($item[$attribute])) {
        $items[$delta][$attribute] = 0;
      }
    }
  }
}



// /**
 // * Implements hook_options_list().
 // */
// function eventbrite_options_list($field) {
  // return empty($field['settings']['exclude']) ? eventbritefield_identifiers_list() : array_diff(eventbritefield_identifiers_list(), $field['settings']['exclude']);
// }

// /**
 // * Fetch a default array of excluded time zones for use when creating a time zone field.
 // * See http://www.php.net/manual/en/timezones.others.php
 // */
// function eventbrite_excluded_timezones() {
  // return preg_grep(';^((Africa|America|Antarctica|Arctic|Asia|Atlantic|Australia|Europe|Indian|Pacific)/.*|UTC)$;', eventbritefield_identifiers_list(), PREG_GREP_INVERT);
// }

// /**
 // * Cache the time zone identifiers list as a static variable.
 // */
// function eventbritefield_identifiers_list() {
  // static $list;
  // if (empty($list)) {
    // $list = drupal_map_assoc(timezone_identifiers_list());
  // }
  // return $list;
// }

/**
 * Implements hook_field_formatter_view().
 */
function eventbritefield_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();
  switch ($display['type']) {
    // case 'eventbrite_iframe':
      // // Try to load the library and check if that worked.
      // //$library = libraries_load('eventbrite');
      // //error_log(print_r($library,true));
      // if (($library = libraries_load('eventbrite')) && !empty($library['loaded'])) {
        // // Do something with the library here.
        // $eb_client = new Eventbrite( array('app_key' => variable_get('eventbrite_app_key','') ,
                                           // 'user_key' =>variable_get('eventbrite_user_key','')));
        // foreach ($items as $delta => $item) {
          // // Give us an embedded iFrame of the eventbrite page
          // try {
            // $resp = $eb_client->event_get( array('id' => $item['value']) );
            // $element[$delta] = array('#markup' => Eventbrite::ticketWidget($resp->event, eventbritefield_calcualate_iframe($resp->event)));
          // } catch (Exception $e) {
            // //lets not do anything for now.
          // }
        // }
      // }
// 
      // break;
    // case 'eventbrite_url':
      // foreach ($items as $delta => $item) {
        // //print out a link going to the site
        // $element[$delta] = array('#markup' => $item['value'] ? eventbritefield_field_url($item['value']) : '');
      // }
      // break;
  }
  return $element;
}

// /*
 // * Lets do some funky iframe math based on what we know as our solid grid. Yah this is horrible for many reasons, but
 // * at least we get a semi-functional iframe and not something with a bunch of white space or scroll bars
 // */
// function eventbritefield_calcualate_iframe($event) {
  // $cost = 0;
  // $base_size = 140;
  // foreach($event->tickets AS $tickets) {
    // $ticket = $tickets->ticket;
    // if($ticket->visible == 'true') {
      // $desc_size = 0;
      // $title_size = 0;
      // $cost += $ticket->price;
// 
      // if($ticket->description != '') {
        // //the magic really happens here -- we estimate on a full length page, a description can go 12 words before wrapping
        // //make smaller if your eventbrite section is smaller. We really should make this dynamic.
        // $desc_size = ceil((int)count(explode(' ', $ticket->description)) / 12);
      // }
      // //like description, this too has a decent size for a full width page. 6 should be enough before wrapping
      // $title_size = ceil((int)count(explode(' ', $ticket->name)) / 6);
// 
      // //all line items need at least 30px. We take away one title size to count for the 30px. Descriptions and additional titles are 10px ea.
      // $base_size += 42 + ($desc_size * 10) + (($title_size-1) * 10);
    // }
  // }
  // //if the event is not free, there is an additional footer that costs 50px.
  // if($cost > 0) {
    // $base_size += 50;
  // }
  // return $base_size . 'px';
// }
// /**
 // * @param $item_id Eventbrite ID
 // * Give us a URL (customized by settings for field) that links to an eventbrite page
 // */
// function eventbritefield_field_url($item_id) {
  // return '<a href="http://www.eventbrite.com/event/'.$item_id.'">Register Now</a>';
// }

/**
 * Implements hook_field_formatter_settings_form().
 */
function eventbrite_field_formatter_settings_form($field, $instance, $view_mode) {
  $form = array();
  /* USe this for custom URL settings when we get there....
  if ($instance['display'][$view_mode]['type'] == 'eventbrite_url') {
    $form['format'] = array(
      '#title' => t('Format'),
      '#type' => 'textfield',
      '#size' => 20,
      '#default_value' => $instance['display'][$view_mode]['settings']['format'],
      '#description' => t('Enter a <a href="http://www.php.net/manual/en/function.date.php">PHP date format string</a>, e.g. <em>T</em> to display the current time zone abbreviation.'),
    );
  }
  return $form;
  */
}

/**
 * Implements hook_field_formatter_settings_summary().
 * Use this to select the format settings for URLS or iFrames

function eventbrite_field_formatter_settings_summary($field, $instance, $view_mode) {
  $summary = '';
  if ($instance['display'][$view_mode]['type'] == 'eventbrite_url') {
    $summary = t('Format string: %format<br />Sample date: %date', array(
      '%format' => $instance['display'][$view_mode]['settings']['format'],
      '%date' => date_create()->format($instance['display'][$view_mode]['settings']['format']),
    ));
  }
  return $summary;
}
 */

/**
 * Implements hook_field_widget_error().
 */
function eventbrite_field_widget_error($element, $error, $form, &$form_state) {
  form_error($element, $error['message']);
}

function eventbrite_field_ajax() {
  $form_parents = func_get_args();
  $form_build_id = (string) array_pop($form_parents);

  if (empty($_POST['form_build_id']) || $form_build_id != $_POST['form_build_id']) {
    // Invalid request.
    drupal_set_message(t('Ticket error'), 'error');
    $commands = array();
    $commands[] = ajax_command_replace(NULL, theme('status_messages'));
    return array('#type' => 'ajax', '#commands' => $commands);
  }

  list($form, $form_state) = ajax_get_form();

  if (!$form) {
    // Invalid form_build_id.
    drupal_set_message(t('An unrecoverable error occurred. Use of this form has expired. Try reloading the page and submitting again.'), 'error');
    $commands = array();
    $commands[] = ajax_command_replace(NULL, theme('status_messages'));
    return array('#type' => 'ajax', '#commands' => $commands);
  }

  // Get the current element and count the number of files.
  $current_element = $form;
  foreach ($form_parents as $parent) {
    $current_element = $current_element[$parent];
  }
  // $current_file_count = isset($current_element['#file_upload_delta']) ? $current_element['#file_upload_delta'] : 0;

  // Process user input. $form and $form_state are modified in the process.
  drupal_process_form($form['#form_id'], $form, $form_state);

  // Retrieve the element to be rendered.
  foreach ($form_parents as $parent) {
    $form = $form[$parent];
  }

  // Add the special Ajax class if a new file was added.
  if (isset($form['#file_upload_delta']) && $current_file_count < $form['#file_upload_delta']) {
    $form[$current_file_count]['#attributes']['class'][] = 'ajax-new-content';
  }
  // Otherwise just add the new content class on a placeholder.
  else {
    $form['#suffix'] .= '<span class="ajax-new-content"></span>';
  }

  $output = theme('status_messages') . drupal_render($form);
  $js = drupal_add_js();
  $settings = call_user_func_array('array_merge_recursive', $js['settings']['data']);

  $commands = array();
  $commands[] = ajax_command_replace(NULL, $output, $settings);
  return array('#type' => 'ajax', '#commands' => $commands);
}

function eventbrite_field_ticket_submit($form, &$form_state) {
  // Determine whether it was the upload or the remove button that was clicked,
  // and set $element to the managed_file element that contains that button.
  $parents = $form_state['triggering_element']['#array_parents'];
  $button_key = array_pop($parents);
  $element = drupal_array_get_nested_value($form, $parents);

  // No action is needed here for the upload button, because all file uploads on
  // the form are processed by file_managed_file_value() regardless of which
  // button was clicked. Action is needed here for the remove button, because we
  // only remove a file in response to its remove button being clicked.
  if ($button_key == 'remove_button') {
    // Update both $form_state['values'] and $form_state['input'] to reflect
    // that the file has been removed, so that the form is rebuilt correctly.
    // $form_state['values'] must be updated in case additional submit handlers
    // run, and for form building functions that run during the rebuild, such as
    // when the managed_file element is part of a field widget.
    // $form_state['input'] must be updated so that file_managed_file_value()
    // has correct information during the rebuild.
    $values_element = $element['#extended'] ? $element['fid'] : $element;
    form_set_value($values_element, NULL, $form_state);
    drupal_array_set_nested_value($form_state['input'], $values_element['#parents'], NULL);
  }

  // Set the form to rebuild so that $form is correctly updated in response to
  // processing the file removal. Since this function did not change $form_state
  // if the upload button was clicked, a rebuild isn't necessary in that
  // situation and setting $form_state['redirect'] to FALSE would suffice.
  // However, we choose to always rebuild, to keep the form processing workflow
  // consistent between the two buttons.
  $form_state['rebuild'] = TRUE;
}


