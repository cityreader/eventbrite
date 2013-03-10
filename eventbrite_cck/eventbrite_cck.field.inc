<?php

/**
 * @file
 * CCK hook_field and hook_field_settings functions for Eventbrite CCK module
 *
 */

/**
 * Implementation of CCK's hook_field_settings($op = 'database columns').
 */
function _eventbrite_cck_field_settings_db_columns($field) {
  // Define the database storage for this field

  $columns['ebid'] = array(
    'type' => 'int',
    'unsigned' => TRUE,
    'size' => 'big',
    'not null' => FALSE,
    'default' => 0,
    'sortable' => TRUE,
    'views' => TRUE,
  );

  $columns['title'] = array(
    'type' => 'varchar',
    'length' => 255,
    'not null' => FALSE,
    'default' => '',
    'sortable' => TRUE,
    'views' => TRUE,
  );

  $columns['status'] = array(
    'type' => 'varchar',
    'length' => 8,
    'default' => '',
    'views' => TRUE,
    'not null' => FALSE,
  );
  $columns['description'] = array(
    'type' => 'text',
    'default' => '',
    'views' => TRUE,
    'not null' => FALSE,
  );

  $columns['start_date'] = array(
    'type' => 'datetime',
    'not null' => FALSE,
    'sortable' => TRUE,
    'views' => TRUE,
  );

  $columns['end_date'] = array(
    'type' => 'datetime',
    'not null' => FALSE,
    'sortable' => TRUE,
    'views' => TRUE,
  );

  $columns['timezone'] = array(
    'type' => 'varchar',
    'length' => 50,
    'not null' => FALSE,
    'sortable' => FALSE,
    'views' => TRUE,
  );
  $columns['organizer'] = array(
    'type' => 'int',
    'unsigned' => TRUE,
    'not null' => FALSE,
    'default' => 0,
    'sortable' => TRUE,
    'views' => TRUE,
  );
  $columns['venue_id'] = array(
    'type' => 'int',
    'unsigned' => TRUE,
    'not null' => FALSE,
    'default' => 0,
    'sortable' => TRUE,
    'views' => TRUE,
  );

  $columns['offset'] = array(
    'type' => 'int',
    'not null' => FALSE,
    'sortable' => FALSE,
    'views' => FALSE,
  );

  $columns['data'] = array(
    'type' => 'text',
    'serialize' => TRUE,
    'views' => TRUE,
  );

  $columns['payment'] = array(
    'type' => 'text',
    'serialize' => TRUE,
    'views' => TRUE,
  );

  $columns['tickets'] = array(
    'type' => 'text',
    'serialize' => TRUE,
    'views' => TRUE,
  );

  $columns['discounts'] = array(
    'type' => 'text',
    'serialize' => TRUE,
    'views' => TRUE,
  );

  return $columns;
}

/**
 * Implementation of CCK's hook_field_settings($op = 'form').
 */
function _eventbrite_cck_field_settings_form($field) {
  // TODO: There will be more settings that go on this form
//         Maybe default date settings

  $form = array();

  $form['eventbrite'] = array(
    '#type' => 'fieldset',
    '#title' => 'Eventbrite Settings',
  );

  $form['eventbrite']['allow_create_edit'] = array(
    '#type' => 'checkbox',
    '#title' => t('Event Creation and Editing'),
    '#default_value' => $field['allow_create_edit'],
    '#description' => t('Allow for events to be created and edited through this field, with changes pushed to Eventbrite'),
  );
  $form['eventbrite']['allow_load'] = array(
    '#type' => 'checkbox',
    '#title' => t('Pulling events from Eventbrite'),
    '#default_value' => $field['allow_load'],
    '#description' => t('Allow for events previously created on Eventbrite to be loaded into this field'),
  );
  $form['eventbrite']['allow_historical'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow historical events to be pulled from Eventbrite'),
    '#default_value' => $field['allow_historical'],
    '#description' => t('If this is left unchecked, past events will not be listed'),
  );
  $form['eventbrite']['use_node_title'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use node title for Event Title field'),
    '#default_value' => $field['use_node_title'],
    '#description' => t('If this is left unchecked, you may have two titles on your node edit form'),
  );
  $form['eventbrite']['use_node_body'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use node body for Event Description field'),
    '#default_value' => $field['use_node_body'],
    '#description' => t('If this is left unchecked, you may have two body/descriptions on your node edit form'),
  );
  $form['eventbrite']['set_draft_event_unpublished'] = array(
    '#type' => 'checkbox',
    '#title' => t('Save draft events as unpublished'),
    '#default_value' => $field['set_draft_event_unpublished'],
    '#description' => t('If this is left unchecked, draft events not ready display may be shown'),
  );
  return $form;
}

