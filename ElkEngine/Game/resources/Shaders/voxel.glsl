#shader vertex
#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

out vec3 v_pos;

void main()
{
	/*
	v_pos = vec3(u_model * vec4(position, 1.0));
	v_normal = normalize(transpose(inverse(mat3(u_model))) * normal);
	v_texCoord = texCoord;
	*/

	v_pos = position;
}

#shader geometry
#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 24) out;

uniform mat4 u_model;
uniform mat4 u_vp;

const vec3 cubeVertex[14] = vec3[14](
    vec3(-1.f, 1.f, 1.f),     // Front-top-left
    vec3(1.f, 1.f, 1.f),      // Front-top-right
    vec3(-1.f, -1.f, 1.f),    // Front-bottom-left
    vec3(1.f, -1.f, 1.f),     // Front-bottom-right
    vec3(1.f, -1.f, -1.f),    // Back-bottom-right
    vec3(1.f, 1.f, 1.f),      // Front-top-right
    vec3(1.f, 1.f, -1.f),     // Back-top-right
    vec3(-1.f, 1.f, 1.f),     // Front-top-left
    vec3(-1.f, 1.f, -1.f),    // Back-top-left
    vec3(-1.f, -1.f, 1.f),    // Front-bottom-left
    vec3(-1.f, -1.f, -1.f),   // Back-bottom-left
    vec3(1.f, -1.f, -1.f),    // Back-bottom-right
    vec3(-1.f, 1.f, -1.f),    // Back-top-left
    vec3(1.f, 1.f, -1.f));      // Back-top-right

void main() 
{
	mat4 mvp = u_vp * u_model;
	vec4 o = gl_in[0].gl_Position.xyzw; 

	for (int i = 0; i < 14; ++i)
	{
		gl_Position = mvp * (vec4(o.x, o.y, o.z, 1) + vec4(cubeVertex[i], 1)); 
		EmitVertex();
	}
}

#shader fragment
#version 330 core

out vec4 FragColor;

void main()
{
    // Final color
    FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
