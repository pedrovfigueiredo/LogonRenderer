//
//  bbox.h
//  RayTracer
//
//  Created by Pedro Figueirêdo on 14/05/17.
//  Copyright © 2017 Lavid. All rights reserved.
//

#ifndef bbox_h
#define bbox_h
#include <glm/glm.hpp>
#include <vector>
#include <algorithm>
#include <iterator>
#include "primitive.h"
#include "ray.h"
#include "intersection_record.h"

class Bbox
{
public:
    Bbox();
    Bbox( const glm::vec3 &min,
         const glm::vec3 &max ) :
    negativeCorner{ min },
    positiveCorner{ max },
    center{ 0.5f * ( min + max ) }
    {};
    
    bool intersect(const Ray& ray) const;

    glm::vec3 center;
    glm::vec3 negativeCorner;
    glm::vec3 positiveCorner;
};


#endif /* bbox_h */
