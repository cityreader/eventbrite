<?php
/**
 * @file
 * The UI controller for the organizer entity.
 */

/**
 * UI controller.
 */
class EventbriteOrganizerUIController extends EntityDefaultUIController {

  /**
   * Overrides EntityDefaultUIController::hook_menu() defaults.
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
    eventbrite_sync('user_list_organizers');

    $form = parent::overviewForm($form, $form_state);

    $enabled_organizers = eventbrite_organizer_get_enabled();

    if (count($enabled_organizers) > 0) {
      $form['default_organizer'] = array(
        '#type' => 'select',
        '#title' => t('Default Organizer'),
        '#default_value' => variable_get('eventbrite_organizer_default', ''),
        '#options' => $enabled_organizers,
        '#required' => TRUE,
      );

      $form['actions'] = array('#type' => 'actions');
      $form['actions']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save configuration'),
        '#weight' => 100,
      );
    }

    return $form;
  }

  /**
   * Overview form validation callback.
   */
  public function overviewFormValidate($form, &$form_state) {
    $default_organizer = $form_state['values']['default_organizer'];
  }

  /**
   * Overview form submit callback.
   */
  public function overviewFormSubmit($form, &$form_state) {
    variable_set('eventbrite_organizer_default', $form_state['values']['default_organizer']);
  }

  /**
   * Overrides EntityDefaultUIController::overviewTableHeaders().
   */
  protected function overviewTableHeaders($conditions, $rows, $additional_header = array()) {
    $header = parent::overviewTableHeaders($conditions, $rows, $additional_header);

    // Change table column name from "Label" to "Organizer Name"
    $header[0] = t('Organizer Name');
    $extra = array(
      t('Description'),
      t('URL'),
    );
    array_splice($header, 1, 0, $extra);
    return $header;
  }

  /**
   * Overrides EntityDefaultUIController::overviewTableRow().
   */
  protected function overviewTableRow($conditions, $id, $entity, $additional_cols = array()) {
    $row = parent::overviewTableRow($conditions, $id, $entity, $additional_cols);

    $extra[] = array(
      'data'=> $entity->description,
    );
    $url = EVENTBRITE_URL_ORG . '/' . $entity->organizer_id;
    $extra[] = array(
      'data'=> l($url, $url, array('attributes' => array('target' => '_blank'))),
    );
    array_splice($row, 1, 0, $extra);
    
    return $row;
  }

}

