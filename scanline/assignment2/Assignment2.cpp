#include "assignment2/Assignment2.h"
#include "common/Utility/Mesh/Simple/PrimitiveCreator.h"
#include "common/Utility/Mesh/Loading/MeshLoader.h"
#include <cmath>

Assignment2::Assignment2(std::shared_ptr<class Scene> inputScene, std::shared_ptr<class Camera> inputCamera):
    Application(std::move(inputScene), std::move(inputCamera))
{
}

std::unique_ptr<Application> Assignment2::CreateApplication(std::shared_ptr<class Scene> scene, std::shared_ptr<class Camera> camera)
{
    return make_unique<Assignment2>(std::move(scene), std::move(camera));
}

std::shared_ptr<class Camera> Assignment2::CreateCamera()
{
    // Specify any old aspect ratio for now, we'll update it later once the window gets made!
    // Read more about Field of View: http://rg3.name/201210281829.html!
    // Note that our field of view is the VERTICAL field of view (in degrees).
    return std::make_shared<PerspectiveCamera>(75.f, 1280.f / 720.f);
}

glm::vec2 Assignment2::GetWindowSize() const
{
    return glm::vec2(1280.f, 720.f);
}

void Assignment2::SetupScene()
{
    SetupExample2();
}

void Assignment2::SetupCamera()
{
    /*PerspectiveCamera* pcamera = static_cast<PerspectiveCamera*>(camera.get());
    pcamera->SetFOV(32.267f);  // 54.43 Angle of view
    pcamera->SetZNear(0.1f);
    pcamera->SetZFar(10000.f);

    camera->SetPosition(glm::vec3(-35.981f, 11.935f, -6.470f));
    camera->Rotate(glm::vec3(1.f, 0.f, 0.f), 10.2f * PI / 180.f);
    camera->Rotate(glm::vec3(0.f, 1.f, 0.f), 258.f * PI / 180.f);
    camera->Rotate(glm::vec3(0.f, 0.f, 1.f), 0.f * PI / 180.f);*/
    
    PerspectiveCamera* pcamera = static_cast<PerspectiveCamera*>(camera.get());
    pcamera->SetFOV(34.45f); //57.59 field of view
    pcamera->SetZNear(0.1f);
    pcamera->SetZFar(10000.f);
    
    camera->SetPosition(glm::vec3(-49.213f, 1.179f, -6.191f));
    camera->Rotate(glm::vec3(1.f, 0.f, 0.f), 13.037f * PI / 180.f);
    camera->Rotate(glm::vec3(0.f, 1.f, 0.f), 260.857f * PI / 180.f);
    camera->Rotate(glm::vec3(0.f, 0.f, 1.f), 0.f * PI / 180.f);
}

void Assignment2::HandleInput(SDL_Keysym key, Uint32 state, Uint8 repeat, double timestamp, double deltaTime)
{   
    if (state != SDL_KEYDOWN) {
        return;
    }

    switch (key.sym) {
    case SDLK_UP:
        camera->Rotate(glm::vec3(SceneObject::GetWorldRight()), -0.1f);
        break;
    case SDLK_DOWN:
        camera->Rotate(glm::vec3(SceneObject::GetWorldRight()), 0.1f);
        break;
    case SDLK_RIGHT:
        camera->Rotate(glm::vec3(SceneObject::GetWorldUp()), 0.1f);
        break;
    case SDLK_LEFT:
        camera->Rotate(glm::vec3(SceneObject::GetWorldUp()), -0.1f);
        break;
    case SDLK_w:
        camera->Translate(glm::vec3(SceneObject::GetWorldForward() * -0.3f));
        break;
    case SDLK_a:
        camera->Translate(glm::vec3(SceneObject::GetWorldRight() * 0.3f));
        break;
    case SDLK_s:
        camera->Translate(glm::vec3(SceneObject::GetWorldForward() * 0.3f));
        break;
    case SDLK_d:
        camera->Translate(glm::vec3(SceneObject::GetWorldRight() * -0.3f));
        break;
    case SDLK_SPACE:
        camera->Translate(glm::vec3(SceneObject::GetWorldUp() * -0.3f));
        break;
    case SDLK_LCTRL:
    case SDLK_RCTRL:
        camera->Translate(glm::vec3(SceneObject::GetWorldUp() * 0.3f));
        break;
    default:
        Application::HandleInput(key, state, repeat, timestamp, deltaTime);
        break;
    }
}

