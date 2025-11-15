#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/constants.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>

#include "camera.h"
#include <GLFW/glfw3.h>

Camera::Camera(glm::vec3 position, float fovy, float aspect, float near, float far)
    : pos(position), yaw(90.0f), pitch(0.0f), speed(0.001f) {
    up = glm::vec3(0.0f, 1.0f, 0.0f);
    right = glm::vec3(1.0f, 0.0f, 0.0f);
    forward = glm::vec3(0.0f, 0.0f, -1.0f);
    m_proj = glm::perspective(fovy, aspect, near, far);
    m_view = glm::mat4(1.0f);
}

Camera::Camera()
{

}

void Camera::update_vectors() {
    forward.x = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
    forward.y = glm::sin(glm::radians(pitch));
    forward.z = glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
    forward = glm::normalize(forward);

    right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));
    up = glm::normalize(glm::cross(right, forward));
}

void Camera::update_view() {
    m_view = glm::lookAt(pos, pos + forward, up);
}

void Camera::update_projection(float fovy, float aspect, float near, float far) {
    m_proj = glm::perspective(fovy, aspect, near, far);
}

void Camera::update() {
    update_vectors();
    update_view();
}

void Camera::rotate_pitch(float y_angle) {
    pitch -= y_angle;
    pitch = glm::clamp(pitch, -89.0f, 89.0f);
}

void Camera::rotate_yaw(float y_angle) {
    yaw -= y_angle;
}

void Camera::move_left(float vel) {
    pos -= right * vel * speed;
}

void Camera::move_right(float vel) {
    pos += right * vel * speed;
}

void Camera::move_up(float vel) {
    pos += up * vel * speed;
}

void Camera::move_down(float vel) {
    pos -= up * vel * speed;
}

void Camera::move_forward(float vel) {
    pos += forward * vel * speed;
}

void Camera::move_backward(float vel) {
    pos -= forward * vel * speed;
}

void Camera::update_rotation(double xpos, double ypos) {

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;  // Y is inverted

    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

   rotate_yaw(-xoffset);
   rotate_pitch(-yoffset);

}
