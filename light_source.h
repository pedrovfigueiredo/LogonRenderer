//
//  light_source.h
//  RayTracer
//
//  Created by Pedro Figueirêdo on 25/03/17.
//  Copyright © 2017 Lavid. All rights reserved.
//

#ifndef light_source_h
#define light_source_h
#include "material.h"
#include <memory>
#include <glm/glm.hpp>

class LightSource : public Material
{
public:
    LightSource(void);
    LightSource(glm::vec3 emittance);
    
    glm::vec3 getBRDF() const;
    glm::vec3 getEmittance() const;
};
#endif /* light_source_h */
