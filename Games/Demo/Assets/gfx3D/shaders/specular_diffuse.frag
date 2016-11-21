precision mediump float;

struct Light{
	vec3 position;
	vec4 color;
};
//material properties
uniform sampler2D uDiffuseTexture;
uniform vec4 uColor;
uniform vec4 uSpecularColor;
uniform float uShininess;
//light properties
uniform vec4 uAmbientColor;
uniform Light uLight;

uniform vec3 uCameraPosition;

varying vec2 vTexCoords;
varying vec3 vNormal;
varying vec3 vFragPosition;

void main() {
	vec4 texturedColor = texture2D(uDiffuseTexture, vTexCoords);
	//ambient
	vec4 ambient = uAmbientColor * uColor * texturedColor;
	//diffuse
	vec3 normal = normalize(vNormal);
	vec3 lightDirection = normalize(uLight.position - vFragPosition);
	float diffEffect = max(dot(normal, lightDirection), 0.0);
	vec4 diffuse = uLight.color * uColor * diffEffect * texturedColor;
	//specular
	vec3 viewDirection = normalize(uCameraPosition - vFragPosition);
	vec3 reflectDirection = reflect(-lightDirection, normal);
	float specEffect = pow(max(dot(viewDirection, reflectDirection), 0.0), uShininess);
	vec4 specular = uLight.color * uSpecularColor * specEffect;
	
	gl_FragColor = ambient + diffuse + specular;
} 