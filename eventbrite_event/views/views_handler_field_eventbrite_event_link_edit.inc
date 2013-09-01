<?php

/**
 * @file
 * Definition of views_handler_field_node_link_edit.
 */

/**
 * Field handler to present a link node edit.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_eventbrite_event_link_edit extends views_handler_field_eventbrite_event_link {

  /**
   * Renders the link.
   */
  function render_link($event, $values) {
    // Ensure user has access to edit this node.
    if (!user_access('edit eventbrite events')) {
      return;
    }

    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['path'] = eventbrite_event_uri() . "/$event->event_id/edit";
    $this->options['alter']['query'] = drupal_get_destination();

    $text = !empty($this->options['text']) ? $this->options['text'] : t('edit');
    return $text;
  }
}
