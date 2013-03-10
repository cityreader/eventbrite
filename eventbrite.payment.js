Drupal.behaviors.eventbritePayment = function (context) {
  $('input:checkbox.eventbrite-payment-settings').click(function() {
    if ($(this).attr('checked')) {
      $(this).parent('label').parent('div.form-item').siblings('div.form-item').show();
    }
    else {
      $(this).parent('label').parent('div.form-item').siblings('div.form-item').hide();
      
    }
  });

  $('input:checkbox.eventbrite-payment-settings').each(function() {
    if ($(this).attr('checked')) {
      $(this).parent('label').parent('div.form-item').siblings('div.form-item').show();
    }
    else {
      $(this).parent('label').parent('div.form-item').siblings('div.form-item').hide();
    }
  });

}