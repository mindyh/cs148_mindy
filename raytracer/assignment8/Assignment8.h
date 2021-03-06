#pragma once

#include "common/Application.h"

class Assignment8 : public Application
{
public:
    virtual std::shared_ptr<class Camera> CreateCamera() const override;
    virtual std::shared_ptr<class Scene> CreateScene() const override;
    virtual std::shared_ptr<class ColorSampler> CreateSampler() const override;
    virtual std::shared_ptr<class Renderer> CreateRenderer(std::shared_ptr<class Scene> scene, std::shared_ptr<class ColorSampler> sampler) const override;
    virtual int GetSamplesPerPixel() const override;
    virtual bool NotifyNewPixelSample(glm::vec3 inputSampleColor, int sampleIndex) override;
    virtual int GetMaxReflectionBounces() const override;
    virtual int GetMaxRefractionBounces() const override;
    virtual glm::vec2 GetImageOutputResolution() const override;
    virtual float GetFocusPlane() const override;
    virtual float GetAperture() const;
private:
    void AddMesh(std::shared_ptr<Scene> scene, std::string filename, float reflectivity = 0,
                glm::vec3 rotation=glm::vec3(0), glm::vec3 translation=glm::vec3(0)) const;
    void AddSkyPlane(std::shared_ptr<Scene> scene, std::string mesh_filename, std::string texture_filename,
                     glm::vec3 rotation=glm::vec3(0), glm::vec3 translation=glm::vec3(0)) const;
    void AddLight(std::shared_ptr<Scene> scene, glm::vec3 position=glm::vec3(0), glm::vec3 color=glm::vec3(0)) const;
    void CreateCornellBoxes(std::shared_ptr<Scene> scene) const; 
    void CreateGlasses(std::shared_ptr<Scene> scene) const; 
    void AddAreaLight(std::shared_ptr<Scene> scene, glm::vec3 position, glm::vec3 rotation, 
                      glm::vec3 color, glm::vec2 size) const;
};
