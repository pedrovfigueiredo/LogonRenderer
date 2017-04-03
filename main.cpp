#include "main.h"

int main()
{
    unsigned int x_resolution = 512;
    unsigned int y_resolution = 512;
    
    PinHoleCamera camera{ -1.25f,
        1.25f,
        -1.25f,
        1.25f,
        2.00f,
        glm::ivec2{ x_resolution, y_resolution },
        glm::vec3{ 0.0f, 1.0f,  2.50f },     // position
        glm::vec3{ 0.0f, 10.0f,  0.0f },     // up
        glm::vec3{ 0.0f, 1.0f,  -1.0f } };   // look at
    
    
    Scene scene;
    
    
    if (!scene.load("/Users/PedroFigueiredo/Documents/RayTracer/RayTracer/CornellBox-Original.obj"))
        return EXIT_FAILURE;
    
    
    //scene.load();
    
    Buffer rendering_buffer{ x_resolution, y_resolution };
    glm::vec3 background_color{ 0.0f, 0.0f, 0.0f };
    const int numRaysPerPixel = 1000;
    const int maximumDepth = 5;
    const int numThreads = 4;
    
    // Set up the renderer.
    PathTracer patht( camera,
                 scene,
                 background_color,
                 rendering_buffer,
                 numRaysPerPixel,
                 maximumDepth);
    
    patht.integrate(numThreads); // Renders the final image.
    
    // Save the rendered image to a .ppm file and convert it to a png afterwards.
    rendering_buffer.save( "/Users/PedroFigueiredo/Documents/RayTracer/RayTracer/output_image.ppm" );
    
    return EXIT_SUCCESS;
}

