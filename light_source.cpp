//
//  light_source.cpp
//  RayTracer
//
//  Created by Pedro Figueirêdo on 25/03/17.
//  Copyright © 2017 Lavid. All rights reserved.
//

#include "light_source.h"

LightSource::LightSource(void)
{}

LightSource::LightSource( glm::vec3 emittance):
Material::Material{emittance, {0,0,0}}
{type_ = type::LightSource;}

glm::vec3 LightSource::getBRDF() const{
    return brdf_;
}

glm::vec3 LightSource::getEmittance() const{
    return emittance_;
}

Ray LightSource::getNewReflectedRay(Ray& ray, glm::vec3& position ,glm::vec3& normal){
    
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
