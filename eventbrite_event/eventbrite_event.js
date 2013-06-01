(function ($) {

Drupal.behaviors.eventbriteEvent = {
  attach: function (context, settings) {
    $('iframe').each(function () {
        var iframe_height = $(this).height();

        if (iframe_height > 81 ) {
          iframe_height += 70;
        }
        console.log(iframe_height);
        $(this).height(iframe_height);
    });
  }
}

})(jQuery);
