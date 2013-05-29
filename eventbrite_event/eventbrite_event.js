(function ($) {

Drupal.behaviors.eventbriteEvent = {
  attach: function (context, settings) {
    $('iframe').each(function () {
        var iframe_height = $(this).height() + 70;
        $(this).height(iframe_height);
    });
  }
}
  
})(jQuery);
