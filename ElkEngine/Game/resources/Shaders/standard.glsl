#shader vertex
#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

out vec3 v_pos;
out vec3 v_normal;
out vec2 v_texCoord;

void main()
{
	v_pos = vec3(u_model * vec4(position, 1.0));
	v_normal = normalize(transpose(inverse(mat3(u_model))) * normal);
	v_texCoord = texCoord;

	gl_Position = u_proj * u_view * vec4(v_pos, 1.0);
}

#shader fragment
#version 330 core

struct Material
{
    vec3 color;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 emissive;
    float shininess;
    bool forceNoTexture;
};

struct Light
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 color;
    float attenuation;
};

in vec3 v_pos;
in vec3 v_normal;
in vec2 v_texCoord;

uniform sampler2D	texture_diffuse1;

uniform bool        u_hasTexture;

uniform vec3		u_viewPos;
uniform Material	u_material;
uniform Light		u_light;

out vec4 FragColor;

void main()
{
    // Determine surface color
    vec4 surfaceColor = u_material.forceNoTexture || !u_hasTexture ? vec4(u_material.color, 1.0f) : texture(texture_diffuse1, v_texCoord) * vec4(u_material.color, 1.0f);

    // Calculate some normals
    vec3 surfaceToLight = normalize(u_light.position - v_pos);
    vec3 surfaceToCamera = normalize(u_viewPos - v_pos);

    // ambient
    vec3 ambient = u_light.ambient * surfaceColor.rgb * u_light.color;

    // diffuse
    float diffuseCoeff = max(0.0f, dot(v_normal, surfaceToLight));
    vec3 diffuse = diffuseCoeff * surfaceColor.rgb * u_light.color;

    // specular
    float specularCoefficient = 0.0f;
    if (diffuseCoeff > 0.0f)
        specularCoefficient = pow(max(0.0, dot(surfaceToCamera, reflect(-surfaceToLight, v_normal))), u_material.shininess);
    vec3 specular = specularCoefficient * u_material.specular * u_light.color;

    // emissive
    vec3 emissive = u_material.emissive;

    // attenuation
    float distanceToLight = length(u_light.position - v_pos);
    float attenuation = 1.0f / (1.0f + u_light.attenuation * pow(distanceToLight, 2));

    // linear color (before gamma correction)
    vec3 linearColor = ambient + emissive + attenuation * (diffuse + specular);

    // Gamme calculation
    vec3 gamma = vec3(1.0f / 2.2f);

    // Final color
    FragColor = vec4(pow(linearColor, gamma), surfaceColor.a);
}
