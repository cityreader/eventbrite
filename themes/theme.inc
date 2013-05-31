<?php

/**
 * Template preprocess function for displaying event venue address.
 */
function theme_eventbrite_venue_address(&$variables) {
  $venue = $variables['venue'];

  $address = array();
  
  foreach (array('addess', 'address_2', 'city', 'region') as $field_name) {
    if (!empty($venue->$field_name)) {
      $address[] = $venue->$field_name;
    }
  }
  
  $address = implode(', ', $address);

  return check_plain($address);
}

