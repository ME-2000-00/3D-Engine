


#include "Dengine.h"

namespace DengineDefs {
    // global variables or Handelers that mutliple classes might need access to
    Level DD_CurrentLevel;

    // camera   DengineDefs::DD_camera
    Camera DD_camera;

    glm::vec3 toWorldSpace(ImVec2 pos, float spacing) {
        return glm::vec3(pos.x / spacing, 0, pos.y / spacing);
    }

    bool DD_Mode3D = true;
}

Dengine::Dengine() {

    init();

    // defining the camera
    int window_width, window_height;
    glfwGetFramebufferSize(window, &window_width, &window_height);
    lvlEditor.window_width = window_width;
    lvlEditor.window_height = window_height;

    DengineDefs::DD_camera = Camera(glm::vec3(0.0f, 3.0f, 0.0f),    // position
        glm::radians(90.0f),           // FOV in radians
        (float)window_width / window_height,  // aspect ratio
        0.1f, 1000.0f);                 // near/far planes
}

void Dengine::openglInit() {
    // Set the clear color (RGBA format, values range from 0.0f to 1.0f)
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);  // Dark cyan, for example

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    // class inits ig

}

void Dengine::cleanup()
{
    if (ImGui::GetCurrentContext() != nullptr) {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    // opengl clean ups \/


    glfwDestroyWindow(window);
    glfwTerminate();
}

void Dengine::init()
{
    Logger::Log(LogLevel::INFO, "Initializing Dengine. . .");

    if (!glfwInit()) {
        Logger::Log(LogLevel::ERROR, "Unable to initialize GLFW");
        return;
    }

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    window = glfwCreateWindow(mode->width, mode->height, "Dengine", monitor, NULL);

    if (!window) {
        Logger::Log(LogLevel::ERROR, "Failed to create window");
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);

    glfwSwapInterval(vsync ? 1 : 0);

    // Let callbacks access this instance
    glfwSetWindowUserPointer(window, this);

    // Register key callback
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetMouseButtonCallback(window, MouseButtonCallback);

    if (glewInit() != GLEW_OK) {
        Logger::Log(LogLevel::ERROR, "Failed to initialize GLEW");
        return;
    }

    DengineDefs::DD_camera.update();

    imguiInit();  // Only uncomment if you also keep ImGui cleanup
    openglInit();

    Logger::Log(LogLevel::INFO, "Initialized Dengine");
}

void Dengine::run()
{
    while (!glfwWindowShouldClose(window) && running) {
        events();
        update();
        render();
    }

    cleanup();
}

void Dengine::imguiInit()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;

     //<<< SET CONFIG FLAGS RIGHT AWAY (before any NewFrame()) >>>
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // optional
    //io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;       // ENABLE DOCKING BEFORE FIRST NewFrame
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;     // optional: allow viewports / platform windows

    // style and colors
    ImGui::StyleColorsDark();
    ImGui::GetStyle().ScaleAllSizes(1.2f);

    ImGuiStyle& style = ImGui::GetStyle();

    // square corners
    style.WindowRounding = 0.0f;
    style.FrameRounding = 0.0f;
    style.ScrollbarRounding = 0.0f;
    style.GrabRounding = 0.0f;
    style.TabRounding = 0.0f;

    // Initialize backends (after setting flags)
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Optional: upload fonts / build atlas here if you added fonts:
    // ImGui_ImplOpenGL3_CreateFontsTexture();
}

void Dengine::render()
{
    // Clear at start of frame
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Do GL rendering (world or editor)
    openglRender();

    // --- ImGui Frame Begin ---
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    dt = ImGui::GetIO().DeltaTime;
    imguiRender();

    // --- ImGui Render (submit draw lists) ---
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }

    // Finally present
    glfwSwapBuffers(window);
}


