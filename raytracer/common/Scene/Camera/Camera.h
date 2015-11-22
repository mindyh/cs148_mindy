#pragma once

#include "common/Scene/SceneObject.h"

class Camera : public SceneObject
{
public:
    Camera();

    virtual std::shared_ptr<class Ray> GenerateRayForNormalizedCoordinates(glm::vec2 coordinate, 
    																		float focus_z=1.f,
    																		float aperture_radius=0.f) const = 0;
};