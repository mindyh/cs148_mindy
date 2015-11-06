#include "assignment4/Assignment4.h"
#include "common/core.h" // <-- haha.
#include "common/Utility/Mesh/Simple/PrimitiveCreator.h"
#include "common/Utility/Mesh/Loading/MeshLoader.h"
#include "common/Utility/Texture/TextureLoader.h"

Assignment4::Assignment4(std::shared_ptr<class Scene> inputScene, std::shared_ptr<class Camera> inputCamera):
    Application(std::move(inputScene), std::move(inputCamera))
{
}

std::unique_ptr<Application> Assignment4::CreateApplication(std::shared_ptr<class Scene> scene, std::shared_ptr<class Camera> camera)
{
    return make_unique<Assignment4>(std::move(scene), std::move(camera));
}

std::shared_ptr<class Camera> Assignment4::CreateCamera()
{
    // Specify any old aspect ratio for now, we'll update it later once the window gets made!
    // Read more about Field of View: http://rg3.name/201210281829.html!
    // Note that our field of view is the VERTICAL field of view (in degrees).
    return std::make_shared<PerspectiveCamera>(60.f, 1280.f / 720.f);
}

glm::vec2 Assignment4::GetWindowSize() const
{
    return glm::vec2(1280.f, 720.f);
}

void Assignment4::SetupScene()
{
    SetupExample2();
}

void Assignment4::SetupCamera()
{
    PerspectiveCamera* pcamera = static_cast<PerspectiveCamera*>(camera.get());
    pcamera->SetFOV(32.267f);  // 54.43 Angle of view
    pcamera->SetZNear(0.1f);
    pcamera->SetZFar(10000.f);

    // camera->SetPosition(glm::vec3(-43.866f, -0.431f, -9.283f));
    camera->SetPosition(glm::vec3(-47.099422, -1.894301, -12.105892));

    camera->Rotate(glm::vec3(1.f, 0.f, 0.f), 22.037f * PI / 180.f);
    camera->Rotate(glm::vec3(0.f, 1.f, 0.f), 256.857f * PI / 180.f);
    camera->Rotate(glm::vec3(0.f, 0.f, 1.f), 0.f * PI / 180.f);
}

void Assignment4::HandleInput(SDL_Keysym key, Uint32 state, Uint8 repeat, double timestamp, double deltaTime)
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
    case SDLK_k:
        std::cout << glm::to_string(camera->GetPosition()) << std::endl;
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

void Assignment4::HandleWindowResize(float x, float y)
{
    Application::HandleWindowResize(x, y);
    std::static_pointer_cast<PerspectiveCamera>(camera)->SetAspectRatio(x / y);
}

void Assignment4::SetupExample1()
{
    scene->ClearScene();

    std::unordered_map<GLenum, std::string> shaderSpec = {
        { GL_VERTEX_SHADER, "brdf/blinnphong/fragTexture/noSubroutine/blinnphong.vert" },
        { GL_FRAGMENT_SHADER, "brdf/blinnphong/fragTexture/noSubroutine/blinnphong.frag"}
    };
    
    std::shared_ptr<BlinnPhongShader> shader = std::make_shared<BlinnPhongShader>(shaderSpec, GL_FRAGMENT_SHADER);
    shader->SetDiffuse(glm::vec4(0.8f, 0.8f, 0.8f, 1.f));
    shader->SetTexture(BlinnPhongShader::TextureSlots::DIFFUSE, TextureLoader::LoadTexture("brick/bricktexture.jpg"));
    shader->SetTexture(BlinnPhongShader::TextureSlots::SPECULAR, TextureLoader::LoadTexture("brick/bricktexture.jpg"));
    shader->SetTexture(BlinnPhongShader::TextureSlots::NORMAL, TextureLoader::LoadTexture("brick/bricktexture_norm.jpg"));
    shader->SetTexture(BlinnPhongShader::TextureSlots::DISPLACEMENT, TextureLoader::LoadTexture("brick/bricktexture_displacement.jpg"));
    shader->SetMaxDisplacement(0.1f);

    std::unique_ptr<LightProperties> lightProperties = make_unique<LightProperties>();
    lightProperties->diffuseColor = glm::vec4(1.f, 1.f, 1.f, 1.f);
    lightProperties->specularColor = glm::vec4(1.f, 1.f, 1.f, 1.f);

    std::shared_ptr<Light> pointLight = std::make_shared<Light>(std::move(lightProperties));
    pointLight->SetPosition(glm::vec3(10.f, 10.f, 10.f));
    scene->AddLight(pointLight);

    std::vector<std::shared_ptr<RenderingObject>> sphereTemplate = MeshLoader::LoadMesh(shader, "sphere.obj");
    std::shared_ptr<class SceneObject> sceneObject = std::make_shared<SceneObject>(sphereTemplate);
    sceneObject->Rotate(glm::vec3(SceneObject::GetWorldRight()), PI / 4.f);
    scene->AddSceneObject(sceneObject);
}

