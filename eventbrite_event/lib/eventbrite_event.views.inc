<?php

/**
 * EventbriteEvent Views Controller class.
 */
class EventbriteEventViewsController extends EntityDefaultViewsController {

  /**
   * Add extra fields to views_data().
   */
  public function views_data() {
    $data = parent::views_data();
    
    $data['eventbrite_event']['event_id']['field']['handler'] = 'views_handler_field_eventbrite_event';
    $data['eventbrite_event']['title']['field']['handler'] = 'views_handler_field_eventbrite_event';

    return $data;
  }
}
