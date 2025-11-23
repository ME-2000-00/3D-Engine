#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"  

#include "camera.h"
#include "Logger.h"
#include "Level.h"

#include "LvlEditor.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/constants.hpp>
#include <gtc/type_ptr.hpp>

// Frame Profiler
struct FrameProfiler
{
    static const int HISTORY_SIZE = 200;
    float fpsHistory[HISTORY_SIZE] = { 0.0f };
    float msHistory[HISTORY_SIZE] = { 0.0f };
    int index = 0;



    void Update(float dt) // dt = time between frames
    {
        float fps = 1.0f / dt;
        float ms = dt * 1000.0f;

        fpsHistory[index] = fps;
        msHistory[index] = ms;

        index = (index + 1) % HISTORY_SIZE;
    }

    void Draw()
    {
        ImGui::Begin("Profiler and Debug");

        // Current values
        float fps = fpsHistory[(index - 1 + HISTORY_SIZE) % HISTORY_SIZE];
        float ms = msHistory[(index - 1 + HISTORY_SIZE) % HISTORY_SIZE];

        ImGui::Text("FPS: %.1f", fps);
        ImGui::Text("Frame time: %.2f ms", ms);

        ImGui::Separator();

        ImGui::Text("FPS History");
        ImGui::PlotLines("##fpsplot", fpsHistory, HISTORY_SIZE, index,
            nullptr, 0.0f, 200.0f, ImVec2(0, 80));

        ImGui::Text("Frame Time History (ms)");
        ImGui::PlotLines("##msplot", msHistory, HISTORY_SIZE, index,
            nullptr, 0.0f, 40.0f, ImVec2(0, 80));

        ImGui::End();
    }
};




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
	void LevelEditorModeRender();

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
	bool LEVEL_EDITOR_MODE = false;

    bool defined_start_wall = false;
    bool CANT_USE_MOUSE = true;
	// ints

	// floats
	float dt = 0;
    float cam_speed = 20.0f;

	// vectors



private:
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

	// vars
	GLFWwindow* window;
    LvlEditor lvlEditor;
    FrameProfiler profiler;
};