#version 330 core


in vec3 texture_coordinates;
uniform sampler3D basic_texture;
out vec4 frag_color;


//out vec4 FragColor;
void main()

{	
	//frag_color = vec4(1.0f, 1.0f, 0.0f, 1.0f);
	vec4 texel = texture(basic_texture, texture_coordinates);
	
	frag_color = texel;		
	
};