<?php

function eventbrite_date_combo_validate($element, &$form_state) {
  $from_date_in = drupal_array_get_nested_value($form_state['values'], $element['start_date']['#parents']);
  $to_date_in = drupal_array_get_nested_value($form_state['values'], $element['end_date']['#parents']);
  
  if (is_null($from_date_in)) {
    $message = t('The Start date is required.');
    $error_field = implode('][', $element['start_date']['#parents']);
    form_set_error($error_field, $message);
  }
  
  if (is_null($to_date_in)) {
    $message = t('The End date is required.');
    $error_field = implode('][', $element['end_date']['#parents']);
    form_set_error($error_field, $message);
  }
  
  if ($from_date_in && $to_date_in) {
    $from_date = new DateObject($from_date_in, $element['start_date']['#date_timezone']);
    $to_date = new DateObject($to_date_in, $element['end_date']['#date_timezone']);

    if (eventbrite_date_compare($from_date, $to_date) !== -1) {
      $error_field = implode('][', $element['end_date']['#parents']);
      $message = t('The End date must be greater than the Start date.');
      form_set_error($error_field, $message);
    }
  }
}

function eventbrite_sale_end_date_validate($form, &$form_state) {
  $values = $form_state['values'];
  $timezone = date_default_timezone();
  
  $langcode = LANGUAGE_NONE;
  if ($values['tickets'][$langcode]) {
    $end_date = new DateObject($values['end_date'], $timezone);
    
    foreach($values['tickets'][$langcode] as $delta=>$ticket) {
      if (!is_numeric($delta)) {
        continue;
      }
      
      $sale_end_date = new DateObject($ticket['end_date'], $timezone);

      if (eventbrite_date_compare($end_date, $sale_end_date) === -1) {
        $error_field = implode('][', array('tickets', $langcode, $delta, 'end_date'));
        $message = t('The End date must be greater than the Start date.');
        form_set_error($error_field, t('End sales date greater than Event’s Ending date.'));
      }
    }
  }
}

/**
 * Compare two dates.
 *
 * @param object $date1
 *   the starting date
 * @param object $date2
 *   the ending date
 * @return
 *   -1 if the first date is lower than the second, 0 if they are equal,
 *   and 1 if the second is lower.
 */
function eventbrite_date_compare($date1, $date2) {
  $diff = date_format($date1, 'U') - date_format($date2, 'U');
  return ($diff > 0 ? 1 : ($diff < 0 ? -1 : 0));
}


function eventbrite_default_date($items = array()) {
  $date = date_now();

  foreach($items as $item=>$count) {
    if ($count > 0) {
      date_modify($date, '+' . $count . ' ' . $item);
    }
    elseif ($count < 0) {
      date_modify($date, $count . ' ' . $item);
    }
  }

  return date_format($date, 'Y-m-d H:i:s');
}