void Assignment4::MakeEpicMesh(std::string file_name,
							   float r, float m, glm::vec4 spec, glm::vec3 pos) {
    std::unordered_map<GLenum, std::string> shaderSpec = {
        { GL_VERTEX_SHADER, "brdf/epic/fragTexture/noSubroutine/epic.vert" },
        { GL_FRAGMENT_SHADER, "brdf/epic/fragTexture/noSubroutine/epic.frag" }
    };

    std::vector<std::shared_ptr<aiMaterial>> loadedMaterials;
    std::vector<std::shared_ptr<RenderingObject>> meshTemplate = MeshLoader::LoadMesh(nullptr, file_name, &loadedMaterials);
    for (size_t i = 0; i < meshTemplate.size(); ++i) {
        std::shared_ptr<EpicShader> shader = std::make_shared<EpicShader>(shaderSpec, GL_FRAGMENT_SHADER);
        shader->LoadMaterialFromAssimp(loadedMaterials[i]);
        shader->SetRoughness(r);
	    shader->SetMetallic(m);
	    shader->SetSpecular(spec);
        meshTemplate[i]->SetShader(std::move(shader));
    }

    std::shared_ptr<class SceneObject> sceneObject = std::make_shared<SceneObject>(meshTemplate);
    sceneObject->Translate(pos);
    scene->AddSceneObject(sceneObject);
}

void Assignment4::MakeMesh(std::string file_name, bool isLit) {
    std::unordered_map<GLenum, std::string> shaderSpec = {
        { GL_VERTEX_SHADER, "brdf/blinnphong/fragTexture/noSubroutine/blinnphong.vert" },
        { GL_FRAGMENT_SHADER, "brdf/blinnphong/fragTexture/noSubroutine/blinnphong.frag" }
    };

    std::vector<std::shared_ptr<aiMaterial>> loadedMaterials;
    std::vector<std::shared_ptr<RenderingObject>> meshTemplate = MeshLoader::LoadMesh(nullptr, file_name, &loadedMaterials);
    for (size_t i = 0; i < meshTemplate.size(); ++i) {
        std::shared_ptr<BlinnPhongShader> shader = std::make_shared<BlinnPhongShader>(shaderSpec, GL_FRAGMENT_SHADER);
        shader->LoadMaterialFromAssimp(loadedMaterials[i]);
        shader->SetLight(isLit);
        meshTemplate[i]->SetShader(std::move(shader));
    }

    std::shared_ptr<class SceneObject> sceneObject = std::make_shared<SceneObject>(meshTemplate);
    sceneObject->SetPosition(glm::vec3(0.f, 0.f, 0.f));
    scene->AddSceneObject(sceneObject);
}

void Assignment4::MakeEpicPointLight(glm::vec3 position, glm::vec4 color, float radius) {
	std::unique_ptr<LightProperties> lightProperties = make_unique<LightProperties>();
    lightProperties->light_color = color;
    lightProperties->light_radius = radius;
    lightProperties->point_position = glm::vec4(position, 1.f);

    std::shared_ptr<Light> pointLight = std::make_shared<Light>(std::move(lightProperties));
    pointLight->SetPosition(position);
    scene->AddLight(pointLight);
}

