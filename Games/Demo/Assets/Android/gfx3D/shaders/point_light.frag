precision mediump float;

struct PointLight{
	vec3 position;
	vec4 color;
	
	float intensity;
};

uniform sampler2D uDiffuseTexture;
uniform sampler2D uSpecularMap;
uniform float uShininess;

uniform PointLight uLight;
uniform vec4 uAmbientLight;

uniform vec3 uCameraPosition;

varying vec2 vTexCoords;
varying vec3 vNormal;
varying vec3 vFragPosition;

void main() {
	vec4 texel = texture2D(uDiffuseTexture, vTexCoords);
	//attenaution
	float dist = length(uLight.position - vFragPosition);
	float attenaution = 1.0 / (1.0 + uLight.intensity * dist + uLight.intensity * dist * dist);
	//ambient
	vec4 ambient = uAmbientLight * texel;
	ambient *= attenaution;
	//diffuse
	vec3 normal = normalize(vNormal);
	vec3 lightDirection = normalize(uLight.position - vFragPosition);
	float diffEffect = max(dot(normal, lightDirection), 0.0);
	vec4 diffuse = uLight.color * diffEffect * texel;
	diffuse *= attenaution;
	//specular
	vec3 viewDirection = normalize(uCameraPosition - vFragPosition);
	vec3 reflectDirection = reflect(-lightDirection, normal);
	float specEffect = pow(max(dot(viewDirection, reflectDirection), 0.0), uShininess);
	vec4 specular = uLight.color * specEffect * texture2D(uSpecularMap, vTexCoords);
	specular *= attenaution;
	gl_FragColor = ambient + diffuse + specular;
} 