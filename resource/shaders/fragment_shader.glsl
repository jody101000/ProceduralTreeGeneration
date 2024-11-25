#version 330 core
out vec4 FragColor;

struct Light {
    vec3 position;
    vec3 color;
};
#define MAX_LIGHTS 4
uniform Light lights[MAX_LIGHTS];
uniform int numLights;

in vec3 Normal;
in vec3 FragPos;
uniform vec3 objectColor;
uniform float ambientStrength  = 0.3f;

void main() {
    vec3 norm = normalize(Normal);
    vec3 result = ambientStrength * objectColor;
    
    for(int i = 0; i < numLights; i++) {
        vec3 lightDir = normalize(lights[i].position - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lights[i].color;
        result += diffuse * objectColor;
    }
    
    FragColor = vec4(result, 1.0);
}