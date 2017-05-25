//
//  mirror.h
//  RayTracer
//
//  Created by Pedro Figueirêdo on 28/04/17.
//  Copyright © 2017 Lavid. All rights reserved.
//

#ifndef mirror_h
#define mirror_h

#include "material.h"
#include "ray.h"
#include "intersection_record.h"
#include "onb.h"
#include <memory>
#include <glm/glm.hpp>

class Mirror : public Material
{
public:
    Mirror(void);
    
    glm::vec3 getfr(glm::vec3& w_i, glm::vec3& w_o) const;
    glm::vec3 getEmittance() const;
    glm::vec3 getNewDirection(glm::vec3& w_i);
};


#endif /* mirror_h */
