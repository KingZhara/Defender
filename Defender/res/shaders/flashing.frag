uniform sampler2D textureaa;
uniform vec3 targetColor;
uniform vec3 replaceColor;

void main() {
    vec2 uv = gl_FragCoord.xy / vec2(textureSize(texture, 0));
    vec4 color = texture(texture, uv);

    if (color.rgb == targetColor) {
        color.rgb = replaceColor;
    }

    gl_FragColor = color;
}