/**
 * Implementation of CCK's hook_field_settings($op = 'save').
 */
function _eventbrite_cck_field_settings_save($field) {
  return array(
    'allow_create_edit',
    'allow_load',
    'allow_historical',
    'use_node_title',
    'use_node_body',
    'set_draft_event_unpublished',
  );
}

/**
 * Implementation of CCK's hook_field_settings($op = 'validate').
 */
function _eventbrite_cck_field_settings_validate($field) {
  if (empty($field['allow_create_edit']) && empty($field['allow_load'])) {
    // TODO: Get the form error set right on this form field
    form_set_error('allow_load', t('You must allow users to either load or create events (or both) to activate an Eventbrite field'));
  }
}

/**
 * Implementation of CCK's hook_field_settings($op = 'views data').
 */
function _eventbrite_cck_field_settings_views_data($field) {
  // TODO: Decide if any changes need to be made to the default $data array created for Views.

  // Start with the $data created by CCK
  // and alter it as needed. The following
  // code illustrates how you would retrieve
  // the necessary data.

  $data = content_views_field_views_data($field);
  $db_info = content_database_info($field);
  $table_alias = content_views_tablename($field);

  // Make changes to $data as needed here.
  //$field_data = $data[$table_alias][$field['field_name'] .'_ebid'];

  // TODO: Create an eventbrite relationship - look at filefield for clues

  /*
   // TODO: Add our special handler for serialized data. - look at filefield functions
   $data[$table_alias][$field['field_name'] .'_data']['field'] = array(
   'title' => $data[$table_alias][$field['field_name'] .'_data']['title'],
   'title short' => $data[$table_alias][$field['field_name'] .'_data']['title short'],
   'field' => $db_info['columns']['data']['column'],
   'table' => $db_info['table'],
   'handler' => 'filefield_handler_field_data',
   'click sortable' => FALSE,
   'access callback' => 'content_access',
   'access arguments' => array('view', $field),
   );

   // Remove handlers that are probably unnecessary.
   unset($data[$table_alias][$field['field_name'] .'_data']['filter']);
   unset($data[$table_alias][$field['field_name'] .'_data']['argument']);
   */

  return $data;
}

/**
 * Implementation of CCK's hook_field($op = 'load').
 */
function _eventbrite_cck_field_load($node, $field, &$items, $teaser, $page) {
  if (empty($items)) {
    return array();
  }
  foreach ($items as $delta => $item) {
    // Despite hook_content_is_empty(), CCK still doesn't filter out
    // empty items from $op = 'load', so we need to do that ourselves.
    if (empty($item['ebid'])) { // || !($file = field_file_load($item['fid']))) {
      $items[$delta] = NULL;
    }
    else {
      $serialized_fields = array('data', 'discounts', 'payment', 'tickets');
      foreach ($serialized_fields as $col) {
        $item[$col] = unserialize($item[$col]);
        // Temporary fix to unserialize data serialized multiple times.
        // See the FileField issue http://drupal.org/node/402860.
        // And the CCK issue http://drupal.org/node/407446.
        while (!empty($item[$col]) && is_string($item[$col])) {
          $item[$col] = unserialize($item[$col]);
        }
      }
      $items[$delta] = $item;
    }
  }

  return array($field['field_name'] => $items);
}

/**
 * Implementation of CCK's hook_field($op = 'validate').
 */
