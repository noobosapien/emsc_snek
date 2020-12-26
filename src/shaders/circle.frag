precision mediump float;

uniform float u_thickness;
uniform float u_radius;
uniform vec2 u_mid;

uniform vec2 u_resolution;
varying vec4 v_color;

float circle(in vec2 _st, in float _radius, in vec2 _mid){
    vec2 dist =_st - _mid - vec2(0.5);
	return smoothstep(_radius-(_radius*0.5), _radius+(_radius*0.1), dot(dist,dist)*1.0);
}

void main(){
	vec2 st = gl_FragCoord.xy/u_resolution.xy;
    vec2 mid = u_mid;

	vec3 color = vec3(circle(st, 0.001, mid), 0, 1);

	gl_FragColor = vec4( color, .4 );
}