#version 330 core

in Varyings {
    vec4 color;
    vec2 tex_coord;
} fs_in;

out vec4 frag_color;

uniform vec4 tint;
uniform sampler2D tex;



void main(){
    //TODO: (Req 7) Modify the following line to compute the fragment color
    // by multiplying the tint with the vertex color and with the texture color 
    // accessing the texture in a given position and returning the color in normalised RGBA
    vec4 tex_color = texture(tex, fs_in.tex_coord);
    frag_color = fs_in.color * tint * tex_color;
}