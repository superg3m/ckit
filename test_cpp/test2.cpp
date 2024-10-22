#include "../ckit.hpp"

void test_func() {

	ckit::Vector2D vec(5.0f);
	ckit::Vector2D vec2(1.0f);

	ckit::Vector2D vec3(3.0f);
	vec3 = vec3 * 4;

	std::cout << "X: " << vec3.x << std::endl; // Access and print the x component
    std::cout << "Y: " << vec3.y << std::endl; // Access and print the y component
}