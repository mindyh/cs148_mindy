#include "assignment5/Assignment5.h"
#include "common/core.h"

std::shared_ptr<Camera> Assignment5::CreateCamera() const
{
    const glm::vec2 resolution = GetImageOutputResolution();
    std::shared_ptr<Camera> camera = std::make_shared<PerspectiveCamera>(resolution.x / resolution.y, 32.267);
	
    // camera->SetPosition(glm::vec3(0.f, -4.1469f, 0.73693f));
    // camera->Rotate(glm::vec3(1.f, 0.f, 0.f), PI / 2.f);


    /* avengers */
	// PerspectiveCamera* pcamera = static_cast<PerspectiveCamera*>(camera.get());

    // pcamera->SetFOV(32.267f);  // 54.43 Angle of view
    // pcamera->SetZNear(0.1f);
    // pcamera->SetZFar(10000.f);

    camera->SetPosition(glm::vec3(-47.099422, -1.894301, -12.105892));

    camera->Rotate(glm::vec3(1.f, 0.f, 0.f), 22.037f * PI / 180.f);
    camera->Rotate(glm::vec3(0.f, 1.f, 0.f), 256.857f * PI / 180.f);
    camera->Rotate(glm::vec3(0.f, 0.f, 1.f), 0.f * PI / 180.f);

    return camera;
}

void Assignment5::MakeBPMesh(std::string file_name, 
							 std::shared_ptr<Scene> scene, 
							 glm::vec3 pos, glm::vec3 rot, float rot_angle) const
{
    // Material
    std::shared_ptr<BlinnPhongMaterial> material = std::make_shared<BlinnPhongMaterial>();
    material->SetDiffuse(glm::vec3(1.f, 1.f, 1.f));
    material->SetSpecular(glm::vec3(0.6f, 0.6f, 0.6f), 40.f);

    std::vector<std::shared_ptr<aiMaterial>> loadedMaterials;
    std::vector<std::shared_ptr<MeshObject>> meshObjects = MeshLoader::LoadMesh(file_name, &loadedMaterials);
    for (size_t i = 0; i < meshObjects.size(); ++i) {
        std::shared_ptr<Material> materialCopy = material->Clone();
        materialCopy->LoadMaterialFromAssimp(loadedMaterials[i]);
        meshObjects[i]->SetMaterial(materialCopy);
    }

    std::shared_ptr<class SceneObject> sceneObject = std::make_shared<SceneObject>();
    sceneObject->AddMeshObject(meshObjects);
    sceneObject->Rotate(rot, rot_angle);
    sceneObject->Translate(pos);
    sceneObject->CreateAccelerationData(AccelerationTypes::NONE);
    scene->AddSceneObject(sceneObject);
}

void Assignment5::MakePointLight(glm::vec3 position, glm::vec3 color, 
								 std::shared_ptr<Scene> scene) const
{
    // Lights
    std::shared_ptr<Light> pointLight = std::make_shared<PointLight>();
    pointLight->SetPosition(position);
    pointLight->SetLightColor(color);
    scene->AddLight(pointLight);
}

void Assignment5::CreateCubes(std::shared_ptr<Scene> scene) const {
	MakeBPMesh("CornellBox/CornellBox-Original.obj", scene, glm::vec3(0), glm::vec3(1.f, 0.f, 0.f), PI / 2.f);
    MakePointLight(glm::vec3(0.01909f, 0.0101f, 1.97028f), glm::vec3(1.f, 1.f, 1.f), scene);
}


