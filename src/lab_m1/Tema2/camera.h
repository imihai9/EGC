// Source: Lab 5, EGC
#pragma once

#include "utils/glm_utils.h"
#include "utils/math_utils.h"

namespace tema2
{
    class Camera
    {
    public:
        Camera()
        {
            position = glm::vec3(1, 2, 1.f);
            forward = glm::vec3(0, 0, -1);
            up = glm::vec3(0, 1, 0);
            right = glm::vec3(1, 0, 0);
            distanceToTarget = 3;
        }

        Camera(const glm::vec3& position, const glm::vec3& center, const glm::vec3& up)
        {
            Set(position, center, up);
        }

        ~Camera()
        { }

        // Update camera
        void Set(const glm::vec3& position, const glm::vec3& center, const glm::vec3& up)
        {
            this->position = position;
            forward = glm::normalize(center - position);
            right = glm::cross(forward, up);
            this->up = glm::cross(right, forward);
        }

        void MoveForward(float distance)
        {
            // Translates the camera using the `dir` vector computed from
            // `forward`. Movement will always keep the camera at the same
            // height. For example, if you rotate your head up/down, and then
            // walk forward, then you will still keep the same relative
            // distance (height) to the ground!

            glm::vec3 dir = glm::normalize(glm::vec3(forward.x, 0, forward.z)); // vector normalizat
            position += dir * distance;
        }

        void TranslateForward(float distance)
        {
            position += forward * distance;
        }

        void TranslateUpward(float distance)
        {
            position += up * distance;
        }

        void TranslateRight(float distance)
        {
            // Translates the camera using the `right` vector, projected to xOz.
            // (ground plane)
            glm::vec3 dir = glm::normalize(glm::vec3(right.x, 0, right.z));
            position += dir * distance; // xOz
        }

        void RotateFirstPerson_OX(float angle)
        {
            glm::mat4 R = glm::rotate(glm::mat4(1), angle, right); // OX rotation
            forward = glm::normalize(R * glm::vec4(forward, 0));
            up = glm::cross(right, forward);
        }

        void RotateFirstPerson_OY(float angle)
        {
            // Computes the new `forward`, `up` and `right vectors.
            glm::mat4 R = glm::rotate(glm::mat4(1), angle, glm::vec3(0, 1, 0)); // rotatie in jurul (0, 1, 0) = axa up a lumii
            forward = glm::normalize(R * glm::vec4(forward, 0));
            right = glm::normalize(R * glm::vec4(right, 0));
            up = glm::cross(right, forward); // produs vectorial

        }

        void RotateFirstPerson_OZ(float angle)
        {
            glm::mat4 R = glm::rotate(glm::mat4(1), angle, forward);
            right = glm::normalize(R * glm::vec4(right, 0));
            up = glm::cross(right, forward);
        }

        void RotateThirdPerson_OX(float angle)
        {
            // Rotate the camera in third-person mode around
            // the OX axis.
            TranslateForward(distanceToTarget); // move to target pos
            RotateFirstPerson_OX(angle);        // first person rotation
            TranslateForward(-distanceToTarget);// move back
        }

        void RotateThirdPerson_OY(float angle)
        {
            // Rotate the camera in third-person mode around
            // the OY axis.
            TranslateForward(distanceToTarget);
            RotateFirstPerson_OY(angle);
            TranslateForward(-distanceToTarget);
        }

        void RotateThirdPerson_OZ(float angle)
        {
            // Rotate the camera in third-person mode around
            // the OZ axis.
            TranslateForward(distanceToTarget);
            RotateFirstPerson_OZ(angle);
            TranslateForward(-distanceToTarget);
        }

        glm::mat4 GetViewMatrix()
        {
            // Returns the view matrix
            return glm::lookAt(position, position + forward, up);
        }

        glm::vec3 GetTargetPosition()
        {
            return position + forward * distanceToTarget;
        }

        glm::vec3 GetTargetPositionFPS()
        {
            return position;
        }

        glm::vec3 GetTargetPositionReverse()
        {
            return position - forward * distanceToTarget;
        }

    public:
        float distanceToTarget;
        glm::vec3 position;
        glm::vec3 forward;
        glm::vec3 right;
        glm::vec3 up;
    };
}   // namespace implemented
