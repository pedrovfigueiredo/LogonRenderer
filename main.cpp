#include "main.h"

int main()
{
    unsigned int x_resolution = 512;
    unsigned int y_resolution = 512;
    
    PinHoleCamera camera{ -1.0f,
        1.0f,
        -1.0f,
        1.0f,
        1.0f,
        glm::ivec2{ x_resolution, y_resolution },
        glm::vec3{ 0.0f, 0.12f,  2.5f },     // position
        glm::vec3{ 0.0f, 1.0f,  0.0f },     // up
        glm::vec3{ 0.0f, 0.0f,  -1.0f } };   // look at
    

    Scene scene;
    
    std::vector<Object*> objects;
    
    //objects.push_back(new Object("Happy_Budda_Copper","/Users/PedroFigueiredo/Downloads/OBJs_baixados/happy_recon/happy_vrip.ply", new RoughConductor(0.05, glm::vec3{0.95, 0.64, 0.54}), glm::vec3{0.09,-0.05,-0.075}));
    //objects.push_back(new Object("Happy_Budda_Silver","/Users/PedroFigueiredo/Downloads/OBJs_baixados/happy_recon/happy_vrip.ply", new  RoughConductor(0.05, glm::vec3{0.95, 0.93, 0.88}), glm::vec3{-0.09,-0.05,-0.075}));
    //objects.push_back(new Object("Happy_Budda_Gold","/Users/PedroFigueiredo/Downloads/OBJs_baixados/happy_recon/happy_vrip.ply", new RoughConductor(0.05, glm::vec3{1.00, 0.71, 0.29}), glm::vec3{0.0,-0.05,0}));
    //objects.push_back(new Object("Floor","/Users/PedroFigueiredo/Downloads/OBJs_baixados/floor3.obj", new Mirror(), new Transformations(1, {}, {0,0,0})));
    //objects.push_back(new Object("Light","/Users/PedroFigueiredo/Downloads/OBJs_baixados/light.obj", new LightSource(glm::vec3{10,10,10}), new Transformations(1, {}, {0,0,2})));
    //objects.push_back(new Object("Happy_Budda_Copper","/Users/PedroFigueiredo/Downloads/OBJs_baixados/happy_recon/happy_vrip.ply", new RoughConductor(0.5, glm::vec3{0.95, 0.64, 0.54}), glm::vec3{0.0,-0.05,0}));
    //objects.push_back(new Object("Monkey", "/Users/PedroFigueiredo/Downloads/OBJs_baixados/monkey90.obj", new SmoothDieletric()));
    //objects.push_back(new Object("Golden_Dragon","/Users/PedroFigueiredo/Downloads/OBJs_baixados/dragon_recon/dragon_vrip.ply", new RoughConductor(0.05, glm::vec3{1.00, 0.71, 0.29}), new Transformations(5, {}, {0,0.7,-0.4})));
    //objects.push_back(new Object("CornellBox_Empty","/Users/PedroFigueiredo/Documents/RayTracer/RayTracer/CornellBox-Empty-CO.obj", new Transformations(1.5, {}, {0,0,0})));
    //objects.push_back(new Object("Corvette","/Users/PedroFigueiredo/Downloads/Chevrolet+Corvette+C7+Stingray+b.obj",new RoughConductor(0.05, glm::vec3{1.00, 0.71, 0.29}), new Transformations(0.1, 90, Transformations::Axis::X, {0,0.2,0})));
    //objects.push_back(new Object("Golden_Asian_Dragon","/Users/PedroFigueiredo/Downloads/OBJs_baixados/xyzrgb_dragon.ply", new RoughConductor(0.05, glm::vec3{1.00, 0.71, 0.29}) , glm::vec3{-0.03,0.1,-0.01}, 0.0015));
    
    objects.push_back(new Object("Box","/Users/PedroFigueiredo/Downloads/OBJs_baixados/FinalScene/box.obj", new Transformations(1.5, {}, {0,0,0})));
    objects.push_back(new Object("Golden_Dragon_Left","/Users/PedroFigueiredo/Downloads/OBJs_baixados/dragon_recon/dragon_vrip.ply", new RoughConductor(0.05, glm::vec3{1.00, 0.71, 0.29}), new Transformations(3, {}, {-0.6,0.85,-1.45})));
    objects.push_back(new Object("Golden_Dragon_Right","/Users/PedroFigueiredo/Downloads/OBJs_baixados/dragon_recon/dragon_vrip.ply", new RoughConductor(0.05, glm::vec3{1.00, 0.71, 0.29}), new Transformations(3, {new Transformations::Rotation{Transformations::Axis::Y, 180}}, {0.6,0.85,-1.45})));
    
    //scene.load(new Scene::AcelerationMethod(Scene::AcelerationMethod::BVH), new BVH::SplitMethod(BVH::SplitMethod::SAH));
    if (!scene.load(objects, new Scene::AcelerationMethod(Scene::AcelerationMethod::BVH), new BVH::SplitMethod(BVH::SplitMethod::SAH)))
        return EXIT_FAILURE;
    
    
    Buffer rendering_buffer{ x_resolution, y_resolution };
    glm::vec3 background_color{ 0.0f, 0.0f, 0.0f };
    const int numRaysPerPixel = 10000;
    const int maximumDepth = 10;
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

