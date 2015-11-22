#include "common/Scene/Camera/Perspective/PerspectiveCamera.h"
#include "common/Scene/Geometry/Ray/Ray.h"

#include <cmath>
#include <random>

PerspectiveCamera::PerspectiveCamera(float aspectRatio, float inputFov):
    aspectRatio(aspectRatio), fov(inputFov * PI / 180.f), zNear(0.f), zFar(std::numeric_limits<float>::max())
{
}

float PerspectiveCamera::RandomFloat(float a, float b) const {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

std::shared_ptr<Ray> PerspectiveCamera::GenerateRayForNormalizedCoordinates(glm::vec2 coordinate, 
                                                                            float focus_z,
                                                                            float aperture_radius) const 
{
    // Send ray from the camera to the focus plane
    // generate rayorigin from random circle around camera location
    float theta = RandomFloat(0, 2*3.14159265);
    float r = RandomFloat(0, aperture_radius);
    const glm::vec2 rand_offsets(r*cos(theta), r*sin(theta));
    const glm::vec3 rayOrigin = glm::vec3(GetPosition()) + 
                                glm::vec3(rand_offsets.x * GetRightDirection()) +   
                                glm::vec3(rand_offsets.y * GetUpDirection());

    // Figure out where the ray is supposed to point to. 
    // Imagine that a frustum exists in front of the camera (which we assume exists at a singular point).
    // Then, given the aspect ratio and vertical field of view we can determine where in the world the 
    // image plane will exist and how large it is.
    const float planeHeight = std::tan(fov / 2.f) * 2.f;
    const float planeWidth = planeHeight * aspectRatio;

    // Assume that (0, 0) is the top left of the image which means that when coordinate is (0.5, 0.5) the 
    // pixel is directly in front of the camera...
    const float xOffset = planeWidth * (coordinate.x - 0.5f) * focus_z;
    const float yOffset = -1.f * planeHeight  * (coordinate.y - 0.5f) * focus_z;
    const glm::vec3 targetPosition = glm::vec3(GetPosition()) + 
                                     glm::vec3(GetForwardDirection()) * focus_z + 
                                     glm::vec3(GetRightDirection()) * xOffset + 
                                     glm::vec3(GetUpDirection()) * yOffset;

    const glm::vec3 rayDirection = glm::normalize(targetPosition - rayOrigin);
    return std::make_shared<Ray>(rayOrigin + rayDirection * zNear, rayDirection, zFar - zNear);
}

void PerspectiveCamera::SetZNear(float input)
{
    zNear = input;
}

void PerspectiveCamera::SetZFar(float input)
{
    zFar = input;
}