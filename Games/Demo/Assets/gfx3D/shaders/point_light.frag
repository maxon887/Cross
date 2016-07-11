precision mediump float;

struct PointLight{
	vec3 position;
	vec3 color;
	
	float linear;
	float quadratic;
};

uniform sampler2D uDiffuseTexture;
uniform sampler2D uSpecularTexture;
uniform float uShininess;

uniform PointLight uLight;
uniform vec3 uAmbientLight;

uniform vec3 uCameraPosition;

varying vec2 vTexCoords;
varying vec3 vNormal;
varying vec3 vFragPosition;

void main() {
	vec3 texel = vec3(texture2D(uDiffuseTexture, vTexCoords));
	//attenaution
	float dist = length(uLight.position - vFragPosition);
	float attenaution = 1.0 / (1.0 + uLight.linear * dist + uLight.quadratic * dist * dist);
	//ambient
	vec3 ambient = uAmbientLight * texel;
	ambient *= attenaution;
	//diffuse
	vec3 normal = normalize(vNormal);
	vec3 lightDirection = normalize(uLight.position - vFragPosition);
	float diffEffect = max(dot(normal, lightDirection), 0.0);
	vec3 diffuse = uLight.color * diffEffect * texel;
	diffuse *= attenaution;
	//specular
	vec3 viewDirection = normalize(uCameraPosition - vFragPosition);
	vec3 reflectDirection = reflect(-lightDirection, normal);
	float specEffect = pow(max(dot(viewDirection, reflectDirection), 0.0), uShininess);
	vec3 specular = uLight.color * specEffect * vec3(texture2D(uSpecularTexture, vTexCoords));
	specular *= attenaution;
	
	vec3 result = ambient + diffuse + specular;
	gl_FragColor = vec4(result, 1.0);
} 