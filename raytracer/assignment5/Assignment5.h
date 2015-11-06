#pragma once

#include "common/Application.h"

class Assignment5 : public Application
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

private:
    void MakeMesh(std::string file_name) const;
	void CreateCubes(std::shared_ptr<Scene> scene) const;
	void CreateAvengers(std::shared_ptr<Scene> scene) const;
	void MakeBPMesh(std::string file_name, std::shared_ptr<Scene> scene, 
					glm::vec3 pos = glm::vec3(0), glm::vec3 rot = glm::vec3(0), float rot_angle = 0) const;
	void MakePointLight(glm::vec3 position, glm::vec3 color, std::shared_ptr<Scene> scene) const;
};