void Dengine::events()
{
    glfwPollEvents();
	// normal key input goes here, like movement, etc.

            // --- Poll input
    if (!ENGINE_MODE) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        DengineDefs::DD_camera.update_rotation(xpos, ypos);
    }

    // Update the camera and movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && (!ENGINE_MODE))
        DengineDefs::DD_camera.move_forward(cam_speed, dt);

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && (!ENGINE_MODE))
        DengineDefs::DD_camera.move_backward(cam_speed, dt);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && (!ENGINE_MODE))
        DengineDefs::DD_camera.move_left(cam_speed, dt);

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && (!ENGINE_MODE))
        DengineDefs::DD_camera.move_right(cam_speed, dt);

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && (!ENGINE_MODE))
        DengineDefs::DD_camera.move_up(cam_speed, dt);

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && (!ENGINE_MODE))
        DengineDefs::DD_camera.move_down(cam_speed, dt);
}



// --------------------------------------------------------------------- keyinput on press
void Dengine::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // We need access to the engine instance
    Dengine* engine = (Dengine*)glfwGetWindowUserPointer(window);
    if (!engine) return;

	// Engine Mode specific keybinds
    if (engine->ENGINE_MODE) {
        // Toggle Logger
        if (key == GLFW_KEY_F2 && action == GLFW_PRESS)
        {
            engine->logger = !engine->logger;
        }
    }





    // Exit
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        engine->running = false;
    }

    // Engine Mode toggle
	if (key == GLFW_KEY_F1 && action == GLFW_PRESS) 
    {
        engine->ENGINE_MODE = !engine->ENGINE_MODE;
        engine->LEVEL_EDITOR_MODE = false;
        glfwSetInputMode(window, GLFW_CURSOR, engine->ENGINE_MODE ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
        engine->LEVEL_EDITOR_MODE ? glClearColor(0.1f, 0.1f, 0.1f, 1.0f) : glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	}

    // Level Editor Mode toggle
    if (key == GLFW_KEY_F7 && action == GLFW_PRESS)
    {
        engine->LEVEL_EDITOR_MODE = !engine->LEVEL_EDITOR_MODE;
        engine->LEVEL_EDITOR_MODE ? glClearColor(0.1f, 0.1f, 0.1f, 1.0f) : glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}



// --------------------------------------------------------------------- mouse on press
void Dengine::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{


    // We need access to the engine instance
    Dengine* engine = (Dengine*)glfwGetWindowUserPointer(window);
    if (!engine) return;


    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && !engine->CANT_USE_MOUSE && engine->LEVEL_EDITOR_MODE) {
        engine->lvlEditor.MouseFunctionalityUtil(UTIL::LEFT);
    }

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS && !engine->CANT_USE_MOUSE && engine->LEVEL_EDITOR_MODE) {
        engine->lvlEditor.MouseFunctionalityUtil(UTIL::RIGHT);
    }

    if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS && !engine->CANT_USE_MOUSE && engine->LEVEL_EDITOR_MODE) {
        engine->lvlEditor.MouseFunctionalityUtil(UTIL::MIDDLE);

    }

}

// In initialization:




// ----------------------------------------------------------------------- update loop
void Dengine::update()
{
    CANT_USE_MOUSE = ImGui::GetIO().WantCaptureMouse;

    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    int width, height;
    glfwGetWindowSize(window, &width, &height);

    // convert to framebuffer coords if needed (see next section)
    mouseY = height - mouseY; // invert Y for your editor coords

    lvlEditor.mouse = glm::vec2(mouseX, mouseY);

    profiler.Update(dt);

    DengineDefs::DD_camera.update();
    // NO swap or clear here
}



