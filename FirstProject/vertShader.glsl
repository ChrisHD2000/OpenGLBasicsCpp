#version 430
layout (location=1) in vec3 pos; // A vertex attribute to receive the cube vertices 
uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

out vec4 varyingColor;

void main(void)
{	
	gl_Position = proj_matrix * mv_matrix * vec4(pos,1.0);
	varyingColor = vec4(pos,1.0) * 0.5 + vec4(0.5, 0.5, 0.5, 0.5);
}