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
	float outer_cut_off;
};

uniform sampler2D uDiffuseMap0;	
uniform sampler2D uSpecularMap0;
uniform float uMaterialShininess;

uniform Light uLight;

uniform vec3 uCameraPosition;

varying vec2 vTexCoords;
varying vec3 vNormal;
varying vec3 vFragPosition;

void main() {
	vec3 lightDirection = normalize(uLight.position - vFragPosition);
	float theta = dot(lightDirection, normalize(-uLight.direction));
	float epsilon = uLight.cut_off - uLight.outer_cut_off;
	float intensity = clamp((theta - uLight.outer_cut_off) / epsilon, 0.0, 1.0);

	float dist = length(uLight.position - vFragPosition);
	float attenaution = 1.0 / (uLight.constant + uLight.linear * dist + uLight.quadratic * dist * dist);
	//ambient
	vec3 ambient = uLight.ambient * vec3(texture2D(uDiffuseMap0, vTexCoords));
	ambient *= attenaution;
	//diffuse
	vec3 normal = normalize(vNormal);
	float diffEffect = max(dot(normal, lightDirection), 0.0);
	vec3 diffuse = uLight.diffuse * diffEffect * vec3(texture2D(uDiffuseMap0, vTexCoords));
	diffuse *= attenaution;
	diffuse *= intensity;
	//specular
	vec3 viewDirection = normalize(uCameraPosition - vFragPosition);
	vec3 reflectDirection = reflect(-lightDirection, normal);
	float specEffect = pow(max(dot(viewDirection, reflectDirection), 0.0), uMaterialShininess);
	vec3 specular = uLight.specular * specEffect * vec3(texture2D(uSpecularMap0, vTexCoords));
	specular *= attenaution;
	specular *= intensity;
	
	vec3 result = ambient + diffuse + specular;
	gl_FragColor = vec4(result, 1.0);

} 