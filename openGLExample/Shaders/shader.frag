#version 420

uniform sampler2D u_base_texture;

in vec2 v_tex_coord;

layout (location = 0) out vec4 final_color;

void main(void) {
	final_color = texture(u_base_texture, v_tex_coord);
	final_color = texture(u_base_texture, vec2(v_tex_coord.x, 1-v_tex_coord.y));
}
