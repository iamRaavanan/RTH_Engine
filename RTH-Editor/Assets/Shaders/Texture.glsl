#type Vertex
#version 460 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in float texIndex;
layout(location = 4) in float tileFactor;

uniform mat4 u_ViewProj;
out vec2 vTexCoord;
out vec4 vColor;
out float vTexIndex;
out float vtileFactor;
void main ()
{
	vTexCoord = texCoord;
	vColor = color;
	vTexIndex = texIndex;
	vtileFactor = tileFactor;
	gl_Position = u_ViewProj * vec4(pos, 1.0);
}

#type Fragment
#version 460 core
layout(location  = 0) out vec4 color;
in vec2 vTexCoord;
in vec4 vColor;
in float vTexIndex;
in float vtileFactor;
uniform sampler2D u_Tex[32];
void main ()
{
	color = texture(u_Tex[int(vTexIndex)], vTexCoord * vtileFactor) * vColor;
}