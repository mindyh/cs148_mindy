#include "assignment8/Assignment8.h"
#include "common/core.h"
#include "common/Utility/Texture/TextureLoader.h"

#include <stdio.h>
#include <stdlib.h>

#define GLASSES true

std::shared_ptr<Camera> Assignment8::CreateCamera() const
{
    const glm::vec2 resolution = GetImageOutputResolution();
    std::shared_ptr<Camera> camera = std::make_shared<PerspectiveCamera>(resolution.x / resolution.y, 32.267f); // 54.43 Angle of view
    
    // glases
    if (GLASSES) {
    	camera->SetPosition(glm::vec3(64.549f, 11.266f, -26.25f));
    	// camera->SetPosition(glm::vec3(62.328f, 22.579f, -26.932f));
	    camera->Rotate(glm::vec3(1.f, 0.f, 0.f), 5.662f * PI / 180.f);  
        camera->Rotate(glm::vec3(0.f, 1.f, 0.f), 137.4f * PI / 180.f);  
        camera->Rotate(glm::vec3(0.f, 0.f, 1.f), 0 * PI / 180.f);  
        // camera->Rotate(glm::vec3(1.f, 0.f, 0.f), -3.862 * PI / 180.f);  
	    // camera->Rotate(glm::vec3(0.f, 1.f, 0.f), 138.2f * PI / 180.f);  
	    // camera->Rotate(glm::vec3(0.f, 0.f, 1.f), 0.f * PI / 180.f);  
    
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

void Assignment8::AddSkyPlane(std::shared_ptr<Scene> scene, std::string mesh_filename,
                              std::string texture_filename,
                              glm::vec3 rotation, glm::vec3 translation) const {
    // Material
    std::shared_ptr<BlinnPhongMaterial> material = std::make_shared<BlinnPhongMaterial>();
    material->SetReflectivity(0);
    material->SetTransmittance(0);
    material->SetAmbient(glm::vec3(0));

    // Objects
    std::vector<std::shared_ptr<aiMaterial>> loadedMaterials;
    std::vector<std::shared_ptr<MeshObject>> meshObjects = MeshLoader::LoadMesh(mesh_filename, &loadedMaterials);
    material->SetTexture(mesh_filename, TextureLoader::LoadTexture(texture_filename));
    meshObjects[0]->SetMaterial(material);

    std::shared_ptr<SceneObject> sceneObject = std::make_shared<SceneObject>();
    sceneObject->AddMeshObject(meshObjects);
    sceneObject->Rotate(glm::vec3(1.f, 0.f, 0.f), rotation.x);
    sceneObject->Rotate(glm::vec3(0.f, 1.f, 0.f), rotation.y);
    sceneObject->Rotate(glm::vec3(0.f, 0.f, 1.f), rotation.z);
    sceneObject->Translate(translation);
    sceneObject->CreateAccelerationData(AccelerationTypes::BVH);

    sceneObject->ConfigureAccelerationStructure([](AccelerationStructure* genericAccelerator) {
        BVHAcceleration* accelerator = dynamic_cast<BVHAcceleration*>(genericAccelerator);
        accelerator->SetMaximumChildren(2);
        accelerator->SetNodesOnLeaves(2);
    });

    sceneObject->ConfigureChildMeshAccelerationStructure([](AccelerationStructure* genericAccelerator) {
        BVHAcceleration* accelerator = dynamic_cast<BVHAcceleration*>(genericAccelerator);
        accelerator->SetMaximumChildren(2);
        accelerator->SetNodesOnLeaves(2);
    });

    scene->AddSceneObject(sceneObject);
}

void Assignment8::AddLight(std::shared_ptr<Scene> scene, glm::vec3 position, glm::vec3 color) const {
	 // Lights
    std::shared_ptr<PointLight> pointLight = std::make_shared<PointLight>();
    pointLight->SetPosition(position);
    pointLight->SetLightColor(color);
    scene->AddLight(pointLight);
}

void Assignment8::AddAreaLight(std::shared_ptr<Scene> scene, glm::vec3 position, glm::vec3 rotation,
                            glm::vec3 color, glm::vec2 size) const {
     // Lights
    std::shared_ptr<AreaLight> areaLight = std::make_shared<AreaLight>(size);
    areaLight->SetPosition(position);
    areaLight->SetLightColor(color);

    areaLight->Rotate(glm::vec3(1.f, 0.f, 0.f), rotation.x * PI / 180.f);  
    areaLight->Rotate(glm::vec3(0.f, 1.f, 0.f), rotation.y * PI / 180.f);  
    areaLight->Rotate(glm::vec3(0.f, 0.f, 1.f), rotation.z * PI / 180.f);  

    areaLight->SetSamplerAttributes(glm::vec3(6.f, 6.f, 6.f), 8);

    scene->AddLight(areaLight);
}

void Assignment8::CreateCornellBoxes(std::shared_ptr<Scene> scene) const 
{
	AddMesh(scene, "CornellBox/CornellBox-Assignment8.obj", 0, glm::vec3(PI/2.f, 0, 0));	
	AddLight(scene, glm::vec3(0.01909f, 0.0101f, 1.97028f), glm::vec3(1.f, 1.f, 1.f));
}

void Assignment8::CreateGlasses(std::shared_ptr<Scene> scene) const 
{
    // skies
    AddSkyPlane(scene, "sky/pier.obj", "sky/6428-SunsetBeach.jpg");
    AddSkyPlane(scene, "sky/beach.obj", "sky/beautiful-sunset-wallpaper.jpg");

	// glasses
	AddMesh(scene, "aviators/ears.obj", 0);
	AddMesh(scene, "aviators/glass.obj", 0.7);
	AddMesh(scene, "aviators/frame.obj", 0.3);
	AddMesh(scene, "aviators/nose.obj", 0);
	// coke
	// AddMesh(scene, "coke_can/can.obj", 0.1);
	// surface
	AddMesh(scene, "table/table.obj", 0);

	// lights
    AddLight(scene, glm::vec3(-307.42f, 297.926f, -1756.349f), glm::vec3(1.f, 1.f, 0.9f));
    AddAreaLight(scene, glm::vec3(-193.893f, 379.69f, -319.701), glm::vec3(171.459f, -23.202f, -179.324f),
 	// // AddAreaLight(scene, glm::vec3(-193.893f, 379.69f, -319.701), glm::vec3(-90, 0, 0),
        glm::vec3(1.f, 1.f, 0.9), glm::vec2(7289.348f, 3182.129f));
}

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
	    accelerator->SetSuggestedGridSize(glm::ivec3(6, 6, 6));
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
    return 4; 
}

float Assignment8::GetFocusPlane() const
{
	if (GLASSES)
		return 126.f;
	else
		return 3.5f;
}

float Assignment8::GetAperture() const
{
	return 2.5f;
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
    // return glm::vec2(720.f, 576.f);
    // return glm::vec2(640.f, 480.f);
    return glm::vec2(950.f, 540.f);
}