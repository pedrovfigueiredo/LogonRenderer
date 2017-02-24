//
//  pinhole_camera.h
//  RayTracer
//
//  Created by Pedro Figueirêdo on 21/02/17.
//  Copyright © 2017 Lavid. All rights reserved.
//

#ifndef pinhole_camera_h
#define pinhole_camera_h


#include "camera.h"
#include "ray.h"

class PinHoleCamera : public Camera{
public:
    
    PinHoleCamera( void );
    
    PinHoleCamera( const float min_x,
                       const float max_x,
                       const float min_y,
                       const float max_y,
                       const float distance,
                       const glm::ivec2 &resolution,
                       const glm::vec3 &position,
                       const glm::vec3 &up_vector,
                       const glm::vec3 &look_at );
    
    Ray getWorldSpaceRay( const glm::vec2 &pixel_coord ) const;
    
    float min_x_;
    
    float max_x_;
    
    float min_y_;
    
    float max_y_;
    
    float distance_;
};

#endif /* pinhole_camera_h */
