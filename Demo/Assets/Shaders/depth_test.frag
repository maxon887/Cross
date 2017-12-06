precision mediump float;
uniform vec4 uColor;
uniform float uFar;
uniform float uNear;

void main() {
	float depth = gl_FragCoord.z;
	float z = depth * 2.0 - 1.0;
	float linearDepth = (2.0 * uNear * uFar) / (uFar + uNear - z * (uFar - uNear)) / uFar;
	gl_FragColor = vec4(vec3(linearDepth), 1.0);
} 