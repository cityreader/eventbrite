Drupal.behaviors.eventBrite = function (context) {

  //-------------------------------------------------------
  // EVENT SOURCE
  //
  // Toggle between load event and create/edit event widgets
  $('input.eventbrite_cck-new-toggle').click(function() {
    if ($(this).attr('value') == 'load' ) {
      $('fieldset.eventbrite_cck-new-load').show();
      $('fieldset.eventbrite_cck-new-create').hide();
    }
    else if ($(this).attr('value') == 'create' ) {
      $('fieldset.eventbrite_cck-new-load').hide();
      $('fieldset.eventbrite_cck-new-create').show();
    }
  });

  // If Event Source "toggle" is displayed, hide unselected fieldsets
  if ($('.eventbrite_cck-new-toggle').length) {
    if ($('input:checked.eventbrite_cck-new-toggle').val() != 'load') {
      $('fieldset.eventbrite_cck-new-load').hide();
    }
    if ($('input:checked.eventbrite_cck-new-toggle').val() != 'create') {
      $('fieldset.eventbrite_cck-new-create').hide();
    }
  }
  //
  //-------------------------------------------------------

  //-------------------------------------------------------
  // VENUE
  //
  // Toggle between existing venue and new venue
  $('input.eventbrite_cck-venue-toggle').click(function() {
    if ($(this).attr('value') == 'existing' ) {
      $('.eventbrite_cck-existing-venue-toggle').show();
      $('.eventbrite_cck-new-venue-toggle').hide();
    }
    else if ($(this).attr('value') == 'new' ) {
      $('.eventbrite_cck-existing-venue-toggle').hide();
      $('.eventbrite_cck-new-venue-toggle').show();
    }
  });

  $('fieldset.eventbrite_cck-existing-venue-toggle').hide();
  $('fieldset.eventbrite_cck-new-venue-toggle').hide();

  // If Event Venue "toggle" is populated, hide unselected fieldsets
  if ($('.eventbrite_cck-venue-toggle').length) {
    if ($('input:checked.eventbrite_cck-venue-toggle').val() == 'existing') {
      $('.eventbrite_cck-existing-venue-toggle').show();
    }
    if ($('input:checked.eventbrite_cck-venue-toggle').val() == 'new') {
      $('.eventbrite_cck-new-venue-toggle').show();
    }
  }
  //
  //-------------------------------------------------------
  
  //-------------------------------------------------------
  // TICKET
  //
  // Set ticket input fields to disabled (readonly does not work for some field elements)
  //   If ticket update does not get checked these fields will get re-enabled on node submit
  updateTicketFields();
  
  // Trying to toggle whether payment fields are editable
  $('input.eventbrite_cck-ticket-toggle').click(function() {
    updateTicketFields();
  });
  
  function updateTicketFields() {
    if ($('input.eventbrite_cck-ticket-toggle').attr('checked')) {
      $('.eventbrite_cck-ticket-input').attr('disabled', false);
      $('.eventbrite_cck-ticket-wrapper').css('display', 'block');
      $('input.eventbrite_cck-ticket-toggle').attr('disabled', 'disabled');
    }
    else {
      if ($('input.eventbrite_cck-ticket-id').length) {
        // This ticket has already been created, show fields disabled
        $('.eventbrite_cck-ticket-input').attr('disabled', 'disabled');
      }
      else {
	// Disable ticket settings if they have been created
        $('.eventbrite_cck-ticket-wrapper').css('display', 'none');
      }
      // Enabling ticket toggle
      $('input.eventbrite_cck-ticket-toggle').attr('disabled', false);
    }
  }

  function reEnableTicketFields() {
    $('input.eventbrite_cck-ticket-input').attr('disabled', false);
    $('textarea.eventbrite_cck-ticket-input').attr('disabled', false);
    $('input.eventbrite_cck-ticket-toggle').attr('disabled', false);
  }
  //
  //-------------------------------------------------------

  //-------------------------------------------------------
  // PAYMENT
  //
  // Set payment input fields to disabled (readonly does not work for some field elements)
  //   If payment update does not get checked these fields will get re-enabled on node submit
  $('input:checkbox.eventbrite_cck-payment-toggle').click(function() {
    if ($(this).attr('checked')) {
      $('div.eventbrite-payment-settings-group').show();
    }
    else {
      $('div.eventbrite-payment-settings-group').hide();
    }
  });

  if ($('input:checkbox.eventbrite_cck-payment-toggle').length > 0) {
    if ($('input:checkbox.eventbrite_cck-payment-toggle').attr('checked')) {
      $('div.eventbrite-payment-settings-group').show();
    }
    else {
      $('div.eventbrite-payment-settings-group').hide();
    }
  }


/*
  updatePaymentFields();


  // Trying to toggle whether payment fields are editable
  $('input.eventbrite_cck-payment-toggle').click(function() {
    updatePaymentFields();
  });

  function reEnablePaymentFields() {
    $('input.eventbrite_cck-payment-toggle').attr('disabled', false);    
  }
  
  function updatePaymentFields() {
    if ($('input.eventbrite_cck-payment-toggle').attr('checked')) {
      $('input.eventbrite-payment-settings').parent().css('display', 'block');
      $('input.eventbrite-payment-settings').attr('disabled', false);
      $('input.eventbrite_cck-payment-toggle').attr('disabled', 'disabled');
    }
    else {
      if ($('input.eventbrite_cck-payment-empty').length) {
        // Hide payment settings if none have been created
        $('input.eventbrite-payment-settings').parent().css('display', 'none');
      }
      else {
        // Disable payment settings if they have been created
        $('input.eventbrite-payment-settings').attr('disabled', 'disabled');
      }
      $('input.eventbrite_cck-payment-toggle').attr('disabled', false);
    }
  }
*/
  //
  //-------------------------------------------------------

  //-------------------------------------------------------
  // GENERAL
  //
  $('fieldset.hide').hide();

  // On submit, re-enable all disabled fields so that values get submitted
  $('.form-submit').click(function() {
    reEnableTicketFields();
    reEnablePaymentFields();
  });
  //
  //-------------------------------------------------------

};
