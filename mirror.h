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
    
    glm::vec3 getBRDF() const;
    glm::vec3 getEmittance() const;
    Ray getNewReflectedRay(Ray& ray, glm::vec3& position ,glm::vec3& normal);
};


#endif /* mirror_h */
