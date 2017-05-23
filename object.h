//
//  object.h
//  RayTracer
//
//  Created by Pedro Figueirêdo on 14/05/17.
//  Copyright © 2017 Lavid. All rights reserved.
//

#ifndef object_h
#define object_h

#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "material.h"

class Object
{
public:
    
    // Initializing with specified material
    
    Object( std::string name , std::string fileName, Material* material);
    
    Object( std::string name , std::string fileName, Material* material, glm::vec3 translation_vector);
    
    // Initializing with MTL's material
    
    Object( std::string name , std::string fileName);
    
    Object( std::string name , std::string fileName, glm::vec3 translation_vector);
    
    const std::string& getFileName();
    
    Material* material_;
    
    glm::vec3 translation_vector_;
    
    bool getMaterialFromMTL;
    
private:
    std::string name_;
    std::string fileName_;
};


#endif /* object_h */
