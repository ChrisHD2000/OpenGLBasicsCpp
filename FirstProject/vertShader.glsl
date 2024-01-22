#version 430
uniform float angle;

mat4 buildRotateZ(float rad)
{ mat4 zrot = mat4(cos(rad), -sin(rad), 0.0, 0.0,
 sin(rad), cos(rad), 0.0, 0.0,
 0.0, 0.0, 1.0, 0.0,
 0.0, 0.0, 0.0, 1.0 );
 return zrot;
}

void main(void){
	vec4 p0 = vec4( 0.25, -0.25, 0.0, 1.0);
	p0 = buildRotateZ(angle) * p0;
	vec4 p1 = vec4(-0.25, -0.25, 0.0, 1.0);
	p1 = buildRotateZ(angle) * p1;
	vec4 p2 = vec4(0.0, 0.25, 0.0, 1.0);
	p2 = buildRotateZ(angle) * p2;

	if (gl_VertexID == 0) 
		gl_Position = p0;
	else if (gl_VertexID == 1) 
		gl_Position = p1;
	else 
		gl_Position = p2;
}