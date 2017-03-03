#include "main.h"

int main()
{
    unsigned int x_resolution = 512;
    unsigned int y_resolution = 512;
    
    PinHoleCamera camera{ -1.25f,
        1.25f,
        -1.25f,
        1.25f,
        3.00f,
        glm::ivec2{ x_resolution, y_resolution },
        glm::vec3{ 0.0f, 0.0f,  1.0f },     // position
        glm::vec3{ 0.0f, 1.0f,  0.0f },     // up
        glm::vec3{ 0.0f, 0.0f,  -1.0f } };   // look at
    Scene scene{};
    
    //scene.load();
    
    if (!scene.load("/Users/PedroFigueiredo/Documents/RayTracer/RayTracer/teapot.obj"))
        return EXIT_FAILURE;
    
    Buffer rendering_buffer{ x_resolution, y_resolution };
    glm::vec3 background_color{ 0.0f, 0.0f, 0.0f };
    
    // Set up the renderer.
    RayTracer rt( camera,
                 scene,
                 background_color,
                 rendering_buffer );
    
    rt.integrate(); // Renders the final image.
    
    // Save the rendered image to a .ppm file.
    rendering_buffer.save( "/Users/PedroFigueiredo/Documents/RayTracer/RayTracer/output_image.ppm" );
    
    return EXIT_SUCCESS;
}

