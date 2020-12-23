attribute vec3 a_position;
attribute vec2 a_texCoord;

varying vec2 v_fragTexCoord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

//attribute vec4 a_color;
//varying vec4 v_color;

void main()
{
	vec4 pos = vec4(a_position, 1.0);

	gl_Position = u_projection * u_view * u_model * pos;
	v_fragTexCoord = a_texCoord;
}
