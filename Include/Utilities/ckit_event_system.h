#pragma once

#include "../ckit_types.h"
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