precision mediump float;

struct Light{
	vec3 position;
	vec4 color;
};
//material properties
uniform vec4 uColor;
uniform vec4 uSpecularColor;
uniform float uShininess;
//light properties
uniform vec4 uAmbientLight;
uniform Light uLight;

uniform vec3 uCameraPosition;

varying vec3 vNormal;
varying vec3 vFragPosition;

void main() {
	//ambient
	vec4 ambient = uAmbientLight * uColor;
	//diffuse
	vec3 lightDirection = normalize(uLight.position - vFragPosition);
	float diffEffect = max(dot(vNormal, lightDirection), 0.0);
	vec4 diffuse = uLight.color * uColor * diffEffect;
	//specular
	vec3 viewDirection = normalize(uCameraPosition - vFragPosition);
	vec3 reflectDirection = reflect(-lightDirection, vNormal);
	float specEffect = pow(max(dot(viewDirection, reflectDirection), 0.0), uShininess);
	vec4 specular = uLight.color * uSpecularColor * specEffect;
	
	gl_FragColor = ambient + diffuse + specular;
} 