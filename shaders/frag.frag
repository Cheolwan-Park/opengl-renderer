#version 450 core

uniform vec3 light_color;

uniform sampler2D tex_object;

in VS_OUT
{
	float diffuse;
	vec2 uv;
	vec3 view_dir;
	vec3 reflection;

	vec3 normal;
} fs_in;

out vec4 color;

void main(){
	vec4 texture_color = texture(tex_object, fs_in.uv);
	vec3 diffuse = vec3(clamp(fs_in.diffuse, 0, 1));

	vec3 reflection = normalize(fs_in.reflection);
	vec3 view_dir = normalize(fs_in.view_dir);
	float specular = 0.0f;
	vec3 specular_col = vec3(0, 0, 0);
	if(diffuse.r > 0.0) {
		specular = clamp(dot(reflection, -view_dir), 0, 1);
		specular = pow(specular, 20.0);
		specular_col = vec3(specular) * light_color;
	}

	vec3 ambient = vec3(0.1f, 0.1f, 0.1f);

	diffuse += ambient;
	diffuse *= light_color * texture_color.rgb;

	color = vec4(diffuse + specular, 1);
//	color = vec4(fs_in.normal, 1);
}