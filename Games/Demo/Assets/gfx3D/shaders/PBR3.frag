precision mediump float;

#define PI 3.1415926535897932384626433832795

struct Light{
	vec3 position;
	vec3 direction;
	vec4 color;
	
	float intensity;
	float cut_off;
	float outer_cut_off;
};

uniform sampler2D uAlbedoMap;
uniform sampler2D uMetallmessMap;
uniform sampler2D uRoughnessMap;
uniform sampler2D uNormalMap;

uniform Light uLight;

varying vec2 vTexCoords;
varying vec3 vNormal;
varying vec3 vFragPosition;
varying vec3 vViewDirection;

float ShickGGX(float NoV, float k){
	return NoV / (NoV * (1 - k) + k);
}

vec3 F_Fresnel(float NoV, vec3 F0, float roughness){
	return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - NoV, 5.0);
}

float G_Schlic(float NoV, float roughness){
	float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NoV;
    float denom = NoV * (1.0 - k) + k;
	
    return nom / denom;
}

float G_Smith(vec3 N, vec3 V, vec3 L, float k){
	float NoV = max(dot(N, V), 0.0);
	float NoL = max(dot(N, L), 0.0);
	float ggxV = ShickGGX(NoV, k);
	float ggxL = ShickGGX(NoL, k);
	return ggxV * ggxL;
}

float D_GGX(vec3 N, vec3 H, float roughness){
	float a = roughness * roughness;
	float a2 = a * a;
	float NoH = max(dot(N, H), 0.0);
	float NoH2 = NoH * NoH;
	
	float denom = (NoH2 * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;
	
	return a2 / denom;
}

void main(){
	vec3 N = normalize(vNormal);
	vec3 V = vViewDirection;
	vec3 L = normalize(uLight.position - vFragPosition);
	
	vec3 albedo = texture2D(uAlbedoMap, vTexCoords);
	vec3 mettalic = texture2D(uMetallmessMap, vTexCoords);
	vec3 roughness = texture2D(uRoughnessMap, vTexCoords);
	//roughness = vec3(0.2);

	vec3 lightColor = vec3(23.4, 21.32, 20.79);
	float radiance = lightColor;
	
	vec3 F0 = vec3(0.04);
	F0 = mix(F0, albedo, mettalic);
	vec3 F = F_Fresnel(max(dot(N, V), 0.0), F0, roughness);
	
	vec3 kS = F;
	vec3 kD = vec3(1.0) - kS;
	kD *= 1.0 - mettalic;
	
	vec3 H = normalize(V + L);
	
	//BRDF
	float NDF = D_GGX(N, H, roughness);
	float G = G_Smith(N, V, L, roughness);
	vec3 nominator    = NDF * G * F;
    float denominator = 4 * max(dot(V, N), 0.0) * max(dot(L, N), 0.0) + 0.001; 
    vec3 brdf = nominator / denominator;
	
	float NoL = max(dot(N, L), 0.0);
	vec3 Lo = (kD * albedo / PI + brdf) * radiance * NoL;
	
	vec3 ambient = vec3(0.03) * albedo;
	vec3 color = Lo + ambient;
	
	color = color / (color + vec3(1.0));
	color = pow(color, vec3(1.0/2.2)); 
	
	gl_FragColor = vec4(color, 1.0);
}
















