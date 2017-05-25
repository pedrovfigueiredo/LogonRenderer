//
//  material.cpp
//  RayTracer
//
//  Created by Pedro Figueirêdo on 24/03/17.
//  Copyright © 2017 Lavid. All rights reserved.
//

#include "material.h"

Material::Material(void)
{}

Material::Material( glm::vec3 emittance, glm::vec3 brdf):
emittance_(emittance),
brdf_(brdf)
{}

Material::~Material( void )
{}




