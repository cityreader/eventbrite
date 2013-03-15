<?php

/**
 * @file
 * Defines venue handling functions
 */

/**
 * UI controller.
 */
class EventbriteOrganizerUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    $items[$this->path]['title'] = t('Organizers');
    $items[$this->path]['type'] = MENU_LOCAL_TASK;
    return $items;
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


/*
 * Organizer Edit Form
 */
/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
//function eventbrite_organizer($form, $form_state, $organizer_id = NULL) {
//  $form = array();
//
//  if (isset($organizer_id) && $organizer = eventbrite_organizer_load($organizer_id)) {
//    $form = eventbrite_organizer_form($organizer);
//
//    $form['submit'] = array(
//      '#type' => 'submit',
//      '#value' => t('Update organizer information'),
//    );
//  }
//  else {
//    $form = eventbrite_organizer_form();
//
//    $form['submit'] = array(
//      '#type' => 'submit',
//      '#value' => t('Create this organizer'),
//    );
//  }
//
//  return $form;
//}


/**
 * Generates the Eventbrite organizer editing form.
 *
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function eventbrite_organizer_form($form, &$form_state, $entity, $op = 'edit') {
  $form = array();

  $form['organizer_id'] = array(
    '#type' => 'value',
    '#value' => $entity->organizer_id,
  );

  // Name is the only required field
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => $entity->name,
    '#required' => 1,
    '#maxlength' => 32,
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#default_value' => $entity->description,
  );

  $submit_value = ($op == 'add') ? t('Create this organizer') : t('Update organizer information');

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => $submit_value,
    '#weight' => 40,
  );

  if ($op != 'add' && $op != 'clone') {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete organization'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
//      '#submit' => array('profile2_type_form_submit_delete')
    );
  }
  return $form;
}

/**
 * Form API submit callback for the type form.
 */
