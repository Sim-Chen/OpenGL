#version 330 core
layout(location=0) in vec2 apos;
layout(location=1) in vec3 acolor;

out vec3 color;


void main()
{
	gl_Position=vec4(apos.x,apos.y,0.0,1.0f);
	color=acolor;
}