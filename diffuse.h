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
#include <memory>
#include <glm/glm.hpp>

class Diffuse : public Material
{
public:
    Diffuse(void);
    Diffuse(glm::vec3 color);
    
    glm::vec3 getBRDF() const;
    glm::vec3 getEmittance() const;
};

#endif /* diffuse_h */
