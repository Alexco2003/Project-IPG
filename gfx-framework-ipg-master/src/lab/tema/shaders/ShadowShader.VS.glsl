#version 330 core

layout(location = 0) in vec3 a_Position;

uniform mat4 Model;
uniform mat4 LightSpaceMatrix;

uniform vec3 playerPos;       
uniform float curvatureFactor; 

void main()
{
    vec4 worldPos = Model * vec4(a_Position, 1.0);

    //float dist = distance(worldPos.xz, playerPos.xz);
    //worldPos.y -= (dist * dist) * curvatureFactor;

    gl_Position = LightSpaceMatrix * worldPos;
}