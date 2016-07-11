precision mediump float;

#define MAX_POINT_LIGHTS 16
#define MAX_DIRECTIONAL_LIGHTS 4
#define MAX_SPOT_LIGHTS 8

struct PointLight{
	vec3 position;
	vec3 color;
	
	float linear;
	float quadratic;
};

struct DirectionalLight{
	vec3 direction;
	vec3 color;
};

struct SpotLight{
	vec3 position;
	vec3 color;

	float linear;
	float quadratic;
	
	vec3 direction;
	float cut_off;
	float outer_cut_off;
};

uniform sampler2D uDiffuseMap0;
uniform sampler2D uSpecularMap0;
uniform float uShininess;

uniform int uPointLightCount;
uniform PointLight uPointLights[MAX_POINT_LIGHTS];

uniform int uDirectionalLightCount;
uniform DirectionalLight uDirectionalLights[MAX_DIRECTIONAL_LIGHTS];

uniform int uSpotLightCount;
uniform SpotLight uSpotLights[MAX_SPOT_LIGHTS];

varying vec2 vTexCoords;
varying vec3 vNormal;
varying vec3 vFragPosition;
varying vec3 vViewDirection;

vec3 CalcPointLight(PointLight light, vec3 diffuseColor, vec3 specularColor){
	vec3 lightDirection = normalize(light.position - vFragPosition);
	//attenaution
	float dist = length(light.position - vFragPosition);
	float attenaution = 1.0 / (light.constant + light.linear * dist + light.quadratic * dist * dist);
	//diffuse
	float diffEffect = max(dot(vNormal, lightDirection), 0.0);
	vec3 diffuse = light.diffuse * diffEffect * diffuseColor;
	diffuse *= attenaution;
	//specular
	vec3 reflectDirection = reflect(-lightDirection, vNormal);
	float specEffect = pow(max(dot(vViewDirection, reflectDirection), 0.0), uMaterialShininess);
	vec3 specular = light.specular * specEffect * specularColor;
	specular *= attenaution;
	
	return (diffuse + specular);
}

vec3 CalcDirectionalLight(DirectionalLight light, vec3 diffuseColor, vec3 specularColor){
	//ambient
	vec3 ambient = light.ambient * diffuseColor;
	//diffuse
	vec3 lightDirection = normalize(-light.direction);
	float diffEffect = max(dot(vNormal, lightDirection), 0.0);
	vec3 diffuse = light.diffuse * diffEffect * diffuseColor;
	//specular
	vec3 reflectDirection = reflect(-lightDirection, vNormal);
	float specEffect = pow(max(dot(vViewDirection, reflectDirection), 0.0), uMaterialShininess);
	vec3 specular = light.specular * specEffect * specularColor;
	return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 diffuseColor, vec3 specularColor){
	vec3 lightDirection = normalize(light.position - vFragPosition);
	
	float theta = dot(lightDirection, normalize(-light.direction));
	float epsilon = light.cut_off - light.outer_cut_off;
	float intensity = clamp((theta - light.outer_cut_off) / epsilon, 0.0, 1.0);

	float dist = length(light.position - vFragPosition);
	float attenaution = 1.0 / (light.constant + light.linear * dist + light.quadratic * dist * dist);
	//ambient
	vec3 ambient = light.ambient * diffuseColor;
	ambient *= attenaution;
	//diffuse
	float diffEffect = max(dot(vNormal, lightDirection), 0.0);
	vec3 diffuse = light.diffuse * diffEffect * diffuseColor;
	diffuse *= attenaution;
	diffuse *= intensity;
	//specular
	vec3 reflectDirection = reflect(-lightDirection, vNormal);
	float specEffect = pow(max(dot(vViewDirection, reflectDirection), 0.0), uMaterialShininess);
	vec3 specular = light.specular * specEffect * specularColor;
	specular *= attenaution;
	specular *= intensity;
	
	return (ambient + diffuse + specular);
}

void main(){
	vec3 result = vec3(0.0);
	
	vec3 diffuseColor = vec3(texture2D(uDiffuseMap0, vTexCoords));
	vec3 specularColor = vec3(texture2D(uSpecularMap0, vTexCoords));
	
	for(int i = 0; i < uPointLightCount; ++i){
		result += CalcPointLight(uPointLights[i], diffuseColor, specularColor);
	}
	
	for(int i = 0; i < uDirectionalLightCount; ++i){
		result += CalcDirectionalLight(uDirectionalLights[i], diffuseColor, specularColor);
	}
	
	for(int i = 0; i < uSpotLightCount; ++i){
		result += CalcSpotLight(uSpotLights[i], diffuseColor, specularColor);
	}
	
	gl_FragColor = vec4(result, 1.0);
}