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
    type_ = type::GenericMaterial;
}

glm::vec3 GenericMaterial::getBRDF() const{
    return brdf_;
}

glm::vec3 GenericMaterial::getEmittance() const{
    return emittance_;
}

Ray GenericMaterial::getNewReflectedRay(Ray& ray, glm::vec3& position ,glm::vec3& normal){
    
    Ray reflectedRay;
    ONB onb;
    double theta, phi, r;
    double phiRandom = ((double)rand()/(RAND_MAX));
    double thetaRandom = ((double)rand()/(RAND_MAX));
    
    onb.setFromV(normal);
    
    theta = glm::acos(1 - thetaRandom);
    phi = 2 * Material::PI * phiRandom;
    r = 1;
    
    //order of 'y' and 'z' are inverted, given that it's used y axis as the vertical one(differing from normal pattern)
    glm::vec3 direction = {r * glm::sin(theta) * glm::cos(phi), r * glm::cos(theta) ,r * glm::sin(theta) * glm::sin(phi)};
    
    // Adding direction * 10e-03 for handling errors with numeric expressions
    reflectedRay = {position + (normal*0.001f), onb.getBasisMatrix() * direction};
    
    return reflectedRay;
}
