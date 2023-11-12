//MSDF Text Shader

#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in int a_EntityID;

uniform mat4 u_ViewProjection;

out vec4 v_Color;
out vec2 v_TexCoord;
out flat int v_EntityID;

void main() {
	v_Color = a_Color;
	v_TexCoord = a_TexCoord;
	v_EntityID = a_EntityID;
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 color;
layout(location = 1) out int entityID;

in vec4 v_Color;
in vec2 v_TexCoord;
in flat int v_EntityID;

uniform sampler2D u_FontAtlas;

float screenPxRange() {
	const float pxRange = 2.0f;
	vec2 unitRange = vec2(pxRange)/vec2(textureSize(u_FontAtlas, 0));
	vec2 screenTexSize = vec2(1.0)/fwidth(v_TexCoord);
	return max(0.5*dot(unitRange, screenTexSize), 1.0);
}

float median(float r, float g, float b) {
	return max(min(r, g), min(max(r, g), b));
}

void main() {
	vec3 msd = texture(u_FontAtlas, v_TexCoord).rgb;
	float sd = median(msd.r, msd.g, msd.b);
	float screenPxDistance = screenPxRange()*(sd - 0.5);
	float opacity = clamp(screenPxDistance + 0.5, 0.0, 1.0);
	if (opacity == 0)
		discard;
	vec4 bgColor = vec4(vec3(0.0), 0.0);
	color = mix(bgColor, v_Color, opacity);
	if (color.a == 0)
		discard;
	entityID = v_EntityID;
}