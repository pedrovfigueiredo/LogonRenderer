//
//  triangle.cpp
//  RayTracer
//
//  Created by Pedro Figueirêdo on 10/02/17.
//  Copyright © 2017 Lavid. All rights reserved.
//

#include "triangle.h"

Triangle::Triangle( void ){}

Triangle::Triangle( const glm::vec3 &a, const glm::vec3 &b, const glm::vec3 &c ) :
a_{ a },
b_{ b },
c_{ c }
{}

bool Triangle::intersect( const Ray &ray, IntersectionRecord &intersection_record ) const{
    
    float beta, gama, t, A;
    
    // Calcula-se o determinante da matriz A, como está no livro
    
    A = determinant3x3(a_.x - b_.x, a_.x - c_.x, ray.direction_.x,        // a  b  c //
                       a_.y -  b_.y, a_.y - c_.y, ray.direction_.y,       // d  e  f //
                       a_.z - b_.z, a_.z - c_.z, ray.direction_.z);       // g  h  i //
    
    // Acha-se o t e checa-se se está contido no intervalo desejado
    
    t = (determinant3x3(a_.x - b_.x, a_.x - c_.x, a_.x - ray.origin_.x,        // a  b  c //
                        a_.y - b_.y, a_.y - c_.y, a_.y - ray.origin_.y,        // d  e  f //
                        a_.z - b_.z, a_.z - c_.z, a_.z - ray.origin_.z)/ A);   // g  h  i //
    
    if (t < 0) {
        return false;
    }
    
    // Calcula-se gama e beta para determinar se o raio intercepta o triângulo
    
    gama = (determinant3x3(a_.x - b_.x, a_.x - ray.origin_.x, ray.direction_.x,        // a  b  c //
                           a_.y - b_.y, a_.y - ray.origin_.y,ray.direction_.y,         // d  e  f //
                           a_.z - b_.z, a_.z - ray.origin_.z, ray.direction_.z) / A);  // g  h  i //
    
    if (gama < 0 || gama > 1) {
        return false;
    }
    
    beta = (determinant3x3(a_.x - ray.origin_.x, a_.x - c_.x, ray.direction_.x,        // a  b  c //
                           a_.y - ray.origin_.y, a_.y - c_.y, ray.direction_.y,        // d  e  f //
                           a_.z - ray.origin_.z, a_.z - c_.z, ray.direction_.z) / A);  // g  h  i //
    
    if (beta < 0 || beta > 1 - gama) {
        return false;
    }
    
    // Nessa parte de registrar o intersectionRecord, a única mudança foi na definição do centro do primitive, já que esse estava explicito na esfera e não está no triângulo.
    
    intersection_record.t_ = t;
    intersection_record.position_ = ray.origin_ + intersection_record.t_ * ray.direction_;
    intersection_record.normal_ = glm::normalize( intersection_record.position_ - glm::vec3 {(a_.x + b_.x + c_.x)/3, (a_.y + b_.y + c_.y)/3, (a_.z + b_.z + c_.z)/3 } );
    
    return true;
}


// Função para calcular o determinante de uma matriz 3x3

const float Triangle::determinant3x3(float a, float b, float c, float d, float e, float f, float g, float h, float i) const{
    return ((a*e*i) + (b*f*g) + (c*d*h) - (c*e*g) - (b*d*i) - (a*f*h));
}

