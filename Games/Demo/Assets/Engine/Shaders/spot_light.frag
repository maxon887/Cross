precision mediump float;

struct Light{
	vec3 position;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	
	float constant;
	float linear;
	float quadratic;
	
	vec3 direction;
	float cut_off;
};

uniform sampler2D uMaterialDiffuse;
uniform sampler2D uMaterialSpecular;
uniform float uMaterialShininess;

uniform Light uLight;

uniform vec3 uCameraPosition;

varying vec2 vTexCoords;
varying vec3 vNormal;
varying vec3 vFragPosition;

void main() {
	//attenaution
	float dist = length(uLight.position - vFragPosition);
	float attenaution = 1.0 / (uLight.constant + uLight.linear * dist + uLight.quadratic * dist * dist);
	//ambient
	vec3 ambient = uLight.ambient * vec3(texture2D(uMaterialDiffuse, vTexCoords));
	ambient *= attenaution;
	//diffuse
	vec3 normal = normalize(vNormal);
	vec3 lightDirection = normalize(uLight.position - vFragPosition);
	float diffEffect = max(dot(normal, lightDirection), 0.0);
	vec3 diffuse = uLight.diffuse * diffEffect * vec3(texture2D(uMaterialDiffuse, vTexCoords));
	diffuse *= attenaution;
	//specular
	vec3 viewDirection = normalize(uCameraPosition - vFragPosition);
	vec3 reflectDirection = reflect(-lightDirection, normal);
	float specEffect = pow(max(dot(viewDirection, reflectDirection), 0.0), uMaterialShininess);
	vec3 specular = uLight.specular * specEffect * vec3(texture2D(uMaterialSpecular, vTexCoords));
	specular *= attenaution;
	
	vec3 result = ambient + diffuse + specular;
	gl_FragColor = vec4(result, 1.0);
} 