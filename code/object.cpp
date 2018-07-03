//
//  object.cpp
//  RayTracer
//
//  Created by Pedro Figueirêdo on 14/05/17.
//  Copyright © 2017 Lavid. All rights reserved.
//

#include "object.h"

Object::Object(std::string name,std::string fileName, Material* material, Transformations* transformations):
fileName_(std::move(fileName)),
material_(material),
getMaterialFromMTL(false){
    if (!transformations)
        transformations_ = new Transformations();
    else
        transformations_ = transformations;
}

Object::Object(std::string name,std::string fileName, Transformations* transformations):
name_(std::move(name)),
fileName_(std::move(fileName)),
material_(nullptr),
getMaterialFromMTL(true){
    if (!transformations)
        transformations_ = new Transformations();
    else
        transformations_ = transformations;
}


const std::string& Object::getFileName(){
    return fileName_;
}
