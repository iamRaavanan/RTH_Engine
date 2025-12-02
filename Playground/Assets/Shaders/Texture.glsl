#type Vertex
#version 460 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 texCoord;

uniform mat4 u_ViewProj;
out vec2 vTexCoord;
out vec4 vColor;
void main ()
{
	vTexCoord = texCoord;
	vColor = color;
	gl_Position = u_ViewProj * vec4(pos, 1.0);
}

#type Fragment
#version 460 core
layout(location  = 0) out vec4 color;
uniform vec4 u_Color;
in vec2 vTexCoord;
in vec4 vColor;
uniform sampler2D u_Tex;
void main ()
{
	//color = texture(u_Tex, vTexCoord * 10.0) * u_Color;
	color = vColor;
}