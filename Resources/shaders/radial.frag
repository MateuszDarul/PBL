#version 450 core

in vec2 vertexTexture;
in vec3 VertexPosition;

out vec4 FragColor;

uniform sampler2D diffuseMapData;
uniform sampler2D specularMapData;
uniform sampler2D normalMapData;

uniform vec4 u_TintColor; //HACK - using rgba channels not for color: r = progress, g = secondary size, b = hollow factor, a = 


void main()
{
    if (length(VertexPosition) < u_TintColor.b) discard;

    float o = 0.0;

    vec3 point = normalize(VertexPosition);
    // float angle = dot(vec3(1.0, 0.0, 0.0), point) * 0.5 + 0.5;
    float angle = acos(dot(vec3(-1.0, 0.0, 0.0), point)) * 0.31830989 * (1.0 + 2.0*o) - o; // 1 / pi

    if (angle < 0.0 || angle > 1.0) discard;

    vec4 mainColor = { 1.0, 1.0, 1.0,  1.0 };
    vec4 backColor = { 0.5, 0.5, 0.5,  0.5 };
    vec4 secondaryColor = { 1.0, 0.0, 0.0,  1.0 };
    
    if (angle > u_TintColor.r)
        FragColor = texture(diffuseMapData, vertexTexture) * backColor;
    else if (angle > u_TintColor.r - u_TintColor.g)
        FragColor = texture(diffuseMapData, vertexTexture) * secondaryColor;
    else
        FragColor = texture(diffuseMapData, vertexTexture) * mainColor;
}
