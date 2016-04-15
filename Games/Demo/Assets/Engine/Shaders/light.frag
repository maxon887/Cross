precision mediump float;

uniform vec4 uColor;
uniform vec3 uCameraPosition;
uniform float uAmbientLightStrength;
uniform vec3 uAmbientLightColor;
uniform vec3 uLightPosition;
uniform vec3 uLightColor;

varying vec3 vNormal;
varying vec3 vFragPosition;

const float specularStrength = 0.5;

void main() {
	//ambient component
	vec3 ambientEffect = uAmbientLightColor * uAmbientLightStrength;
	//diffuse component
	vec3 lightDirection = uLightPosition - vFragPosition;
	lightDirection = normalize(lightDirection);
	vec3 normal = normalize(vNormal);
	float lightStrength = max(dot(normal, lightDirection), 0.0);
	vec3 diffuseEffect = uLightColor * lightStrength;
	//specular component
	vec3 viewDirection = normalize(uCameraPosition - vFragPosition);
	vec3 reflectDirection = reflect(-lightDirection, normal);
	float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), 32.0);
	vec3 specularEffect = specularStrength * spec * uLightColor;

	vec3 result = (ambientEffect + diffuseEffect + specularEffect) * vec3(uColor);
	gl_FragColor = vec4(result, 1.0);
} 