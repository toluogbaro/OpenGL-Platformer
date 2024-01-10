#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoordinates;

out vec2 v_TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
   gl_Position = projection * view * model * position;
   v_TexCoord = texCoordinates;
};

#shader fragment
#version 410 core


layout(location = 0) out vec4 color;

vec4 u_AmbientColour = vec4(0.5);
uniform vec4 u_Colour;
uniform sampler2D u_Texture;

void main()
{
   
    float ambientStrength = 0.1;
    vec4 ambient = ambientStrength * u_AmbientColour;
    vec4 result = ambient * u_Colour;

    color = u_Colour;
   
};