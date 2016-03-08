precision mediump float;
varying vec2 vTexCoord;

uniform sampler2D uTexture;
uniform vec4 uColor;

void main() {
	//gl_FragColor = texture2D(uTexture, vTexCoord) * uColor;
	gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
} 