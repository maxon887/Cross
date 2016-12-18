precision mediump float;

#define PI 3.1415926535897932384626433832795
#define PI_MODIFIER 4.0

struct Light{
	vec3 position;
	vec3 direction;
	vec4 color;
	
	float intensity;
	float cut_off;
	float outer_cut_off;
};

float somestep(float t) {
    return pow(t,4.0);
}

vec3 textureAVG(samplerCube tex, vec3 tc) {
    const float diff0 = 0.35;
    const float diff1 = 0.12;
 	vec3 s0 = textureCube(tex,tc).xyz;
    vec3 s1 = textureCube(tex,tc+vec3(diff0)).xyz;
    vec3 s2 = textureCube(tex,tc+vec3(-diff0)).xyz;
    vec3 s3 = textureCube(tex,tc+vec3(-diff0,diff0,-diff0)).xyz;
    vec3 s4 = textureCube(tex,tc+vec3(diff0,-diff0,diff0)).xyz;
    
    vec3 s5 = textureCube(tex,tc+vec3(diff1)).xyz;
    vec3 s6 = textureCube(tex,tc+vec3(-diff1)).xyz;
    vec3 s7 = textureCube(tex,tc+vec3(-diff1,diff1,-diff1)).xyz;
    vec3 s8 = textureCube(tex,tc+vec3(diff1,-diff1,diff1)).xyz;
    
    return (s0 + s1 + s2 + s3 + s4 + s5 + s6 + s7 + s8) * 0.111111111;
}

vec3 textureBlured(samplerCube tex, vec3 tc) {
   	vec3 r = textureAVG(tex,vec3(1.0,0.0,0.0));
    vec3 t = textureAVG(tex,vec3(0.0,1.0,0.0));
    vec3 f = textureAVG(tex,vec3(0.0,0.0,1.0));
    vec3 l = textureAVG(tex,vec3(-1.0,0.0,0.0));
    vec3 b = textureAVG(tex,vec3(0.0,-1.0,0.0));
    vec3 a = textureAVG(tex,vec3(0.0,0.0,-1.0));
        
    float kr = dot(tc,vec3(1.0,0.0,0.0)) * 0.5 + 0.5; 
    float kt = dot(tc,vec3(0.0,1.0,0.0)) * 0.5 + 0.5;
    float kf = dot(tc,vec3(0.0,0.0,1.0)) * 0.5 + 0.5;
    float kl = 1.0 - kr;
    float kb = 1.0 - kt;
    float ka = 1.0 - kf;
    
    kr = somestep(kr);
    kt = somestep(kt);
    kf = somestep(kf);
    kl = somestep(kl);
    kb = somestep(kb);
    ka = somestep(ka);    
    
    float d;
    vec3 ret;
    ret  = f * kf; d  = kf;
    ret += a * ka; d += ka;
    ret += l * kl; d += kl;
    ret += r * kr; d += kr;
    ret += t * kt; d += kt;
    ret += b * kb; d += kb;
    
    return ret / d;
}

uniform samplerCube iChannel0;
uniform samplerCube iChannel1;

uniform sampler2D uAlbedoMap;
uniform sampler2D uMetallmessMap;
uniform sampler2D uRoughnessMap;
uniform sampler2D uNormalMap;

uniform Light uLight;

varying vec2 vTexCoords;
varying vec3 vNormal;
varying vec3 vFragPosition;
varying vec3 vViewDirection;


void main(){
	float roughness = texture2D(uRoughnessMap, vTexCoords);
	
	vec3 N = vNormal;
	vec3 L = normalize(uLight.position - vFragPosition);
	vec3 R = vViewDirection;
	
	// dots
	float NdotV = clamp(dot(N, -R), 0.0, 1.0);
	float NdotL = clamp(dot(L,  N), 0.0, 1.0);
	vec3  RrefN = reflect(R, N);
	
	// specular power
	float specular_mod   = 1.0 - roughness;
	float specular_power = clamp(pow(clamp(dot(L, RrefN), 0.0, 1.0), 1.0 / specular_mod) * specular_mod, 0.0, 1.0);
	
	// IBL
	vec3 ibl_diffuse    = textureBlured(iChannel1, N);
	vec3 ibl_reflection = textureBlured(iChannel1, RrefN);
	
	// fresnel
	float fresnel_pow  = 35.0;
	float fresnel_base = NdotV;
	float fresnel_exp  = pow(fresnel_base, fresnel_pow);
	float fresnel_term = specular_power + fresnel_exp;
	
	// specular
	float normalization_term = ((specular_power + PI_MODIFIER) / PI_MODIFIER * PI);
	float specular_term      = normalization_term * specular_power;
	float vis_alpha          = 1.0 / (sqrt((PI / 4.0) * specular_power + (PI / 2.0)));
	float vis_term           = clamp((NdotL * (1.0 - vis_alpha) + vis_alpha) * (NdotV * (1.0 - vis_alpha) + vis_alpha), 0.0, 1.0);
	
	// reflection        
	vec3 refl = textureCube(iChannel1, RrefN).xyz;
	refl = mix(refl, ibl_reflection, 1.0 - vis_term);
	refl = mix(refl, ibl_reflection, roughness);
	
	// final colors
	vec3 specular_color = specular_term * fresnel_term * vis_term * uLight.color;
	vec3 diffuse_color  = mix(ibl_diffuse, refl, vis_term);

	vec3 final_color = diffuse_color + specular_color;

	gl_FragColor = vec4(final_color, 1.0);
}
















