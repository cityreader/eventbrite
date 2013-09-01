<?php

/**
 * @file
 * Definition of views_handler_field_node_link_delete.
 */

/**
 * Field handler to present a link to delete a node.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_eventbrite_event_link_delete extends views_handler_field_eventbrite_event_link {

  /**
   * Renders the link.
   */
  function render_link($event, $values) {
    // Ensure user has access to delete this node.
    if (!user_access('administer eventbrite settings')) {
      return;
    }

    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['path'] = eventbrite_event_uri() . "/$event->event_id/delete";
    $this->options['alter']['query'] = drupal_get_destination();

    $text = !empty($this->options['text']) ? $this->options['text'] : t('delete');
    return $text;
  }
}
