#version 330 core

out vec4 frag_color;

// In this shader, we want to draw a checkboard where the size of each tile is (size x size).
// The color of the bottom-left most tile should be "colors[0]" and the 2 tiles adjacent to it
// should have the color "colors[1]".

//TODO: (Req 1) Finish this shader.

uniform int size = 32;
uniform vec3 colors[2];

void main(){
    // Get X and Y coordinate and divide it over size to get the tile
    int x_tile = int(gl_FragCoord.x) / size;
    int y_tile = int(gl_FragCoord.y) / size;
    // coloring the tile with the correct color (the bottom-left most tile be colors[0] and the 2 tiles adjacent color[1] )
    // for example tile[0,0] will be color[0] , 2 tiles adjacent tile[1,0] will be color[1] and tile[0,1] will be color[1]
    // tile [1,1] will will be color[0] because x_tile = 1 , y_tile = 1 and (x_tile + y_tile) % 2 = 0 which is color[0] and so on
    vec3 tile_color = colors[(x_tile + y_tile) % 2];
    frag_color = vec4(tile_color, 1.0);
}