// attribute vec2 a_vertex;
// attribute vec2 a_texCoord;
// varying vec2 v_fragTexCoord;

// uniform mat4 u_projection;

// void main(){
//     gl_Position = u_projection * vec4(a_vertex.xy, 0.0, 1.0);
//     v_fragTexCoord = a_texCoord;   
// }

attribute vec2 a_vertex;
attribute vec2 a_texCoord;

varying vec2 v_fragTexCoord;

uniform mat4 u_model;
uniform mat4 u_viewproj;

//uniform mat4 u_view;
//uniform mat4 u_projection;

void main()
{
	vec4 pos = vec4(a_vertex, 0.0, 1.0);

	//gl_Position = u_projection * u_view * u_model * pos;
	gl_Position = pos;
	
	v_fragTexCoord = a_texCoord;
}
