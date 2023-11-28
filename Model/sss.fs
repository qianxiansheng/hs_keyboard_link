#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  

uniform vec3 lightPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 viewerPos;

uniform float _Power;
uniform float _Scale;
uniform float _Distortion;

void main()
{
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
            
    vec3 result = (ambient + diffuse) * objectColor;

    // sss
    vec3 L = lightPos;
    vec3 V = normalize(viewerPos - FragPos);
    vec3 N = norm;

    vec3 H = normalize(L + N * _Distortion);
    float I = pow(clamp(dot(V, -H), 0.0, 1.0), _Power) * _Scale;
    
    vec3 final = result + (lightColor * I);

    FragColor = vec4(final, 1.0);
} 