function _eventbrite_cck_field_validate(&$node, $field, &$items, $teaser, $page) {
  // Do validation on the field values here. The widget
  // will do its own validation and you cannot make any
  // assumptions about what kind of widget has been used,
  // so don't validate widget values, only field values.
  if (is_array($items)) {
    foreach ($items as $delta => $item) {
      // DO validation here for each field
      if ($field['use_node_title']) {
        if (('create' == $item['eventbrite_new']) || isset($item['ebid'])) {
          $items[$delta]['title'] = $item['title'] = $node->title;
        }
      }

      // If the field is empty just skip, wasn't filled in, that is valid
      if (eventbrite_cck_content_is_empty($item, $field)) {
        return $items;
      }

      if ($field['use_node_body']) {
        $items[$delta]['description'] = $node->body;
      }

      if (isset($item['ebid']) && !empty($item['ebid'])) {
        // EB_ID is set, that means all required field elements need to be set as well if editing is allowed
        if ($field['allow_create_edit']) {
          if (empty($item['title'])) {
            form_set_error($field['field_name'], t('The title field is required'));
            return FALSE;
          }
          // description can be empty
          if (empty($item['start_date']) || empty($item['end_date'])) {
            form_set_error($field['field_name'], t('The date fields are required'));
            return FALSE;
          }
        }
      }
      else {
        /* Not sure about this one
         // EB ID is blank, that means everything else should be blank
         * Should we set an error if EB is blank and other fields have values?
         if (!empty($item['title'])) {
         form_set_error($field['field_name'], t('No ebid, so title should be empty'));
         return false;
         }
         */
      }

      // TODO: Add some date validation stuff back in later if there is a need
    }
  }

  return $items;
}

/**
 * Implementation of CCK's hook_field($op = 'sanitize').
 */
function _eventbrite_cck_field_sanitize(&$node, $field, &$items, $teaser, $page) {
  // Get the 'data' column stored by CCK into an array. This is necessary
  // for Views, which doesn't call the "load" $op and to fix an issue with
  // CCK double-serializing data.
  // See the FileField issue http://drupal.org/node/402860.
  // And the CCK issue http://drupal.org/node/407446.
  while (!empty($items[$delta]['data']) && is_string($items[$delta]['data'])) {
    $items[$delta]['data'] = unserialize($items[$delta]['data']);
  }

  // TODO: this should be replaced to sanitize calls to the actual field item keys.
  foreach ($items as $delta => $item) {
    // TODO: Should not have to test that $item is non-empty,
//       empty fields should not be in sanitize data
    if ($item) {
      foreach ( $item as $col => $dat ) {
        switch ($col) {
          case 'data':
          case 'discounts':
          case 'payment':
          case 'tickets':
            // Get the 'data' column stored by CCK into an array. This is necessary
            // for Views, which doesn't call the "load" $op and to fix an issue with
            // CCK double-serializing data.
            // See the FileField issue http://drupal.org/node/402860.
            // And the CCK issue http://drupal.org/node/407446.
            while (!empty($items[$delta][$col]) && is_string($items[$delta][$col])) {
              $items[$delta][$col] = unserialize($items[$delta][$col]);
            }
            // TODO: maybe add some processing for serialized values
            break;

          case 'description':
            $items[$delta]['safe_' . $col] = check_markup($item[$col]);
            break;
          default:
            $items[$delta]['safe_' . $col] = check_plain($item[$col]);
        }
      }
    }
  }
}

/**
 * Implementation of CCK's hook_field($op = 'presave').
 */
