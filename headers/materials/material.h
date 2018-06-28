//
//  material.h
//  RayTracer
//
//  Created by Pedro Figueirêdo on 24/03/17.
//  Copyright © 2017 Lavid. All rights reserved.
//

#ifndef material_h
#define material_h

#include "ray.h"
#include <memory>
#include <glm/glm.hpp>

class Material
{
public:

    enum type {Diffuse, LightSource, GenericMaterial, Mirror, SmoothDieletric, RoughConductor};

    Material::type type_;

    constexpr const static double PI = 3.141593;

    Material (void);

    Material( glm::vec3 emittance, glm::vec3 brdf);

    virtual ~Material( void );

    virtual glm::vec3 getfr(glm::vec3& w_i, glm::vec3& w_o, float pathLength, float& distanceInObject) const = 0;

    virtual glm::vec3 getEmittance() const = 0;

    virtual glm::vec3 getNewDirection(glm::vec3& w_i) = 0;


protected:
    glm::vec3 emittance_;
    glm::vec3 brdf_;

};

#endif /* material_h */
