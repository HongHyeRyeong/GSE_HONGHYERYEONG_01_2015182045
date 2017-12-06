#version 330

in vec2 v_TexPos;
in float v_Alpha;

out vec4 FragColor;

uniform sampler2D u_Texture;
uniform vec4 u_Color;

void main()
{
    FragColor = texture(u_Texture, v_TexPos);//*u_Color;
    FragColor.a *= v_Alpha;
    FragColor *= u_Color;
    //FragColor = vec4(1, 1, 1, 1);
}
