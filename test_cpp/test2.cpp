#include "../ckit.hpp"


void test_func() {

	ckit::Vector2 vec(1.0f, 2.0f);
	ckit::Vector2 vec2(3.0f, -4.0f);

	ckit::Vector2 vec3 = vec2 - vec;
	float angle_from_origin = vec3.getAngleFromOrigin();

	std::cout << "X: " << vec3.x << std::endl; // Access and print the x component
    std::cout << "Y: " << vec3.y << std::endl; // Access and print the y component

	std::cout << "Origin: " << angle_from_origin << std::endl;
}