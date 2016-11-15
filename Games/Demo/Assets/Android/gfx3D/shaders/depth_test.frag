precision mediump float;
uniform vec4 uColor;

void main() {
	float depth = gl_FragCoord.z;
	float z = depth * 2.0 - 1.0;
	const float near = 0.1;
	const float far = 100.0;
	float linearDepth = (2.0 * near * far) / (far + near - z * (far - near)) / far;
	gl_FragColor = vec4(vec3(linearDepth), 1.0);
} 