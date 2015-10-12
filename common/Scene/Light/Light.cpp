#include "common/Scene/Light/Light.h"
#include "common/Scene/Light/LightProperties.h"
#include "common/Rendering/Shaders/ShaderProgram.h"

const std::string Light::LIGHT_UNIFORM_NAME = "pointLight";

Light::Light(std::unique_ptr<LightProperties> inProperties, LightType type, std::string name):
    properties(std::move(inProperties)), 
    lightType(type), 
    name_(name),
    constantAttenuation(1.f), 
    linearAttenuation(0.f), 
    quadraticAttenuation(0.f)
{
}

Light::~Light()
{
}

void Light::GetAttenuation(float& constant, float& linear, float& quadratic) const
{
    constant = constantAttenuation;
    linear = linearAttenuation;
    quadratic = quadraticAttenuation;
}

const LightProperties* Light::GetPropertiesRaw() const
{
    return properties.get();
}


// Deprecated, only used for blinnphong
void Light::SetupShaderUniforms(const ShaderProgram* program) const
{
    program->SetShaderUniform(LIGHT_UNIFORM_NAME + ".pointPosition", GetPosition());
}

void Light::SetConstantAttenuation(float inValue)
{
    constantAttenuation = inValue;
}

void Light::SetLinearAttenuation(float inValue)
{
    linearAttenuation = inValue;
}

void Light::SetQuadraticAttenuation(float inValue)
{
    quadraticAttenuation = inValue;
}