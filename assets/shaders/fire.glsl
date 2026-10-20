const float circle_radius = 0.4f;
const vec2 quad_center = vec2(0.5);

void main()
{
    vec2 uv = vec2(sin(textureCoordinate.x * 2.0f), textureCoordinate.y);

    float delta = distance(uv, quad_center);
    float attenuation = (delta / circle_radius);

    float circle_fade = 1.0 - attenuation;

    if(circle_fade < circle_radius || opacity < 0.05) discard;

    vec4 ember_base_colour = vec4(1.0f, 0.1f, 0.0f, 0.1f);
    vec4 black = vec4(0.0f, 0.0f, 0.0f, 1.0f);

    vec4 ember = ember_base_colour + opacity;
    //  vec4 particle = mix(black, ember, circle_fade);
    vec4 particle = ember * circle_fade;

    outFragment = particle;

    return;
};