void Assignment4::MakeBPPointLight(glm::vec3 position, glm::vec4 color_d, glm::vec4 color_s, float radius) {
    std::unique_ptr<LightProperties> lightProperties = make_unique<LightProperties>();
    lightProperties->diffuseColor = color_d;
    lightProperties->specularColor = color_s;
    lightProperties->light_radius = radius;

    std::shared_ptr<Light> pointLight = std::make_shared<Light>(std::move(lightProperties));
    pointLight->SetPosition(position);
    scene->AddLight(pointLight);
}

void Assignment4::SetupExample2() {
    scene->ClearScene();

    // r, m, spec
    MakeEpicMesh("BlackWidow/black_widow.obj", 0.6, 0.1, glm::vec4(0.4));
    MakeEpicMesh("gun/gun.obj", 0.45, 0.9, glm::vec4(0.8));
    MakeEpicMesh("gun/gun2.obj", 0.45, 0.9, glm::vec4(0.8));

    // // Iron Man
    MakeEpicMesh("IronMan/IronMan.obj", 0.6, 0.8, glm::vec4(0.6));
    MakeMesh("IronMan/leftHandBeam_ironman.obj", false);

    // hawkeye
    MakeEpicMesh("Hawkeye/hawkeye.obj", 0.6, 0.1, glm::vec4(0.1), glm::vec3(5.f,2.f, 5.f));

    // Thor
    MakeMesh("Thor/thor.obj");
    MakeEpicMesh("Thor/thor_hammer.obj", 0.45, 0.9, glm::vec4(0.5));
    MakeMesh("Thor/thor_cape.obj");
    // Hulk
    MakeEpicMesh("Hulk/hulk.obj", 0.5, 0.1, glm::vec4(0.01), glm::vec3(0.f,-10.f, 0.f));
    // Captain
    MakeEpicMesh("CaptainAmerica/Captain_America.obj", 0.6, 0.2, glm::vec4(0.8));
    MakeEpicMesh("shield/shield.obj", 0.45, 0.9, glm::vec4(0.8));
    // Buildings and bg
    MakeEpicMesh("buildings/destroyed_building_left.obj", 0.7, 0.2, glm::vec4(0.8));
    MakeEpicMesh("City/city.obj", 0.7, 0.2, glm::vec4(0.8));
    MakeMesh("explosion/explosion1.obj", false);
    MakeMesh("explosion/explosion2.obj", false);
    MakeMesh("explosion/explosion3.obj", false);
    MakeMesh("sky/sky.obj", false);

    glm::vec4 color(1, 0.5, 0.5, 1);
    // explosion  right
    MakeBPPointLight(glm::vec3(1058.218f, 670.524f, 820.973f), color, color);
    MakeEpicPointLight(glm::vec3(1058.218f, 670.524f, 820.973f), color, 3000.0f);
    // explosion left
    MakeBPPointLight(glm::vec3(726.656f, 190.738f, -137.344f), color, color);
    MakeEpicPointLight(glm::vec3(726.656f, 190.738f, -137.344f), color, 2000.0f);
  
    // front left
    color = glm::vec4 (0.5,0.5,0.5,1);
    MakeBPPointLight(glm::vec3(-39.220, 50.757, -61.093), color, color);
    color = glm::vec4 (3.5,3.5,3.5,1);
    MakeEpicPointLight(glm::vec3(-39.220, 50.757, -61.093), color, 1100.0);

    // center
    color = glm::vec4 (0.3,0.3,0.3,1);
    MakeBPPointLight(glm::vec3(-51.106, 51.156, 25.089), color, color);
    color = glm::vec4 (2,2,2,1);
    MakeEpicPointLight(glm::vec3(-51.106, 51.156, 25.089), color, 50.0);

    // front right
    color = glm::vec4 (0.3,0.3,0.3,1);
    // MakeBPPointLight(glm::vec3(-51.106, 51.156, 57.406), color, color);
    color = glm::vec4 (3.5,3.5,3.5,1);
    MakeEpicPointLight(glm::vec3(-51.106, 51.156, 57.406), color, 100.0);

    // iron man hand
    color = glm::vec4 (1,1,1,1);
    MakeBPPointLight(glm::vec3(-15.501f, 18.457f, 4.407f), color, color, 10);
}

