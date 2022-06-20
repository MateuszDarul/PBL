#version 450 core

in vec2 vertexTexture;
in float lifetimePercentage;

out vec4 FragColor;

uniform sampler2D particleTexture;

uniform vec4 u_ColorStart;
uniform vec4 u_ColorEnd;

void main()
{
    vec4 frag = texture(particleTexture, vertexTexture);

    float t = min(1.0, max(0.0, lifetimePercentage));

    vec4 col = u_ColorStart * (1.0 - t) + u_ColorEnd * t;
    
    frag.rgb = frag.rgb * col.rgb;
    frag.a *= col.a;

    if(frag.a < 0.05)
	    discard;
    FragColor = frag;
}
