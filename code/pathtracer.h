#ifndef PATHTRACER_H_
#define PATHTRACER_H_

#include <sstream>
#include <iomanip>
#include <cmath>
#include <random>
#include <algorithm>
#include <ctime>
#include <thread>
#include <chrono>

#include "camera.h"
#include "scene.h"
#include "buffer.h"
#include "onb.h"

class PathTracer
{
public:

    PathTracer( Camera &camera,
               const Scene &scene,
               const glm::vec3 background_color,
               Buffer &buffer,
               const int numRaysperPixel,
               const int maximumDepth);

    void integrate( std::size_t numThreads );

private:

    void integrate_parallel();

    glm::vec3 calculateRadiance(Ray& ray, int currDepth, float distanceInObject);

    void printProgress(struct timespec& begin);

    void updateEstimatedTime(double& elapsed_secs, double& estimated_secs);

    const Camera &camera_;

    const Scene &scene_;

    glm::dvec3 background_color_;

    Buffer &buffer_;

    const int numRaysperPixel_;

    const int maximumDepth_;

    std::atomic_int blockController;

    //For estimating time left
    //double lastSecs;

};

#endif /* PathTracer.h */
