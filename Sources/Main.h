/**
 * @file Main.h
 * @brief Main header aladdin 3d file.
 * @version 1.0.0 (2022-10-21)
 * @date 2022-10-21
 * @author Borja García Quiroga <garcaqub@tcd.ie>
 *
 *
 * Copyright (c) Borja García Quiroga, All Rights Reserved.
 *
 * The information and material provided below was developed as partial
 * requirements for the MSc in Computer Science at Trinity College Dublin,
 * Ireland.
 */

#ifndef ALADDIN_3D_MAIN_H_
#define ALADDIN_3D_MAIN_H_

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 800
#define GAME_NAME "Aladdin 3D"

#include <vector>
#include <string>
#include <ctime>

#include "Classes/Camera/Camera.h"
#include "Classes/Object/Object.h"
#include "Classes/Shader/Shader.h"

std::vector<aladdin_3d::Camera> cameras;	/// Holds all the existing cameras.
unsigned int current_camera = 0;			/// Current camera activated.
std::vector<aladdin_3d::Object> characters;	/// Holds all the displayed characters.
std::vector<aladdin_3d::Object> objects;	/// Holds all the displayed objects.
std::vector<unsigned int> character_shader;	/// Holds all the relationships between shaders and characters.
std::vector<unsigned int> object_shader;	/// Holds all the relationships between shaders and objects.
std::vector<aladdin_3d::Shader> shaders;	/// Holds all the initialized shanders.
int window = 0;								/// Window ID.
std::vector<float> obstacles_positions;		/// The positions of the obstacles in the game.
std::vector<std::string> obstacles_type;	/// The type of the obstacles in the game.
double internal_time = 0;					/// Time that will rule everything in the game.
double time_start = 0;						/// Time that will count as the beginning.
bool is_paused = true;						/// Control if the game is paused.
float jump_start = -1.0f;					/// The time point where the jump started.
int lives = 3;								/// Current lives of the player.
float corridor_length = 0;					/// Max length of the corridor.
double last_hit = -10;						/// The moment when the character hit an obstacle the last time.

const float velocity = 5.0;				/// The usual running speed of a person in m/s.
const float jump_velocity = 4.0f;			/// The initial velocity of the jump.
const float gravity = -10.0f;				/// This is just the gravity, in case we wanted another value.
const glm::vec4 fog(0.9, 0.7, 0.4, 1.0);	// This is just the fog color.

/**
 * @brief Clean everything to end the program.
 *
 * Clean everything to end the program.
 */
void clean();

/**
 * @brief Create the obstacles.
 * 
 * Create the obstacles.
 */
void createObstacles();

/**
 * @brief Create the place where lives appear.
 * 
 * Create the place where lives appear.
 */
void createLives();

/**
 * This function generates the floor.
 * 
 * This function generates the floor for the scenario.
 */
void createFloor();

/**
 * @brief Display the elements.
 *
 * This function will be called in the main loop to display the elements.
 */
void display();

/**
 * @brief Display the characters.
 * 
 * Display the characters.
 */
void displayCharacters();

/**
 * @brief Handles the Freeglut events.
 *
 * Handles the freeglut events.
 */
void handleSpecialEvents(int key, int x, int y);

/**
 * @brief Handles the key events.
 *
 * Handles the freeglut key events.
 */
void handleKeyEvents(unsigned char key, int x, int y);

/**
 * @brief Init the buildings.
 * 
 * Init the buildings.
 */
float initBuildings(std::vector<aladdin_3d::Object> base_objects, std::vector<int> building_guide, float x_scale);

/**
 * @brief Init the elements of the program
 *
 * Initialize the objects, elements and all.
 */
void initElements();

/**
 * @brief Init the environment.
 *
 * Initialize the OpenGL, Glew and Freeglut environments.
 */
void initEnvironment(int argc, char** argv);

/**
 * @brief Main function.
 * 
 * Main function.
 */
int main(int argc, char** argv);

#endif  //!ALADDIN_3D_MAIN_H_
