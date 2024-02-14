#version 430
layout (binding=0) uniform sampler2D samp;
in vec4 varyingColor;
out vec4 color;
uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
void main(void)
{  
	color = varyingColor;
}