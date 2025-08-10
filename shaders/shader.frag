#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 wPos;

layout(location = 0) out vec4 outColor;
layout(binding = 1) uniform sampler2D texSampler;

layout(binding = 2) uniform sampler2D hatch[4];

const float hatchSize = 20.0f;

vec4 getMIPDebug() {
    vec2 lodInfo = textureQueryLod(hatch[0], texCoord*hatchSize);
    float lod = lodInfo.x; // actual LOD chosen

    // Map LOD to a color for visualization
    // e.g., mip 0 = red, mip 1 = green, mip 2 = blue, etc.
    if (lod < 0.5)
        return vec4(1.0, 0.0, 0.0, 1.0); // mip 0
    else if (lod < 1.5)
        return vec4(0.0, 1.0, 0.0, 1.0); // mip 1
    else if (lod < 2.5)
        return vec4(0.0, 0.0, 1.0, 1.0); // mip 2
    else if (lod < 3.5)
        return vec4(1.0, 1.0, 0.0, 1.0); // mip 3
    else
        return vec4(1.0, 0.0, 1.0, 1.0); // mip 4+
}

void main() {
    vec3 lightColor = vec3(1.0f);
    vec3 ambLightColor = vec3(0.0f);
    float diffCoeff = 1.0;

    vec3 lightPos = vec3(50.0f, 40.0f, 20.0f);

    vec3 lightDir = normalize(lightPos-wPos);

    vec3 norm = normalize(normal);

    vec3 diff = max(dot(norm, lightDir), 0.0f) * diffCoeff * lightColor;

    vec3 amb = diffCoeff * ambLightColor;

    vec3 color = clamp(vec3(0.0f), vec3(1.0f), texture(texSampler, vec2(texCoord.x, texCoord.y)).rgb * 3.0f);
    // color = vec3(0.5f, 0.2f, 0.1f);

    vec3 lightingCol = color;
    float lightingValue = clamp(0.0f, 1.0f, (lightingCol.r + lightingCol.g + lightingCol.b)/3.0f);

    vec3 hatchCol = texture(hatch[0], texCoord*hatchSize).rgb;

    vec3 hatchColors[6];
    //first and last colors are white and black respectively
    hatchColors[0] = vec3(1.0f);
    hatchColors[5] = vec3(0.0f);
    for (int i = 1; i < 5; i++) {
        hatchColors[i] = texture(hatch[i-1], texCoord*hatchSize).rgb;
    }

    float hatchVal = pow(1.0f-clamp(lightingValue, 0.0f, 1.0f), 1.3);

    float level = fract(hatchVal * 5.0f);
    int index = int(floor(hatchVal * 5.0f));
    vec3 mixValue = vec3(0.0f);
        
    if (index == 5 || index == 0) { // it cannot be half black or half white
        if (level > 0.5) {
            mixValue = hatchColors[index+1];
        }else {
            mixValue = hatchColors[index];
        }
    }else {
        mixValue = mix(hatchColors[index], hatchColors[index+1], level);
    }
    //vec2 lodInfo = textureQueryLod(hatch[0], texCoord);
    //outColor = vec4(textureQueryLod, 1.0f);
    outColor = vec4(mixValue, 1.0f);
}