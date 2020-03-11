#version 400

uniform mat4 u_ModelViewProjectionMatrix;

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_tex_coord;

out vec2 v_tex_coord;

void main(void) {	
	v_tex_coord = a_tex_coord;
	gl_Position = u_ModelViewProjectionMatrix * vec4(a_position, 1.0f);
}