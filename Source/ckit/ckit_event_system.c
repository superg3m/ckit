#include "ckit_types.h"

typedef struct EventSystem {
	int a;
	
} EventSystem;

typedef enum EventPermissions {
	EXLUSIVE_OWNERSHIP, // Only one owners of the event
	// VOLATILE_OWNERSHIP, // There exists an owner, but you allow events to be stolen with the promise that the owner retains power. I don't know the usecase here
	MUTUAL_OWNERSHIP, // multiple owners of the event
} EventPermissions;

typedef u32 EventID;

// Concept of ownership
typedef struct EventPacket {
	u32 number_of_owners; // if exlusive only allowed 1 owner
	void* owners; // Linked list of owners and tail is the most current owner this allows you to have shared ownership in a good constant time
	void* callbacks; // linked list of callbacks
	u32 packet_data_size;
	void* packet_data;
	EventPermissions event_permissions;
} EventPacket;

// VOLATILE_OWNERSHIP
EventPacket event_steal_ownership(EventID id); // ensure 
void event_transfer_ownership(EventID id, void* old_owner, void* new_owner);

void event_process();

// Prob not gonna live here
typedef struct KeyCodeDown {
  Boolean A;
  Boolean B;
  Boolean C;
  Boolean D;
  Boolean E;
  Boolean F;
  Boolean G;
  Boolean H;
  Boolean I;
  Boolean J;
  Boolean K;
  Boolean L;
  Boolean M;
  Boolean N;
  Boolean O;
  Boolean P;
  Boolean Q;
  Boolean R;
  Boolean S;
  Boolean T;
  Boolean U;
  Boolean V;
  Boolean W;
  Boolean X;
  Boolean Y;
  Boolean Z;
  Boolean SPACE;
  Boolean TAB;
  Boolean SHIFT;
  Boolean CTRL;
  Boolean ALT;
  Boolean ESC;
  Boolean ENTER;
  Boolean BACKSPACE;
  Boolean F1;
  Boolean F2;
  Boolean F3;
  Boolean F4;
  Boolean F5;
  Boolean F6;
  Boolean F7;
  Boolean F8;
  Boolean F9;
  Boolean F10;
  Boolean F11;
  Boolean F12;
} KeyCodeDown;