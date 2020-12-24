precision mediump float;

uniform float u_thickness;
uniform float u_radius;

uniform vec2 u_resolution;
varying vec4 v_color;

void main(){
    /*vec2 st = gl_FragCoord.xy / u_resolution;
    float pct = 0.0;

    vec2 toCenter = vec2(0.5) - st;
    pct = length(toCenter);

    float lessthan = (u_radius * u_resolution.x / u_resolution.y) - (u_thickness * u_resolution.x / u_resolution.y);
    float radius = (u_radius * u_resolution.x / u_resolution.y);

    if(pct < lessthan || pct > radius)
        discard;
    */
    gl_FragColor = v_color;
}