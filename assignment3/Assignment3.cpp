#include "assignment3/Assignment3.h"
#include "common/Utility/Mesh/Simple/PrimitiveCreator.h"
#include "common/Utility/Mesh/Loading/MeshLoader.h"
#include "common/Utility/Texture/TextureLoader.h"

//#include "instructor/Light/DirectionalLight.h"
//#include "instructor/Light/HemisphereLight.h"

#include <cmath>

Assignment3::Assignment3(std::shared_ptr<class Scene> inputScene, std::shared_ptr<class Camera> inputCamera):
    Application(std::move(inputScene), std::move(inputCamera))
{
}

std::unique_ptr<Application> Assignment3::CreateApplication(std::shared_ptr<class Scene> scene, std::shared_ptr<class Camera> camera)
{
    return make_unique<Assignment3>(std::move(scene), std::move(camera));
}

std::shared_ptr<class Camera> Assignment3::CreateCamera()
{
    // Specify any old aspect ratio for now, we'll update it later once the window gets made!
    // Read more about Field of View: http://rg3.name/201210281829.html!
    // Note that our field of view is the VERTICAL field of view (in degrees).
    return std::make_shared<PerspectiveCamera>(60.f, 1280.f / 720.f);
}

glm::vec2 Assignment3::GetWindowSize() const
{
    return glm::vec2(1280.f, 720.f);
}

void Assignment3::SetupScene()
{
    SetupExample2();
}

void Assignment3::SetupCamera(bool isFirst)
{
    // if (isFirst) {
    //     camera->SetPosition(glm::vec3(-35.698f, 40.571f, -2.285f));
    //     camera->Rotate(glm::vec3(SceneObject::GetWorldUp()), -3.3f * PI / 5.f);
    //     camera->Rotate(glm::vec3(camera->GetRightDirection()), -PI / 8.f);
    //     camera->Translate(glm::vec3(camera->GetRightDirection()) * 10.f);
    //     camera->Translate(glm::vec3(camera->GetForwardDirection()) * -10.f);

    //     PerspectiveCamera* pcamera = static_cast<PerspectiveCamera*>(camera.get());
    //     pcamera->SetZFar(1000.f);    
    // } else {
    PerspectiveCamera* pcamera = static_cast<PerspectiveCamera*>(camera.get());
    pcamera->SetFOV(32.267f);  // 54.43 Angle of view
    pcamera->SetZNear(0.1f);
    pcamera->SetZFar(10000.f);

    camera->SetPosition(glm::vec3(-43.866f, -0.431f, -9.283f));
    camera->Rotate(glm::vec3(1.f, 0.f, 0.f), 22.037f * PI / 180.f);
    camera->Rotate(glm::vec3(0.f, 1.f, 0.f), 256.857f * PI / 180.f);
    camera->Rotate(glm::vec3(0.f, 0.f, 1.f), 0.f * PI / 180.f);
    // }
}

void Assignment3::HandleInput(SDL_Keysym key, Uint32 state, Uint8 repeat, double timestamp, double deltaTime)
{   
    if (state != SDL_KEYDOWN) {
        return;
    }

    switch (key.sym) {
    case SDLK_1:
        if (!repeat && state == SDL_KEYDOWN) {
            SetupExample1();
        }
        break;
    case SDLK_2:
        if (!repeat && state == SDL_KEYDOWN) {
            SetupExample2();
        }
        break;
    case SDLK_UP:
        camera->Rotate(glm::vec3(camera->GetRightDirection()), 0.1f);
        break;
    case SDLK_DOWN:
        camera->Rotate(glm::vec3(camera->GetRightDirection()), -0.1f);
        break;
    case SDLK_RIGHT:
        camera->Rotate(glm::vec3(SceneObject::GetWorldUp()), -0.1f);
        break;
    case SDLK_LEFT:
        camera->Rotate(glm::vec3(SceneObject::GetWorldUp()), 0.1f);
        break;
    case SDLK_w:
        camera->Translate(glm::vec3(camera->GetForwardDirection() * 0.3f));
        break;
    case SDLK_a:
        camera->Translate(glm::vec3(camera->GetRightDirection() * -0.3f));
        break;
    case SDLK_s:
        camera->Translate(glm::vec3(camera->GetForwardDirection() * -0.3f));
        break;
    case SDLK_d:
        camera->Translate(glm::vec3(camera->GetRightDirection() * 0.3f));
        break;
    case SDLK_SPACE:
        camera->Translate(glm::vec3(camera->GetUpDirection() * 0.3f));
        break;
    case SDLK_LCTRL:
    case SDLK_RCTRL:
        camera->Translate(glm::vec3(camera->GetUpDirection() * -0.3f));
        break;
    default:
        Application::HandleInput(key, state, repeat, timestamp, deltaTime);
        break;
    }
}

