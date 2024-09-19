#pragma once

#include "../../Include/Utilities/ckit_event_system.h"

typedef struct Event {
	CKIT_RegisteredEvent_ID registered_id;
	void* callback
} Event;

// Concept of ownership
typedef struct EventPacket {
	void* owner; // singlular owner (the first one to register to a created event is the owner)
	void* renter; // Vector of renter that might want to use the event. (subsequent registers are the renters of an event)
	Event* events; // Vector of callbacks first one is owner.
	EventPermissions event_permissions;
} EventPacket;

// VOLATILE_OWNERSHIP
// EventPacket event_steal_ownership(EventID id);
void ckit_event_register() {

}

void event_process() {

}
/*

usage code!

void custom_application_close_callback(void* device) {
	ckit_audio_device_close((CKIT_AudioDevice*)device);
}

int main() {

	// When a new event get created it has properties.

	ckit_event_create(CKIT_EVENT_APPLICATION_CLOSE, EXLUSIVE_OWNERSHIP);

	ckit_event_register(CKIT_EVENT_APPLICATION_CLOSE, custom_application_close_callback); // is owner now
	ckit_event_register(CKIT_EVENT_APPLICATION_CLOSE, custom_application_close_callback); // is renter
	ckit_event_invoke(CKIT_EVENT_APPLICATION_CLOSE);

}

*/