#version 330

in vec2 fragmentUV;
in vec4 vertexWorldPosition;
in vec3 vertexWorldNormal;

out vec4 finalColor;

uniform InputMaterial {
    float matMetallic;
    float matRoughness;
    vec4 matSpecular;
    bool matLight;
    vec4 matDiffuse;
} material;

struct EpicLightProperties {
    float light_radius;
    vec4 light_color;
    vec4 point_position;

    vec4 sky_color;
    vec4 ground_color;
    
    vec4 forward_direction;
};

uniform EpicLightProperties pointLight;
uniform EpicLightProperties directionalLight;
uniform EpicLightProperties hemisphereLight;

uniform vec4 cameraPosition;

uniform float constantAttenuation;
uniform float linearAttenuation;
uniform float quadraticAttenuation;

uniform sampler2D diffuseTexture;

uniform int lightingType;

const float PI = 3.1415926;
const int GLOBAL = 0;
const int POINT = 1;
const int DIRECTIONAL = 2;
const int HEMISPHERE = 3;

vec4 calcBRDF(vec4 worldPosition, vec3 worldNormal, vec4 L, vec3 light_color, bool is_affected_by_light) {
     // Normal to the surface
    vec4 N = vec4(normalize(worldNormal), 0.f);

    vec3 color = material.matDiffuse.xyz != vec3(0) ? material.matDiffuse.xyz : texture(diffuseTexture, fragmentUV).xyz;

    // Direction from the surface to the camera
    vec4 V = normalize(cameraPosition - worldPosition);
    // Direction of maximum highlights (see paper!)
    vec4 H = normalize(L + V);

    // diffuse reflection
    vec3 d = (1 - material.matMetallic) * color / PI;

    // specular
    vec3 s = vec3(0.f); 
    if (lightingType != HEMISPHERE) { 
        // D
        float alphaSquared = pow(material.matRoughness, 4);
        float D_denom = clamp(dot(N, H), 0,1) * clamp(dot(N, H), 0,1) * (alphaSquared - 1) + 1;
        float D = alphaSquared / (PI * D_denom * D_denom);

        // F
        vec3 c_spec = mix(0.08 * material.matSpecular.xyz, color, material.matMetallic);
        // clamping this line turns everything dark
        vec3 F = c_spec + (1 - c_spec) * pow(2, (-5.55473 * dot(V, H) - 6.98316)*dot(V,H)); 

        // G
        float k = (material.matRoughness + 1) * (material.matRoughness + 1) / 8.0;
        float G_L = clamp(dot(N, L), 0,1) / (clamp(dot(N, L), 0,1) * (1 - k) + k);
        float G_V = clamp(dot(N, V), 0,1) / (clamp(dot(N, V), 0,1) * (1 - k) + k);
        float G = G_L * G_V;

        // specular
        // clamping this line turns everything dark
        s = (D * F * G) / (4.0 * dot(N, L) * dot(N, V));
    }
    if (is_affected_by_light)
        return vec4(light_color * clamp(dot(N, L), 0, 1) * (d + s), 1.f);
    else 
        return vec4(light_color * clamp(dot(N, L), 0, 1), 1.f);
}

vec4 hemisphereLightSubroutine(vec4 worldPosition, vec3 worldNormal, bool is_affected_by_light)
{
    // Normal to the surface
    vec3 N = normalize(worldNormal);

    float interp_val = clamp(dot(N, vec3(0.f, 1.f, 0.f))*0.5 + 0.5f, 0.f, 1.f);
    vec3 light_color = mix(hemisphereLight.ground_color.xyz, hemisphereLight.sky_color.xyz, interp_val); 

    vec4 L = vec4(N, 0.f);

    return calcBRDF(worldPosition, worldNormal, L, light_color, is_affected_by_light);
}

vec4 directionalLightSubroutine(vec4 worldPosition, vec3 worldNormal, bool is_affected_by_light)
{    
    vec4 L = -directionalLight.forward_direction;
    
    return calcBRDF(worldPosition, worldNormal, L, directionalLight.light_color.xyz, is_affected_by_light);
}

vec4 pointLightSubroutine(vec4 worldPosition, vec3 worldNormal, bool is_affected_by_light)
{    
    // Direction from the surface to the light
    vec4 L = normalize(pointLight.point_position - worldPosition);

    return calcBRDF(worldPosition, worldNormal, L, pointLight.light_color.xyz, is_affected_by_light);
}

vec4 globalLightSubroutine(vec4 worldPosition, vec3 worldNormal)
{
    return vec4(0);
}

float AttenuateLight(vec4 worldPosition)
{
    float lightDistance = length(pointLight.point_position - worldPosition);
    float numerator = clamp(1 - pow(lightDistance / pointLight.light_radius, 4), 0, 1);
    float denom = 1.0 / (constantAttenuation + lightDistance * linearAttenuation + lightDistance * lightDistance * quadraticAttenuation);
    return numerator * numerator * denom;
}

void main()
{
    vec4 lightingColor = vec4(0);
    if (!material.matLight) {
        lightingColor = pointLightSubroutine(vertexWorldPosition, vertexWorldNormal, false);
    } else if (lightingType == GLOBAL) {
        finalColor = globalLightSubroutine(vertexWorldPosition, vertexWorldNormal);
    } else if (lightingType == POINT) {
        lightingColor = pointLightSubroutine(vertexWorldPosition, vertexWorldNormal, true);
        finalColor = AttenuateLight(vertexWorldPosition) * lightingColor;
    } else if (lightingType == DIRECTIONAL) {
        finalColor = directionalLightSubroutine(vertexWorldPosition, vertexWorldNormal, true);
    } else if (lightingType == HEMISPHERE) {
        finalColor = hemisphereLightSubroutine(vertexWorldPosition, vertexWorldNormal, true);
    }
}
