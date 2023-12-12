#shader vertex
#version 330 core

layout(location = 0) in vec4 position;

out vec3 texCoord;

uniform mat4 skyboxView;
uniform mat4 skyboxProjection;

void main()
{
	vec4 pos = skyboxView * skyboxProjection * position;
	gl_Position = vec4(pos.x, pos.y, pos.w, pos.w);
	texCoord = vec3(pos.x, pos.y, -pos.z);
};

#shader fragment
#version 410 core

layout(location = 0) out vec4 color;

in vec3 texCoord;

uniform samplerCube u_Texture;

void main()
{
	vec4 textureColour = texture(u_Texture, texCoord);
	color = textureColour;

};