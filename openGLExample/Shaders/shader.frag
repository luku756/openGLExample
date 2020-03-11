#version 400

uniform sampler2D u_base_texture;

in vec2 v_tex_coord;

layout (location = 0) out vec4 final_color;

void main(void) {
	final_color = texture(u_base_texture, v_tex_coord);
	//final_color = vec4(1,1,0,1);
}
