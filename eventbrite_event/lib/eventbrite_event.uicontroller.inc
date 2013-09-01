<?php
/**
 * @file
 * The UI controller for the event entity.
 */

 /**
 * UI controller.
 */
class EventbriteEventUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    // $items = parent::hook_menu();
    // $items[$this->path]['type'] = MENU_LOCAL_TASK;
    
    $items = array();
    
    $eventbrite_event_uri = eventbrite_event_uri();
    $id_count = count(explode('/', $eventbrite_event_uri));
    $path = drupal_get_path('module', 'eventbrite_event');


    // $id_count = count(explode('/', $this->path));
    $plural_label = isset($this->entityInfo['plural label']) ? $this->entityInfo['plural label'] : $this->entityInfo['label'] . 's';

    $items[$this->path] = array(
      'title' => $plural_label,
      'page callback' => 'drupal_get_form',
      'page arguments' => array($this->entityType . '_overview_form', $this->entityType),
      'description' => 'Manage ' . $plural_label . '.',
      'access callback' => 'entity_access',
      'access arguments' => array('view', $this->entityType),
      'file' => 'includes/entity.ui.inc',
      'type' => MENU_LOCAL_TASK,
    );
    $items[$this->path . '/list'] = array(
      'title' => 'List',
      'type' => MENU_DEFAULT_LOCAL_TASK,
      'weight' => -10,
    );
    $items[$this->path . '/add'] = array(
      'title callback' => 'entity_ui_get_action_title',
      'title arguments' => array('add', $this->entityType),
      'page callback' => 'entity_ui_get_form',
      'page arguments' => array($this->entityType, NULL, 'add'),
      'access callback' => 'entity_access',
      'access arguments' => array('create', $this->entityType),
      'type' => MENU_LOCAL_ACTION,
      'file path' => $path . '/includes',
      'file' => 'eventbrite_event.forms.inc',
    );
    $items[$eventbrite_event_uri . '/%eventbrite_event/edit'] = array(
      'title' => 'Edit',
      'page callback' => 'entity_ui_get_form',
      'page arguments' => array($this->entityType, $id_count),
      'load arguments' => array($this->entityType),
      'access callback' => 'entity_access',
      'access arguments' => array('update', $this->entityType, $id_count),
      'type' => MENU_LOCAL_TASK,
      'file path' => $path . '/includes',
      'file' => 'eventbrite_event.forms.inc',
      'weight' => -9,
    );

    // Clone form, a special case for the edit form.
    $items[$eventbrite_event_uri . '/%eventbrite_event/clone'] = array(
      'title' => 'Clone',
      'page callback' => 'entity_ui_get_form',
      'page arguments' => array($this->entityType, $id_count, 'clone'),
      'load arguments' => array($this->entityType),
      'access callback' => 'entity_access',
      'access arguments' => array('create', $this->entityType),
      'type' => MENU_LOCAL_TASK,
      'file path' => $path . '/includes',
      'file' => 'eventbrite_event.forms.inc',
    );
    // Menu item for operations like revert and delete.
    // $items[$eventbrite_event_uri . '/%eventbrite_event/%'] = array(
      // 'page callback' => 'drupal_get_form',
      // 'page arguments' => array($this->entityType . '_operation_form', $this->entityType, $id_count, $id_count + 1),
      // 'load arguments' => array($this->entityType),
      // 'access callback' => 'entity_access',
      // 'access arguments' => array('delete', $this->entityType, $id_count),
      // 'file' => 'includes/entity.ui.inc',
    // );

    // if (!empty($this->entityInfo['exportable'])) {
      // // Menu item for importing an entity.
      // $items[$this->path . '/import'] = array(
        // 'title callback' => 'entity_ui_get_action_title',
        // 'title arguments' => array('import', $this->entityType),
        // 'page callback' => 'drupal_get_form',
        // 'page arguments' => array($this->entityType . '_operation_form', $this->entityType, NULL, 'import'),
        // 'access callback' => 'entity_access',
        // 'access arguments' => array('create', $this->entityType),
        // 'file' => 'includes/entity.ui.inc',
        // 'type' => MENU_LOCAL_ACTION,
      // );
    // }
dpm($items);
    return $items;
  }

  /**
   * Overrides EntityDefaultUIController::overviewForm() defaults.
   */
  public function overviewForm($form, &$form_state) {
    // Synchronize organizer data first if cache is expired
//    eventbrite_sync('user_list_venues');

    $form = parent::overviewForm($form, $form_state);
    return $form;
  }

  /**
   * Overrides EntityDefaultUIController::overviewTableHeaders().
   */
  protected function overviewTableHeaders($conditions, $rows, $additional_header = array()) {
    $header = parent::overviewTableHeaders($conditions, $rows, $additional_header);

    // Change table column name from "Label" to "Event Name"
    $header[0] = t('Event Name');
    $extra = array(
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

    $extra = array(
      'data'=> $entity->status ? t('Live') : t('Draft'),
    );

    array_splice($row, 1, 0, $extra);
    
    return $row;
  }
}

