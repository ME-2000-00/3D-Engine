#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"  

#include "2DMode.h"
#include "camera.h"
#include "Logger.h"
#include "Level.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/constants.hpp>
#include <gtc/type_ptr.hpp>



class Dengine {
public:
	Dengine();

	void render();
	void update();
	void events();
	void cleanup();

	// render functions
	void openglRender();
	void imguiRender();

	void init();

	//init functions
	void imguiInit();
	void openglInit();

	void run();

	// bools
	bool running = true;
	bool logger = false;
	bool vsync = false;

	bool ENGINE_MODE = true;

	// ints

	// floats

	// vectors



private:
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	// vars
	GLFWwindow* window;
	Mode2D mode2d;

};