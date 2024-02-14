/**
 * @file main.cpp
 * @brief Main aladdin 3d file.
 * @version 1.0.0 (2022-10-21)
 * @date 2022-10-21
 * @author Borja Garc�a Quiroga <garcaqub@tcd.ie>
 *
 *
 * Copyright (c) Borja Garc�a Quiroga, All Rights Reserved.
 *
 * The information and material provided below was developed as partial
 * requirements for the MSc in Computer Science at Trinity College Dublin,
 * Ireland.
 */

#include "Main.h"

#include <math.h>

#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <vector>

#include "glew/glew.h"
#include "freeglut/freeglut.h"
#include <glm/gtx/string_cast.hpp>

#include "Classes/Camera/Camera.h"
#include "Classes/Light/Light.h"
#include "Classes/Object/Object.h"
#include "Classes/Shader/Shader.h"
#include "Structs/BoundingBox/BoundingBox.h"

void clean() {

	// Delete all the shaders.
	for (size_t i = 0; i < shaders.size(); i++)
		shaders[i].remove();

}

void createObstacles() {

	// Load the box object.
	aladdin_3d::Object box("Models/Box/Box.gltf", "GLTF");

	// Generate the basic spaces.
	float box_z = corridor_length - 3;
	float box_dist = 1.0;

	while (box_z >= 5) {

		// Append a new box position.
		obstacles_positions.push_back(box_z);
		obstacles_type.push_back("box");

		// Calculate the new box.
		box_z -= box_dist;
		box_dist += 0.2;

	}

	// Get the bounding box.
	aladdin_3d::BoundingBox bb = box.getBoundingBox();

	// Get the object info the object.
	glm::vec3 centre = (bb.min + bb.max) / 2.0f;
	glm::vec3 size = bb.max - bb.min;

	// Add the boxes to the scenario.
	for (size_t i = 0; i < obstacles_positions.size(); i++) {

		// Copy the original box.
		aladdin_3d::Object box_copy = box;

		// Put the box in its right position.
		box_copy.translate(0.0f, size.y * 0.25f, obstacles_positions[i]);

		// Scale the box.
		box_copy.scale(0.5f, 0.5f, 0.5f);

		// Center the box in 0,0.
		box_copy.translate(-centre.x, -centre.x, -centre.z);

		// Append the box.
		objects.push_back(box_copy);
		object_shader.push_back(0);

	}

}