void Assignment2::HandleWindowResize(float x, float y)
{
    Application::HandleWindowResize(x, y);
    std::static_pointer_cast<PerspectiveCamera>(camera)->SetAspectRatio(x / y);
}

void Assignment2::MakeMesh(std::string file_name, std::shared_ptr<BlinnPhongShader> shader) {
    std::vector<std::shared_ptr<RenderingObject>> meshTemplate = MeshLoader::LoadMesh(shader, file_name);
    if (meshTemplate.empty()) {
        std::cerr << "ERROR: Failed to load the model " << file_name << " . Check your paths." << std::endl;
        return;
    }

    std::shared_ptr<class SceneObject> sceneObject = std::make_shared<SceneObject>(meshTemplate);
    sceneObject->SetPosition(glm::vec3(0.f, 0.f, 0.f));
    scene->AddSceneObject(sceneObject);
}

void Assignment2::MakeLight(glm::vec3 position) {
    std::unique_ptr<BlinnPhongLightProperties> lightProperties = BlinnPhongShader::CreateLightProperties();
    lightProperties->diffuseColor = glm::vec4(0.5f, 0.5f, 0.5f, 1.f);

    std::shared_ptr<class Light> pointLight = std::make_shared<Light>(std::move(lightProperties));
    pointLight->SetPosition(position);
    scene->AddLight(pointLight);
}

void Assignment2::SetupExample2()
{
    scene->ClearScene();
#ifndef DISABLE_OPENGL_SUBROUTINES
    std::unordered_map<GLenum, std::string> shaderSpec = {
        { GL_VERTEX_SHADER, "brdf/blinnphong/frag/blinnphong.vert" },
        { GL_FRAGMENT_SHADER, "brdf/blinnphong/frag/blinnphong.frag"}
    };
#else
    std::unordered_map<GLenum, std::string> shaderSpec = {
        { GL_VERTEX_SHADER, "brdf/blinnphong/frag/noSubroutine/blinnphong.vert" },
        { GL_FRAGMENT_SHADER, "brdf/blinnphong/frag/noSubroutine/blinnphong.frag"}
    };
#endif
    std::shared_ptr<BlinnPhongShader> shader = std::make_shared<BlinnPhongShader>(shaderSpec, GL_FRAGMENT_SHADER);
    shader->SetDiffuse(glm::vec4(0.8f, 0.8f, 0.8f, 1.f));
    shader->SetAmbient(glm::vec4(0.5f));

    MakeMesh("Hawkeye/hawkeye.obj", shader);
    MakeMesh("BlackWidow/black_widow.obj", shader);
    MakeMesh("IronMan/IronMan.obj", shader);
    MakeMesh("buildings/destroyed_building1.obj", shader);
    MakeMesh("buildings/destroyed_building2.obj", shader);
    MakeMesh("CaptainAmerica/Captain_America_The_First_Avenger.obj", shader);
    MakeMesh("Thor/Thor_Avengers.obj", shader);
    MakeMesh("Hulk/Hulk_Avengers.obj", shader);
    MakeMesh("shield/shield.obj", shader);
    // MakeMesh("Floor/floor.obj", shader);

    MakeLight(glm::vec3(208.711f, 99.565f, 173.785f));
    MakeLight(glm::vec3(-129.669f, 126.841f, -70.083f));
    MakeLight(glm::vec3(270.366f, 98.014f, 53.169f));
}

void Assignment2::Tick(double deltaTime)
{
}
