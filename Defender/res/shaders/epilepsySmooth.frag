#version 130

uniform sampler2D currentFrame;
uniform sampler2D lastFrame;
uniform float maxDelta;

void main() {
    vec2 uv = gl_FragCoord.xy / textureSize(currentFrame, 0);

    vec3 current = texture(currentFrame, uv).rgb;
    vec3 previous = texture(lastFrame, uv).rgb;

    vec3 delta = clamp(current - previous, -maxDelta, maxDelta);
    vec3 outputColor = previous + delta;

    gl_FragColor = vec4(outputColor, 1.0);
}
    