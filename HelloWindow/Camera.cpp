#include "Camera.hpp"

namespace gps {

    //Camera constructor
    Camera::Camera(glm::vec3 cameraPosition, glm::vec3 cameraTarget, glm::vec3 cameraUp) {
        this->cameraPosition = cameraPosition;
        this->cameraTarget = cameraTarget;
        this->cameraUpDirection = cameraUp;
        this->cameraUp = cameraUp;

        //TODO - Update the rest of camera parameters
        this->cameraFrontDirection = glm::normalize(cameraTarget - cameraPosition);
        this->cameraRightDirection = glm::normalize(glm::cross(this->cameraFrontDirection, cameraUp));
        //
        this->cameraUpDirection = glm::cross(this->cameraRightDirection, this->cameraFrontDirection);
    }

    //return the view matrix, using the glm::lookAt() function
    glm::mat4 Camera::getViewMatrix() {
        return glm::lookAt(cameraPosition, cameraTarget, cameraUpDirection);
    }

    //update the camera internal parameters following a camera move event
    void Camera::move(MOVE_DIRECTION direction, float speed) {
        //TODO
        switch (direction)
        {
        case MOVE_FORWARD:
            this->cameraPosition += this->cameraFrontDirection * speed;
            this->cameraTarget += this->cameraFrontDirection * speed;
            break;
        case MOVE_RIGHT:
            this->cameraPosition += this->cameraRightDirection * speed;
            this->cameraTarget += this->cameraRightDirection * speed;;
            break;
        case MOVE_LEFT:
            this->cameraPosition -= this->cameraRightDirection * speed;
            this->cameraTarget -= this->cameraRightDirection * speed;
            break;
        case MOVE_BACKWARD:
            this->cameraPosition -= this->cameraFrontDirection * speed;
            this->cameraTarget -= this->cameraFrontDirection * speed;
            break;
        default:
            break;
        }
    }

    //update the camera internal parameters following a camera rotate event
    //yaw - camera rotation around the y axis
    //pitch - camera rotation around the x axis
    void Camera::rotate(float pitch, float yaw) {
        //TODO
        this->cameraFrontDirection.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        this->cameraFrontDirection.y = sin(glm::radians(pitch));
        this->cameraFrontDirection.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        this->cameraFrontDirection = glm::normalize(cameraFrontDirection);

        this->cameraTarget = this->cameraPosition + this->cameraFrontDirection;
        this->cameraRightDirection = glm::normalize(glm::cross(this->cameraFrontDirection, this->cameraUp));
        this->cameraUpDirection = glm::cross(this->cameraRightDirection, this->cameraFrontDirection);
        
    }
}