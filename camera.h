// camera.h
#pragma once

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

class Camera {
private :
    double lastX = 400, lastY = 300;  // Start in the middle of the screen
    double deltaX = 0.0, deltaY = 0.0;
    bool firstMouse = true;
public:
    glm::vec3 pos;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 forward;
    glm::mat4 m_proj;
    glm::mat4 m_view;
    float yaw;
    float pitch;
    float speed;

    // Only declare the constructor here
    Camera(glm::vec3 position, float fovy, float aspect, float near, float far);
    Camera();

    void update_vectors();
    void update_view();
    void update_projection(float fovy, float aspect, float near, float far);
    void update();
    void rotate_pitch(float y_angle);
    void rotate_yaw(float y_angle);
    void move_left(float vel);
    void move_right(float vel);
    void move_up(float vel);
    void move_down(float vel);
    void move_forward(float vel);
    void move_backward(float vel);
    void update_rotation(double xpos, double ypos);
};
