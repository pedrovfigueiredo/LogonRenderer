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
#include "ray.h"
#include "intersection_record.h"
#include "onb.h"
#include <memory>
#include <glm/glm.hpp>

class GenericMaterial : public Material
{
public:
    GenericMaterial(void);
    GenericMaterial(glm::vec3 emittance, glm::vec3 brdf);
    
    glm::vec3 getBRDF() const;
    glm::vec3 getEmittance() const;
    Ray getNewReflectedRay(Ray& ray, glm::vec3& position ,glm::vec3& normal);
};
#endif /* generic_material_h */
