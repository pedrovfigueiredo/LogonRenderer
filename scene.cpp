#include "scene.h"

Scene::Scene( void )
{}

Scene::~Scene( void )
{}

bool Scene::intersect( const Ray &ray,
                       IntersectionRecord &intersection_record ) const
{
    bool intersection_result = false;
    IntersectionRecord tmp_intersection_record;
    std::size_t num_primitives = primitives_.size();

    // Loops over the list of primitives, testing the intersection of each primitive against the given ray 
    for ( std::size_t primitive_id = 0; primitive_id < num_primitives; primitive_id++ )
        if ( primitives_[primitive_id]->intersect( ray, tmp_intersection_record ) )
            if ( ( tmp_intersection_record.t_ < intersection_record.t_ ) && ( tmp_intersection_record.t_ > 0.0 ) )
            {
                intersection_record = tmp_intersection_record;
                intersection_result = true; // the ray intersects a primitive!
            }

    return intersection_result;
}

void Scene::load( void ) 
{
    //cabeça
    primitives_.push_back( Primitive::PrimitiveUniquePtr( new Sphere{ glm::vec3{  0.0f, 0.0f,  -7.0f }, 0.8f } ) );
    
    //chapéu
    primitives_.push_back( Primitive::PrimitiveUniquePtr( new Triangle{ glm::vec3{  -0.9f, 0.5f, -2.5f }, glm::vec3{  0.9f, 0.5f, -2.5f }, glm::vec3{  0.0f, 1.0f, -2.5f } }) );
    
    //olhos
    primitives_.push_back( Primitive::PrimitiveUniquePtr( new Sphere{ glm::vec3{ -0.35f, 0.2f, -1.25f }, 0.2f } ) );
    primitives_.push_back( Primitive::PrimitiveUniquePtr( new Sphere{ glm::vec3{ 0.35f, 0.2f, -1.25f }, 0.2f } ) );
    
    //nariz
    primitives_.push_back( Primitive::PrimitiveUniquePtr( new Triangle{ glm::vec3{  -0.15f, -0.15f, -2.0f }, glm::vec3{  0.15f, -0.15f, -2.0f }, glm::vec3{  0.0f, 0.15f, -2.0f } }) );
    
    //boca
    primitives_.push_back( Primitive::PrimitiveUniquePtr( new Triangle{ glm::vec3{  -0.5f, -0.3f, -1.5f }, glm::vec3{  0.5f, -0.3f, -1.5f }, glm::vec3{  0.0f, -0.7f, -1.5f } }) );
    
    //primitives_.push_back( Primitive::PrimitiveUniquePtr( new Sphere{ glm::vec3{  0.0f, 0.5f, -3.0f }, 0.2f } ) );
}