function _eventbrite_cck_field_presave(&$node, $field, &$items, $teaser, $page) {
  if (is_array($items) && !empty($items)) {
    // Loop through multiple field values
    foreach ($items as $delta => $item) {
      // Skip empty fields
      if ($field['use_node_title']) {
        if (('create' == $item['eventbrite_new']) || isset($item['ebid'])) {
          $items[$delta]['title'] = $item['title'] = $node->title;
        }
      }
      if (eventbrite_cck_content_is_empty($item, $field)) {
        continue;
      }
      if ($field['use_node_body']) {
        $items[$delta]['description'] = $item['description'] = $node->body;
      }

      // If user is loading a pre-created event from the Eventbrite server
      if ($item['load_ebid'] && (!$field['allow_create_edit'] || $item['eventbrite_new'] == 'load')) {
        if (_eventbrite_cck_field_presave_event_load($item)) {
          // TODO: OK Message
        }
        else {
          // TODO: Set form error maybe
        }
        return;
      }

      // An event is being created or updated through the widget
      $event = _eventbrite_cck_populate_event_from_form($items[$delta]);
      $items[$delta]['offset'] = $event->tz_offset;

      // If a new venue has been created, save it
      if (isset($event->new_venue)) {
        if ($new_venue_id = eventbrite_venue_save($event->new_venue)) {
          $event->venue_id = $new_venue_id;
        }
        else {
          // TODO: Should we exit here?  or just set event status to draft?
        }
      }

      // If there's no event id then it is a new event to create
      if (!isset($event->event_id)) {
        $op = 'event_new';
        if ($event->event_id = eventbrite_save('event_new', $event)) {
          // TODO: Add event name
          drupal_set_message(t('The new event was successfully created on the Eventbrite server. '));
        }
        else {
          // Create request could not be completed
          // TODO: set a form error here so that the field does not validate (rather than dsm())
          // TODO: Clear the event here so that it does not get saved in DB (probably unnecessary if no ID)
          drupal_set_message('This event could not be created on the Eventbrite server', 'error');
          return;
        }
      }
      else {
        $op = 'event_update';
      }

      // Update the ticket settings
      // TODO: Allow for multiple ticket processing
      if ($event->ticket->toggle) {
        if ($ticket_id = _eventbrite_cck_field_presave_ticket($event->ticket, $event->event_id)) {
          $items[$delta]['tickets'][0]['ticket_id'] = $ticket_id;
          unset($items[$delta]['tickets']['0']['ticket_toggle']);
        }
        else {
          // TODO: Clearing the ticket field works with only one ticket, but really old values should be reloaded
          if ($event->ticket->disabled) {
            $items[$delta]['tickets']['0'] = array('disabled' => 1);
          }
          if (!isset($event->ticket->id)) {
            $items[$delta]['tickets']['0'] = array();
          }
        }
      }

      // Update the payment settings
      // TODO: Allow for different processing
      // TODO: Payment toggle may be unchecked, because autocreate is set and overrides are off

      if ($event->payment->toggle) {
        _eventbrite_cck_field_presave_payment_settings($event->payment, $event->event_id);
        unset($items[$delta]['payment']['payment_empty']);

        // TODO: There should be some error handling here, payment in $items should be reset if no save
        $massaged_payment_settings = eventbrite_payment_get_settings($items[$delta]['payment']);
        $items[$delta]['payment'] = array_merge($items[$delta]['payment'], $massaged_payment_settings);

        unset($items[$delta]['payment']['eventbrite_allow_payment_override']);
        unset($items[$delta]['payment']['eventbrite_autocreate_payment']);
        unset($items[$delta]['payment']['eventbrite_accept_paypal']);
        unset($items[$delta]['payment']['eventbrite_paypal_email']);
        unset($items[$delta]['payment']['eventbrite_accept_google']);
        unset($items[$delta]['payment']['eventbrite_google_merchant_id']);
        unset($items[$delta]['payment']['eventbrite_google_merchant_key']);
        unset($items[$delta]['payment']['eventbrite_accept_check']);
        unset($items[$delta]['payment']['eventbrite_instructions_check']);
        unset($items[$delta]['payment']['eventbrite_accept_cash']);
        unset($items[$delta]['payment']['eventbrite_instructions_cash']);
        unset($items[$delta]['payment']['eventbrite_accept_invoice']);
        unset($items[$delta]['payment']['eventbrite_instructions_invoice']);
      }
      else {
        // If payment toggle wasn't set, don't resave payment values from form
        // Get original payment content from saved value of node
        $field_name = $field['field_name'];
        $saved_node = node_load($node->nid);
        $saved_payment_settings = $saved_node->$field_name;
        $items[$delta]['payment'] = $saved_payment_settings[$delta]['payment'];
      }

      // Update the event if it has been created before
      if ('event_update' == $op) {
        if (eventbrite_save('event_update', $event)) {
          // TODO: Add event name
          drupal_set_message(t('This event was successfully updated on the Eventbrite server. '));
        }
        else {
          // Save request could not be completed
          drupal_set_message(t('This event could not be updated on the Eventbrite server'), 'error');
          // TODO: Definitely need to change these fields
          // TODO: Update didn't work so $item needs to be reset
        }
      }

      // Update the event if it is new and needs to be live
      if ('event_new' == $op && 'Live' == $event->status) {
        if (eventbrite_save('event_update', $event)) {
          // TODO: Add event name
          drupal_set_message(t('This event is now live on the Eventbrite server.'));
        }
        else {
          // Save request could not be completed
          drupal_set_message(t('This event could not be made live on the Eventbrite server'), 'error');
          // TODO: Definitely need to change these fields
          // TODO: Update didn't work so $item needs to be cleared or set to draft here in any case
        }
      }

      // Get the fresh updated event data from the server
      if ($updated_event = eventbrite_get('event_get', $event->event_id)) {
        $updated_item = _eventbrite_cck_populate_fields_from_event($updated_event);
        $items[$delta] = array_merge($items[$delta], $updated_item);
      }
      else {
        // TODO: Perhaps make this a watchdog entry?  Not sure what is going on here
        drupal_set_message('Connection Error: This event could not be loaded from the Eventbrite server', 'error');
      }

      // Set drafts to unpublished
      if ($field['set_draft_event_unpublished']) {
        if ('Draft' == $event->status) {
          $node->status = 0;
        }
      }
    }
  }
}

