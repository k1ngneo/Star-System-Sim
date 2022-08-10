#vertex_shader
#version 330 core

layout (location = 0) in vec2 inPos;
layout (location = 1) in vec2 inUV;

out vec2 fragUV;

void main() {
	gl_Position = vec4(inPos, 0.0, 1.0);
	fragUV = inUV;
}

#fragment_shader
#version 330 core

in vec2 fragUV;

uniform sampler2D _texture;
uniform sampler2D _bloomTex;

layout (location = 0) out vec4 outColor;

void main() {
	vec3 mainImage = texture2D(_texture, fragUV).rgb;
	vec3 bloom = texture2D(_bloomTex, fragUV).rgb;

	outColor = vec4(mainImage + bloom, 1.0);
}