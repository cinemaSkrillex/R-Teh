uniform sampler2D texture;
uniform float saturation;
uniform float gamma;

void main() {
    vec4 color = texture2D(texture, gl_TexCoord[0].xy);
    float gray = dot(color.rgb, vec3(0.299, 0.587, 0.114));
    vec3 desaturated = vec3(gray);
    vec3 adjustedColor = mix(desaturated, color.rgb, saturation);
    adjustedColor = pow(adjustedColor, vec3(1.0 / gamma));
    gl_FragColor = vec4(adjustedColor, color.a);
}