#version 330 core
out vec4 fragColor;

uniform vec3 uCameraPos;
uniform vec2 uScreenRes;

void main() {
    vec2 pos = gl_FragCoord.xy / uScreenRes.xy;
    fragColor = vec4(pos.x, pos.y, 1.0f, 1.0f);
}
