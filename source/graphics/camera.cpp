#include "StarSystemSim/graphics/camera.h"

#include "StarSystemSim/app/app.h"
#include "StarSystemSim/graphics/object.h"
#include "StarSystemSim/utilities/error.h"
#include "StarSystemSim/utilities/lerp.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cstdlib>
#include <cmath>

namespace graphics {

	Camera::Camera(const glm::vec3& pos) {
        this->mode = Mode::LOOK_AROUND;

        this->pos = pos;
        this->front = glm::vec3(0.0f, 0.0f, -1.0f);
        this->up = glm::vec3(0.0f, 1.0f, 0.0f);

        this->dir = glm::vec3(0.0f, 0.0f, 0.0f);

        this->pitch = 0.0f;
        this->yaw = 0.0f;
        this->roll = 0.0f;

        this->gPitch = this->pitch;
        this->gYaw = this->yaw;
        this->gRoll = this->roll;

        this->fov = 45.0f;
        this->radius = 1.0f;

        this->target = nullptr;

        this->viewMatrix = glm::mat4x4(1.0f);
        this->projMatrix = glm::mat4x4(1.0f);

        m_PosRelTarget = { 0.0f, 0.0f, 0.0f };
        m_CameraLockedOnTarget = false;
	}

    void Camera::update(glm::mat4x4* viewMatrix) {
        switch (this->mode) {
            case Mode::LOOK_AROUND:
                if (viewMatrix)
                    *viewMatrix = this->lookAround();
                else
                    this->lookAround();
                break;
            case Mode::LOOK_AT:
                if (this->target == nullptr) {
                    utils::printError("Camera tried to look at a not set target");
                    break;
                }

                if ((this->target->getPos() - this->pos).length() - this->radius < 0.001f) {
                    m_CameraLockedOnTarget = true;
                }

                if (viewMatrix)
                    *viewMatrix = this->lookAt(this->target->getPos());
                else
                    this->lookAt(this->target->getPos());
                break;
        }
    }

    void Camera::changeTarget(Object* newTarget) {
        m_CameraLockedOnTarget = false;
        this->target = newTarget;
    }

    glm::mat4& Camera::lookAround() {
        this->dir.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
        this->dir.y = sin(glm::radians(this->pitch));
        this->dir.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
        this->front = glm::normalize(this->dir);

        this->viewMatrix = glm::lookAt(this->pos, this->front, this->up);
        return this->viewMatrix;
    }

    glm::mat4& Camera::lookAt(const glm::vec3& target) {
        this->pitch = utils::lerp(this->pitch, this->gPitch, 3.0f * App::s_Instance->mainTimer.deltaTime);

        float cappedGoalYaw;

        if (std::abs(this->yaw - this->gYaw) < std::abs(this->yaw - (this->gYaw + 360.0f))
            && std::abs(this->yaw - this->gYaw) < std::abs(this->yaw - (this->gYaw - 360.0f)))
        {
            cappedGoalYaw = this->gYaw;
        }
        else if (std::abs(this->yaw - (this->gYaw + 360.0f)) < std::abs(this->yaw - (this->gYaw - 360.0f))) {
            cappedGoalYaw = this->gYaw + 360.0f;
        }
        else {
            cappedGoalYaw = this->gYaw - 360.0f;
        }

        this->yaw = utils::lerp(this->yaw, cappedGoalYaw, 3.0f * App::s_Instance->mainTimer.deltaTime);

        this->yaw = this->yaw - 360.0f * (float)((int32_t)(this->yaw / 360.0f));
        if (this->yaw < 0.0f)
            this->yaw += 360.0f;

        float radPitch = glm::radians(this->pitch);
        float radYaw = glm::radians(this->yaw);

        m_PosRelTarget.x = glm::cos(-radYaw) * glm::cos(radPitch);
        m_PosRelTarget.y = glm::sin(radPitch);
        m_PosRelTarget.z = glm::sin(-radYaw) * glm::cos(radPitch);

        m_PosRelTarget *= this->radius;

        //this->dir = glm::normalize(-m_PosRelTarget);

        if (m_CameraLockedOnTarget) {
            this->pos = target + m_PosRelTarget;
            this->front = target;
        }
        else {
            this->pos = utils::lerp(this->pos, target + m_PosRelTarget, App::s_Instance->mainTimer.deltaTime);
            this->front = utils::lerp(this->front, target, App::s_Instance->mainTimer.deltaTime);
        }

        this->viewMatrix = glm::lookAt(this->pos, this->front, this->up);
        return this->viewMatrix;
    }

}