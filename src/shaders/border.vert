attribute vec3 a_position;
attribute vec2 a_vertCoord;
attribute vec3 a_color;

uniform mat4 u_model;
uniform mat4 u_viewproj;

varying vec2 v_fragCoord;
varying vec3 v_color;

void main(){
    gl_Position = u_viewproj * u_model * vec4(a_position, 1.0);
    v_fragCoord = a_vertCoord;
    v_color = a_color;
}