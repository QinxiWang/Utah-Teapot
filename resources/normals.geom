#version 330

// You must specify what type of primitive is passed in
layout(triangles) in;

// ... and what type of primitive you are outputing and how many vertices. The geometry shader is run once
// for each primitive so we will output three lines (6 vertices), one for each normal.
layout(line_strip, max_vertices = 6) out;

uniform float normalLength;

uniform mat4 projection_mat, view_mat, model_mat;

// Example of a geometry shader custom input
in vec3 normal[3];

// Example of a geometry shader custom output
out vec3 color;

void main() {
    
    //TODO: Fill me in. For each vertex in the triangle, you need to output a line strip primitive with two vertices to draw the vertex normal.
    //Think carefully about where the vertices should be positioned in world space. The length of the drawn normal should be normalLength.
    //Each vertex needs to have gl_position set by multiplying the world space position by the model matrix, the view matrix, and finally the projection matrix (order matters!).
    //The first vertex should be colored red and the second one blue so it is easy to see what direction the normal is pointing.
    
    for(int i=0; i < 3; i++){
        // Just like in the vertex shader, geom shaders need to set the position of the vertex in screen space. We do this by multiplying the matrices.
        // The gl_in array holds the gl_positions that were set in the vertex shader for the vertices in this triangle.
        gl_Position = projection_mat * view_mat * model_mat * (gl_in[i].gl_Position + vec4(normal[i]*normalLength, 0.0));
        color = i % 2 == 0 ? vec3(0,0,1): vec3(1,0,0);
        // Tell the gpu that we want to send this vertex along the pipeline
        EmitVertex();
        gl_Position = projection_mat * view_mat * model_mat * gl_in[i].gl_Position;
        EmitVertex();
        EndPrimitive();
    }
    
}












