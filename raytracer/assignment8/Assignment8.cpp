#include "assignment8/Assignment8.h"
#include "common/core.h"

#define GLASSES true

std::shared_ptr<Camera> Assignment8::CreateCamera() const
{
    const glm::vec2 resolution = GetImageOutputResolution();
    std::shared_ptr<Camera> camera = std::make_shared<PerspectiveCamera>(resolution.x / resolution.y, 32.267f); // 54.43 Angle of view
    
    // glases
    if (GLASSES) {
    	camera->SetPosition(glm::vec3(62.328f, 26.579f, -26.932f));
    	// camera->SetPosition(glm::vec3(62.328f, 22.579f, -26.932f));
	    camera->Rotate(glm::vec3(1.f, 0.f, 0.f), -3.862 * PI / 180.f);  
	    camera->Rotate(glm::vec3(0.f, 1.f, 0.f), 138.2f * PI / 180.f);  
	    camera->Rotate(glm::vec3(0.f, 0.f, 1.f), 0.f * PI / 180.f);  
    } else {
    	// box
	    camera->SetPosition(glm::vec3(0.f, -4.1469f, 0.73693f));
	    camera->Rotate(glm::vec3(1.f, 0.f, 0.f), PI / 2.f);
    }

    return camera;
}

void Assignment8::AddMesh(std::shared_ptr<Scene> scene, std::string filename,
							float reflectivity,
							glm::vec3 rotation, glm::vec3 translation) const 
{
 	// Material
    std::shared_ptr<BlinnPhongMaterial> cubeMaterial = std::make_shared<BlinnPhongMaterial>();
    cubeMaterial->SetReflectivity(reflectivity);

    // Objects
    std::vector<std::shared_ptr<aiMaterial>> loadedMaterials;
    std::vector<std::shared_ptr<MeshObject>> cubeObjects = MeshLoader::LoadMesh(filename, &loadedMaterials);
    for (size_t i = 0; i < cubeObjects.size(); ++i) {
        std::shared_ptr<Material> materialCopy = cubeMaterial->Clone();
        materialCopy->LoadMaterialFromAssimp(loadedMaterials[i]);
        cubeObjects[i]->SetMaterial(materialCopy);
    }

    std::shared_ptr<SceneObject> cubeSceneObject = std::make_shared<SceneObject>();
    cubeSceneObject->AddMeshObject(cubeObjects);
    cubeSceneObject->Rotate(glm::vec3(1.f, 0.f, 0.f), rotation.x);
    cubeSceneObject->Rotate(glm::vec3(0.f, 1.f, 0.f), rotation.y);
    cubeSceneObject->Rotate(glm::vec3(0.f, 0.f, 1.f), rotation.z);
    cubeSceneObject->Translate(translation);
    cubeSceneObject->CreateAccelerationData(AccelerationTypes::BVH);

	cubeSceneObject->ConfigureAccelerationStructure([](AccelerationStructure* genericAccelerator) {
        BVHAcceleration* accelerator = dynamic_cast<BVHAcceleration*>(genericAccelerator);
        accelerator->SetMaximumChildren(2);
        accelerator->SetNodesOnLeaves(2);
    });

    cubeSceneObject->ConfigureChildMeshAccelerationStructure([](AccelerationStructure* genericAccelerator) {
        BVHAcceleration* accelerator = dynamic_cast<BVHAcceleration*>(genericAccelerator);
        accelerator->SetMaximumChildren(2);
        accelerator->SetNodesOnLeaves(2);
    });

    scene->AddSceneObject(cubeSceneObject);
}

void Assignment8::AddLight(std::shared_ptr<Scene> scene, glm::vec3 position, glm::vec3 color) const {
	 // Lights
    std::shared_ptr<PointLight> pointLight = std::make_shared<PointLight>();
    pointLight->SetPosition(position);
    pointLight->SetLightColor(color);
    scene->AddLight(pointLight);
}

