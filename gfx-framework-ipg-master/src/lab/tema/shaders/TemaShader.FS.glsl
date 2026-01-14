#version 330 core

in vec3 v_FragPos;
in vec3 v_Normal;
in vec3 v_Color;
in vec2 v_TexCoord;

out vec4 FragColor;

uniform sampler2D texture_1;
uniform int u_HasTexture = 0; // 1 if texture is used, 0 otherwise

// object color override
uniform int  u_UseObjectColor = 0;
uniform vec3 u_ObjectColor = vec3(1.0, 1.0, 1.0);

// emissive / pulsing
uniform int  u_UseEmission = 0;
uniform vec3 u_EmissionColor = vec3(1.0, 1.0, 1.0);
uniform float u_EmissionIntensity = 0.0;
uniform float u_EmissionSpeed = 1.0;

// lighting
uniform vec3 lightDir = vec3(0.3, -1.0, 0.5);
uniform vec3 lightColor = vec3(1.0, 0.95, 0.9);
uniform vec3 ambientColor = vec3(0.1, 0.12, 0.14);

// faruri tractor
uniform vec3 headlightPos[2];
uniform vec3 headlightDir;
uniform int headlightsOn = 1;

// stalpi
uniform vec3 streetLightPos[20]; 
uniform int streetLightCount;
uniform vec3 streetLightColor = vec3(1.0, 0.8, 0.4);

// mori de vant
uniform vec3 windmillLightPos[20];
uniform int windmillLightCount;
uniform vec3 windmillLightColor;

// camera
uniform vec3 eyePos = vec3(0.0, 2.0, 5.0);

// time
uniform float time = 0.0;

uniform vec3 playerPos;       
uniform float curvatureFactor; 


vec3 ComputePointLight(vec3 lightPos, vec3 col)
{
    float dist = length(lightPos - v_FragPos);
    float att = 1.0 / (0.5 + 0.5 * dist + 1.0 * dist * dist);

    vec3 N = normalize(v_Normal);
    vec3 L = normalize(lightPos - v_FragPos);
    float diff = max(dot(N, L), 0.0);
    
    return diff * col * att * 10.0;
}


vec3 ComputeSpotLight(vec3 lightPos, vec3 dir, vec3 col, float cutOffDeg)
{
    vec3 L = normalize(lightPos - v_FragPos);
    vec3 D = normalize(-dir);
    
    float theta = dot(L, D);

    float cut_off = radians(cutOffDeg);
    float outer_cut_off = cos(cut_off + radians(2.0));
    float inner_cut_off = cos(cut_off);

    if (theta > outer_cut_off)
    {
        float dist = length(lightPos - v_FragPos);
    
        float att = 1.0 / (1.0 + 0.2 * dist + 0.05 * dist * dist);
        
        float intensity = clamp((theta - outer_cut_off) / (inner_cut_off - outer_cut_off), 0.0, 1.0);

        vec3 N = normalize(v_Normal);
        float diff = max(dot(N, L), 0.0);
        
        vec3 V = normalize(eyePos - v_FragPos);
        vec3 H = normalize(L + V);
        float spec = pow(max(dot(N, H), 0.0), 32.0);

        return (diff + spec) * col * att * intensity * 2.5;
    }
    return vec3(0.0);
}


vec3 ApplyCurvature(vec3 position) {
    float dist = distance(position.xz, playerPos.xz);
    position.y -= (dist * dist) * curvatureFactor;
    return position;
}

void main()
{
    vec3 baseColor;
    
    if (u_HasTexture == 1) 
    {

        vec4 texColor = texture(texture_1, v_TexCoord);
        
        if(texColor.a < 0.5) 
            discard; 
            
        baseColor = texColor.rgb;
        FragColor = texColor;
    } 
    else 
    {
        
        baseColor = (u_UseObjectColor == 1) ? u_ObjectColor : v_Color;
    }

    // ambient
    vec3 ambient = ambientColor * baseColor;

    // diffuse
    vec3 N = normalize(v_Normal);
    vec3 L = normalize(-lightDir);
    float diff = max(dot(N, L), 0.0);
    vec3 diffuse = diff * lightColor * baseColor;

    // specular (Blinn-Phong)
    vec3 V = normalize(eyePos - v_FragPos);
    vec3 H = normalize(L + V);
    float spec = pow(max(dot(N, H), 0.0), 32.0);
    vec3 specular = spec * lightColor * 0.6;

    vec3 totalLight = vec3(0.0);

    // faruri
    if (headlightsOn == 1) {

        float intensity = 2;
        
        totalLight += ComputeSpotLight(headlightPos[0], headlightDir, vec3(1.0, 1.0, 0.9) * intensity, 20.0);
        totalLight += ComputeSpotLight(headlightPos[1], headlightDir, vec3(1.0, 1.0, 0.9) * intensity, 20.0);
    }

    // stalpi
    for(int i=0; i<streetLightCount; i++) {
  
        vec3 curvedPos = ApplyCurvature(streetLightPos[i]);
        totalLight += ComputeSpotLight(curvedPos, vec3(0, -1, 0), streetLightColor, 12.0);
    }

    // mori de vant
    for(int i=0; i<windmillLightCount; i++) {
        vec3 curvedPos = ApplyCurvature(windmillLightPos[i]);
        totalLight += ComputePointLight(curvedPos, windmillLightColor);
    }

    // emission pulse
    vec3 emission = vec3(0.0);
    if (u_UseEmission == 1) {
        float pulse = (sin(time * u_EmissionSpeed) * 0.5 + 0.5) * u_EmissionIntensity;
        emission = u_EmissionColor * pulse;
    }

    vec3 color = ambient + diffuse + specular + (totalLight * baseColor) + emission;
    FragColor = vec4(color, 1.0);
}