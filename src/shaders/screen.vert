attribute vec3 a_position;
attribute vec2 a_texCoord;

varying vec2 v_fragTexCoord;

void main()
{
    v_fragTexCoord = a_texCoord;
    gl_Position = vec4(a_position.x, a_position.y, 0.0, 1.0); 
}  