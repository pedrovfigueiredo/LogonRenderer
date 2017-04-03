//
//  generic_material.cpp
//  RayTracer
//
//  Created by Pedro Figueirêdo on 30/03/17.
//  Copyright © 2017 Lavid. All rights reserved.
//

#include "generic_material.h"

GenericMaterial::GenericMaterial(void)
{}

GenericMaterial::GenericMaterial(glm::vec3 emittance, glm::vec3 brdf){
    if (brdf.x > 1 || brdf.y > 1 || brdf.z > 1){
        brdf_ /= 255;
        brdf_ /= PI;
    }
    else
        brdf_ /= PI;
    emittance_ = emittance;
}

glm::vec3 GenericMaterial::getBRDF() const{
    return brdf_;
}

glm::vec3 GenericMaterial::getEmittance() const{
    return emittance_;
}
