#version 330

in vec3 a_Position;
in vec2 a_TexPos;
in vec4 a_Velocity;

uniform vec3 u_TrailDir;
uniform vec4 u_Trans;
uniform float u_ElapsedTime;

out vec2 v_TexPos;
out float v_Alpha;

const vec3 c_Gravity = vec3(0, -0.98, 0);

void main()
{
	vec4 newPosition;
	newPosition.x = a_Position.x*u_Trans.z;
	newPosition.y = a_Position.y*u_Trans.w;
	newPosition.xy += u_Trans.xy;
	newPosition.z = 0;
	newPosition.w= 1;

	float newTime = u_ElapsedTime - a_Velocity.w;

	newTime = fract(newTime/1) * 1;
	
	if(newTime > 0)
	{
      newPosition = 
	  vec4(
		  newPosition.x + a_Velocity.x*newTime + 0.5*u_TrailDir.x*newTime*newTime,
		  newPosition.y + a_Velocity.y*newTime + 0.5*u_TrailDir.y*newTime*newTime,
		  newPosition.z + a_Velocity.z*newTime + 0.5*u_TrailDir.z*newTime*newTime,		
		  1.0
		  );
	}
	else
	{
	  newPosition = 
	  vec4(
		  -1000,
		  -1000,
		  -1000,		
		  1.0
		  );
	}

	gl_Position = newPosition;

	v_TexPos = a_TexPos;
	v_Alpha = 1.0 - newTime;
}

