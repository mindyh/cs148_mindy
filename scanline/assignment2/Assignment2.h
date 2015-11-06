#pragma once

#ifndef __ASSIGNMENT_2__
#define __ASSIGNMENT_2__

#include "common/Application.h"
#include "common/core.h" // <-- haha.

class Assignment2: public Application
{
public:
    Assignment2(std::shared_ptr<class Scene> inputScene, std::shared_ptr<class Camera> inputCamera);

    static std::unique_ptr<Application> CreateApplication(std::shared_ptr<class Scene> scene, std::shared_ptr<class Camera> camera);
    static std::shared_ptr<class Camera> CreateCamera();

    virtual glm::vec2 GetWindowSize() const;

    virtual void HandleInput(SDL_Keysym key, Uint32 state, Uint8 repeat, double timestamp, double deltaTime);
    virtual void Tick(double deltaTime);
protected:
    virtual void HandleWindowResize(float x, float y);

private:
    virtual void SetupScene();
    virtual void SetupExample2();
    virtual void SetupCamera();

    void MakeLight(glm::vec3 position);
    void MakeMesh(std::string file_name, std::shared_ptr<BlinnPhongShader> shader);
};

#endif
