#version 330

in vec4 vertexWorldPosition;
in vec3 vertexWorldNormal;
in vec2 fragmentUV;

out vec4 finalColor;

uniform InputMaterial {
    vec4 matDiffuse;
    vec4 matSpecular;
    float matShininess;
    vec4 matAmbient;
    bool matLight;
} material;

struct LightProperties {
    vec4 diffuseColor;
    vec4 specularColor;
    float light_radius;
};
uniform LightProperties genericLight;

struct PointLight {
    vec4 pointPosition;
};
uniform PointLight pointLight;

uniform vec4 cameraPosition;

uniform float constantAttenuation;
uniform float linearAttenuation;
uniform float quadraticAttenuation;

uniform int lightingType;

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;

const float PI = 3.1415926;
const int GLOBAL = 0;
const int POINT = 1;
const int DIRECTIONAL = 2;
const int HEMISPHERE = 3;

vec4 pointLightSubroutine(vec4 worldPosition, vec3 worldNormal, bool is_affected_by_light)
{
    // Normal to the surface
    vec4 N = vec4(normalize(worldNormal), 0.f);
    
    // Direction from the surface to the light
    vec4 L = normalize(pointLight.pointPosition - worldPosition);

    // Direction from the surface to the eye
    vec4 E = normalize(cameraPosition - worldPosition);

    // Direction of maximum highlights (see paper!)
    vec4 H = normalize(L + E);

    // Amount of diffuse reflection
    float d = clamp(dot(N, L), 0, 1);
    vec4 diffuseColor = d * genericLight.diffuseColor * material.matDiffuse * texture(diffuseTexture, fragmentUV);
    
    // Amount of specular reflection
    float s = pow(clamp(dot(N, H), 0,1), material.matShininess);
    vec4 specularColor = s * genericLight.specularColor * material.matSpecular * texture(specularTexture, fragmentUV);

    if (is_affected_by_light)
        return diffuseColor + specularColor;  
    else 
        return texture(diffuseTexture, fragmentUV);
}

vec4 directionalLightSubroutine(vec4 worldPosition, vec3 worldNormal)
{
    return vec4(0);
}

vec4 hemisphereLightSubroutine(vec4 worldPosition, vec3 worldNormal)
{
    return vec4(0);
}

vec4 globalLightSubroutine(vec4 worldPosition, vec3 worldNormal)
{
    return vec4(0);
}

float AttenuateLight(vec4 worldPosition)
{
    float lightDistance = length(pointLight.pointPosition - worldPosition);
    float numerator = clamp(1 - pow(lightDistance / genericLight.light_radius, 4), 0, 1);
    float denom = 1.0 / (constantAttenuation + lightDistance * linearAttenuation + lightDistance * lightDistance * quadraticAttenuation);
    return numerator * numerator * denom;
}
void main()
{
    vec4 lightingColor = vec4(0);

    if (!material.matLight) {
        finalColor = pointLightSubroutine(vertexWorldPosition, vertexWorldNormal, false);
    } else if (lightingType == GLOBAL) {
        finalColor = globalLightSubroutine(vertexWorldPosition, vertexWorldNormal);
    } else if (lightingType == POINT) {
        lightingColor = pointLightSubroutine(vertexWorldPosition, vertexWorldNormal, true);
        finalColor = AttenuateLight(vertexWorldPosition) * lightingColor;
    } else if (lightingType == DIRECTIONAL) {
        finalColor = directionalLightSubroutine(vertexWorldPosition, vertexWorldNormal);
    } else if (lightingType == HEMISPHERE) {
        finalColor = hemisphereLightSubroutine(vertexWorldPosition, vertexWorldNormal);
    }
}
