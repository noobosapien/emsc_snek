precision mediump float;

uniform float u_radius;
uniform vec2 u_resolution;
uniform vec2 u_center;

varying vec4 v_color;
varying vec3 v_position;

float circle(vec2 st, vec2 center, float radius){

	return 1.0;
}


void main(){

	float radius = u_radius / u_resolution.x;
	vec2 st = gl_FragCoord.xy / u_resolution.xy;

	vec3 color = vec3(1.0, 1.0, 0.0) * circle(st, u_center, radius);
	gl_FragColor = vec4(color, 0.6);
}


/*
varying vec4 v_color;
varying vec4 position;

float circleShape(vec2 pos, float radius){
	return step(radius, length(pos - vec2(0.0)));
}

void main(){
	vec2 pos = position.xy;
	
	vec3 col = vec3(0.0);

	float circle = circleShape(pos, 1.2);
	col = vec3(circle);
	gl_FragColor = vec4(col, 1.0);
}*/