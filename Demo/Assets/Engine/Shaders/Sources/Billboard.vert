uniform mat4 uMVP;
uniform vec3 uCameraPosition;

attribute vec3 aPosition;
attribute vec2 aTexCoords;

varying vec2 vTexCoords;

void main() {
    vec3 forward = normalize(uCameraPosition);
    vec3 right = normalize(cross(vec3(0.0, 1.0, 0.0), forward));
    vec3 up = normalize(cross(forward, right));
    mat4 lookAtMatrix = mat4(   vec4(right, 0.0),
                                vec4(up, 0.0),
                                vec4(forward, 0.0),
                                vec4(0.0, 0.0, 0.0, 1.0)  );
	vTexCoords = aTexCoords;
	gl_Position = uMVP * lookAtMatrix * vec4(aPosition, 1.0);
}