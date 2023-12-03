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

in vec2 v_TexCoord;

uniform vec4 u_Colour;
uniform sampler2D u_Texture;

void main()
{
   vec4 textureColour = texture(u_Texture, v_TexCoord);
   color = u_Colour;
   
};