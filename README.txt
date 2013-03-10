Eventbrite 2.x

Eventbrite 2.x offers Drupal sites the ability to manage Eventbrite events from with
  Drupal.  Site builders can create and edit events as well as other Eventbrite entities
  such as venues, tickets, payment settings and organizers.  CCK serves as the bridge
  to the Eventbrite API.  There is an event field that can be attached to nodes.
  
Evenbrite 6.x-2.0-beta2 is about to be released.  If you have installed 6.x-2.0-beta1 you
  will need to reinstall.  DB changes have been made since 6.x-2.0-beta1.

This module also offers integration with Organic Groups.  Each group may be associated
  with a sub-user of the site's primary Eventbrite account.

The base module exposes an API for other modules to call.


Eventbrite Base Module (API)

Calls made to the server  are cached to cut down on unnecessary requests as well as 
  decreasing page render time.
  
Exposed API functions (callable by UI submodules)

eventbrite_get($op, $id = NULL);
  Params: $op  The remote API method at Eventbrite server
               Current supported $op values are:
                 user_list_events
                 event_get
          $id  Optional param for the remote API calls that take an id key

eventbrite_save($op, $object);
  Params: $op  The remote API method at Eventbrite server
               Current supported $op values are:
                 event_new
          $id  Object to be saved

Exposed Hooks

hook_eventbrite_request_alter(&$request);
  This hook allows UI submodules to alter the request before it goes to the Eventbrite
    server.  This will allow modules to alter the maximum cache time as well as change
    the user making the request.  It is used by Eventbrite OG to allow for user 
    authentication by group.
