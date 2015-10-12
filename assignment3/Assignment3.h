#pragma once

#ifndef __ASSIGNMENT_3__
#define __ASSIGNMENT_3__

#include "common/Application.h"
#include "common/core.h" // <-- haha.

class Assignment3: public Application
{
public:
    Assignment3(std::shared_ptr<class Scene> inputScene, std::shared_ptr<class Camera> inputCamera);

    static std::unique_ptr<Application> CreateApplication(std::shared_ptr<class Scene> scene, std::shared_ptr<class Camera> camera);
    static std::shared_ptr<class Camera> CreateCamera();

    virtual glm::vec2 GetWindowSize() const;

    virtual void HandleInput(SDL_Keysym key, Uint32 state, Uint8 repeat, double timestamp, double deltaTime);
    virtual void Tick(double deltaTime);
protected:
    virtual void HandleWindowResize(float x, float y);

private:
    virtual void SetupScene();

    virtual void SetupExample1();
    virtual void SetupExample2();
    virtual void GenericSetupExample(std::shared_ptr<class ShaderProgram> shader, std::shared_ptr<ShaderProgram> groundShader);

    virtual void SetupCamera(bool isFirst);

    void MakePointLight(glm::vec3 position, glm::vec4 color, float radius = 100.f);
    void MakeDirectionalLight(glm::vec3 position, glm::vec3 forward_direction, glm::vec4 color);
    void MakeHemisphereLight(glm::vec4 sky_color, glm::vec4 ground_color);
    void MakeMesh(std::string file_name, std::shared_ptr<EpicShader> shader);

    std::shared_ptr<class Light> sunLight;
    // std::shared_ptr<class Light> hemisphereLight;
    // std::shared_ptr<class Light> pointLight;
    std::vector<std::shared_ptr<class SceneObject>> sphereDance;
};

#endif
