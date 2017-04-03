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
{}

glm::vec3 LightSource::getBRDF() const{
    return brdf_;
}

glm::vec3 LightSource::getEmittance() const{
    return emittance_;
}
