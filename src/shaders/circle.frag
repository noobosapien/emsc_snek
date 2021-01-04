precision mediump float;

uniform float u_radius;

varying vec4 v_color;

void main(){
	gl_FragColor = v_color;
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