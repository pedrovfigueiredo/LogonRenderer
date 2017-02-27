//
//  triangle.h
//  RayTracer
//
//  Created by Pedro Figueirêdo on 10/02/17.
//  Copyright © 2017 Lavid. All rights reserved.
//

#ifndef triangle_h
#define triangle_h

#include <glm/glm.hpp>
#include "primitive.h"
#include "intersection_record.h"
#include "ray.h"

class Triangle : public Primitive
{
public:
    Triangle( void );
    
    Triangle( glm::vec3 color, const glm::vec3 &a, const glm::vec3 &b, const glm::vec3 &c);
    
    bool intersect( const Ray &ray,
                   IntersectionRecord &intersection_record ) const;
    
    
    glm::vec3 a_ = { 0.0f, 0.0f, 0.0f };
    glm::vec3 b_ = { 0.0f, 0.0f, 0.0f };
    glm::vec3 c_ = { 0.0f, 0.0f, 0.0f };
    
    const float determinant3x3(float a, float b, float c, float d, float e, float f, float g, float h, float i) const;
};

#endif /* triangle_h */
