//
//  diffuse.cpp
//  RayTracer
//
//  Created by Pedro Figueirêdo on 24/03/17.
//  Copyright © 2017 Lavid. All rights reserved.
//

#include "diffuse.h"

Diffuse::Diffuse(void)
{}

Diffuse::Diffuse( glm::vec3 color){
    type_ = type::Diffuse;
    emittance_ = {0,0,0};
    if (color.x > 1 || color.y > 1 || color.z > 1)
        brdf_ = {(color.x/255) / PI, (color.y/255) / PI, (color.z/255) / PI};
    else
        brdf_ = {color.x / PI, color.y / PI, color.z / PI};
}

glm::vec3 Diffuse::getfr(glm::vec3& w_i, glm::vec3& w_o, float pathLength, float& distanceInObject) const{
    // pdf == 1/2pi
    // w_o.y == dot(normal,w_o)
    return (brdf_ * w_o.y * (float)(2 *Material::PI));
}

glm::vec3 Diffuse::getEmittance() const{
    return emittance_;
}

glm::vec3 Diffuse::getNewDirection(glm::vec3& w_i){

    double theta, phi, r;
    double phiRandom = ((double)rand()/(RAND_MAX));
    double thetaRandom = ((double)rand()/(RAND_MAX));

    theta = glm::acos(1 - thetaRandom);
    phi = 2 * Material::PI * phiRandom;
    r = 1;

    //order of 'y' and 'z' are inverted, given that it's used y axis as the vertical one(differing from normal pattern)
    glm::vec3 direction = {r * glm::sin(theta) * glm::cos(phi), r * glm::cos(theta) ,r * glm::sin(theta) * glm::sin(phi)};

    return direction;
}