// ----------------------------------------------------------------------- OPENGL render 
void Dengine::openglRender()
{
    // ... crosshair code, etc.

    if (!LEVEL_EDITOR_MODE) {
        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(glm::value_ptr(DengineDefs::DD_camera.m_proj));

        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf(glm::value_ptr(DengineDefs::DD_camera.m_view));
    }
    else {
        // Save state, switch to pixel-space ortho for editor
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();

        int fb_w, fb_h;
        glfwGetFramebufferSize(window, &fb_w, &fb_h);
        glOrtho(0, fb_w, 0, fb_h, -1, 1);

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        // disable depth when drawing 2D editor
        glDisable(GL_DEPTH_TEST);
    }

    // render either level or editor
    LEVEL_EDITOR_MODE ? LevelEditorModeRender() : DengineDefs::DD_CurrentLevel.render();

    // If we entered editor-mode branch above, restore matrices & GL state
    if (LEVEL_EDITOR_MODE) {
        // restore modelview then projection (LIFO)
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();

        glMatrixMode(GL_PROJECTION);
        glPopMatrix();

        glMatrixMode(GL_MODELVIEW); // restore default mode
        glEnable(GL_DEPTH_TEST);   // restore depth test
    }
}

// ----------------------------------------------------------------------- IMGUI render
void Dengine::imguiRender()
{
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    profiler.Draw();

    if (!ENGINE_MODE && !LEVEL_EDITOR_MODE) {
        return;
    }

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Dengine"))
        {
            if (ImGui::MenuItem("Exit", "Esc")) {
                running = false;

            }
            if (ImGui::MenuItem("Logger", "F2")) {
                logger = !logger;

            }
            if (ImGui::MenuItem("Vsync", NULL, vsync)) {
                vsync = !vsync;
                glfwSwapInterval(vsync ? 1 : 0);

            }
            if (ImGui::MenuItem("Reload Level")) {
                
				DengineDefs::DD_CurrentLevel = lvlEditor.get_Level();
				DengineDefs::DD_camera.pos = glm::vec3(lvlEditor.plr_pos.x, 0, lvlEditor.plr_pos.y);

            }
            if (ImGui::MenuItem("Level Editor Mode", "F7", LEVEL_EDITOR_MODE)) {
                DengineDefs::DD_camera.pos = glm::vec3(0, 0, 0);

                LEVEL_EDITOR_MODE = !LEVEL_EDITOR_MODE;

                LEVEL_EDITOR_MODE ? glClearColor(0.1f, 0.1f, 0.1f, 1.0f) : glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            }

            ImGui::EndMenu();
        }

        //Logger::Log(LogLevel::INFO, std::string("x: " + std::to_string(pos.x) + ", y: " + std::to_string(pos.y) + ", z: " + std::to_string(pos.z)));


        ImGui::SameLine(ImGui::GetWindowWidth() - 200);
        ImGui::Text("Switch To Play Mode: F1");

        ImGui::SameLine(ImGui::GetWindowWidth() - 350);
        ImGui::Text("FPS: %.1f", io.Framerate);

        ImGui::EndMainMenuBar();
    }

    if (logger) {
        // add differnet colors for log types
        static bool autoScroll = true;

        ImGui::SetNextWindowPos(ImVec2(0, 20)); // left side, below menu bar
        ImGui::SetNextWindowSize(ImVec2(400, ImGui::GetIO().DisplaySize.y - 20));

        ImGuiWindowFlags flags =
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoTitleBar;

        if (ImGui::Begin("Engine Log", &logger, flags))
        {
            const auto& logs = Logger::GetMessages();
            for (const auto& msg : logs)
            {
                ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(msg.r, msg.g, msg.b, 255));

                ImGui::TextUnformatted(msg.msg.c_str());
                ImGui::PopStyleColor();
            }

            if (autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
                ImGui::SetScrollHereY(1.0f);

            ImGui::Checkbox("Auto-scroll", &autoScroll);
        }

        ImGui::End();
    }

    if (LEVEL_EDITOR_MODE) {
        lvlEditor.ImGuirender();
    }
}

void Dengine::LevelEditorModeRender()
{
    // the 2D editor for my engine
    // based on jdh's level editor

    lvlEditor.render();
}

