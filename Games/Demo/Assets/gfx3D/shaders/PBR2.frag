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

vec3 getFishEye(vec2 uv, float level) {
    float len = length(uv);
    float a = len * level;
    return vec3(uv / len * sin(a), -cos(a));
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

float phong(vec3 l, vec3 e, vec3 n, float power) {
    float nrm = (power + 8.0) / (PI * 8.0);
    return pow(max(dot(l,reflect(e,n)),0.0), power) * nrm;
}

// GGX code from https://www.shadertoy.com/view/MlB3DV
float G1V ( float dotNV, float k ) {
	return 1.0 / (dotNV*(1.0 - k) + k);
}
float GGX(vec3 N, vec3 V, vec3 L, float roughness, float F0) {
    	float alpha = roughness*roughness;
	vec3 H = normalize (V + L);

	float dotNL = clamp (dot (N, L), 0.0, 1.0);
	float dotNV = clamp (dot (N, V), 0.0, 1.0);
	float dotNH = clamp (dot (N, H), 0.0, 1.0);
	float dotLH = clamp (dot (L, H), 0.0, 1.0);

	float D, vis;
	float F;

	// NDF : GGX
	float alphaSqr = alpha*alpha;
	float pi = 3.1415926535;
	float denom = dotNH * dotNH *(alphaSqr - 1.0) + 1.0;
	D = alphaSqr / (pi * denom * denom);

	// Fresnel (Schlick)
	float dotLH5 = pow (1.0 - dotLH, 5.0);
	F = F0 + (1.0 - F0)*(dotLH5);

	// Visibility term (G) : Smith with Schlick's approximation
	float k = alpha / 2.0;
	vis = G1V (dotNL, k) * G1V (dotNV, k);

	return /*dotNL */ D * F * vis;
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
	//vec3 point = ray * dist;
	//vec3 normal = point - obj_pos;
	vec3 normal = vNormal;
	vec3 ray = -vViewDirection;
	
	// material
	float metallic = texture2D(uMetallmessMap, vTexCoords);
	//float metallic = 0.1;
	float roughness = texture2D(uRoughnessMap, vTexCoords);
	float fresnel_pow = 1.5;// / (0.5 + roughness);
	vec3 color_mod = vec3(1.0);
	//vec3 color_mod = texture2D(uAlbedoMap, vTexCoords);
	vec3 light_color = textureCube(iChannel0,vec3(1.0,0.0,0.0)).xyz * 1.2;
			
			
	// IBL
	vec3 ibl_diffuse = textureBlured(iChannel0,normal);
	vec3 ibl_reflection = textureBlured(iChannel0,reflect(ray,normal));
	
	// fresnel
	float fresnel = max(1.0 - dot(normal,-ray), 0.0);
	fresnel = pow(fresnel,fresnel_pow);    
	
	// reflection        
	vec3 refl = textureCube(iChannel0,reflect(ray,normal)).xyz;
	refl = mix(refl,ibl_reflection,(1.0-fresnel)*roughness);
	refl = mix(refl,ibl_reflection,roughness);
	
	// specular
	vec3 light = normalize(vec3(-0.5,1.0,0.0));
	float power = 1.0 / max(roughness * 0.4,0.01);
	//vec3 spec = light_color * phong(light,ray,normal,power);
	vec3 spec = light_color * GGX(normal,-ray,light,roughness*0.7, 0.2);
	refl -= spec;
	
	// diffuse
	vec3 diff = ibl_diffuse * texture2D(uAlbedoMap, vTexCoords);
	diff = mix(diff * color_mod,refl,fresnel);        

	vec3 color = mix(diff,refl * color_mod,metallic) + spec;

	gl_FragColor = vec4(color, 1.0);
}
















