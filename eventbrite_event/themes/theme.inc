<?php

/**
 * Processes variables for node.tpl.php
 *
 * Most themes utilize their own copy of node.tpl.php. The default is located
 * inside "modules/node/node.tpl.php". Look in there for the full list of
 * variables.
 *
 * The $variables array contains the following arguments:
 * - $node
 * - $view_mode
 * - $page
 *
 * @see node.tpl.php
 */
function template_preprocess_eventbrite_event(&$variables) {
  // $variables['view_mode'] = $variables['elements']['#view_mode'];
  // // Provide a distinct $teaser boolean.
  // $variables['teaser'] = $variables['view_mode'] == 'teaser';
  // $variables['node'] = $variables['elements']['#node'];
  // $node = $variables['node'];
// 
  // $variables['date']      = format_date($node->created);
  // $variables['name']      = theme('username', array('account' => $node));
// 
  // $uri = entity_uri('node', $node);
  // $variables['node_url']  = url($uri['path'], $uri['options']);
  // $variables['title']     = check_plain($node->title);
  // $variables['page']      = $variables['view_mode'] == 'full' && node_is_page($node);
// 
  // // Flatten the node object's member fields.
  // $variables = array_merge((array) $node, $variables);
// 
  // Helpful $content variable for templates.
  $variables += array('content' => array());
  foreach (element_children($variables['elements']) as $key) {
    $variables['content'][$key] = $variables['elements'][$key];
  }

// 
  // // Make the field variables available with the appropriate language.
  // field_attach_preprocess('node', $node, $variables['content'], $variables);
// 
  // // Display post information only on certain node types.
  // if (variable_get('node_submitted_' . $node->type, TRUE)) {
    // $variables['display_submitted'] = TRUE;
    // $variables['submitted'] = t('Submitted by !username on !datetime', array('!username' => $variables['name'], '!datetime' => $variables['date']));
    // $variables['user_picture'] = theme_get_setting('toggle_node_user_picture') ? theme('user_picture', array('account' => $node)) : '';
  // }
  // else {
    // $variables['display_submitted'] = FALSE;
    // $variables['submitted'] = '';
    // $variables['user_picture'] = '';
  // }
// 
  // // Gather node classes.
  // $variables['classes_array'][] = drupal_html_class('node-' . $node->type);
  // if ($variables['promote']) {
    // $variables['classes_array'][] = 'node-promoted';
  // }
  // if ($variables['sticky']) {
    // $variables['classes_array'][] = 'node-sticky';
  // }
  // if (!$variables['status']) {
    // $variables['classes_array'][] = 'node-unpublished';
  // }
  // if ($variables['teaser']) {
    // $variables['classes_array'][] = 'node-teaser';
  // }
  // if (isset($variables['preview'])) {
    // $variables['classes_array'][] = 'node-preview';
  // }
// 
  // // Clean up name so there are no underscores.
  // $variables['theme_hook_suggestions'][] = 'node__' . $node->type;
  // $variables['theme_hook_suggestions'][] = 'node__' . $node->nid;
}

function theme_eventbrite_event_ticket($variables) {
  libraries_load('eventbrite.php');

  $element = $variables['element'];
  $event = $element['#event'];

  drupal_add_js(drupal_get_path('module', 'eventbrite_event') . '/eventbrite_event.js');

  $output = Eventbrite::ticketWidget($event, '100%');
  return $output;
}

/**
 * Template preprocess function for displaying event venue.
 */
function template_preprocess_eventbrite_event_venue(&$variables) {
  $venue = $variables['venue'];

  $address = array();
  
  foreach (array('addess', 'address_2', 'city', 'region') as $field_name) {
    if (!empty($venue->$field_name)) {
      $address[] = $venue->$field_name;
    }
  }
  $address = implode(', ', $address);

  $variables['address'] = check_plain($address);
  $variables['name'] = check_plain($venue->name);
}

/**
 * Template preprocess function for displaying event date.
 */
function template_preprocess_eventbrite_event_date(&$variables) {
  $day1 =new DateObject($variables['start_date']);
  $day2 = new DateObject($variables['end_date']);

  $start_date = $day1->format('l, F j, Y \a\t g:i A');

  if ($day1->format('Ymd') === $day2->format('Ymd')) {
    $end_date = $day2->format('g:i A');
  }
  else {
    $end_date = $day2->format('l, F j, Y \a\t g:i A');
  }

  $variables['start_date'] = $start_date;
  $variables['end_date'] = $end_date;
  $variables['timezone'] = date_default_timezone();

  $dateTime = new DateTime();
  $dateTime->setTimeZone(new DateTimeZone($variables['timezone']));
  $variables['timezone_abbr'] = $dateTime->format('T');
}








