#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in vec3 a_Color;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;


uniform vec3 playerPos;      
uniform float curvatureFactor; 

out vec3 v_FragPos;
out vec3 v_Normal;
out vec3 v_Color;
out vec2 v_TexCoord;

void main()
{

    vec4 worldPos = Model * vec4(a_Position, 1.0);


    float dist = distance(worldPos.xz, playerPos.xz);
    
    worldPos.y -= (dist * dist) * curvatureFactor;

    v_FragPos = worldPos.xyz;

    mat3 normalMatrix = transpose(inverse(mat3(Model)));
    v_Normal = normalize(normalMatrix * a_Normal);

    v_Color = a_Color;
    v_TexCoord = a_TexCoord;

    gl_Position = Projection * View * worldPos;
}