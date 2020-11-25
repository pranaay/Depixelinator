#version 330 core
out vec4 color;
in vec2 v_TexCoord;
uniform vec4 u_color;
uniform sampler2D u_Texture;
void main()
{
    vec4 texColor = texture(u_Texture, v_TexCoord);
     color = texColor;

}
