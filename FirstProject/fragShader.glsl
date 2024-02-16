#version 430
in vec2 tc; // interpolated incoming texture coordinate
layout (binding=0) uniform sampler2D samp;

out vec4 color;
uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
void main(void)
{  
	color = texture(samp, tc);
}