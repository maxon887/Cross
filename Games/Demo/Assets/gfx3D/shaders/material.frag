precision mediump float;

struct Material{
	//vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light{
	vec3 position;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Material uMaterial;
uniform Light uLight;

uniform vec3 uCameraPosition;

varying vec3 vNormal;
varying vec3 vFragPosition;

void main() {
	//ambient
	vec3 ambient = uLight.ambient * uMaterial.ambient;
	//diffuse
	vec3 normal = normalize(vNormal);
	vec3 lightDirection = normalize(uLight.position - vFragPosition);
	float diffEffect = max(dot(normal, lightDirection), 0.0);
	vec3 diffuse = uLight.diffuse * uMaterial.diffuse * diffEffect;
	//specular
	vec3 viewDirection = normalize(uCameraPosition - vFragPosition);
	vec3 reflectDirection = reflect(-lightDirection, normal);
	float specEffect = pow(max(dot(viewDirection, reflectDirection), 0.0), uMaterial.shininess);
	vec3 specular = uLight.specular * uMaterial.specular * specEffect;
	vec3 result = ambient + diffuse + specular;
	gl_FragColor = vec4(result, 1.0);
} 