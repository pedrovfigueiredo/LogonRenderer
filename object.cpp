//
//  object.cpp
//  RayTracer
//
//  Created by Pedro Figueirêdo on 14/05/17.
//  Copyright © 2017 Lavid. All rights reserved.
//

#include "object.h"

Object::Object(std::string name,std::string fileName, Material* material):
name_(std::move(name)),
fileName_(std::move(fileName)),
material_(material),
translation_vector_({0,0,0}),
getMaterialFromMTL(false){}

Object::Object(std::string name,std::string fileName, Material* material, glm::vec3 translation_vector):
name_(std::move(name)),
fileName_(std::move(fileName)),
material_(material),
translation_vector_(translation_vector),
getMaterialFromMTL(false){}

Object::Object(std::string name,std::string fileName):
name_(std::move(name)),
fileName_(std::move(fileName)),
material_(nullptr),
translation_vector_({0,0,0}),
getMaterialFromMTL(true){}

Object::Object(std::string name,std::string fileName, glm::vec3 translation_vector):
name_(std::move(name)),
fileName_(std::move(fileName)),
material_(nullptr),
translation_vector_(translation_vector),
getMaterialFromMTL(true){}

const std::string& Object::getFileName(){
    return fileName_;
}

