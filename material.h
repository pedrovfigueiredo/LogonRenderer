//
//  material.h
//  RayTracer
//
//  Created by Pedro Figueirêdo on 24/03/17.
//  Copyright © 2017 Lavid. All rights reserved.
//

#ifndef material_h
#define material_h

#include <memory>
#include <glm/glm.hpp>

class Material
{
public:
    constexpr const static double PI = 3.141593;
    
    Material (void);
    
    Material( glm::vec3 emittance, glm::vec3 brdf );
    
    virtual ~Material( void );
    
    virtual glm::vec3 getBRDF() const = 0;
    
    virtual glm::vec3 getEmittance() const = 0;
    
protected:
    glm::vec3 emittance_;
    glm::vec3 brdf_;
    
};

#endif /* material_h */