function eventbrite_organizer_form_submit(&$form, &$form_state) {
  $eventbrite_organizer = entity_ui_form_submit_build_entity($form, $form_state);

  // Save and go back.
  $eventbrite_organizer->save();
  $form_state['redirect'] = 'admin/config/services/eventbrite/organizers';
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function eventbrite_organizer_load($organizer_id) {
  $result = db_query('SELECT * FROM {eventbrite_organizer} WHERE organizer_id = :organizer_id', array(':organizer_id' => $organizer_id));
  foreach ($result AS $row) {
    return $row;
  }
}

///*
// * Default Payment Settings Form Validate
// */
///**
// * @todo Please document this function.
// * @see http://drupal.org/node/1354
// */
//function eventbrite_organizer_submit($form, &$form_state) {
//  $organizer = _eventbrite_populate_organizer_from_form($form_state['values']);
//  eventbrite_organizer_save($organizer);
//
//  return;
//}

///*
// * This function pulls venue from the form results and builds a venue object for base Eventbrite module
// */
//function _eventbrite_populate_organizer_from_form($item) {
//  $organizer = new stdClass();
//
//  if (!empty($item['organizer_id'])) {
//    $organizer->organizer_id = $item['organizer_id'];
//  }
//  $organizer->name = $item['name'];
//  $organizer->description = $item['description'];
//
//  return $organizer;
//}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function eventbrite_organizer_get_enabled() {
  // If Eventbrite OG is enabled and there is a group context
  // TODO: This filtering of organizers by sub modules should really happen via a hook
  if (module_exists('eventbrite_og') && $group_node = og_get_group_context()) {
    $filter_organizers = eventbrite_og_get_enabled_organizers($group_node);
  }
  else {
    // If Eventbrite OG is not overriding this field, use default user key for filtering
    $default_organizers = eventbrite_get('user_list_organizers');
    $filter_organizers = array_keys($default_organizers);
  }

  $enabled = array();
  $result = db_query('SELECT organizer_id, name FROM {eventbrite_organizer} WHERE status = :status', array(':status' => 1));
  foreach ($result AS $row) {
    if (!isset($filter_organizers) || in_array($row->organizer_id, $filter_organizers)) {
      $enabled[$row->organizer_id] = $row->name;
    }
  }
  return $enabled;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function eventbrite_organizer_list_form($form, $form_state, $user_key = '') {
  // Set default user key to site wide Eventbrite user key
  if (empty($user_key)) {
    $user_key = variable_get('eventbrite_user_key', '');
    if (empty($user_key)) {
      return t('No Eventbrite user key has been configured for this account.');
    }
  }

  // This refreshes organizers for the site-wide account
  $organizers = eventbrite_get('user_list_organizers');

  if ($organizers) {
    $form = array();

    $header = array(
      array(),
      array(
        'data' => t('Organizer Name'),
        'field' => 'name',
      ),
      array(
        'data' => t('URL'),
        'field' => 'url',
      ),
      array(
        'data' => t('Description'),
        'field' => 'description',
      ),
    );
    $results = db_select('eventbrite_organizer', 'eo')
                ->condition('user_key', $user_key, '=')
                ->fields('eo', array('organizer_id', 'status', 'name', 'url', 'description'))
                ->extend('TableSort')->orderByHeader($header)
                ->extend('PagerDefault')->limit(50)
                ->execute();

    $destination = drupal_get_destination();

    $organizer_array = array();
    foreach ($results AS $organizer) {
      print_r($organizer);
      $organizer_id = $organizer->organizer_id;
      $organizer_array[$organizer_id] = '';

      $form['name'][$organizer_id] = array(
        '#value' => l(
          $organizer->name,
          'admin/config/eventbrite/organizers/' . $organizer_id,
          array('query' => $destination)
        ),
      );
      $form['url'][$organizer_id] = array(
        '#value' => l(
          $organizer->url,
          $organizer->url
        ),
      );
      $form['description'][$organizer_id] = array('#value' => $organizer->description);
    }

    $enabled_organizers = eventbrite_organizer_get_enabled();
    $form['enabled_organizers'] = array(
      '#type' => 'checkboxes',
      '#options' => $organizer_array,
      '#default_value' => array_keys($enabled_organizers),
    );

    if ($enabled_organizers) {
      $form['default_organizer'] = array(
        '#type' => 'select',
        '#title' => t('Default Organizer'),
        '#default_value' => variable_get('eventbrite_organizer_default', ''),
        '#options' => $enabled_organizers,
        '#required' => TRUE,
      );
    }

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save enabled organizers'),
      '#weight' => 100,
    );
    $form['pager'] = array('#value' => theme('pager', array('tags' => NULL, 'element' => 0)));

    return $form;
  }

  return t('There are currently no organizers conifigured for this user account.');
}

/*
 * Eventbrite Organizer Form Validate
 */
