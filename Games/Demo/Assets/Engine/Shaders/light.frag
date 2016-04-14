precision mediump float;

uniform vec4 uColor;
uniform float uAmbientLightStrength;
uniform vec3 uAmbientLightColor;
uniform vec3 uLightPosition;
uniform vec3 uLightColor;

varying vec3 vNormal;
varying vec3 vFragPosition;

void main() {
	vec3 ambientEffect = uAmbientLightColor * uAmbientLightStrength;

	vec3 lightDirection = uLightPosition - vFragPosition;
	lightDirection = normalize(lightDirection);
	float lightStrength = max(dot(vNormal, lightDirection), 0.0);
	vec3 diffuseEffect = uLightColor * lightStrength;

	vec3 result = (ambientEffect + diffuseEffect) * vec3(uColor);
	gl_FragColor = vec4(result, 1.0);
} 