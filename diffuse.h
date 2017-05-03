//
//  diffuse.h
//  RayTracer
//
//  Created by Pedro Figueirêdo on 24/03/17.
//  Copyright © 2017 Lavid. All rights reserved.
//

#ifndef diffuse_h
#define diffuse_h
#include "material.h"
#include "ray.h"
#include "intersection_record.h"
#include "onb.h"
#include <memory>
#include <glm/glm.hpp>

class Diffuse : public Material
{
public:
    Diffuse(void);
    Diffuse(glm::vec3 color);
    
    glm::vec3 getfr(glm::vec3 w_i, glm::vec3 w_o) const;
    glm::vec3 getEmittance() const;
    glm::vec3 getNewDirection(glm::vec3& w_i);
};

#endif /* diffuse_h */
