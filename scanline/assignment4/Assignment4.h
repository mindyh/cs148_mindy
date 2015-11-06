#pragma once

#ifndef __ASSIGNMENT_4__
#define __ASSIGNMENT_4__

#include "common/Application.h"
#include "common/core.h" // <-- haha.

class Assignment4: public Application
{
public:
    Assignment4(std::shared_ptr<class Scene> inputScene, std::shared_ptr<class Camera> inputCamera);

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
    virtual void SetupExample3();
 
    virtual void SetupCamera();

    void MakeEpicPointLight(glm::vec3 position, glm::vec4 color, float radius = 1000.f);
    void MakeDirectionalLight(glm::vec3 forward_direction, glm::vec4 color);
    // void MakeHemisphereLight(glm::vec4 sky_color, glm::vec4 ground_color);

    void MakeBPPointLight(glm::vec3 position, glm::vec4 color_d, glm::vec4 color_s, float radius = 10000);
    // void MakeBPHemisphereLight(glm::vec4 sky_color, glm::vec4 ground_color);
    // void MakeBPDirectionalLight(glm::vec4 sky_color, glm::vec4 ground_color);
    void MakeMesh(std::string file_name, bool isLit = true);

    void MakeEpicMesh(std::string file_name, float r, float m, glm::vec4 spec, glm::vec3 pos = glm::vec3(0));

    std::shared_ptr<class Light> sunLight;
    // std::shared_ptr<class Light> hemisphereLight;
    // std::shared_ptr<class Light> pointLight;
    std::vector<std::shared_ptr<class SceneObject>> sphereDance;
};

#endif