void Assignment5::CreateAvengers(std::shared_ptr<Scene> scene) const {
    // r, m, spec
    // MakeBPMesh("BlackWidow/black_widow.obj", scene);
    // MakeBPMesh("gun/gun.obj", scene);
    // MakeBPMesh("gun/gun2.obj", scene);

    // // Iron Man
    // MakeBPMesh("IronMan/IronMan.obj", scene);
    // MakeBPMesh("IronMan/leftHandBeam_ironman.obj", scene);

    // hawkeye
    // MakeBPMesh("Hawkeye/hawkeye.obj", scene, glm::vec3(5.f,2.f, 5.f));

    // // Thor
    // MakeBPMesh("Thor/thor.obj", scene);
    // MakeBPMesh("Thor/thor_hammer.obj", scene);
    // MakeBPMesh("Thor/thor_cape.obj", scene);
    // // Hulk
    // MakeBPMesh("Hulk/hulk.obj", scene, glm::vec3(0.f,-10.f, 0.f));
    // // Captain
    // MakeBPMesh("CaptainAmerica/Captain_America.obj", scene);
    // MakeBPMesh("shield/shield.obj", scene);
    // // Buildings and bg
    MakeBPMesh("buildings/destroyed_building_left.obj", scene);
    // MakeBPMesh("City/city.obj", scene);
    // MakeBPMesh("explosion/explosion1.obj", scene);
    // MakeBPMesh("explosion/explosion2.obj", scene);
    // MakeBPMesh("explosion/explosion3.obj", scene);
    // MakeBPMesh("sky/sky.obj", scene);

    glm::vec3 color(1.f, 1.f, 1.f);
    // explosion  right
    // MakePointLight(glm::vec3(1058.218f, 670.524f, 820.973f), color, scene);
    // explosion left
    // MakePointLight(glm::vec3(726.656f, 190.738f, -137.344f), color, scene);
  
    // // front left
    // color = glm::vec3(0.5,0.5,0.5);
    // MakePointLight(glm::vec3(-39.220, 50.757, -61.093), color, scene);

    // center
    // color = glm::vec3(0.3,0.3,0.3);
    // MakePointLight(glm::vec3(-51.106, 51.156, 25.089), color, scene);

    // // front right
    // color = glm::vec3(0.3,0.3,0.3);
    // MakePointLight(glm::vec3(-51.106, 51.156, 57.406), color, scene);

    // // iron man hand
    // color = glm::vec3(1,1,1);
    // MakePointLight(glm::vec3(-15.501f, 18.457f, 4.407f), color, scene);
}

std::shared_ptr<Scene> Assignment5::CreateScene() const
{
    std::shared_ptr<Scene> scene = std::make_shared<Scene>();
    
    CreateAvengers(scene);
    // CreateCubes(scene);

    return scene;
}

std::shared_ptr<ColorSampler> Assignment5::CreateSampler() const
{
    std::shared_ptr<JitterColorSampler> jitter = std::make_shared<JitterColorSampler>();
    // ASSIGNMENT 5 TODO: Change the grid size to be glm::ivec3(X, Y, 1).
    jitter->SetGridSize(glm::ivec3(1, 1, 1));

    std::shared_ptr<SimpleAdaptiveSampler> sampler = std::make_shared<SimpleAdaptiveSampler>();
    sampler->SetInternalSampler(jitter);

    // ASSIGNMENT 5 TODO: Change the '1.f' in '1.f * SMALL_EPSILON' here to be higher and see what your results are. (Part 3)
    sampler->SetEarlyExitParameters(SMALL_EPSILON, 4);

    // ASSIGNMENT 5 TODO: Comment out 'return jitter;' to use the adaptive sampler. (Part 2)
    return jitter;
    return sampler;
}

std::shared_ptr<class Renderer> Assignment5::CreateRenderer(std::shared_ptr<Scene> scene, std::shared_ptr<ColorSampler> sampler) const
{
    return std::make_shared<ForwardRenderer>(scene, sampler);
}

int Assignment5::GetSamplesPerPixel() const
{
    // ASSIGNMENT 5 TODO: Change the '1' here to increase the maximum number of samples used per pixel. (Part 1).
    return 1; 
}

bool Assignment5::NotifyNewPixelSample(glm::vec3 inputSampleColor, int sampleIndex)
{
    return true;
}

int Assignment5::GetMaxReflectionBounces() const
{
    return 0;
}

int Assignment5::GetMaxRefractionBounces() const
{
    return 0;
}

glm::vec2 Assignment5::GetImageOutputResolution() const
{
    return glm::vec2(1280.f, 720.f);
}