


#include "Dengine.h"

namespace DengineDefs {
    // global variables or Handelers that mutliple classes might need access to
    Level DD_CurrentLevel;

    // camera   DengineDefs::DD_camera
    Camera DD_camera;

    glm::vec3 toWorldSpace(ImVec2 pos, float spacing) {
        return glm::vec3(pos.x / spacing, 0, pos.y / spacing);
    }
}

Dengine::Dengine() {

    init();

    // defining the camera
    int window_width, window_height;
    glfwGetFramebufferSize(window, &window_width, &window_height);
    DengineDefs::DD_camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f),    // position
        glm::radians(90.0f),           // FOV in radians
        (float)window_width / window_height,  // aspect ratio
        0.1f, 1000.0f);                 // near/far planes
}

void Dengine::openglInit() {
    // Set the clear color (RGBA format, values range from 0.0f to 1.0f)
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);  // Dark cyan, for example

    // def lighting
    //glEnable(GL_LIGHTING);
    //glEnable(GL_LIGHT0);
    //glEnable(GL_NORMALIZE);
    //glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    //glFrontFace(GL_CW);

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
    // Your GL rendering (clear, draw scene, etc.)
    openglRender();

    // --- ImGui Frame Begin ---
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();   // MUST be called every frame

    // Build your UI
    imguiRender();

    // --- ImGui Render (submit draw lists) ---
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // If using multi-viewport, update and render additional platform windows
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
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
    float cam_speed = 1.0f;

    // Update the camera and movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && !ENGINE_MODE)
        DengineDefs::DD_camera.move_forward(cam_speed);

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && !ENGINE_MODE)
        DengineDefs::DD_camera.move_backward(cam_speed);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && !ENGINE_MODE)
        DengineDefs::DD_camera.move_left(cam_speed);

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && !ENGINE_MODE)
        DengineDefs::DD_camera.move_right(cam_speed);

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && !ENGINE_MODE)
        DengineDefs::DD_camera.move_up(cam_speed);

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && !ENGINE_MODE)
        DengineDefs::DD_camera.move_down(cam_speed);
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
        glfwSetInputMode(window, GLFW_CURSOR, engine->ENGINE_MODE ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
	}
}


// ----------------------------------------------------------------------- update loop
void Dengine::update() 
{
    // Swap buffers and poll events
    glfwSwapBuffers(window);
    // may be moved back to openglRender()
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    DengineDefs::DD_camera.update();
}


// ----------------------------------------------------------------------- OPENGL render 
void Dengine::openglRender()
{
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(DengineDefs::DD_camera.m_proj));

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(DengineDefs::DD_camera.m_view));

    //GLfloat sun[] = { -1.0f,-1.0f ,-1.0f, 0.0f };
    //glLightfv(GL_LIGHT0, GL_POSITION, sun);

    //GLfloat A[] = { 0.2f,  0.2f ,0.2f, 1.0f };
    //GLfloat D[] = { 0.8f,  0.8f ,0.8f, 1.0f };
    //GLfloat S[] = { 1.0f,  1.0f ,1.0f, 1.0f };

    //glLightfv(GL_LIGHT0, GL_AMBIENT, A);
    //glLightfv(GL_LIGHT0, GL_DIFFUSE, D);
    //glLightfv(GL_LIGHT0, GL_SPECULAR, S);


    DengineDefs::DD_CurrentLevel.render();
    //int viewLoc = glGetUniformLocation(shader, "u_View");
    //glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(DengineDefs::DD_camera.m_view));

    //int projLoc = glGetUniformLocation(shader, "u_Projection");
    //glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(DengineDefs::DD_camera.m_proj));
}


// ----------------------------------------------------------------------- IMGUI render
void Dengine::imguiRender()
{
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    if (!ENGINE_MODE) {
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

            ImGui::EndMenu();
        }

        //Logger::Log(LogLevel::INFO, std::string("x: " + std::to_string(pos.x) + ", y: " + std::to_string(pos.y) + ", z: " + std::to_string(pos.z)));


        ImGui::SameLine(ImGui::GetWindowWidth() - 150);
        ImGui::Text("Switch modes: F1");

        ImGui::SameLine(ImGui::GetWindowWidth() - 250);
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
                // TODO: add different colors for different types of logs
                // this did not work for some reason
                //if (msg.rfind("[WARNING]"))

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

	lvlEditor.render();
}

