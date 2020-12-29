attribute vec3 a_position;
attribute vec3 a_color;

varying vec4 v_color;

uniform mat4 u_model;
uniform mat4 u_viewproj;

//uniform mat4 u_view;
//uniform mat4 u_projection;

void main(){
    v_color = vec4(a_color, 0.3);
    
    //gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0);
    gl_Position = u_viewproj * u_model * vec4(a_position, 1.0);
}