void Assignment4::SetupExample3()
{
    scene->ClearScene();

    std::unordered_map<GLenum, std::string> shaderSpec = {
        { GL_VERTEX_SHADER, "brdf/blinnphong/fragTexture/noSubroutine/blinnphong.vert" },
        { GL_FRAGMENT_SHADER, "brdf/blinnphong/fragTexture/noSubroutine/blinnphong.frag"}
    };

    std::shared_ptr<BlinnPhongShader> shader = std::make_shared<BlinnPhongShader>(shaderSpec, GL_FRAGMENT_SHADER);
    shader->SetDiffuse(glm::vec4(0.8f, 0.8f, 0.8f, 1.f));
    shader->SetTexture(BlinnPhongShader::TextureSlots::DIFFUSE, TextureLoader::LoadTexture("brick/bricktexture.jpg"));
    shader->SetTexture(BlinnPhongShader::TextureSlots::SPECULAR, TextureLoader::LoadTexture("brick/bricktexture.jpg"));
    shader->SetTexture(BlinnPhongShader::TextureSlots::NORMAL, TextureLoader::LoadTexture("brick/bricktexture_norm.jpg"));
    shader->SetTexture(BlinnPhongShader::TextureSlots::DISPLACEMENT, TextureLoader::LoadTexture("brick/bricktexture_displacement.jpg"));
    shader->SetMaxDisplacement(0.1f);

    std::unique_ptr<LightProperties> lightProperties = make_unique<LightProperties>();
    lightProperties->diffuseColor = glm::vec4(1.f, 1.f, 1.f, 1.f);
    lightProperties->specularColor = glm::vec4(1.f, 1.f, 1.f, 1.f);

    std::shared_ptr<Light> pointLight = std::make_shared<Light>(std::move(lightProperties));
    pointLight->SetPosition(glm::vec3(10.f, 10.f, 10.f));
    scene->AddLight(pointLight);

    std::vector<std::shared_ptr<RenderingObject>> sphereTemplate = MeshLoader::LoadMesh(shader, "sphere.obj");
    for (size_t i = 0; i < sphereTemplate.size(); ++i) {
        sphereTemplate[i]->ComputeTangentSpace();
    }

    std::shared_ptr<class SceneObject> sceneObject = std::make_shared<SceneObject>(sphereTemplate);
    sceneObject->Rotate(glm::vec3(SceneObject::GetWorldRight()), PI / 4.f);
    scene->AddSceneObject(sceneObject);


    std::unordered_map<GLenum, std::string> cubeShaderSpec = {
        { GL_VERTEX_SHADER, "cubemap/cubemap.vert" },
        { GL_FRAGMENT_SHADER, "cubemap/cubemap.frag" }
    };
    std::shared_ptr<CubeMapTexture> skyboxTexture = TextureLoader::LoadCubeTexture("GrandCanyon_C_YumaPoint/cubemap/grandcanyone_c05.bmp", 
        "GrandCanyon_C_YumaPoint/cubemap/grandcanyone_c01.bmp", 
        "GrandCanyon_C_YumaPoint/cubemap/grandcanyone_c00.bmp", 
        "GrandCanyon_C_YumaPoint/cubemap/grandcanyone_c02.bmp", 
        "GrandCanyon_C_YumaPoint/cubemap/grandcanyone_c03.bmp", 
        "GrandCanyon_C_YumaPoint/cubemap/grandcanyone_c04.bmp");
    std::shared_ptr<CubeMapShader> cubeShader = std::make_shared<CubeMapShader>(cubeShaderSpec, skyboxTexture);
    std::vector<std::shared_ptr<RenderingObject>> cubeTemplate = MeshLoader::LoadMesh(cubeShader, "cube.obj");
    for (size_t i = 0; i < cubeTemplate.size(); ++i) {
        cubeTemplate[i]->ReverseVertexOrder();
    }

    std::shared_ptr<class SceneObject> cubeObject = std::make_shared<SceneObject>(cubeTemplate);
    cubeObject->SetPosition(glm::vec3(0.f, 0.f, 2.f));
    scene->AddSceneObject(cubeObject);
}

void Assignment4::Tick(double deltaTime)
{
}
