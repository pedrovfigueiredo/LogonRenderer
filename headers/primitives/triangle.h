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
#include <algorithm>
#include <vector>
#include "primitive.h"
#include "intersection_record.h"
#include "ray.h"
#include "material.h"

class Triangle : public Primitive
{
public:
    Triangle( void );

    Triangle( Material* material, const glm::vec3 &a, const glm::vec3 &b, const glm::vec3 &c);

    bool intersect( const Ray &ray,
                   IntersectionRecord &intersection_record ) const;


    glm::vec3 a_ = { 0.0f, 0.0f, 0.0f };
    glm::vec3 b_ = { 0.0f, 0.0f, 0.0f };
    glm::vec3 c_ = { 0.0f, 0.0f, 0.0f };

private:
    static const float kIntersectionTestEpsilon_;
};

#endif /* triangle_h */
