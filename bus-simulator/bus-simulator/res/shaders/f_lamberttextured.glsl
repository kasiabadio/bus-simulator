#version 330

uniform sampler2D tex;

out vec4 pixel_color; 

//Zmienne interpolowane
in vec2 i_tc;
in float i_nl;
in float i_rv;


void main(void) {
    vec4 color=texture(tex, i_tc);
	if (color.a < 0.1)
		discard;
	// ls*ks*rv = vec4(1,1,1,0)*vec4(1,1,1,0)*i_rv
	pixel_color=vec4(color.rgb*i_nl,color.a)+vec4(i_rv,i_rv,i_rv,0);
	//pixel_color=vec4(color.rgb*i_nl,color.a);
}
