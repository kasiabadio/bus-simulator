#version 330

//Zmienne jednorodne
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

uniform vec4 light_dir_first = vec4(-50,100,-30,0);
uniform vec4 light_dir_second = vec4(50,50,20,0);
uniform vec4 light_dir_third = vec4(25,-100,-180,0);
uniform vec4 light_dir_fourth = vec4(-25,-50,200,0);

layout (location=0) in vec4 vertex; 
layout (location=1) in vec4 normal; 
layout (location=2) in vec2 texCoord; 


out vec2 i_tc;
out float i_nl[4];
out float i_rv[4];

void main(void) {
    
    gl_Position=P*V*M*vertex;

    vec4 n = normalize(V*M*normal);
    vec4 l_first = normalize(V*light_dir_first - V*M*vertex); //lightDir
    vec4 l_second = normalize(V*light_dir_second - V*M*vertex);
    vec4 l_third = normalize(V*light_dir_third - V*M*vertex);
    vec4 l_fourth = normalize(V*light_dir_fourth - V*M*vertex);

    // znormalizowany wektor r w przestrzeni oka
    vec4 r_first = reflect(-l_first, n); //reflectDir
    vec4 r_second = reflect(-l_second, n); //reflectDir
    vec4 r_third = reflect(-l_third, n);
    vec4 r_fourth = reflect(-l_fourth, n);
    
    // znormalizowany wektor v w przestrzeni oka
    // od powierzchni do obserwatora
    /*vertex jest w przestrzeni modelu dlatego mno¿ymy
    zeby przeniesc z przestrzeni modelu do œwiata i potem razy macierz widoku
    zeby przeniesc z przestrzeni swiata do przestrzeni oka */

    vec4 v = normalize(vec4(0,0,0,1) - V*M*vertex); 

    // test czy mieœci siê w przedziale dwóch argumentów
    i_nl[0] = clamp(dot(n, l_first), 0, 1);
    i_nl[1] = clamp(dot(n, l_second), 0, 1);
    i_nl[2] = clamp(dot(n, l_third), 0, 1);
    i_nl[3] = clamp(dot(n, l_fourth), 0, 1);

    i_rv[0] = pow(clamp(dot(r_first, v), 0, 1), 140); 
    i_rv[1] = pow(clamp(dot(r_second, v), 0, 1), 180); 
    i_rv[2] = pow(clamp(dot(r_third, v), 0, 1), 180); 
    i_rv[3] = pow(clamp(dot(r_fourth, v), 0, 1), 180); 

    i_tc = texCoord;
}