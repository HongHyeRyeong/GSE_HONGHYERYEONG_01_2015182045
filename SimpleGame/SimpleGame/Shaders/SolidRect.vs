#version 330

in vec3 a_Position;
uniform vec4 u_Trans;

void main()
{
	vec4 newPosition;
	newPosition.x = a_Position.x;
	newPosition.y = a_Position.y;
	newPosition.xy = newPosition.xy*u_Trans.w + u_Trans.xy;
	newPosition.z = 0;
	newPosition.w= 1;
	gl_Position = newPosition;
}
