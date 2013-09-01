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

//    $data['eventbrite_event']['title']['field']['handler'] = 'views_handler_field_eventbrite_event';
    $data['eventbrite_event']['edit_eventbrite_event'] = array(
      'field' => array(
        'title' => t('Edit link'),
        'help' => t('Provide a simple link to edit the event.'),
        'handler' => 'views_handler_field_eventbrite_event_link_edit',
      ),
    );

    $data['eventbrite_event']['delete_eventbrite_event'] = array(
      'field' => array(
        'title' => t('Delete link'),
        'help' => t('Provide a simple link to delete the event.'),
        'handler' => 'views_handler_field_eventbrite_event_link_delete',
      ),
    );

    return $data;
  }
}
