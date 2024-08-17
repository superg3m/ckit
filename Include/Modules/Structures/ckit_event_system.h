#pragma once

#include "./ckit_types.h"
//========================== Begin Types ==========================
typedef u32 CKIT_EventID;
typedef u32 CKIT_RegisteredEvent_ID;

typedef enum EventPermissions {
	EXLUSIVE_OWNERSHIP, // Only one owners of the event
	VOLATILE_OWNERSHIP, // There exists an owner, but you allow events to be stolen with the promise that the owner retains power.
	MUTUAL_OWNERSHIP, // multiple owners of the event
} EventPermissions;

typedef void (CKIT_EventCallback)(void*);

typedef struct EventPacket EventPacket;
//=========================== End Types ===========================

//************************* Begin Functions *************************
#ifdef __cplusplus
extern "C" {
#endif
	// VOLATILE_OWNERSHIP
	// EventPacket event_steal_ownership(EventID id);

	ckit_event_create(CKIT_EventID id);
	CKIT_RegisteredEvent_ID ckit_event_register(CKIT_EventID id);
	void ckit_event_steal_event(CKIT_EventID id, CKIT_RegisteredEvent_ID registered_id); // if you try to steal an exclusively owned event it asserts
	void ckit_event_process();
#ifdef __cplusplus
}
#endif
//************************** End Functions **************************

//+++++++++++++++++++++++++++ Begin Macros ++++++++++++++++++++++++++
//++++++++++++++++++++++++++++ End Macros +++++++++++++++++++++++++++

#if defined(CKIT_IMPL)
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
	void ckit_event_register();
	void event_process();
#endif // CKIT_IMPL

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