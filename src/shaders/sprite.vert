attribute vec3 a_position;
attribute vec2 a_texCoord;

varying vec2 v_fragTexCoord;

uniform mat4 u_worldTransform;
uniform mat4 u_viewProj;

//attribute vec4 a_color;
//varying vec4 v_color;

void main()
{
	vec4 pos = vec4(a_position, 1.0);

	gl_Position = pos * u_worldTransform * u_viewProj;
	v_fragTexCoord = a_texCoord;
}
