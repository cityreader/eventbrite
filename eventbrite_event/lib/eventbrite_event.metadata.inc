<?php

/**
 * Controls metadata for Eventbrite event
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
      'label' => t('Event ID'),
      'type' => 'integer',
      'description' => t("The unique ID of the event."),
      'schema field' => 'event_id',
    );

    $properties['title'] = array(
      'label' => t('Title'),
      'description' => t('The event title.'),
      'type' => 'text',
      'getter callback' => 'entity_property_verbatim_get',
      'schema field' => 'title',
      'required' => TRUE,
    );

    $properties['description'] = array(
      'label' => t('Description'),
      'description' => t('The event description.'),
      'type' => 'text',
      'setter callback' => 'entity_property_verbatim_set',
      'schema field' => 'description',
    );

    $properties['privacy'] = array(
      'label' => t('Privacy'),
      'description' => t('Private or public event.'),
      'type' => 'integer',
      'options list' => 'eventbrite_event_privacy_options',
      'schema field' => 'privacy',
    );
    
    $properties['personalized_url'] = array(
      'label' => t('Personalize URL'),
      'description' => t('The event registration URL.'),
      'type' => 'uri',
      'setter callback' => 'entity_property_verbatim_set',
      'schema field' => 'personalized_url',
    );

    $properties['status'] = array(
      'label' => t('Status'),
      'description' => t('The event status.'),
      'type' => 'text',
      'options list' => 'eventbrite_event_status_options',
      'schema field' => 'status',
    );

    $properties['created'] = array(
      'label' => t('Created'),
      'description' => t("The date the event was created."),
      'type' => 'date',
      'setter callback' => 'entity_property_verbatim_set',
      'schema field' => 'created',
    );

    $properties['modified'] = array(
      'label' => t('Updated'),
      'description' => t('The date the event was modified.'),
      'type' => 'date',
      'setter callback' => 'entity_property_verbatim_set',
      'schema field' => 'modified',
    );

    // Entities
    $properties['venue'] = array(
      'label' => t('Venue'),
      'description' => t('The venue of the event'),
      'type' => 'eventbrite_venue',
      'schema field' => 'venue_id'
    );

    $properties['organizer'] = array(
      'label' => t('Organizer'),
      'description' => t('The organizer of the event'),
      'type' => 'eventbrite_venue',
      'schema field' => 'organizer_id',
    );

    return $info;
  }

}

