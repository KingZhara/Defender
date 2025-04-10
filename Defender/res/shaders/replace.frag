#version 130

uniform sampler2D texture;
uniform vec3 targetColor;
uniform vec3 replaceColor;

void main() {
    vec2 uv = gl_TexCoord[0].xy;
    vec4 color = texture2D(texture, uv);

    float threshold = 0.01;
    if (distance(color.rgb, targetColor) < threshold) {
        color.rgb = replaceColor;
    }

    gl_FragColor = color;
}
