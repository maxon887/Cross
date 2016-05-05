precision mediump float;

#define MAX_POINT_LIGHTS 32

struct PointLight{
	vec3 position;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	
	float constant;
	float linear;
	float quadratic;
};

uniform sampler2D uMaterialDiffuse;
uniform sampler2D uMaterialSpecular;
uniform float uMaterialShininess;

uniform int uPointLightCount;
uniform PointLight uPointLights[MAX_POINT_LIGHTS];

uniform vec3 uCameraPosition;  

varying vec2 vTexCoords;
varying vec3 vNormal;
varying vec3 vFragPosition;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPosition, vec3 viewDirection, vec3 diffuseColor, vec3 specularColor){
	vec3 lightDirection = normalize(light.position - fragPosition);
	//attenaution
	float dist = length(light.position - vFragPosition);
	float attenaution = 1.0 / (light.constant + light.linear * dist + light.quadratic * dist * dist);
	//diffuse
	float diffEffect = max(dot(normal, lightDirection), 0.0);
	vec3 diffuse = light.diffuse * diffEffect * diffuseColor;
	diffuse *= attenaution;
	//specular
	vec3 reflectDirection = reflect(-lightDirection, normal);
	float specEffect = pow(max(dot(viewDirection, reflectDirection), 0.0), uMaterialShininess);
	vec3 specular = light.specular * specEffect * specularColor;
	specular *= attenaution;
	
	return (diffuse + specular);
}

void main(){
	vec3 normal = normalize(vNormal);
	vec3 viewDirection = normalize(uCameraPosition - vFragPosition);
	
	vec3 result = vec3(0.0);
	
	vec3 diffuseColor = vec3(texture2D(uMaterialDiffuse, vTexCoords));
	vec3 specularColor = vec3(texture2D(uMaterialSpecular, vTexCoords));
	
	for(int i = 0; i < uPointLightCount; i++){
		result += CalcPointLight(uPointLights[i], normal, vFragPosition, viewDirection, diffuseColor, specularColor);
	}
	
	gl_FragColor = vec4(result, 1.0);
}















