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
        brdf_ = {(brdf.x/255) / PI, (brdf.y/255) / PI, (brdf.z/255) / PI};
    }
    else
        brdf_ = {brdf.x / PI, brdf.y / PI, brdf.z / PI};
    emittance_ = emittance;
}

glm::vec3 GenericMaterial::getBRDF() const{
    return brdf_;
}

glm::vec3 GenericMaterial::getEmittance() const{
    return emittance_;
}
