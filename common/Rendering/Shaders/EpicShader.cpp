#include "common/Rendering/Shaders/EpicShader.h"
#include "common/Rendering/Textures/Texture2D.h"
#include "common/Scene/Light/Light.h"
#include "common/Scene/Light/LightProperties.h"
#include "common/Scene/Camera/Camera.h"
#include "common/Utility/Texture/TextureLoader.h"
#include "assimp/material.h"

std::array<const char*, 5> EpicShader::MATERIAL_PROPERTY_NAMES = {
    "InputMaterial.matMetallic", 
    "InputMaterial.matRoughness", 
    "InputMaterial.matSpecular",
    "InputMaterial.matLight",
    "InputMaterial.matDiffuse"
};

const int EpicShader::MATERIAL_BINDING_POINT = 0;

EpicShader::EpicShader(const std::unordered_map<GLenum, std::string>& inputShaders, GLenum lightingStage):
    ShaderProgram(inputShaders), metallic(1.f), roughness(1.f), specular(glm::vec3(0.f), 1.f), diffuse(glm::vec3(0.f), 1.f),
    is_affected_by_light_(true), materialBlockLocation(0), materialBlockSize(0), materialBuffer(0),
    lightingShaderStage(lightingStage)
{
    if (!shaderProgram) {
        return;
    }

    SetupUniformBlock<5>("InputMaterial", MATERIAL_PROPERTY_NAMES, materialIndices, materialOffsets, materialStorage, materialBlockLocation, materialBlockSize, materialBuffer);
    UpdateMaterialBlock();

    (void)lightingShaderStage;

    defaultTexture = TextureLoader::LoadTexture("required/defaultTexture.png");
    if (!defaultTexture) {
        std::cerr << "WARNING: Failed to load the default texture." << std::endl;
        return;
    }
}

EpicShader::~EpicShader()
{
    OGL_CALL(glDeleteBuffers(1, &materialBuffer));
}

void EpicShader::SetupShaderLighting(const Light* light) const
{
    if (!light) {
        SetShaderUniform("lightingType", static_cast<int>(Light::LightType::GLOBAL));
    } else {
        // Get the light's properties
        const LightProperties* lightProperty = static_cast<const LightProperties*>(light->GetPropertiesRaw());

        // Select proper lighting subroutine based on the light's type.
        switch(light->GetLightType()) {
            case Light::LightType::POINT:
                SetShaderUniform("lightingType", static_cast<int>(Light::LightType::POINT));
                SetShaderUniform("pointLight.light_color", lightProperty->light_color);
                SetShaderUniform("pointLight.point_position", lightProperty->point_position);
                SetShaderUniform("pointLight.light_radius", lightProperty->light_radius);
                break;
            case Light::LightType::DIRECTIONAL:
                SetShaderUniform("lightingType", static_cast<int>(Light::LightType::DIRECTIONAL));
                SetShaderUniform(light->GetName() + ".light_color", lightProperty->light_color);
                SetShaderUniform(light->GetName() + ".forward_direction", light->GetForwardDirection());
                SetShaderUniform(light->GetName() + ".point_position", lightProperty->point_position);
                break;
            case Light::LightType::HEMISPHERE:
                SetShaderUniform("lightingType", static_cast<int>(Light::LightType::HEMISPHERE));
                SetShaderUniform(light->GetName() + ".sky_color", lightProperty->sky_color);
                SetShaderUniform(light->GetName() + ".ground_color", lightProperty->ground_color);
                break;
            default:
                std::cerr << "WARNING: Light type is not supported. Defaulting to global light. Your output may look wrong. -- Ignoring: " << static_cast<int>(light->GetLightType()) << std::endl;
                SetShaderUniform("lightingType", static_cast<int>(Light::LightType::GLOBAL));
                break;
	}
        light->SetupShaderUniforms(this);
    }
    UpdateAttenuationUniforms(light);
}

