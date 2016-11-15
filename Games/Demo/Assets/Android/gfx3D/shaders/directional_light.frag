precision mediump float;

struct Light{
	vec3 color;
	vec3 direction;
};

uniform sampler2D uDiffuseTexture;
uniform sampler2D uSpecularMap;
uniform float uShininess;
uniform vec4 uColor;

uniform Light uLight;
uniform vec3 uAmbientLight;

uniform vec3 uCameraPosition;

varying vec2 vTexCoords;
varying vec3 vNormal;
varying vec3 vFragPosition;

void main() {
	vec3 texel = vec3(texture2D(uDiffuseTexture, vTexCoords));
	//ambient
	vec3 ambient = uAmbientLight * uColor * texel;
	//diffuse
	vec3 normal = normalize(vNormal);
	vec3 lightDirection = normalize(-uLight.direction);
	float diffEffect = max(dot(normal, lightDirection), 0.0);
	vec3 diffuse = uLight.color * diffEffect * texel;
	//specular
	vec3 viewDirection = normalize(uCameraPosition - vFragPosition);
	vec3 reflectDirection = reflect(-lightDirection, normal);
	float specEffect = pow(max(dot(viewDirection, reflectDirection), 0.0), uShininess);
	vec3 specular = uLight.color * specEffect * texture2D(uSpecularMap, vTexCoords);
	vec3 result = ambient + diffuse + specular;
	gl_FragColor = vec4(result, 1.0);
} 