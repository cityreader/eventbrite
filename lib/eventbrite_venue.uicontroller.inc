<?php
/**
 * @file
 * The UI controller for the venue entity.
 */

/**
 * UI controller.
 */
class EventbriteVenueUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    $items[$this->path]['type'] = MENU_LOCAL_TASK;
    return $items;
  }

  /**
   * Overrides EntityDefaultUIController::overviewForm() defaults.
   */
  public function overviewForm($form, &$form_state) {
    // Synchronize organizer data first if cache is expired
    eventbrite_sync('user_list_venues');

    $form = parent::overviewForm($form, $form_state);
    return $form;
  }

  /**
   * Overrides EntityDefaultUIController::overviewTableHeaders().
   */
  protected function overviewTableHeaders($conditions, $rows, $additional_header = array()) {
    $header = parent::overviewTableHeaders($conditions, $rows, $additional_header);

    // Change table column name from "Label" to "Organizer Name"
    $header[0] = t('Venue Name');
    $extra = array(
      t('Address'),
      t('Status'),
    );
    array_splice($header, 1, 0, $extra);
    return $header;
  }

  /**
   * Overrides EntityDefaultUIController::overviewTableRow().
   */
  protected function overviewTableRow($conditions, $id, $entity, $additional_cols = array()) {
    $row = parent::overviewTableRow($conditions, $id, $entity, $additional_cols);

    $address = $entity->address;
    if (!empty($entity->address_2)) {
      $address .= ', '. $entity->address_2;
    }
    if (!empty($entity->city)) {
      $address .= ', '. $entity->city;
    }
    if (!empty($entity->region)) {
      $address .= ', '. $entity->region;
    }
    if (!empty($entity->country_code)) {
      include_once DRUPAL_ROOT . '/includes/locale.inc';
      $country_list = country_get_list();

      $address .= ', '. $country_list[$entity->country_code];
    }
    if (!empty($entity->postal_code)) {
      $address .= ', '. $entity->postal_code;
    }
    $extra[] = array(
      'data'=> $address,
    );
    $extra[] = array(
      'data'=> $entity->status ? t('Active') : t('Inactive'),
    );
    array_splice($row, 1, 0, $extra);
    
    return $row;
  }
}

 
