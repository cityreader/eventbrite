<?php
/**
 * @file
 * Eventbrite Venue entity class
 */
 
/**
 * The class used for Eventbrite organizer entities.
 */
class EventbriteVenue extends Entity {

  public $venue_id;
  public $organizer_id;
  public $name;
  public $address;
  public $address_2;
  public $city;
  public $region;
  public $postal_code;
  public $country_code;
  public $status;

  public function defaultLabel() {
    return $this->name;
  }

  /**
   * Overrides Entity::save().
   */
  public function save() {
    // Only save data remotely to Eventbrite in a form submission 
    if (isset($this->form_id)) {

      $params = array(
        'name' => $this->name,
        'address' => $this->address,
        'address_2' => $this->address_2,
        'city' => $this->city,
        'region' => $this->region,
        'postal_code' => $this->postal_code,
        'country_code' => $this->country_code,
      );

      if ($this->venue_id) {
        $params['id'] = $this->venue_id;
      }

      if (!empty($this->is_new)) {
        $params['organizer_id'] = $this->organizer_id;
      }

      $api_callback = !empty($this->is_new) && $this->venue_id ? 'venue_new' : 'venue_update';

      $result = eventbrite_save($api_callback, $params);

      if (!$result || $result->process->status !== 'OK') {
        return;
      }

      if (!empty($this->is_new)) {
        drupal_set_message(t('This venue has been created on the Eventbrite server.'));
      }
      else {
        drupal_set_message(t('This venue has been updated on the Eventbrite server.'));
      }

      $this->venue_id = $result->process->id;

      dpm($result);
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