void Assignment3::HandleWindowResize(float x, float y)
{
    Application::HandleWindowResize(x, y);
    std::static_pointer_cast<PerspectiveCamera>(camera)->SetAspectRatio(x / y);
}

void Assignment3::SetupExample1()
{
    scene->ClearScene();
    // SetupCamera(true);

    std::unordered_map<GLenum, std::string> shaderSpec = {
        { GL_VERTEX_SHADER, "brdf/epic/frag/noSubroutine/epic.vert" },
        { GL_FRAGMENT_SHADER, "brdf/epic/frag/noSubroutine/epic.frag"}
    };

    std::shared_ptr<EpicShader> shader = std::make_shared<EpicShader>(shaderSpec, GL_FRAGMENT_SHADER);
    shader->SetRoughness(0.2f);
    shader->SetMetallic(0.7f);
    shader->SetSpecular(glm::vec4(0.f, 1.f, 0.f, 1.f));

    std::shared_ptr<EpicShader> groundShader = std::make_shared<EpicShader>(shaderSpec, GL_FRAGMENT_SHADER);
    groundShader->SetRoughness(0.7f);
    groundShader->SetMetallic(0.2f);
    groundShader->SetSpecular(glm::vec4(1.f, 1.f, 1.f, 1.f));

    MakePointLight(glm::vec3(10,10,10), glm::vec4(1.f, 0.5f, 0.f, 1.f), 100.f);
    MakeDirectionalLight(glm::vec3(0.f, 1.f, 3.f), glm::vec4(0.f, 0.0f, 1.f, 1.f));
    MakeHemisphereLight(glm::vec4(0.f, 0.1f, 0.f, 1.f), glm::vec4(0.89, 0.349f, 0.f, 1.f));

    // set up scene objects
    GenericSetupExample(shader, groundShader);
}

void Assignment3::MakeMesh(std::string file_name, float r, float m, glm::vec4 spec) {
    std::unordered_map<GLenum, std::string> shaderSpec = {
        { GL_VERTEX_SHADER, "brdf/epic/frag/noSubroutine/epic.vert" },
        { GL_FRAGMENT_SHADER, "brdf/epic/frag/noSubroutine/epic.frag"}
    };

    std::shared_ptr<EpicShader> shader = std::make_shared<EpicShader>(shaderSpec, GL_FRAGMENT_SHADER);
    shader->SetRoughness(r);
    shader->SetMetallic(m);
    shader->SetSpecular(spec);

    std::vector<std::shared_ptr<RenderingObject>> meshTemplate = MeshLoader::LoadMesh(shader, file_name);
    if (meshTemplate.empty()) {
        std::cerr << "ERROR: Failed to load the model " << file_name << " . Check your paths." << std::endl;
        return;
    }

    std::shared_ptr<class SceneObject> sceneObject = std::make_shared<SceneObject>(meshTemplate);
    sceneObject->SetPosition(glm::vec3(0.f, 0.f, 0.f));
    scene->AddSceneObject(sceneObject);
}

void Assignment3::MakePointLight(glm::vec3 position, 
                                 glm::vec4 color,
                                 float radius) {
    std::unique_ptr<EpicLightProperties> pointLightProperties = EpicShader::CreateLightProperties();
    pointLightProperties->light_color = color;
    pointLightProperties->point_position = glm::vec4(position, 0.f);
    pointLightProperties->light_radius = radius;
    std::shared_ptr<class Light> pointLight = std::make_shared<Light>(std::move(pointLightProperties),
                                                                      Light::LightType::POINT, "pointLight");
    scene->AddLight(pointLight);
}

void Assignment3::MakeDirectionalLight(glm::vec3 forward_direction, 
                                       glm::vec4 color) {
    std::unique_ptr<EpicLightProperties> directionalLightProperties = EpicShader::CreateLightProperties();
    directionalLightProperties->light_color = color;
    std::shared_ptr<class Light> directionalLight = std::make_shared<Light>(std::move(directionalLightProperties),
                                                                      Light::LightType::DIRECTIONAL, "directionalLight");
    directionalLight->Rotate(glm::vec3(1.f, 0.f, 0.f), forward_direction.x * PI / 180.f);
    directionalLight->Rotate(glm::vec3(0.f, 1.f, 0.f), forward_direction.y * PI / 180.f);
    directionalLight->Rotate(glm::vec3(0.f, 0.f, 1.f), forward_direction.z * PI / 180.f);

    scene->AddLight(directionalLight);
}

