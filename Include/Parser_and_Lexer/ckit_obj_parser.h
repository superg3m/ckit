#include "../ckit_types.h"
#include "../Basic/ckit_math.h"

typedef struct Vertex {
	CKIT_Vector3 position;
	CKIT_Vector3 normal;
	CKIT_Vector3 texture_coordinate;
} Vertex;

// This will contain both mesh and model!