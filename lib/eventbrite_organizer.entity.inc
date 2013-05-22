<?php
/**
 * @file
 * Eventbrite Organizer entity class
 */

/**
 * The class used for Eventbrite organizer entities.
 */
class EventbriteOrganizer extends Entity {

  public $organizer_id;
  public $status;
  public $name;
  public $url;
  public $description;

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
        'description' => $this->description,
      );

      if ($this->organizer_id) {
        $params['id'] = $this->organizer_id;
      }

      $api_callback = !empty($this->is_new) && $this->organizer_id ? 'organizer_new' : 'organizer_update';

      $result = eventbrite_save($api_callback, $params);

      if (!$result || $result->process->status !== 'OK') {
        return;
      }

      $this->organizer_id = $result->process->id;

      dpm($result);
    }

    $this->status = 1;

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