/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function eventbrite_organizer_list_form_validate($form, &$form_state) {
  // TODO: Check that default organizer is an enabled organizer
  $default_organizer = $form_state['values']['default_organizer'];
  $enabled_organizers = $form_state['values']['enabled_organizers'];

  if (array_sum($enabled_organizers) == 0) {
    form_set_error('enabled_organizers', t('You must enable at least one Organizer.'));
  }

  if (isset($enabled_organizers[$default_organizer])) {
    if (empty($enabled_organizers[$default_organizer])) {
      form_set_error('enabled_organizers', t('Your default Organizer must be enabled in the Organizer list.'));
    }
  }
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function eventbrite_organizer_list_form_submit($form, &$form_state) {
  $enabled_organizers = $form_state['values']['enabled_organizers'];

  variable_set('eventbrite_organizer_default', $form_state['values']['default_organizer']);

  // Get current status for all organizers in DB
  $db_organizers = array();
  // Make sure DB matches enabled organizers list
  // TODO Please convert this statement to the D7 database API syntax.
  $result = db_query('SELECT organizer_id, status FROM {eventbrite_organizer} eo WHERE 1');
  foreach ($result AS $organizer) {
    $db_organizers[$organizer->organizer_id] = $organizer;
  }

  // Scroll through organizers returned by form.  Since this was a pager query we don't
//   want to affect any organizers that weren't listed on this pager page
  foreach ($enabled_organizers as $organizer_id => $form_organizer_status) {
    $form_organizer_status = ($form_organizer_status) ? 1 : 0;
    // For each organizer, if status has changed update the DB
    $db_organizer = $db_organizers[$organizer_id];
    if ($form_organizer_status != $db_organizer->status) {
      $db_organizer->status = $form_organizer_status;
      drupal_write_record('eventbrite_organizer', $db_organizer, array('organizer_id'));
      // TODO: Log to watchdog if this doesn't write
    }
  }

}


///**
// * @todo Please document this function.
// * @see http://drupal.org/node/1354
// */
//function eventbrite_organizer_save($organizer) {
////  Check to see if this organizer already exists on Eventbrite server by organizer_id
//  if (isset($organizer->organizer_id) && !empty($organizer->organizer_id)) {
//    // This venue already has an organizer_id, so organizer_update is the API call
//    if ($saved_result = eventbrite_save('organizer_update', $organizer)) {
//      drupal_set_message(t('This organizer has been updated on the Eventbrite server.'));
//    }
//    else {
//      drupal_set_message(t('This organizer could not be updated on the Eventbrite server'), 'error');
//      return;
//    }
//  }
//  else {
//    // This organizer does not exist, so organizer_new is the API call
//    if ($saved_result = eventbrite_save('organizer_new', $organizer)) {
//      drupal_set_message(t('This organizer has been created on the Eventbrite server.'));
//    }
//    else {
//      drupal_set_message(t('This organizer could not be created on the Eventbrite server'), 'error');
//      return;
//    }
//  }
//  if ($saved_result) {
//    eventbrite_organizer_save_local($organizer);
//    return $saved_result;
//  }
//}

///**
// * @todo Please document this function.
// * @see http://drupal.org/node/1354
// */
//function eventbrite_organizer_save_local($organizer) {
//  // Only save locally if there is already an Eventbrite Organizer ID
//  if (isset($organizer->organizer_id) && !empty($organizer->organizer_id)) {
//    $result = db_query('SELECT * FROM {eventbrite_organizer} WHERE organizer_id = :organizer_id', array(':organizer_id' => $organizer->organizer_id));
//    if ($result) {
//      foreach ($result AS $organizer) {
//        // This event already exists in the local DB, so update it
//        return drupal_write_record('eventbrite_organizer', $organizer, array('organizer_id'));
//      }
//    }
//    else {
//      // This venue does not exist in the local DB, so insert it
//      // Set status here
//      if (empty($organizer->name)) {
//        $organizer->status = 0;
//      }
//      else {
//        $organizer->status = 1;
//      }
//      return drupal_write_record('eventbrite_organizer', $organizer);
//    }
//  }
//}

///* Theme organizer list form.
// *
// * @ingroup themeable
// */
///**
// * @todo Please document this function.
// * @see http://drupal.org/node/1354
// */
//function theme_eventbrite_organizer_list_form($variables) {
//  $form = $variables['form'];
//  // TODO Please change this theme call to use an associative array for the $variables parameter.
//  $header = array(
//    theme('table_select_header_cell'),
//    array(
//      'data' => t('Organizer Name'),
//      'field' => 'eo.name',
//    ),
//    array(
//      'data' => t('URL'),
//      'field' => 'eo.url',
//    ),
//    array(
//      'data' => t('Description'),
//      'field' => 'eo.description',
//    ),
//  );
//
//  $output = drupal_render($form['default_organizer']);
//
//  $output .= drupal_render($form['options']);
//  if (isset($form['name']) && is_array($form['name'])) {
//    foreach (element_children($form['name']) as $key) {
//      $rows[] = array(
//        drupal_render($form['enabled_organizers'][$key]),
//        drupal_render($form['name'][$key]),
//        drupal_render($form['url'][$key]),
//        drupal_render($form['description'][$key]),
//      );
//    }
//  }
//  else {
//    $rows[] = array(array(
//        'data' => t('No organizers were found in this Eventbrite account.'),
//        'colspan' => '7',
//      ));
//  }
//
//  $output .= '<div class="form-item"><label>' . t('Enabled Organizers:') . '</label></div>';
//  $output .= theme('table', array('header' => $header, 'rows' => $rows));
//  if ($form['pager']['#value']) {
//    $output .= drupal_render($form['pager']);
//  }
//
//  $output .= drupal_render_children($form);
//
//  return $output;
//}
