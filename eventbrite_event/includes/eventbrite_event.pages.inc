<?php

function eventbrite_event_page_view($event) {
  drupal_set_title($event->title);
  
  return eventbrite_event_show($event);
}

function eventbrite_event_page_title($event) {
  return $event->title;
}

/**
 * Generates an array which displays a node detail page.
 *
 * @param $node
 *   A node object.
 * @param $message
 *   A flag which sets a page title relevant to the revision being viewed.
 *
 * @return
 *   A $page element suitable for use by drupal_render().
 */
function eventbrite_event_show($event, $message = FALSE) {
  // if ($message) {
    // drupal_set_title(t('Revision of %title from %date', array('%title' => $node->title, '%date' => format_date($node->revision_timestamp))), PASS_THROUGH);
  // }

  // For markup consistency with other pages, use node_view_multiple() rather than node_view().
  $events = eventbrite_event_view_multiple(array($event->id => $event), 'full');

  // // Update the history table, stating that this user viewed this node.
  // node_tag_new($node);

  return $events;
}

/**
 * Generate an array for rendering the given event.
 *
 * @param $event
 *   A comment object.
 * @param $view_mode
 *   View mode, e.g. 'full', 'teaser'...
 * @param $langcode
 *   (optional) A language code to use for rendering. Defaults to the global
 *   content language of the current request.
 *
 * @return
 *   An array as expected by drupal_render().
 */
function eventbrite_event_view($event, $view_mode = 'full', $langcode = NULL) {
  if (!isset($langcode)) {
    $langcode = $GLOBALS['language_content']->language;
  }

  // Populate $comment->content with a render() array.
  eventbrite_event_build_content($event, $view_mode, $langcode);

  $build = $event->content;
  // We don't need duplicate rendering info in $event->content.
  unset($event->content);

  $build += array(
    '#theme' => 'eventbrite_event',
    '#event' => $event,
    '#view_mode' => $view_mode,
    '#language' => $langcode,
  );

  // if (empty($event->in_preview)) {
    // $prefix = '';
    // $is_threaded = isset($event->divs) && variable_get('comment_default_mode_' . $node->type, COMMENT_MODE_THREADED) == COMMENT_MODE_THREADED;
// 
    // // Add 'new' anchor if needed.
    // if (!empty($comment->first_new)) {
      // $prefix .= "<a id=\"new\"></a>\n";
    // }
// 
    // // Add indentation div or close open divs as needed.
    // if ($is_threaded) {
      // $prefix .= $comment->divs <= 0 ? str_repeat('</div>', abs($comment->divs)) : "\n" . '<div class="indented">';
    // }
// 
    // // Add anchor for each comment.
    // $prefix .= "<a id=\"comment-$comment->cid\"></a>\n";
    // $build['#prefix'] = $prefix;
// 
    // // Close all open divs.
    // if ($is_threaded && !empty($comment->divs_final)) {
      // $build['#suffix'] = str_repeat('</div>', $comment->divs_final);
    // }
  // }

  // Allow modules to modify the structured comment.
  $type = 'eventbrite_event';
  drupal_alter(array('eventbrite_event_view', 'entity_view'), $build, $type);

  return $build;
}

/**
 * Builds a structured array representing the event's content.
 *
 * The content built for the comment (field values, comments, file attachments or
 * other comment components) will vary depending on the $view_mode parameter.
 *
 * @param $comment
 *   A eventbrite_event object.
 * @param $view_mode
 *   View mode, e.g. 'full', 'teaser'...
 * @param $langcode
 *   (optional) A language code to use for rendering. Defaults to the global
 *   content language of the current request.
 */
function eventbrite_event_build_content($event, $view_mode = 'full', $langcode = NULL) {
  if (!isset($langcode)) {
    $langcode = $GLOBALS['language_content']->language;
  }

  // Remove previously built content, if exists.
  $event->content = array();

  // Allow modules to change the view mode.
  $context = array(
    'entity_type' => 'eventbrite_event',
    'entity' => $event,
    'langcode' => $langcode,
  );
  drupal_alter('entity_view_mode', $view_mode, $context);

  // Build fields content.
  field_attach_prepare_view('eventbrite_event', array($event->id => $event), $view_mode, $langcode);
  entity_prepare_view('eventbrite_event', array($event->id => $event), $langcode);
  $event->content += field_attach_view('eventbrite_event', $event, $view_mode, $langcode);

  $event->content['venue'] = array(
    '#theme' => 'eventbrite_event_venue',
    '#venue' => eventbrite_venue_load($event->venue_id),
  );

  $event->content['tickets'] = array(
    '#theme' => 'eventbrite_event_ticket',
    '#event' => $event,
  );

  $event->content['description'] = array(
    '#type' => 'fieldset',
    '#title' => t('Event Details'),
    '#value' => filter_xss_admin($event->description),
  );
  
  // if (empty($event->in_preview)) {
    // $comment->content['links']['comment'] = array(
      // '#theme' => 'links__comment__comment',
      // '#links' => comment_links($comment, $node),
      // '#attributes' => array('class' => array('links', 'inline')),
    // );
  // }

  // Allow modules to make their own additions to the comment.
  module_invoke_all('eventbrite_event_view', $event, $view_mode, $langcode);
  module_invoke_all('entity_view', $event, 'eventbrite_event', $view_mode, $langcode);

  // Make sure the current view mode is stored if no module has already
  // populated the related key.
  $event->content += array('#view_mode' => $view_mode);

}

/**
 * Constructs a drupal_render() style array from an array of loaded nodes.
 *
 * @param $nodes
 *   An array of nodes as returned by node_load_multiple().
 * @param $view_mode
 *   View mode, e.g. 'full', 'teaser'...
 * @param $weight
 *   An integer representing the weight of the first node in the list.
 * @param $langcode
 *   (optional) A language code to use for rendering. Defaults to NULL which is
 *   the global content language of the current request.
 *
 * @return
 *   An array in the format expected by drupal_render().
 */
function eventbrite_event_view_multiple($events, $view_mode = 'full', $weight = 0, $langcode = NULL) {
  field_attach_prepare_view('eventbrite_event', $events, $view_mode, $langcode);
  entity_prepare_view('eventbrite_event', $events, $langcode);
  $build = array();
  foreach ($events as $event) {
    $build['events'][$event->id] = eventbrite_event_view($event, $view_mode, $langcode);
    $build['events'][$event->id]['#weight'] = $weight;
    $weight++;
  }
  $build['events']['#sorted'] = TRUE;
  return $build;
}

function eventbrite_contextual_page_tab($object, $entity_type) {

  switch ($entity_type) {
    case 'eventbrite_event':
      $eventbrite_event_uri = eventbrite_event_uri();

      // Let's always go back to the node page.
      $destination = $eventbrite_event_uri . '/' . $object->id;
      break;
  }

  $info = entity_get_info($entity_type);

  // Get the manage display URI.
  $admin_path = $info['admin ui']['path'] . '/manage/' . $object->id .'/edit';

  drupal_goto($admin_path, array('query' => array('destination' => $destination)));
}

