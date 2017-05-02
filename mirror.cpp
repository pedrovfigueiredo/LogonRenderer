//
//  mirror.cpp
//  RayTracer
//
//  Created by Pedro Figueirêdo on 28/04/17.
//  Copyright © 2017 Lavid. All rights reserved.
//

#include "mirror.h"

Mirror::Mirror(void):
Material::Material{{0,0,0}, {0,0,0}}
{type_ = type::Mirror;}

glm::vec3 Mirror::getBRDF() const{
    return brdf_;
}

glm::vec3 Mirror::getEmittance() const{
    return emittance_;
}

Ray Mirror::getNewReflectedRay(Ray& ray, glm::vec3& position ,glm::vec3& normal){
    Ray reflectedRay;
    ONB onb;
    
    onb.setFromV(normal);
    
    glm::vec3 localDirection = glm::transpose(onb.getBasisMatrix()) * ray.direction_;
    localDirection = {localDirection.x, -localDirection.y, localDirection.z};
    reflectedRay = {position + (normal*0.001f), onb.getBasisMatrix() * localDirection};
    return reflectedRay;
    
}
