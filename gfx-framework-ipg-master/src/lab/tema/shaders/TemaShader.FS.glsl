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

// camera
uniform vec3 eyePos = vec3(0.0, 2.0, 5.0);

// time
uniform float time = 0.0;

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

    // emission pulse
    vec3 emission = vec3(0.0);
    if (u_UseEmission == 1) {
        float pulse = (sin(time * u_EmissionSpeed) * 0.5 + 0.5) * u_EmissionIntensity;
        emission = u_EmissionColor * pulse;
    }

    vec3 color = ambient + diffuse + specular + emission;
    FragColor = vec4(color, 1.0);
}