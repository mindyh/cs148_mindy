#pragma once

#ifndef __EPIC_LIGHT_PROPERTIES__
#define __EPIC_LIGHT_PROPERTIES__

#include "common/Scene/Light/LightProperties.h"

struct EpicLightProperties: public LightProperties {
    float light_radius;
    glm::vec4 point_position;
    glm::vec4 light_color;
    
    glm::vec4 forward_direction;

    glm::vec4 sky_color;
    glm::vec4 ground_color;
};

#endif