void createFloor() {

	// Vertex 1.
	aladdin_3d::Vertex floor_vert_1{
		glm::vec3(-10.0f, 0.0f, -11.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec2(-10.0f, -11.0f)
	};

	// Vertex 2.
	aladdin_3d::Vertex floor_vert_2{
		glm::vec3(-10.0f, 0.0f, corridor_length + 10.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec2(-10.0f, corridor_length + 10.0f)
	};

	// Vertex 3.
	aladdin_3d::Vertex floor_vert_3{
		glm::vec3(10.0f, 0.0f, corridor_length + 10.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec2(10.0f, corridor_length + 10.0f)
	};

	// Vertex 4.
	aladdin_3d::Vertex floor_vert_4{
		glm::vec3(10.0f, 0.0f, -11.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec2(10.0f, -11.0f)
	};

	// Create the vector of vertices.
	std::vector<aladdin_3d::Vertex> vertices;
	vertices.push_back(floor_vert_1);
	vertices.push_back(floor_vert_2);
	vertices.push_back(floor_vert_4);
	vertices.push_back(floor_vert_2);
	vertices.push_back(floor_vert_3);
	vertices.push_back(floor_vert_4);

	// Create the indices.
	std::vector<GLuint> indices{0, 1, 2, 3, 4, 5};

	// Create the textures.
	aladdin_3d::Texture base_texture("Textures/sand.png", "baseColor", 0);
	aladdin_3d::Texture specular_map("Textures/black.png", "specularMap", 1);
	std::vector<aladdin_3d::Texture> textures{base_texture, specular_map};

	// Create the geometry.
	aladdin_3d::Geometry floor_geom(vertices, indices, textures);
	aladdin_3d::Object floor(std::vector<aladdin_3d::Geometry>{floor_geom});

	// Add the objects to the list.
	objects.push_back(floor);
	object_shader.push_back(0);

}

void createLives() {

	// Vertex 1.
	aladdin_3d::Vertex lives_vert_1{
		glm::vec3(1.448528, 4.621320, 5.803935),
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec2(3.0f, 0.0f)
	};

	// Vertex 2.
	aladdin_3d::Vertex lives_vert_2{
		glm::vec3(1.448528, 6.035533, 5.803935),
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec2(3.0f, 1.0f)
	};

	// Vertex 3.
	aladdin_3d::Vertex lives_vert_3{
		glm::vec3(-1.448528, 6.035533, 5.803935),
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec2(0.0f, 1.0f)
	};

	// Vertex 4.
	aladdin_3d::Vertex lives_vert_4{
		glm::vec3(-1.448528, 4.621320, 5.803935),
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec2(0.0f, 0.0f)
	};

	// Create the vector of vertices.
	std::vector<aladdin_3d::Vertex> vertices;
	vertices.push_back(lives_vert_1);
	vertices.push_back(lives_vert_2);
	vertices.push_back(lives_vert_4);
	vertices.push_back(lives_vert_2);
	vertices.push_back(lives_vert_3);
	vertices.push_back(lives_vert_4);

	// Create the indices.
	std::vector<GLuint> indices{ 0, 1, 2, 3, 4, 5 };

	// Create the textures.
	aladdin_3d::Texture base_texture("Textures/hearts.png", "baseColor", 0);
	aladdin_3d::Texture specular_map("Textures/black.png", "specularMap", 1);
	std::vector<aladdin_3d::Texture> textures{ base_texture, specular_map };

	// Create the geometry.
	aladdin_3d::Geometry lives_geom(vertices, indices, textures);
	aladdin_3d::Object lives_obj(std::vector<aladdin_3d::Geometry>{lives_geom});

	// Add the objects to the list.
	objects.push_back(lives_obj);
	object_shader.push_back(2);

}

void display() {

	// Specify the color of the background
	glClearColor(fog.x, fog.y, fog.z, fog.w);

	// Clean the back buffer and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Get the real current time.
	auto current_time = std::chrono::system_clock::now();
	double real_time = std::chrono::duration<double>(current_time.time_since_epoch()).count();

	// Control time if the game is paused.
	if (is_paused)
		time_start = real_time - internal_time;

	// Transformthis into the internal time.
	internal_time = real_time - time_start;

	// Get the current camera;
	aladdin_3d::Camera camera = cameras[current_camera];
	
	// Updates and exports the camera matrix to the Vertex Shader
	camera.update();

	// Pass the time to all the shaders.
	for (size_t i = 0; i < shaders.size(); i++) {

		shaders[i].activate();
		shaders[i].passFloat("velocity", velocity);
		shaders[i].passFloat("time", internal_time);
		shaders[i].passInt("lives", lives);

	}

	// Draw all the models.
	for (size_t i = 0; i < objects.size(); i++) {

		objects[i].draw(shaders[object_shader[i]], camera);

	}

	// Draw all the characters.
	displayCharacters();

	// Swap the back buffer with the front buffer.
	glutSwapBuffers();

}

void displayCharacters() {

	// Make sure GLUT will refresh.
	glutPostRedisplay();

	// Get the current camera;
	aladdin_3d::Camera camera = cameras[current_camera];

	// Updates and exports the camera matrix to the Vertex Shader
	camera.update();

	// Check what's going on with the jumps.
	float jump_time = internal_time - jump_start;
	float char_y = jump_velocity * jump_time + 0.5 * gravity * jump_time * jump_time;

	// Get the current x.
	float char_z = velocity * internal_time;

	// Set winner if is further the door.
	if (char_z >= corridor_length)
		is_paused = true;

	// If has reached the gound, end the jump.
	if (char_y <= 0.0f)
		jump_start = -1.0f;

	// If now there is an obstacle.
	for (size_t i = 0; i < obstacles_positions.size() && lives > 0; i++) {

		// If this position is before, pass.
		if (obstacles_positions[i] > char_z - 0.5 && obstacles_positions[i] < char_z + 0.5) {

			if (internal_time - last_hit >= 1.0 && char_y < 0.5) {

				lives--;
				last_hit = internal_time;

			}

		}

	}

	// If there are no lives left, pause again.
	if (lives <= 0)
		is_paused = true;

	// Iterate through the characters.
	for (size_t i = 0; i < characters.size(); i++) {

		// Get the character.
		aladdin_3d::Object character = characters[i];

		// Get the centre and size of the legs.
		aladdin_3d::BoundingBox bb_legs = character.getGeometries()[1].getBoundingBox();
		glm::vec3 leg_size = bb_legs.max - bb_legs.min;

		// Calculate the angle.
		float angle = sin(internal_time * 6) * 45.0;

		// Move the legs.
		character.translate(1, 0, leg_size.y, 0);
		character.rotate(1, 1.0f, 0.0f, 0.0f, -angle);
		character.translate(1, 0, -leg_size.y, 0);

		character.translate(2, 0, leg_size.y, 0);
		character.rotate(2, 1.0f, 0.0f, 0.0f, angle);
		character.translate(2, 0, -leg_size.y, 0);

		// Apply 90 rotations to the characters.
		character.rotate(0.0f, 1.0f, 0.0f, -90.0);

		// Get the centre and size of the character.
		aladdin_3d::BoundingBox bb = character.getBoundingBox();
		glm::vec3 centre = (bb.min + bb.max) / 2.0f;
		glm::vec3 size = bb.max - bb.min;

		// Center the object and get it in the right position.
		character.translate(-centre.x, 0, -centre.z);

		// Get the character shader.
		aladdin_3d::Shader char_shader = shaders[character_shader[i]];

		// Pass some things to the character to jump.
		char_shader.activate();
		char_shader.passFloat("jump_start", jump_start);
		char_shader.passFloat("jump_velocity", jump_velocity);
		char_shader.passFloat("gravity", gravity);
		char_shader.passFloat("last_hit", last_hit);

		// Draw the char.
		character.draw(char_shader, camera);

	}

}

void handleSpecialEvents(int key, int x, int y) {

	// Make sure GLUT will refresh.
	glutPostRedisplay();

	// Top arrow will move front.
	if (key == GLUT_KEY_UP) {

		handleKeyEvents('w', x, y);

	}

	// Top arrow will move front.
	if (key == GLUT_KEY_DOWN) {

		handleKeyEvents('s', x, y);

	}

	// Right arrow will move right.
	if (key == GLUT_KEY_RIGHT) {

		handleKeyEvents('d', x, y);

	}

	// Left arrow will move left.
	if (key == GLUT_KEY_LEFT) {

		handleKeyEvents('a', x, y);

	}

}

void handleKeyEvents(unsigned char key, int x, int y) {

	// Handle keyboard events.

	// Make sure GLUT will refresh.
	glutPostRedisplay();

	// Key W will move camera 0 forward.
	if (key == 'w') {

		// Check if camera 0 is the chosen camera.
		if (current_camera != 0) return;

		// Move forward.
		cameras[0].moveFront();
		
	}

	// Key S will move camera 0 backwards.
	if (key == 's') {

		// Check if camera 0 is the chosen camera.
		if (current_camera != 0) return;

		// Move backwards.
		cameras[0].moveBack();

	}

	// Key D will move camera 0 to the right.
	if (key == 'd') {

		// Check if camera 0 is the chosen camera.
		if (current_camera != 0) return;

		// Move right.
		cameras[0].moveRight();

	}

	// Key A will move camera 0 to the right.
	if (key == 'a') {

		// Check if camera 0 is the chosen camera.
		if (current_camera != 0) return;

		// Move to the left.
		cameras[0].moveLeft();

	}

	// Key R will move camera up.
	if (key == 'r') {

		// Check if camera 0 is the chosen camera.
		if (current_camera != 0) return;

		// Move up.
		cameras[0].moveUp();

	}

	// Key F will move camera down.
	if (key == 'f') {

		// Check if camera 0 is the chosen camera.
		if (current_camera != 0) return;

		// Move up.
		cameras[0].moveDown();

	}

	// Key E will rotate camera left.
	if (key == 'e') {

		// Check if camera 0 is the chosen camera.
		if (current_camera != 0) return;

		// Move up.
		cameras[0].rotateLeft();

	}

	// Key Q will rotate the camera right.
	if (key == 'q') {

		// Check if camera 0 is the chosen camera.
		if (current_camera != 0) return;

		// Move up.
		cameras[0].rotateRight();

	}

	// Key T will bring the camera up.
	if (key == 't') {

		// Check if camera 0 is the chosen camera.
		if (current_camera != 0) return;

		// Move up.
		cameras[0].rotateUp();

	}

	// Key G will bring the camera down.
	if (key == 'g') {

		// Check if camera 0 is the chosen camera.
		if (current_camera != 0) return;

		// Move up.
		cameras[0].rotateDown();

	}

	// Key C will toggle the camera.
	if (key == 'c') {

		current_camera = (current_camera + 1) % cameras.size();

	}

	// Key P will pause/play the game.
	if (key == 'p') {

		// Get the current x.
		float char_z = velocity * internal_time;

		// Set winner if is further the door.
		if (char_z >= corridor_length || lives <= 0) {

			is_paused = true;

		} else {

			is_paused = not is_paused;

		}

	}

	// Key space will jump.
	if (key == ' ') {

		if (is_paused) {

			handleKeyEvents('p', x, y);

		} else {

			if (jump_start == -1)
				jump_start = internal_time;

		}

	}

}

float initBuildings(std::vector<aladdin_3d::Object> base_objects, std::vector<int> building_guide, float x_scale) {

	// Get the separation.
	const float separation = 1.0f;

	// Get the sizes and centers.
	std::vector<glm::vec3> sizes(base_objects.size());
	std::vector<glm::vec3> centres(base_objects.size());

	for (size_t i = 0; i < base_objects.size(); i++) {

		// Get the bounding box.
		aladdin_3d::BoundingBox bb = base_objects[i].getBoundingBox();

		// Get the object info the object.
		glm::vec3 centre = (bb.min + bb.max) / 2.0f;
		glm::vec3 size = bb.max - bb.min;

		// Store them.
		sizes[i] = size;
		centres[i] = centre;

	}

	// Cumulative displacement.
	float displacement = -10.0;

	// Now, iterate for each object that we have to create.
	for (size_t i = 0; i < building_guide.size(); i++) {

		// Get the building index from the guide.
		int ind = building_guide[i];

		// Copy the corresponding object.
		aladdin_3d::Object building = base_objects[ind];
		glm::vec3 centre = centres[ind];
		glm::vec3 size = sizes[ind];

		// Add the cumulative displacement.
		building.translate(0, 0, displacement);

		// Add the scaling to flip the object if necessary.
		building.scale(x_scale, 1.0f, 1.0f);

		// Move the buildings 2 units to the back.
		building.translate(-2.0f, 0.0f, 0.0f);

		// Center the object and get it in the right position.
		building.translate(-size.x / 2.0f, 0, size.z / 2.0f);
		building.translate(-centre.x, 0, -centre.z);

		// Add the objects to the list.
		objects.push_back(building);
		object_shader.push_back(0);

		// Add the size and separation so that the next building will get it as cumulative.
		displacement += (size.z + separation);

	}

	return displacement;

}

void initElements() {

	// Create a white light in the center of the world.
	glm::vec3 light_pos = glm::vec3(1.0f, 50.0f, 1.0f);
	glm::vec4 light_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	aladdin_3d::Light sun(light_pos, light_color);

	// Get the shaders.
	aladdin_3d::Shader shader("Shaders/default.vert", "Shaders/default.frag");

	// Pass the light to the shader.
	shader.activate();
	shader.passLight(sun);

	shaders.push_back(shader);

	// Get the shaders.
	aladdin_3d::Shader shader_character("Shaders/character.vert", "Shaders/character.frag");

	// Pass the light to the shader.
	shader_character.activate();
	shader_character.passLight(sun);

	shaders.push_back(shader_character);

	// Get the shaders.
	aladdin_3d::Shader shader_lives("Shaders/lives.vert", "Shaders/lives.frag");

	// Pass the light to the shader.
	shader_lives.activate();

	shaders.push_back(shader_lives);

	// Creates the first camera object
	aladdin_3d::Camera camera1(glm::vec3(0.0f, 2.0f, -2.0f), glm::vec3(0.0f, 0.0f, 1.0f), 45.0f, 0.1f, 300.0f, WINDOW_WIDTH, WINDOW_HEIGHT);
	cameras.push_back(camera1);

	// Creates the first camera object
	aladdin_3d::Camera camera2(glm::vec3(-10.0f, 3.0f, 3.0f), glm::vec3(1.0f, 0.0f, 0.0f), 45.0f, 8.1f, 100.0f, WINDOW_WIDTH, WINDOW_HEIGHT);
	cameras.push_back(camera2);

	// Activate the second camera.
	current_camera = 0;

	// Load the objects.
	std::vector<aladdin_3d::Object> base_objects;
	base_objects.push_back(aladdin_3d::Object("Models/building_1/building_1.gltf", "GLTF"));
	base_objects.push_back(aladdin_3d::Object("Models/building_2/building_2.gltf", "GLTF"));
	base_objects.push_back(aladdin_3d::Object("Models/building_3/building_3.gltf", "GLTF"));
	base_objects.push_back(aladdin_3d::Object("Models/building_4/building_4.gltf", "GLTF"));

	// Generate the random guides for the objects.
	const int num_of_each = 100;
	std::vector<int> right_building_guide(num_of_each * base_objects.size(), 0);
	for (size_t i = 0; i < right_building_guide.size(); i++) right_building_guide[i] = i / num_of_each;

	// Clone the vector to get the left one.
	std::vector<int> left_building_guide = right_building_guide;

	// Shuffle the buildings.
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::shuffle(right_building_guide.begin(), right_building_guide.end(), std::default_random_engine(seed));
	seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::shuffle(left_building_guide.begin(), left_building_guide.end(), std::default_random_engine(seed));

	// Pass the objects to include them in the object.
	corridor_length = initBuildings(base_objects, right_building_guide, 1.0f);
	corridor_length = initBuildings(base_objects, left_building_guide, -1.0f);

	// Add the wall at the end.
	aladdin_3d::Object wall("Models/wall/wall.gltf", "GLTF");

	// Get the details of the wall.
	aladdin_3d::BoundingBox wall_bb = wall.getBoundingBox();
	glm::vec3 centre = (wall_bb.min + wall_bb.max) / 2.0f;
	glm::vec3 size = wall_bb.max - wall_bb.min;

	// Add the cumulative displacement.
	wall.translate(0, 0, corridor_length + 1.0f);

	// Center the object and get it in the right position.
	wall.translate(-size.x / 2.0f, 0, size.z / 2.0f);
	wall.translate(-centre.x, 0, -centre.z);

	// Add the objects to the list.
	objects.push_back(wall);
	object_shader.push_back(0);

	// Create the floor.
	createFloor();

	// Create the lives.
	createLives();

	// Create the obstacles.
	createObstacles();

	// Load the character.
	aladdin_3d::Object char_body("Models/Stickman/Body/Stickman.gltf", "GLTF");
	aladdin_3d::Object char_leg_1("Models/Stickman/Leg1/Stickman.gltf", "GLTF");
	aladdin_3d::Object char_leg_2("Models/Stickman/Leg2/Stickman.gltf", "GLTF");

	// Join the three objects into one joint hierarchy.
	std::vector<aladdin_3d::Geometry> new_geom;
	new_geom.push_back(char_body.getGeometries()[0]);
	new_geom.push_back(char_leg_1.getGeometries()[0]);
	new_geom.push_back(char_leg_2.getGeometries()[0]);
	
	// Create the final object and append it to the objects.
	aladdin_3d::Object character(new_geom);
	characters.push_back(character);
	character_shader.push_back(1);

}

void initEnvironment(int argc, char** argv) {

	// Create the window.
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	window = glutCreateWindow(GAME_NAME);

	// Error check if the window fails to create
	if (window < 1) {

		std::cerr << "Error 121-1001 - Failed to create the window." << std::endl;
		exit(1);

	}

	// Tell glut where the display function is
	glutDisplayFunc(display);

	// Tell what function handles the events.
	glutSpecialFunc(handleSpecialEvents);
	glutKeyboardFunc(handleKeyEvents);

	// Initialize GLEW and OpenGL.
	GLenum res = glewInit();

	// Check for any errors.
	if (res != GLEW_OK) {

		std::cerr << "Error 121-1002 - GLEW could not be initialized:" << glewGetErrorString(res) << std::endl;
		exit(1);

	}

	// Specify the viewport of OpenGL in the Window.
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

}

int main(int argc, char** argv) {

	// Initialise the environment.
	initEnvironment(argc, argv);

	// Initialise the objects and elements.
	initElements();

	// Init the initial time.
	auto current_time = std::chrono::system_clock::now();
	time_start = std::chrono::duration<double>(current_time.time_since_epoch()).count();

	// Main loop.
	glutMainLoop();

	// Clean everything and terminate.
	clean();

	return 0;

}
