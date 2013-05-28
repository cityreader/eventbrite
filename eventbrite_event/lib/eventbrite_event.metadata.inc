<?php

/**
 * Controls metadata for Redhen contacts
 *
 * Redhen utilizes custom property attributes to determine if a property
 * should be available as a filter on the listing page. These attributes are
 *   filter: TRUE to add as a filter.
 *   filter_operator: EFQ supported operators. Defaults to = or IN depending on value submitted
 *   field_type: textfield, select, etc.
 *   options list: callback that returns the options for this field.
 */
class EventbriteEventMetadataController extends EntityDefaultMetadataController {

  public function entityPropertyInfo() {
    $info = parent::entityPropertyInfo();
    $properties = &$info[$this->type]['properties'];

    $properties['event_id'] = array(
      'label' => t("Event ID"),
      'type' => 'integer',
      'description' => t("The unique ID of the event."),
    );

    $properties['title'] = array(
      'label' => t('Title'),
      'description' => t('The event title.'),
      'type' => 'text',
      'setter callback' => 'entity_property_verbatim_set',
    );

    $properties['description'] = array(
      'label' => t('Description'),
      'description' => t('The event description.'),
      'type' => 'text',
      'setter callback' => 'entity_property_verbatim_set',
    );

    $properties['start_date'] = array(
      'label' => t('Start date'),
      'description' => t("The start date of the event."),
      'type' => 'date',
      'setter callback' => 'entity_property_verbatim_set',
    );

    $properties['modified'] = array(
      'label' => t('Updated'),
      'description' => t('The date the event was modified.'),
      'type' => 'date',
      'schema field' => 'modified',
      'setter callback' => 'entity_property_verbatim_set',
    );

    $properties['privacy'] = array(
      'label' => t('Privacy'),
      'filter' => TRUE,
      'type' => 'list',
      'options list' => 'eventbrite_event_privacy_options',
      'setter callback' => 'entity_property_verbatim_set',
    );
    
    $properties['personalized_url'] = array(
      'label' => t('Personalize URL'),
      'description' => t('The event registration URL.'),
      'type' => 'uri',
      'setter callback' => 'entity_property_verbatim_set',
    );

    $properties['venue'] = array(
      'label' => t('Venue'),
      'description' => t('The venue of the event'),
      'type' => 'eventbrite_venue',
      'setter callback' => 'entity_property_verbatim_set',
      'schema field' => 'venue_id',
    );

    $properties['organizer'] = array(
      'label' => t('Venue'),
      'description' => t('The organizer of the event'),
      'type' => 'eventbrite_venue',
      'setter callback' => 'entity_property_verbatim_set',
      'schema field' => 'organizer_id',
    );

    $properties['status'] = array(
      'label' => t('Status'),
      'description' => t('The event status.'),
      'type' => 'boolean',
      'setter callback' => 'entity_property_verbatim_set',
    );

    $properties['end_date'] = array(
      'label' => t('End date'),
      'description' => t("The end date of the event."),
      'type' => 'date',
      'setter callback' => 'entity_property_verbatim_set',
    );

    $properties['created'] = array(
      'label' => t('Created'),
      'description' => t("The date the event was created."),
      'type' => 'date',
      'schema field' => 'created',
      'setter callback' => 'entity_property_verbatim_set',
    );

    return $info;
  }

}
