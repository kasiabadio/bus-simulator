#version 330

//Zmienne jednorodne
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

uniform vec4 light_dir_first = vec4(-5,10,-5,0);

layout (location=0) in vec4 vertex; 
layout (location=1) in vec4 normal; 
layout (location=2) in vec2 texCoord; 


out vec2 i_tc;
out float i_nl;
out float i_rv;

void main(void) {
    
    gl_Position=P*V*M*vertex;

    vec4 n = normalize(V*M*normal);
    vec4 l = normalize(V*light_dir_first - V*M*vertex);
    // znormalizowany wektor r w przestrzeni oka
    vec4 r = reflect(-l, n);
    
    // znormalizowany wektor v w przestrzeni oka

    /*vertex jest w przestrzeni modelu dlatego mno¿ymy
    zeby przeniesc z przestrzeni modelu do œwiata i potem razy macierz widoku
    zeby przeniesc z przestrzeni swiata do przestrzeni oka */

    vec4 v = normalize(vec4(0,0,0,1) - V*M*vertex);

    // test czy mieœci siê w przedziale dwóch argumentów
    i_nl = clamp(dot(n, l), 0, 1);
    
    i_rv = pow(clamp(dot(r, v), 0, 1), 40);

    i_tc = texCoord;
}
