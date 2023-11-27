#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoordinates;

out vec2 v_TexCoord;

void main()
{
   gl_Position = position;
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
   color = textureColour;
};