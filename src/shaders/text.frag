precision mediump float;

varying vec2 v_fragTexCoord;

uniform sampler2D u_text;
uniform vec3 u_textColor;

void main(){
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture2D(u_text, v_fragTexCoord).r);
    // vec4 sampled = texture2D(u_text, v_fragTexCoord);
    gl_FragColor = vec4(u_textColor, 1.0) * sampled;
    // gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
