attribute vec2 a_vertex;
attribute vec2 a_texCoord;

varying vec2 v_fragTexCoord;

uniform mat4 u_projection;

void main(){
    gl_Position = u_projection * vec4(a_vertex.xy, 0.0, 1.0);
    v_fragTexCoord = a_texCoord;
}