void Assignment3::MakeHemisphereLight(glm::vec4 sky_color, glm::vec4 ground_color) {
    std::unique_ptr<EpicLightProperties> hemisphereLightProperties = EpicShader::CreateLightProperties();
    hemisphereLightProperties->sky_color = sky_color;
    hemisphereLightProperties->ground_color = ground_color;
    std::shared_ptr<class Light> hemisphereLight = std::make_shared<Light>(std::move(hemisphereLightProperties),
                                                                      Light::LightType::HEMISPHERE, "hemisphereLight");
    scene->AddLight(hemisphereLight);

}
void Assignment3::SetupExample2() {
    scene->ClearScene();
    SetupCamera(false);

    // r, m, spec
    MakeMesh("BlackWidow/black_widow.obj", 0.5, 0.2, glm::vec4(0.7,0.7,0.7,1));
    MakeMesh("Hawkeye/hawkeye.obj", 0.7, 0., glm::vec4(0.3,0.3,0.3,1));
    MakeMesh("Helicopter/helicopter.obj", 0.5, 0.9, glm::vec4(0.7,0.7,0.7,1));
    MakeMesh("IronMan/IronMan.obj", 0.5, 0.9, glm::vec4(0.7,0.7,0.7,1));
    MakeMesh("Thor/thor.obj", 0.5, 0., glm::vec4(0.2,0.2,0.2,1));
    MakeMesh("Hulk/hulk.obj", 0.5, 0., glm::vec4(0.2,0.2,0.2,1));
    MakeMesh("CaptainAmerica/Captain_America.obj", 0.5, 0.2, glm::vec4(0.2,0.2,0.2,1));
    MakeMesh("buildings/destroyed_building_left.obj", 0.5, 0.6, glm::vec4(0.2,0.2,0.2,1));
    MakeMesh("buildings/destroyed_building_right.obj", 0.5, 0.6, glm::vec4(0.2,0.2,0.2,1));

    // sunlight
    MakeDirectionalLight(glm::vec3(-123.992f, -2.667, 33.612), glm::vec4(0.7,0.7,0.7,1));
    // explosion  right
    MakePointLight(glm::vec3(352.634f, 174.693f, 265.913f), glm::vec4(1.5,0.3,0,1), 10000);
    // explosion left
    MakePointLight(glm::vec3(726.656f, 190.738f, -137.344f), glm::vec4(1,0.5,0,1), 10000);
    // hemisphere light
    MakeHemisphereLight(glm::vec4(0.7f, 0.2f, 0.0f, 1.f), glm::vec4(0.f, 0.5f, 1.5f, 1.f));
}

void Assignment3::GenericSetupExample(std::shared_ptr<ShaderProgram> shader, std::shared_ptr<ShaderProgram> groundShader)
{
    std::shared_ptr<RenderingObject> sphereTemplate = PrimitiveCreator::CreateIcoSphere(shader, 5.f, 4);

    // Give a R/G/B color to each vertex to visualize the sphere.
    auto totalVertices = sphereTemplate->GetTotalVertices();

    std::unique_ptr<RenderingObject::ColorArray> vertexColors = make_unique<RenderingObject::ColorArray>();
    vertexColors->reserve(totalVertices);

    for (decltype(totalVertices) i = 0; i < totalVertices; ++i) {
        vertexColors->emplace_back(1.f, 1.f, 1.f, 1.f);
    }
    sphereTemplate->SetVertexColors(std::move(vertexColors));

    sphereDance.clear();
    // 10x10 grid of spheres.
    for (int x = 0; x < 10; ++x)  {
        for (int y = 0; y < 10; ++y) {
            std::shared_ptr<class SceneObject> sceneObject = std::make_shared<SceneObject>(sphereTemplate);
            sceneObject->SetPosition(glm::vec3(x * 12.f, 0.f, y * 12.f));
            scene->AddSceneObject(sceneObject);
            sphereDance.push_back(sceneObject);
        }
    }

    std::shared_ptr<RenderingObject> plane = PrimitiveCreator::CreatePlane(groundShader);
    std::shared_ptr<class SceneObject> groundObject = std::make_shared<SceneObject>(plane);
    groundObject->MultScale(150.f);
    groundObject->SetPosition(glm::vec3(-10.f, 0.f, -10.f));
    groundObject->Translate(glm::vec3(SceneObject::GetWorldUp() * -5.f));

    std::unique_ptr<RenderingObject::ColorArray> groundColor = make_unique<RenderingObject::ColorArray>();
    groundColor->reserve(4);

    for (int i = 0; i < 4; ++i) {
        groundColor->emplace_back(0.89, 0.349f, 0.f, 1.f);
    }
    plane->SetVertexColors(std::move(groundColor));

    scene->AddSceneObject(groundObject);
}

void Assignment3::Tick(double deltaTime)
{
    if (sunLight) {
        sunLight->Rotate(glm::vec3(SceneObject::GetWorldRight()), PI * deltaTime / -4.f);
    }

    static float elapsedTime = 0.f;
    elapsedTime += deltaTime;

    for (size_t i = 0; i < sphereDance.size(); ++i) {
        glm::vec4 og = sphereDance[i]->GetPosition();
        sphereDance[i]->SetPosition(glm::vec3(og.x, std::abs(std::sin(elapsedTime + (float)i)) * 6.f, og.z));
    }
}
