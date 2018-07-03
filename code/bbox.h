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
    bool intersect(const Ray& ray) const;

    Bbox* leftChild = nullptr;
    Bbox* rightChild = nullptr;
    std::vector<int> primitives_id_;
    glm::vec3 center;
    glm::vec3 negativeCorner;
    glm::vec3 positiveCorner;
};


#endif /* bbox_h */
