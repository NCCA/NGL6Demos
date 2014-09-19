#version 400
uniform sampler2D tex;
/// @brief MVP passed from app
uniform mat4 MVP;
// first attribute the vertex values from our VAO
layout (location = 0) in vec3 inVert;
// second attribute the UV values from our VAO
layout (location = 1) in vec2 inUV;
// we use this to pass the UV values to the frag shader
out vec2 vertUV;




void main()
{
    float fx = 594.21f;
        float fy = 591.04f;
        float a = -0.0030711f;
        float b = 3.3309495f;
        float cx = 339.5f;
        float cy = 242.7f;
        mat4 mat = mat4(
            1/fx,     0,  0, 0,
            0,   -1/fy,  0, 0,
            0,       0,  0, a,
            -cx/fx, cy/fy, -1, b
        );

    // pre-calculate for speed we will use this a lot

    // calculate the vertex position
    gl_Position = MVP*vec4(inVert, 1.0);
    // pass the UV values to the frag shader
    vertUV=inUV.st;
}
