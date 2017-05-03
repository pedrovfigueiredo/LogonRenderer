//
//  mirror.cpp
//  RayTracer
//
//  Created by Pedro Figueirêdo on 28/04/17.
//  Copyright © 2017 Lavid. All rights reserved.
//

#include "mirror.h"

Mirror::Mirror(void):
Material::Material{{0,0,0}, {0,0,0}, {0,0,0}}
{type_ = type::Mirror;}

glm::vec3 Mirror::getfr(glm::vec3 w_i, glm::vec3 w_o) const{
    // The perfect mirror reflects all radiance.
    return {1,1,1};
}

glm::vec3 Mirror::getEmittance() const{
    return emittance_;
}

glm::vec3 Mirror::getNewDirection(glm::vec3& w_i){
    return {-w_i.x, w_i.y, -w_i.z};
}
