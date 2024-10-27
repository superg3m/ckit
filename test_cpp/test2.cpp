#include "../ckit.hpp"


void test_func() {

	ckit::Vector vec(1.0f, 2.0f);
	ckit::Vector vec2(3.0f, -4.0f);

	ckit::Vector vec3 = vec2 - vec;
	float angle_from_origin = vec3.getAngleFromOrigin();

	std::cout << "X: " << vec3.x << std::endl; // Access and print the x component
    std::cout << "Y: " << vec3.y << std::endl; // Access and print the y component

	ckit::Vector view_vector_player(2.0f, 2.0f);
	ckit::Vector view_vector_enemy(1.0f, 1.0f); // trying to backstab

	ckit::Vector normalized_enemy_to_player_vector = view_vector_enemy - view_vector_player;
	normalized_enemy_to_player_vector.normalize();

	float backstab_value = ckit::Vector::dotProduct(view_vector_player.normalize(), normalized_enemy_to_player_vector);

	const u8 threshold_angle = 60; // degrees
    float backstab_threshold = -cosf(threshold_angle * PI / 180.0f);

	std::cout << "Backstab Threshhold: " << backstab_threshold << std::endl;
	std::cout << "Backstab Value: " << backstab_value << std::endl;

	if (backstab_value < backstab_threshold) {
		std::cout << "You have been backstabbed!" << std::endl;
	}
}