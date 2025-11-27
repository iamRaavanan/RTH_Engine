#type Vertex
#version 460 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 texCoord;

uniform mat4 u_ViewProj;
uniform mat4 u_Transform;
out vec2 vTexCoord;
void main ()
{
	vTexCoord = texCoord;
	gl_Position = u_ViewProj * u_Transform * vec4(pos, 1.0);
}

#type Fragment
#version 460 core
layout(location  = 0) out vec4 color;
uniform vec4 u_Color;
in vec2 vTexCoord;
uniform sampler2D u_Tex;
void main ()
{
	color = texture(u_Tex, vTexCoord * 10.0) * u_Color;
}