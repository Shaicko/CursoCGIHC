#version 330 core
struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light
{
    vec3 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 color;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;
uniform Light newlight;

uniform sampler2D texture_diffuse;

void main()
{
    // Ambient
    vec3 ambient = light.ambient *material.diffuse;
    vec3 newAmbient = newlight.ambient * material.diffuse;
    
    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    vec3 newLightDir = normalize(newlight.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    float newDiff = max(dot(norm, newLightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    vec3 newDiffuse = newlight.diffuse * newDiff * material.diffuse;
    
    // Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);//Coeficiente de reflexión
    vec3 newReflectDir = reflect(-newLightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    float newSpec = pow(max(dot(viewDir, newReflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);
    vec3 newSpecular = newlight.specular * (newSpec * material.specular);
    
    //Combinado
    vec3 result = ambient + diffuse + specular + newAmbient + newDiffuse + newSpecular;    
    color = vec4(result, 1.0f)*texture(texture_diffuse,TexCoords);
}