#pragma once

#include "common/Scene/Camera/Camera.h"

class PerspectiveCamera : public Camera
{
public:
    // inputFov is in degrees. 
    PerspectiveCamera(float aspectRatio, float inputFov);
    virtual std::shared_ptr<class Ray> GenerateRayForNormalizedCoordinates(glm::vec2 coordinate, 
    																		float focus_z=1.f,
    																		float aperture_radius=0.f) const override;

    void SetZNear(float input);
    void SetZFar(float input);

private:
    float aspectRatio;
    float fov; // fov is stored as radians

    float zNear;
    float zFar;

	float RandomFloat(float a, float b) const;

};
