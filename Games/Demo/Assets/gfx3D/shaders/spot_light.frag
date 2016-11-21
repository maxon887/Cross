precision mediump float;

struct Light{
	vec3 direction;
	vec3 position;
	vec4 color;
	
	float intensity;
	
	float cut_off;
	float outer_cut_off;
};

uniform sampler2D uDiffuseTexture;	
uniform sampler2D uSpecularMap;
uniform float uShininess;

uniform Light uLight;
uniform vec4 uAmbientLight;

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
	float attenaution = 1.0 / (1.0 + uLight.intensity * dist + uLight.intensity * dist * dist);
	
	vec4 texel = texture2D(uDiffuseTexture, vTexCoords);
	//ambient
	vec4 ambient = uAmbientLight * texel;
	ambient *= attenaution;
	//diffuse
	vec3 normal = normalize(vNormal);
	float diffEffect = max(dot(normal, lightDirection), 0.0);
	vec4 diffuse = uLight.color * diffEffect * texel;
	diffuse *= attenaution;
	diffuse *= intensity;
	//specular
	vec3 viewDirection = normalize(uCameraPosition - vFragPosition);
	vec3 reflectDirection = reflect(-lightDirection, normal);
	float specEffect = pow(max(dot(viewDirection, reflectDirection), 0.0), uShininess);
	vec4 specular = uLight.color * specEffect * texture2D(uSpecularMap, vTexCoords);
	specular *= attenaution;
	specular *= intensity;
	
	gl_FragColor = ambient + diffuse + specular;

} 