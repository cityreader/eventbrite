<?php

/**
 * Controls metadata for Eventbrite venue
 *
 * Redhen utilizes custom property attributes to determine if a property
 * should be available as a filter on the listing page. These attributes are
 *   filter: TRUE to add as a filter.
 *   filter_operator: EFQ supported operators. Defaults to = or IN depending on value submitted
 *   field_type: textfield, select, etc.
 *   options list: callback that returns the options for this field.
 */
class EventbriteVenueMetadataController extends EntityDefaultMetadataController {

  public function entityPropertyInfo() {
    $info = parent::entityPropertyInfo();
    $properties = &$info[$this->type]['properties'];

    $properties['full_address'] = array(
      'label' => t('Full address'),
      'filter' => TRUE,
      'type' => 'text',
      'getter callback' => 'eventbrite_property_venue_get',
      'computed' => TRUE,
      'entity views field' => TRUE,
    );

    return $info;
  }

}

