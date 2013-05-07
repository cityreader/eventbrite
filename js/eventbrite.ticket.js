(function($) {

Drupal.behaviors.eventbriteTicket = {
  attach: function(context, settings) {
    $('.eventbrite_ticket_remove').click(function() {
      var row = $(this).closest('td').parent('tr');
      $(row).hide();
      $(row).find('input[name$="name]"]').val('');
      
      ////Zebra color settings////////
      var table_id = $(row).parent('tbody').parent('table').attr('id');
      $('#'+table_id+' tr.draggable:visible').each(function(index, element){
        $(element).removeClass('odd').removeClass('even');
        if((index%2) == 0){
          $(element).addClass('odd');
        } else {
          $(element).addClass('even');
        }
      });

      return false;
    });
    
  }  
};

})(jQuery);
