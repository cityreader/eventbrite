(function($) {

Drupal.behaviors.eventbritePayment = {
  attach: function(context, settings) {
    console.log($('input:checkbox.eventbrite-payment-settings').length);
    $('input:checkbox.eventbrite-payment-settings').click(function() {
      if ($(this).attr('checked')) {
        $(this).parent('div.form-item').siblings('div.form-item').show();
      }
      else {
        $(this).parent('div.form-item').siblings('div.form-item').hide();
        
      }
    });

    $('input:checkbox.eventbrite-payment-settings').each(function() {
      if ($(this).attr('checked')) {
        $(this).parent('div.form-item').siblings('div.form-item').show();
      }
      else {
        $(this).parent('div.form-item').siblings('div.form-item').hide();
      }
    });
  }
};

})(jQuery);