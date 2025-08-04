#version 450

layout(location = 0) out vec4 outColor;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 wPos;

layout(binding = 1) uniform sampler2D texSampler;


void main() {

    vec3 lightColor = vec3(1.0f);
    vec3 ambLightColor = vec3(0.0f);
    float diffCoeff = 1.0;

    vec3 lightPos = vec3(-20.0f, 20.0f, 20.0f);

    vec3 lightDir = normalize(lightPos-wPos);

    vec3 norm = normalize(normal);

    vec3 diff = max(dot(norm, lightDir), 0.0f) * diffCoeff * lightColor;

    vec3 amb = diffCoeff * ambLightColor;

    vec3 color = texture(texSampler, vec2(texCoord.x, texCoord.y)).rgb;
    outColor = vec4((diff+amb)*color, 1.0f);
}