void EpicShader::UpdateMaterialBlock() const
{
    StartUseShader();

    memcpy((void*)(materialStorage.data() + materialOffsets[0]), &metallic, sizeof(float));
    memcpy((void*)(materialStorage.data() + materialOffsets[1]), &roughness, sizeof(float));
    memcpy((void*)(materialStorage.data() + materialOffsets[2]), glm::value_ptr(specular), sizeof(glm::vec4));
    memcpy((void*)(materialStorage.data() + materialOffsets[3]), &is_affected_by_light_, sizeof(bool));
    memcpy((void*)(materialStorage.data() + materialOffsets[4]), glm::value_ptr(diffuse), sizeof(glm::vec4));

    if (materialBuffer && materialBlockLocation != GL_INVALID_INDEX) {
        OGL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, materialBuffer));
        OGL_CALL(glBufferData(GL_UNIFORM_BUFFER, materialBlockSize, materialStorage.data(), GL_STATIC_DRAW));
        OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, MATERIAL_BINDING_POINT, materialBuffer));
        OGL_CALL(glUniformBlockBinding(shaderProgram, materialBlockLocation, MATERIAL_BINDING_POINT));
        OGL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, 0));
    }

    StopUseShader();
}

void EpicShader::UpdateAttenuationUniforms(const Light* light) const
{
    float constant = 1.f, linear = 0.f, quadratic = 0.f;
    if (light) {
        light->GetAttenuation(constant, linear, quadratic);
    }

    SetShaderUniform("constantAttenuation", constant);
    SetShaderUniform("linearAttenuation", linear);
    SetShaderUniform("quadraticAttenuation", quadratic);
}

void EpicShader::SetupShaderMaterials() const
{
    // Need to make sure the material buffer is bound.
    OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, MATERIAL_BINDING_POINT, materialBuffer));

    // Make sure the right textures are bound.
    const Texture* diffuseTexture = defaultTexture.get();
    if (textureSlotMapping.find(TextureSlots::DIFFUSE) != textureSlotMapping.end()) {
        diffuseTexture = textureSlotMapping.at(TextureSlots::DIFFUSE).get();
    }
    assert(diffuseTexture);
    diffuseTexture->BeginRender(static_cast<int>(TextureSlots::DIFFUSE));

    // While we're here, also setup the textures too.
    SetShaderUniform("diffuseTexture", static_cast<int>(TextureSlots::DIFFUSE));
}

void EpicShader::SetupShaderCamera(const class Camera* camera) const
{
    SetShaderUniform("cameraPosition", camera->GetPosition());
}

void EpicShader::SetMetallic(float inMetallic) 
{ 
    metallic = inMetallic; 
    UpdateMaterialBlock();
}

void EpicShader::SetRoughness(float inRoughness) 
{ 
    roughness = inRoughness; 
    UpdateMaterialBlock();
}


void EpicShader::SetSpecular(glm::vec4 inSpecular) 
{ 
    specular = inSpecular; 
    UpdateMaterialBlock();
}


void EpicShader::SetDiffuse(glm::vec4 inDiffuse) 
{ 
    diffuse = inDiffuse; 
    UpdateMaterialBlock();
}

void EpicShader::SetLight(bool inLight) 
{ 
    is_affected_by_light_ = inLight; 
    UpdateMaterialBlock();
}

void EpicShader::SetTexture(TextureSlots::Type slot, std::shared_ptr<class Texture> inputTexture)
{
    textureSlotMapping[slot] = std::move(inputTexture);
}

void EpicShader::LoadMaterialFromAssimp(std::shared_ptr<aiMaterial> assimpMaterial)
{
    if (!assimpMaterial) {
        return;
    }

    // for mtl files with only colors...
    assimpMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, glm::value_ptr(diffuse), nullptr);

    if (assimpMaterial->GetTextureCount(aiTextureType_DIFFUSE)) {
        aiString aiDiffusePath;
        assimpMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &aiDiffusePath);
        std::string diffusePath(aiDiffusePath.C_Str());
        std::cout << diffusePath << std::endl;
        SetTexture(TextureSlots::DIFFUSE, TextureLoader::LoadTexture(diffusePath));
    }

    UpdateMaterialBlock();
}

void EpicShader::SetMaxDisplacement(float input)
{
    maxDisplacement = input;
}
