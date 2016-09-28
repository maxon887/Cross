precision mediump float;

struct Light{
	vec3 position;
	vec3 color;
};
//material properties
uniform vec3 uColor;
uniform vec3 uSpecularColor;
uniform float uShininess;
//light properties
uniform vec3 uAmbientLight;
uniform Light uLight;

uniform vec3 uCameraPosition;

varying vec3 vNormal;
varying vec3 vFragPosition;

void main() {
	//ambient
	vec3 ambient = uAmbientLight * uColor;
	//diffuse
	vec3 lightDirection = normalize(uLight.position - vFragPosition);
	float diffEffect = max(dot(vNormal, lightDirection), 0.0);
	vec3 diffuse = uLight.color * uColor * diffEffect;
	//specular
	vec3 viewDirection = normalize(uCameraPosition - vFragPosition);
	vec3 reflectDirection = reflect(-lightDirection, vNormal);
	float specEffect = pow(max(dot(viewDirection, reflectDirection), 0.0), uShininess);
	vec3 specular = uLight.color * uSpecularColor * specEffect;
	vec3 result = ambient + diffuse + specular;
	
	gl_FragColor = vec4(result, 1.0);
} 