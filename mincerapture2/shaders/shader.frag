#version 330 core

in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D TileMap;

uniform vec3 lightColor;
uniform vec3 lightdir = normalize(vec3(1.0, -2.0, -1.0));

void main()
{
	float normalFactor = -min(dot(lightdir, Normal), 0);
	FragColor = vec4(lightColor * texture(TileMap, TexCoord).xyz * min(max(normalFactor, 0.2) * 2.0, 1.0), 1.0);
	FragColor = vec4(vec3(0),1.0);
}