#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <algorithm>
#include "shader.hpp"

// opengl version
constexpr int OPENGL_VERSION_MEJOR = 4;
constexpr int OPENGL_VERSION_MINOR = 1;

// window settings
constexpr int WINDOW_WIDTH  = 512;
constexpr int WINDOW_HEIGHT = 512;
const char*   WINDOW_TITLE  = "ray-marching";

// control variables
bool can_animate = false;
bool display_gui = true;

// uniform variables
glm::ivec2 resolution;
glm::dvec2 mouse_pos;
float playback_time;

// callback functions
void error_callback(int error, const char* description);
void mouse_pos_callback(GLFWwindow* window, double x, double y);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// control functions
void switch_flag(bool *flag);
void reset_timer();

int main(int argc, char** argv) {
    // initialize glfw
    if(!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(1);
    }

    // configure opengl
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VERSION_MEJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VERSION_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // create a window
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr);
    if(window == nullptr) {
        std::cerr << "Failed to create a window" << std::endl;
        exit(1);
    }
    glfwMakeContextCurrent(window);

    // initialize GLEW
    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        exit(1);
    }

    // register callback functions
    glfwSetErrorCallback(error_callback);
    glfwSetCursorPosCallback(window, mouse_pos_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // configure imgui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    // rectangle
    float vertices[] = {
         1.0f,  1.0f,
         1.0f, -1.0f,
        -1.0f, -1.0f,
        -1.0f,  1.0f
    };

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    unsigned int vao, vbo, ibo;     
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ibo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // fragment shaders
    std::vector<std::string> fragment_shaders;
    int selected_shader = 0;
    for(const auto &file : std::filesystem::directory_iterator("./shader/fragment/")) {
        fragment_shaders.push_back(file.path().stem());
    }
    std::sort(fragment_shaders.begin(), fragment_shaders.end());

    auto fragment_shader_getter = [](void* data, int idx, const char** out_text) -> bool {
        std::string* fragment_shaders = (std::string*)data;
        std::string& fragment_name    = fragment_shaders[idx];
        *out_text = fragment_name.c_str();
        return true;
    };

    // shader
    std::string vertex_shader   = std::string("./shader/vertex/vert.glsl");
    std::string fragment_shader = std::string("./shader/fragment/") + fragment_shaders[selected_shader] + std::string(".glsl");
    Shader shader; 
    shader.load(vertex_shader.c_str(), fragment_shader.c_str());

    // initialize uniform variables
    glfwGetFramebufferSize(window, &resolution.x, &resolution.y);
    glfwGetCursorPos(window, &mouse_pos.x, &mouse_pos.y);
    playback_time = 0.0;

    // timer
    float current_time = 0.0;
    float last_time = 0.0;
    float elapsed_time = 0.0;

    // render loop
    while(!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        // update timer
        current_time = glfwGetTime();
        elapsed_time = current_time - last_time;
        last_time = current_time;
        if(can_animate) {
            playback_time += elapsed_time;
        }

        // set uniform variables
        shader.set_ivec2("resolution", resolution);
        shader.set_vec2("mouse_pos", (glm::vec2)mouse_pos);
        shader.set_float("playback_time", playback_time);

        // render
        shader.use();
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // display gui
        if(display_gui) {
            ImGui_ImplOpenGL3_NewFrame(); 
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            ImGui::Begin("ImGui Window");
            ImGui::Text("Resolution: %4d x %4d", resolution[0], resolution[1]);
            ImGui::Text("Mouse position: (%.2f, %.2f)", mouse_pos[0], mouse_pos[1]);
            ImGui::Text("Framerate: %.3f ms/frame (%.1f fps)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::Text("Playback time: %.3f s", playback_time);
            if(ImGui::Button("Start/Stop")) { switch_flag(&can_animate); }
            if(ImGui::Button("Reset")) { reset_timer(); }
            if(ImGui::ListBox("Fragment shader", &selected_shader, fragment_shader_getter, fragment_shaders.data(), fragment_shaders.size(), 4)) { 
                fragment_shader = std::string("./shader/fragment/") + fragment_shaders[selected_shader] + std::string(".glsl");
                shader.load(vertex_shader.c_str(), fragment_shader.c_str());
                reset_timer();
            }
            ImGui::End();
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // terminate imgui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // terminate glfw
    glfwDestroyWindow(window);
    glfwTerminate();
}

void error_callback(int error, const char* description) {
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

void mouse_pos_callback(GLFWwindow* window, double x, double y) {
    // update mouse position
    mouse_pos.x = x;
    mouse_pos.y = y;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    // ESC, Q: terminate
    // S     : switch animation
    // R     : reset
    // V     : gui on/off
    if(action == GLFW_PRESS) {
        switch(key) {
            case GLFW_KEY_ESCAPE:
            case GLFW_KEY_Q:
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;
            case GLFW_KEY_S:
                switch_flag(&can_animate);
                break;
            case GLFW_KEY_R:
                reset_timer();
                break;
            case GLFW_KEY_V:
                switch_flag(&display_gui);
                break;
            default:
                break;
        }
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // update resolution
    resolution.x = width;
    resolution.y = height;

    // update viewport
    glViewport(0, 0, width, height);
}

void switch_flag(bool *flag) {
    *flag ^= 1;
}

void reset_timer() {
    playback_time = 0.0;
}
