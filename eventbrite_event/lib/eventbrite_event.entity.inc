<?php
/**
 * @file
 * Eventbrite Event entity classses
 */
 
/**
 * The class used for Eventbrite organizer entities.
 */
class EventbriteEvent extends Entity {

  public $event_id;
  public $title;
  public $description;
  public $description_format;
  public $start_date;
  public $end_date;
  public $privacy;
  public $personalized_url;
  public $venue_id;
  public $organizer_id;
  public $capacity;
  public $status;
  public $created;
  public $modified;
  public $tickets;

  /**
   * Override Entity:defaultLabel().
   */
  public function defaultLabel() {
    return $this->title;
  }

  /**
   * Override Entity:defaultUri().
   */
  protected function defaultUri() {
    $eventbrite_event_uri = eventbrite_event_uri();
    
    return array('path' => $eventbrite_event_uri . '/' . $this->identifier());
  }

  /**
   * Overrides Entity::save().
   */
  public function save() {
    // Only save data remotely to Eventbrite in a form submission 
    if (isset($this->form_id)) {

      if (is_array($this->description)) {
        $this->description_format = $this->description['format'];
        $this->description = $this->description['value'];
      }

      $params = array(
        'title' => $this->title,
        'start_date' => format_date($this->start_date, 'custom', EVENTBRITE_DATE_FORMAT),
        'end_date' => format_date($this->end_date, 'custom', EVENTBRITE_DATE_FORMAT),
        'organizer_id' => $this->organizer_id,
        'status' => $this->status,
        'modified' => REQUEST_TIME,
      );

      if (!empty($this->is_new)) {
        $params['created'] = $params['modified'] = REQUEST_TIME;
      }

      if ($this->description) {
        $params['description'] = $this->description;
      }
      
      if ($this->venue_id) {
        $params['venue_id'] = $this->venue_id;
      }
      
      if ($this->capacity) {
        $params['capacity'] = $this->capacity;
      }

      if ($this->personalized_url) {
        $params['personalized_url'] = $this->personalized_url;
      }

      $params['status'] = $this->status ? 'live' : 'draft';
      
      $params['hide'] = $this->status ? 'y' : 'n';

      if ($this->event_id) {
        $params['id'] = $this->event_id;
      }
dpm($params);
      $api_callback = !empty($this->is_new) && $this->venue_id ? 'event_new' : 'event_update';

      $result = eventbrite_save($api_callback, $params);

      if (!$result || $result->process->status !== 'OK') {
        return;
      }

      dpm($result);

      if (!empty($this->is_new)) {
        drupal_set_message(t('This event has been created on the Eventbrite server.'));
      }
      else {
        drupal_set_message(t('This event has been updated on the Eventbrite server.'));
      }

      $this->event_id = $result->process->id;

      foreach ($this->tickets[LANGUAGE_NONE] as $delta=>$ticket) {
        $params = array(
          'event_id' => $this->event_id,
          'is_donation' => $ticket['is_donation'],
          'name' => $ticket['name'],
          'price' => $ticket['price'],
          'start_date' => format_date($ticket['start_date'], 'custom', EVENTBRITE_DATE_FORMAT),
          'end_date' => ormat_date($ticket['end_date'], 'custom', EVENTBRITE_DATE_FORMAT),
          'include_fee' => $ticket['include_fee'],
          // 'min' => $ticket['min'],
          'hide ' => $ticket['hide'],
        );
        
        if ($ticket['description']) {
          $params['description'] = $ticket['description'];
        }

        if ($ticket['quantity_available'] > 0) {
          $params['quantity_available'] = $ticket['quantity_available'];
        }
        
        if ($ticket['max'] > 0) {
          $params['max'] = $ticket['max'];
        }
        
        if ($ticket['ticket_id'] > 0) {
          $params['ticket_id'] = $ticket['ticket_id'];
          
          $api_callback = 'ticket_update';
        }
        else {
          $api_callback = 'ticket_new';
        }
        dpm($params);
        $result = eventbrite_save($api_callback, $params);
        
        if (!$result || $result->process->status !== 'OK') {
          return;
        }
      
        $this->tickets[LANGUAGE_NONE][$delta]['ticket_id'] = $result->process->id;

        dpm($result);
      }

    }


    dpm($this);
//
//    dpm(eventbrite_get('organizer_get', $params));

    parent::save();
//    // Update the static cache from profile2_load_by_user().
//    $cache = &drupal_static('profile2_load_by_user', array());
//    if (isset($cache[$this->uid])) {
//      $cache[$this->uid][$this->type] = $this->pid;
//    }
  }
}

