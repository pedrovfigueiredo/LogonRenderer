#ifndef SCENE_H_
#define SCENE_H_

#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include <random>

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include "primitive.h"
#include "sphere.h" 
#include "triangle.h"
#include "diffuse.h"
#include "light_source.h"
#include "generic_material.h"
#include "mirror.h"
#include "smooth_dieletric.h"
#include "rough_conductor.h"
#include "bvh.h"
#include "object.h"

class Scene
{
public:
    
    enum AcelerationMethod{ NONE, BVH};

    Scene( void );

    ~Scene( void );

    bool intersect( const Ray &ray,
                    IntersectionRecord &intersection_record ) const;
    
    void load(const AcelerationMethod* method, const BVH::SplitMethod* splitMethod);
    
    bool load( std::vector<Object*> &objects, const AcelerationMethod* method, const BVH::SplitMethod* splitMethod);
    
    
    AcelerationMethod method_ = BVH;
    
private:
    
    void setMethod(const BVH::SplitMethod* splitMethod);
    
    class BVH* bvh;

    std::vector< Primitive::PrimitiveUniquePtr > primitives_;

};

#endif /* SCENE_H_ */

