#include "pathtracer.h"

PathTracer::PathTracer( Camera &camera,
                      const Scene &scene,
                      const glm::vec3 background_color,
                      Buffer &buffer,
                      const int numRaysperPixel,
                      const int maximumDepth) :
        camera_( camera ),
        scene_( scene ),
        background_color_{ background_color },
        buffer_( buffer ),
        numRaysperPixel_(numRaysperPixel),
        maximumDepth_(maximumDepth)
{}

void PathTracer::integrate( std::size_t numThreads )
{

    // Image space origin (i.e. x = 0 and y = 0) at the top left corner.
    
    struct timespec start;
    
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    std::thread progressTracker(&PathTracer::printProgress, this, std::ref(start));
    std::vector<std::thread*> threads(numThreads);
    
    for (std::size_t thread = 0; thread < numThreads; thread++) {
        threads[thread] = new std::thread(&PathTracer::integrate_parallel, this);
    }
    
    for (std::size_t thread = 0; thread < numThreads; thread++) {
        threads[thread]->join();
    }
    
    progressTracker.join();
}

void PathTracer::integrate_parallel(){
    
    std::size_t block, initX, initY, endX, endY, blocksizeX, blocksizeY;
    double xRandom, yRandom;
    
    if ((!(buffer_.h_resolution_ % 16)) && (!(buffer_.v_resolution_ % 16))) {
        blocksizeX = buffer_.h_resolution_ / 16;
        blocksizeY = buffer_.v_resolution_ / 16;
    }else{
        blocksizeX = buffer_.h_resolution_ / 15;
        blocksizeY = buffer_.v_resolution_ / 15;
    }
    
    while (true) {
        block = blockController++;
        
        if (block > 255) break;
        
        initX = (block % 16) * blocksizeX;
        initY = (block / 16) * blocksizeY;
        
        endX = std::min(initX + blocksizeX, (std::size_t)buffer_.h_resolution_);
        endY = std::min(initY + blocksizeY, (std::size_t)buffer_.v_resolution_);
        
        // Loops over image rows
        for (std::size_t y = initY; y < endY; y++ )
        {
            
            // Loops over image columns
            for ( std::size_t x = initX; x < endX; x++ )
            {
                for (std::size_t n = 0; n < numRaysperPixel_; n++) {
                    
                    xRandom = ((double)rand()/(RAND_MAX));
                    yRandom = ((double)rand()/(RAND_MAX));
                    
                    Ray ray{ camera_.getWorldSpaceRay( glm::vec2{ x + xRandom, y + yRandom } ) };
                    
                    buffer_.buffer_data_[x][y] += calculateRadiance(ray, 0) ;
                }
                
                buffer_.buffer_data_[x][y] /= numRaysperPixel_;
                
            }
        }
    }
    
}

glm::vec3 PathTracer::calculateRadiance(Ray& ray, int currDepth){
    
    glm::vec3 lo = {0,0,0};
    float pdf = 1.0f/(2.0f * Material::PI);
    IntersectionRecord intersection_record;
    
    if (currDepth < maximumDepth_) {
        intersection_record.t_ = std::numeric_limits< double >::max();
        
        if ( scene_.intersect( ray, intersection_record ) ){
            /*
            // If it's light source, there's no additional computing needed (return at next recursive call)
            if (intersection_record.material->getEmittance() != glm::vec3 {0,0,0})
                currDepth = maximumDepth_;
            */
            Ray reflectedRay = getNewReflectedRay(intersection_record);
            
            lo = (intersection_record.material->getEmittance() +
                 (intersection_record.material->getBRDF() *
                 calculateRadiance(reflectedRay, currDepth + 1) *
                 glm::dot(intersection_record.normal_, reflectedRay.direction_))/pdf);
        }
    }
    
    return lo;
}

Ray PathTracer::getNewReflectedRay(IntersectionRecord &intersection_record){
    
    Ray ray;
    ONB onb;
    double theta, phi, r;
    double phiRandom = ((double)rand()/(RAND_MAX));
    double thetaRandom = ((double)rand()/(RAND_MAX));
    
    theta = glm::acos(1 - thetaRandom);
    phi = 2 * Material::PI * phiRandom;
    r = 1;
    
    onb.setFromV(intersection_record.normal_);
    
    //order of 'y' and 'z' are inverted, given that it's used y axis as the vertical one(differing from normal pattern)
    glm::vec3 direction = {r * glm::sin(theta) * glm::cos(phi), r * glm::cos(theta) ,r * glm::sin(theta) * glm::sin(phi)};
    
    // Adding direction * 10e-03 for handling errors with numeric expressions
    ray = {intersection_record.position_ + (intersection_record.normal_*0.001f), onb.getBasisMatrix() * direction};
    
    return ray;
}


void PathTracer::printProgress(struct timespec& begin){
    
    int block = 0;
    
    double estimated_secs = INFINITY;
    double elapsed_secs = 0;
    struct timespec finish;
    
    std::thread estSecs(&PathTracer::updateEstimatedTime, this, std::ref(elapsed_secs), std::ref(estimated_secs));
    
    while (block < 255) {
        block = blockController;
        clock_gettime(CLOCK_MONOTONIC, &finish);
        elapsed_secs = double(finish.tv_sec - begin.tv_sec);
        
        std::stringstream progress_stream;
        progress_stream << "\r  Progress: "
        << std::fixed << std::setw( 6 )
        << std::setprecision( 2 )
        << (100.0f / 256.0f) * (float)block
        << "%"
        << " "
        << "(" << block << "/256) "
        << "Elapsed time: " << ((int)(elapsed_secs/60))/60 << "h " << ((int)(elapsed_secs/60)) % 60 << "m " << ((int)round(elapsed_secs)) % 60 << "s"
        << " "
        << "Estimated time left: " << ((int)((estimated_secs - elapsed_secs)/60))/60 << "h " << ((int)((estimated_secs - elapsed_secs)/60)) % 60 << "m " << ((int)round((estimated_secs - elapsed_secs))) % 60 << "s"
        << std::endl;
        
        
        std::clog << progress_stream.str();
        
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    estSecs.join();
}

void PathTracer::updateEstimatedTime(double& elapsed_secs, double& estimated_secs){
    unsigned int period = numRaysperPixel_/100;
    int block = 0;
    while(block < 255){
        block = blockController;
        estimated_secs = ((256*elapsed_secs)/(block + 1));
        std::this_thread::sleep_for(std::chrono::seconds(period));
    }
}

