//
//  generic_material.h
//  RayTracer
//
//  Created by Pedro Figueirêdo on 30/03/17.
//  Copyright © 2017 Lavid. All rights reserved.
//

#ifndef generic_material_h
#define generic_material_h

#include "material.h"
#include <memory>
#include <glm/glm.hpp>

class GenericMaterial : public Material
{
public:
    GenericMaterial(void);
    GenericMaterial(glm::vec3 emittance, glm::vec3 brdf);
    
    glm::vec3 getBRDF() const;
    glm::vec3 getEmittance() const;
};
#endif /* generic_material_h */
