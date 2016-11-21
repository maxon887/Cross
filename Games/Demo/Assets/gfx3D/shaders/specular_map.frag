precision mediump float;

struct Light{
	vec3 position;
	vec4 color;
};

uniform sampler2D uDiffuseTexture;
uniform sampler2D uSpecularTexture;
uniform float uShininess;
uniform vec4 uColor;

uniform vec4 uAmbientLight;
uniform Light uLight;

uniform vec3 uCameraPosition;

varying vec2 vTexCoords;
varying vec3 vNormal;
varying vec3 vFragPosition;

void main() {
	vec4 texel = texture2D(uDiffuseTexture, vTexCoords);
	//ambient
	vec4 ambient = uAmbientLight * uColor * texel;
	//diffuse
	vec3 normal = normalize(vNormal);
	vec3 lightDirection = normalize(uLight.position - vFragPosition);
	float diffEffect = max(dot(normal, lightDirection), 0.0);
	vec4 diffuse = uLight.color * diffEffect * texel;
	//specular
	vec3 viewDirection = normalize(uCameraPosition - vFragPosition);
	vec3 reflectDirection = reflect(-lightDirection, normal);
	float specEffect = pow(max(dot(viewDirection, reflectDirection), 0.0), uShininess);
	vec4 specular = uLight.color * specEffect * texture2D(uSpecularTexture, vTexCoords);
	gl_FragColor = ambient + diffuse + specular;
} 