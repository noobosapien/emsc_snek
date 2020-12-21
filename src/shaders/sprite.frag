precision mediump float;
//varying vec4 v_color;

varying vec2 v_fragTexCoord;

uniform sampler2D uTexture;

void main()
{
	gl_FragColor = texture2D(uTexture, v_fragTexCoord);
}