void Assignment8::AddAreaLight(std::shared_ptr<Scene> scene, glm::vec3 position, glm::vec3 color,
                               float size, ) const {
     // Lights
    std::shared_ptr<AreaLight> areaLight = std::make_shared<AreaLight>();
    areaLight->SetPosition(position);
    areaLight->SetLightColor(color);
    scene->AddLight(areaLight);
}

void Assignment8::CreateCornellBoxes(std::shared_ptr<Scene> scene) const 
{
	AddMesh(scene, "CornellBox/CornellBox-Assignment8.obj", 0, glm::vec3(PI/2.f, 0, 0));	
	AddLight(scene, glm::vec3(0.01909f, 0.0101f, 1.97028f), glm::vec3(1.f, 1.f, 1.f));
}

void Assignment8::CreateGlasses(std::shared_ptr<Scene> scene) const 
{
	// glasses
	AddMesh(scene, "aviators/ears.obj", 0);
	AddMesh(scene, "aviators/glass.obj", 0.7);
	AddMesh(scene, "aviators/frame.obj", 0.2);
	AddMesh(scene, "aviators/nose.obj", 0);
	// coke
	AddMesh(scene, "coke_can/can.obj", 0.1);
	// surface
	AddMesh(scene, "surface/surface.obj", 0);

	// lights
    AddLight(scene, glm::vec3(-63.386f, 188.667f, -5.751f), glm::vec3(1.f, 1.f, 1.f));
// 	AddAreaLight(scene, );
// }

std::shared_ptr<Scene> Assignment8::CreateScene() const
{
    std::shared_ptr<Scene> newScene = std::make_shared<Scene>();

    if(GLASSES)
    	CreateGlasses(newScene);
    else 
   		CreateCornellBoxes(newScene);

	#define ACCELERATION_TYPE 1
	#if ACCELERATION_TYPE == 0
	    newScene->GenerateAccelerationData(AccelerationTypes::NONE);
	#elif ACCELERATION_TYPE == 1
	    newScene->GenerateAccelerationData(AccelerationTypes::BVH);
	#else
	    UniformGridAcceleration* accelerator = dynamic_cast<UniformGridAcceleration*>(newScene->GenerateAccelerationData(AccelerationTypes::UNIFORM_GRID));
	    assert(accelerator);
	    // Assignment 6 Part 2 TODO: Change the glm::ivec3(10, 10, 10) here.
	    accelerator->SetSuggestedGridSize(glm::ivec3(10, 10, 10));
	#endif    

    return newScene;
}

std::shared_ptr<ColorSampler> Assignment8::CreateSampler() const
{
    std::shared_ptr<JitterColorSampler> jitter = std::make_shared<JitterColorSampler>();
    jitter->SetGridSize(glm::ivec3(6, 6, 1));

   	std::shared_ptr<SimpleAdaptiveSampler> sampler = std::make_shared<SimpleAdaptiveSampler>();
    sampler->SetInternalSampler(jitter);

    // ASSIGNMENT 5 TODO: Change the '1.f' in '1.f * SMALL_EPSILON' here to be higher and see what your results are. (Part 3)
    sampler->SetEarlyExitParameters(10.f * SMALL_EPSILON, 4);

    return sampler;
}

std::shared_ptr<class Renderer> Assignment8::CreateRenderer(std::shared_ptr<Scene> scene, std::shared_ptr<ColorSampler> sampler) const
{
    return std::make_shared<BackwardRenderer>(scene, sampler);
}

int Assignment8::GetSamplesPerPixel() const
{
    return 16; 
}

float Assignment8::GetFocusPlane() const
{
	if (GLASSES)
		return 130.f;
	else
		return 3.5f;
}

float Assignment8::GetAperture() const
{
	return 2.f;
}

bool Assignment8::NotifyNewPixelSample(glm::vec3 inputSampleColor, int sampleIndex)
{
    return true;
}

int Assignment8::GetMaxReflectionBounces() const
{
    return 3;
}

int Assignment8::GetMaxRefractionBounces() const
{
    return 3;
}

glm::vec2 Assignment8::GetImageOutputResolution() const
{
    return glm::vec2(720.f, 576.f);
    // return glm::vec2(640.f, 480.f);
}