function _eventbrite_cck_field_presave_event_load(&$item) {
  $event_ebid = $item['load_ebid'];

  // Get load the fresh event data from the server
  if ($new_event = eventbrite_get('event_get', $event_ebid)) {
    $item += _eventbrite_cck_populate_fields_from_event($event);

    // Disable ticket management for events loaded from the eventbrite server
    $item['tickets'][0] = array('disabled' => 1);

    // Disable payment management for events loaded from the eventbrite server
    $item['payment'] = array('disabled' => 1);

    return $new_event;
  }
  else {
    // TODO: Perhaps make this a watchdog entry?  Not sure what is going on here
    drupal_set_message('Connection Error: This event could not be loaded from the Eventbrite server', 'error');
  }
}



function _eventbrite_cck_field_clear_ticket($ticket, &$ticket_item) {
  if ($ticket->disabled) {
    $ticket_item = array('disabled' => 1);
  }
  if (!isset($ticket->id)) {
    $ticket_item = array();
  }
}

/*
 * This function massages the payment setting before the ticket is saved on the EB server and in the DB
 */
function _eventbrite_cck_field_presave_ticket($ticket, $event_id) {
  // If this ticket has already been created
  if (isset($ticket->id)) {
    // There is a ticket id, which means that an edit/update needs to happen
    if ($ticket_id = eventbrite_save('ticket_update', $ticket)) {
      drupal_set_message(t('This ticket was successfully updated on the Eventbrite server'));
      return $ticket_id;
    }
    else {
      // Save request could not be completed
      // TODO: clear the ticket values--the event has not been submitted
      // TODO: set a form error here so that the field does not validate (rather than dsm())
      drupal_set_message('Connection Error: Could not update the ticket on the Eventbrite server', 'error');
    }
  }
  else {
    // There is no ticket id, which means ticket needs to be created
    $ticket->event_id = $event_id;

    if ($ticket_id = eventbrite_save('ticket_new', $ticket)) {
      drupal_set_message(t('The new ticket was successfully created on the Eventbrite server.'));
      return $ticket_id;
    }
    else {
      // Save request could not be completed
      // TODO: clear the ticket values--the event has not been submitted
      // TODO: set a form error here so that the field does not validate (rather than dsm())
      drupal_set_message('Connection Error: Could not create the ticket on the Eventbrite server', 'error');
    }
  }
}

/*
 * This function massages the payment setting before the event is saved on the EB server and in the DB
 */
function _eventbrite_cck_field_presave_payment_settings($payment, $event_id) {
  $payment->event_id = $event_id;

  if ($message = eventbrite_save('payment_update', $payment)) {
    drupal_set_message(t('The payment settings were successfully saved on the Eventbrite server.'));
    return $message;
  }
  else {
    // Save request could not be completed
    // TODO: reset the payment values--the event has not been submitted
    // TODO: set a form error here so that the field does not validate (rather than dsm())
    drupal_set_message('Connection Error: Could not update the payment settings on the Eventbrite server', 'error');
  }
}

/*
 * This function pulls event info from the form results and builds an event object for base Eventbrite module
 */
