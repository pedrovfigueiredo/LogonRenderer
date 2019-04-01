//
//  acceleration_structure.h
//  RayTracer
//
//  Created by Pedro Figueirêdo on 12/03/19.
//  Copyright © 2019 Lavid. All rights reserved.
//

#ifndef acceleration_structure_h
#define acceleration_structure_h

#include <vector>

#include "ray.h"
#include "intersection_record.h"
#include "primitive.h"

class AccelerationStructure
{
public:
    
    AccelerationStructure(const std::vector< Primitive::PrimitiveUniquePtr > &primitives):primitives_(primitives){}
    
    virtual ~AccelerationStructure(){}
    
    virtual bool intersect( const Ray &ray,
                           IntersectionRecord &intersection_record ) const = 0;
    
    virtual void construct() = 0;
    
protected:
    glm::vec3 max_components(const glm::vec3 &vecA, const glm::vec3 &vecB){
        return glm::vec3{std::max({vecA.x, vecB.x}), std::max({vecA.y, vecB.y}), std::max({vecA.z, vecB.z})};
    }
    
    
    glm::vec3 min_components(const glm::vec3 &vecA, const glm::vec3 &vecB){
        return glm::vec3{std::min({vecA.x, vecB.x}), std::min({vecA.y, vecB.y}), std::min({vecA.z, vecB.z})};
    }
    
    const std::vector< Primitive::PrimitiveUniquePtr > &primitives_;
};

#endif /* acceleration_structure_h */
