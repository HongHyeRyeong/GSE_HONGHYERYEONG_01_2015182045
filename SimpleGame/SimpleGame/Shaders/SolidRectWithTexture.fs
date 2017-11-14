#version 330

out vec4 FragColor;

in vec2 v_TexPosition;

uniform sampler2D u_Texture;
uniform vec4 u_Color;

void main()
{
	vec2 newTexPos = vec2(v_TexPosition.x, 1-v_TexPosition.y);
	FragColor = texture2D(u_Texture, newTexPos);
}
