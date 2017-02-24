//
//  pinhole_camera.cpp
//  RayTracer
//
//  Created by Pedro Figueirêdo on 21/02/17.
//  Copyright © 2017 Lavid. All rights reserved.
//

#include "pinhole_camera.h"

PinHoleCamera::PinHoleCamera( void )
{}

PinHoleCamera::PinHoleCamera( const float min_x,
                                       const float max_x,
                                       const float min_y,
                                       const float max_y,
                                       const float distance,
                                       const glm::ivec2 &resolution,
                                       const glm::vec3 &position,
                                       const glm::vec3 &up_vector,
                                       const glm::vec3 &look_at ) :
Camera::Camera{ resolution,
    position,
    up_vector,
    look_at },
distance_{distance},
min_x_{ min_x },
max_x_{ max_x },
min_y_{ min_y },
max_y_{ max_y }
{}

Ray PinHoleCamera::getWorldSpaceRay( const glm::vec2 &pixel_coord ) const
{
    float a = max_x_ - min_x_;
    float b = -(max_y_ - min_y_);
    
    float u = ((pixel_coord[0] + 0.5)/static_cast< float >(resolution_[0])) * a + min_x_;
    float v = (-(pixel_coord[1] + 0.5)/static_cast< float >(resolution_[1])) * b + min_y_;
    
    glm::vec3 s {u*a,v*b, -distance_};
   
    
    return Ray{ onb_.getBasisMatrix() * position_,
        glm::normalize( onb_.getBasisMatrix() * s ) };
}
