#include "../core/Basic/ckit_types.h"
#include "../core/Basic/ckit_math.h"

typedef struct Vertex {
	Vec3 position;
	Vec3 normal;
	Vec3 texture_coordinate;
} Vertex;

// This will contain both mesh and model!