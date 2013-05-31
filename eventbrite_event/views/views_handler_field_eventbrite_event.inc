<?php

/**
 * @file
 * Contains the basic 'node' field handler.
 */

/**
 * Field handler to provide simple renderer that allows linking to a node.
 * Definition terms:
 * - link_to_node default: Should this field have the checkbox "link to node" enabled by default.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_eventbrite_event extends views_handler_field {

  function init(&$view, &$options) {
    parent::init($view, $options);
    // Don't add the additional fields to groupby
    if (!empty($this->options['link_to_event'])) {
      $this->additional_fields['event_id'] = array('table' => 'eventbrite_event', 'field' => 'event_id');
    }
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['link_to_event'] = array('default' => isset($this->definition['link_to_event default']) ? $this->definition['link_to_event default'] : FALSE, 'bool' => TRUE);
    return $options;
  }

  /**
   * Provide link to node option
   */
  function options_form(&$form, &$form_state) {
    $form['link_to_event'] = array(
      '#title' => t('Link this field to the original piece of event'),
      '#description' => t("Enable to override this field's links."),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_to_event']),
    );

    parent::options_form($form, $form_state);
  }

  /**
   * Render whatever the data is as a link to the node.
   *
   * Data should be made XSS safe prior to calling this function.
   */
  function render_link($data, $values) {
    if (!empty($this->options['link_to_event']) && !empty($this->additional_fields['event_id'])) {
      if ($data !== NULL && $data !== '') {
        $this->options['alter']['make_link'] = TRUE;
        $this->options['alter']['path'] = eventbrite_event_uri() . '/' . $this->get_value($values, 'event_id');
      }
      else {
        $this->options['alter']['make_link'] = FALSE;
      }
    }
    return $data;
  }

  function render($values) {
    $value = $this->get_value($values);
    return $this->render_link($this->sanitize_value($value), $values);
  }
}
