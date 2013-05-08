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
    '#date_format' => 'd/m/Y - H:i:s',
    '#date_increment' => 1,
    '#date_year_range' => '0:+1',
    '#default_value' => isset($item['start_date']) ? $item['start_date'] : eventbrite_default_date(array('month' => 1)),
  );

  $element['date']['end_date'] = array(
    '#title' => t('Sale Ends'),
    '#type' => 'date_popup',
    '#date_type' => DATE_DATETIME,
    '#date_timezone' => $timezone,
    '#date_format' => 'd/m/Y - H:i:s',
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

  $element['order_limit'] = array(
    '#title' => t('Tickets permitted per order'),
    '#type' => 'fieldset',
    '#parents' => array($field['field_name'], $langcode, $delta),
  );

  $element['order_limit']['min'] = array(
    '#title' => t('Minimum'),
    '#description' => t('The minimum number of tickets per order.'),
    '#type' => 'textfield',
    '#default_value' => isset($item['min']) ? $item['min'] : 1,
  );

  $element['order_limit']['max'] = array(
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
  
  $parents = array($field['field_name'], $langcode, $delta);
  
  $element['remove_button'] = array(
    '#name' => implode('_', $parents) . '_remove_button',
    '#delta' => $delta,
    '#type' => 'submit',
    '#value' => t('Remove'),
    '#validate' => array(),
    '#submit' => array('eventbrite_ticket_remove_submit'),
    '#limit_validation_errors' => array(),
    '#ajax' => array(
      'path' => 'eventbrite/ajax',
      'wrapper' => 'tickets-add-more-wrapper',
    ),
  );
  
  return $element;
}

/**
 * Implements hook_field_formatter_info().
 */
function eventbrite_field_formatter_info() {
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

function eventbrite_ticket_remove_js() {
  // drupal_html_id() very helpfully ensures that all html IDS are unique
  // on a page. Unfortunately what it doesn't realize is that the IDs
  // we are generating are going to replace IDs that already exist, so
  // this actually works against us.
  if (isset($_POST['ajax_html_ids'])) {
    unset($_POST['ajax_html_ids']);
  }
    
  list($form, $form_state) = ajax_get_form();
  drupal_process_form($form['#form_id'], $form, $form_state);

  // Get the information on what we're removing.
  $button = $form_state['triggering_element'];
  // Go two levels up in the form, to the whole widget.
  $element = drupal_array_get_nested_value($form, array_slice($button['#array_parents'], 0, -3));
  // Now send back the proper AJAX command to replace it.
  $return = array(
    '#type' => 'ajax',
    '#commands' => array(
      ajax_command_replace('#' . $element['#id'], drupal_render($element))
    ),
  );

  // Because we're doing this ourselves, messages aren't automatic. We have
  // to add them.
  $messages = theme('status_messages');
  if ($messages) {
    $return['#commands'][] = ajax_command_prepend('#' . $element['#id'], $messages);
  }

  return $return;
}

function eventbrite_ticket_remove_submit($form, &$form_state) {
  $button = $form_state['triggering_element'];
  $delta = $button['#delta'];

  // Where in the form we'll find the parent element.
  $address = array_slice($button['#array_parents'], 0, -2);
  
  // Go one level up in the form, to the widgets container.
  $parent_element = drupal_array_get_nested_value($form, $address);
  $field_name = $parent_element['#field_name'];
  $langcode = $parent_element['#language'];
  $parents = $parent_element['#field_parents'];

  $field_state = field_form_get_state($parents, $field_name, $langcode, $form_state);

  // Go ahead and renumber everything from our delta to the last
  // item down one. This will overwrite the item being removed.
  for ($i = $delta; $i < $field_state['items_count']; $i++) {
    $old_element_address = array_merge($address, array($i + 1));
    $new_element_address = array_merge($address, array($i));

    $moving_element = drupal_array_get_nested_value($form, $old_element_address);
    $moving_element_value = drupal_array_get_nested_value($form_state['values'], $old_element_address);
    $moving_element_input = drupal_array_get_nested_value($form_state['input'], $old_element_address);

    // Tell the element where it's being moved to.
    $moving_element['#parents'] = $new_element_address;

    // Move the element around.
    form_set_value($moving_element, $moving_element_value, $form_state);
    drupal_array_set_nested_value($form_state['input'], $moving_element['#parents'], $moving_element_input);

    // Move the entity in our saved state.
    //$field_state['entity'][$i] = $field_state['entity'][$i + 1];
  }

  // For the final item there is nothing to move into its place, so remove it.
  $i = $field_state['items_count'];
  $removed_element_address = array_merge($address, array($i));
  $removed_element = drupal_array_get_nested_value($form, $removed_element_address);
  form_set_value($removed_element, NULL, $form_state);
  drupal_array_set_nested_value($form_state['input'], $removed_element['#parents'], NULL);

  // Replace the deleted entity with an empty one. This helps to ensure that
  // trying to add a new entity won't ressurect a deleted entity from the
  // trash bin.
//  $field_state['entity'][$field_state['items_count']] = entity_create('field_collection_item', array('field_name' => $field_name));

  // Then remove the last item. But we must not go negative.
  if ($field_state['items_count'] > 0) {
    $field_state['items_count']--;
  }

  // Fix the weights. Field UI lets the weights be in a range of
  // (-1 * item_count) to (item_count). This means that when we remove one,
  // the range shrinks; weights outside of that range then get set to
  // the first item in the select by the browser, floating them to the top.
  // We use a brute force method because we lost weights on both ends
  // and if the user has moved things around, we have to cascade because
  // if I have items weight weights 3 and 4, and I change 4 to 3 but leave
  // the 3, the order of the two 3s now is undefined and may not match what
  // the user had selected.
  $input = drupal_array_get_nested_value($form_state['input'], $address);
  // Sort by weight
  uasort($input, '_field_sort_items_helper');

  // Reweight everything in the correct order.
  $weight = -1 * $field_state['items_count'];
  foreach ($input as $key => $item) {
    if ($item) {
      $input[$key]['_weight'] = $weight++;
    }
  }

  drupal_array_set_nested_value($form_state['input'], $address, $input);

  field_form_set_state($parents, $field_name, $langcode, $form_state, $field_state);

  $form_state['rebuild'] = TRUE;
}

