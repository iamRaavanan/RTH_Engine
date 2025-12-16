#type Vertex
#version 460 core
layout(location = 0) in vec3 pos;

uniform mat4 u_ViewProj;
uniform mat4 u_Transform;
void main ()
{
	gl_Position = u_ViewProj * u_Transform * vec4(pos, 1.0);
}

#type Fragment
#version 460 core
layout(location  = 0) out vec4 color;
uniform vec4 u_Color;
void main ()
{
	color = u_Color;
}