function _eventbrite_cck_populate_event_from_form($item) {
  $event = new stdClass();

  // Event details
  if ($item['ebid']) {
    $event->event_id = $item['ebid'];
  }
  $event->title = $item['title'];
  $event->description = check_markup($item['description']);
  $event->status = $item['status'];
  $event->organizer_id = $item['organizer'];

  // Date / Timezone handling
  $event->start_date = $item['start_date'];
  $event->end_date = $item['end_date'];
  $start_date = date_create($item['start_date'], timezone_open($item['timezone']));
  $event->tz_offset = $start_date->getOffset() / 3600;
  $event->timezone = eventbrite_timezone_offset($event->tz_offset);

  // Venue handling
  switch ($item['venue_source']) {
    case 'existing':
      $event->venue_id = $item['venue_id'];
      break;
    case 'new':
      $venue = new stdClass();
      $new_venue_item = $item['where']['new_venue'];

      $venue->name = $new_venue_item['name'];
      $venue->organizer = $item['organizer'];
      $venue->address = $new_venue_item['address'];
      $venue->address2 = $new_venue_item['address2'];
      $venue->city = $new_venue_item['city'];
      $venue->region = $new_venue_item['region'];
      $venue->postal_code = $new_venue_item['postal_code'];
      $venue->country = $new_venue_item['country'];

      $event->new_venue = $venue;
      break;
  }

  // Ticket handling
  $event->ticket = _eventbrite_cck_populate_ticket_from_form($item['tickets'][0]);

  // Payment handling
  $event->payment = _eventbrite_cck_populate_payment_settings_from_form($item['payment']);

  return $event;
}

/*
 * This function pulls ticket info from the form results and builds a ticket object for base Eventbrite module
 */
function _eventbrite_cck_populate_ticket_from_form($item) {
  $ticket = new stdClass();

  // Add ticket data to request
  $ticket->toggle = $item['ticket_toggle'];
  if (isset($item['ticket_id'])) {
    $ticket->id = $item['ticket_id'];
  }
  if (isset($item['disabled'])) {
    $ticket->disabled = 1;
  }
  if ($ticket->toggle) {
    $ticket->name = $item['ticket_name'];
    $ticket->quantity = $item['quantity'];
    if (!empty($item['ticket_description'])) {
      $ticket->description = $item['ticket_description'];
    }
    switch ($item['ticket_type']) {
      case 'fixed':
        $ticket->price = $item['price'];
        $ticket->is_donation = 0;
        break;
      case 'free':
        $ticket->price = 0.00;
        $ticket->is_donation = 0;
        break;
      case 'donation':
        $ticket->price = '';
        $ticket->is_donation = 1;
        break;
      default:
        $ticket->price = $item['price'];
        $ticket->is_donation = 0;
        break;
    }
  }
  return $ticket;
}

/*
 * This function pulls payment settings info from the form results and builds a payment settings object for base Eventbrite module
 */
function _eventbrite_cck_populate_payment_settings_from_form($item) {
  $payment = new stdClass();

  // Add ticket data to request
  $payment->toggle = $item['payment_toggle'];
  if (isset($item['payment_empty'])) {
    $payment->empty = $item['payment_empty'];
  }
  if (isset($item['disabled'])) {
    $payment->disabled = 1;
  }

  if ($payment->toggle) {
    $payment->accept_paypal = $item['eventbrite_accept_paypal'];
    $payment->paypal_email = $item['eventbrite_paypal_email'];
    $payment->accept_google = $item['eventbrite_accept_google'];
    $payment->google_merchant_id = $item['eventbrite_google_merchant_id'];
    $payment->google_merchant_key = $item['eventbrite_google_merchant_key'];
    $payment->accept_check = $item['eventbrite_accept_check'];
    $payment->instructions_check = $item['eventbrite_instructions_check'];
    $payment->accept_cash = $item['eventbrite_accept_cash'];
    $payment->instructions_cash = $item['eventbrite_instructions_cash'];
    $payment->accept_invoice = $item['eventbrite_accept_invoice'];
    $payment->instructions_invoice = $item['eventbrite_instructions_invoice'];
  }

  return $payment;
}

function _eventbrite_cck_populate_fields_from_event($event) {
  return array(
    'ebid' => $event->id,
    'title' => $event->title,
    'status' => $event->status,
    'description' => $event->description,
    'start_date' => $event->start_date,
    'end_date' => $event->end_date,
    // TODO: These timezones coming from Eventbrite server are DEPRECATED & will not work in D7
    'timezone' => $event->timezone,
    'venue_id' => $event->venue_id,
    'organizer' => $event->organizer_id,
  );
}
