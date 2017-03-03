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
    
    //esferas com z = 0 // Cor Vermelha
    primitives_.push_back( Primitive::PrimitiveUniquePtr( new Sphere{ glm::vec3{ 255, 0, 0 },
        glm::vec3{ -0.35f, 0.3f, 0.00f }, 0.2f } ) );
    primitives_.push_back( Primitive::PrimitiveUniquePtr( new Sphere{ glm::vec3{ 255, 0, 0 },
        glm::vec3{ 0.35f, 0.3f, 0.00f }, 0.2f } ) );
    primitives_.push_back( Primitive::PrimitiveUniquePtr( new Sphere{ glm::vec3{ 255, 0, 0 },
        glm::vec3{ -0.35f, -0.3f, 0.00f }, 0.2f } ) );
    primitives_.push_back( Primitive::PrimitiveUniquePtr( new Sphere{ glm::vec3{ 255, 0, 0 },
        glm::vec3{ 0.35f, -0.3f, 0.00f }, 0.2f } ) );
    
    //esferas com z = -3 // Cor Verde
    primitives_.push_back( Primitive::PrimitiveUniquePtr( new Sphere{ glm::vec3{ 0, 255, 0 },
        glm::vec3{ -0.35f, 0.3f, -1.50f }, 0.2f } ) );
    primitives_.push_back( Primitive::PrimitiveUniquePtr( new Sphere{ glm::vec3{ 0, 255, 0 },
        glm::vec3{ 0.35f, 0.3f, -1.50f }, 0.2f } ) );
    primitives_.push_back( Primitive::PrimitiveUniquePtr( new Sphere{ glm::vec3{ 0, 255, 0 },
        glm::vec3{ -0.35f, -0.3f, -1.50f }, 0.2f } ) );
    primitives_.push_back( Primitive::PrimitiveUniquePtr( new Sphere{ glm::vec3{ 0, 255, 0 },
        glm::vec3{ 0.35f, -0.3f, -1.50f }, 0.2f } ) );
    
    //primitives_.push_back( Primitive::PrimitiveUniquePtr( new Sphere{ glm::vec3{  0.0f, 0.5f, -3.0f }, 0.2f } ) );
}

bool Scene::load( const std::string& pFile )
{
    // Create an instance of the Importer class
    Assimp::Importer importer;
    // And have it read the given file with some example postprocessing
    // Usually - if speed is not the most important aspect for you - you'll
    // propably to request more postprocessing than we do in this example.
    const aiScene* scene = importer.ReadFile( pFile,
                                             aiProcess_CalcTangentSpace       |
                                             aiProcess_Triangulate            |
                                             aiProcess_JoinIdenticalVertices  |
                                             aiProcess_SortByPType);
    
    // If the import failed, report it
    if( !scene)
    {
        std::cerr << importer.GetErrorString();
        return false;
    }
    
    int red,green,blue;
    
    
    
    
    for (unsigned int mesh = 0 ; mesh < scene->mNumMeshes; mesh++) {
        for (unsigned int face = 0; face < scene->mMeshes[mesh]->mNumFaces; face++) {
            
            if (scene->mMeshes[mesh]->mFaces[face].mNumIndices != 3) {
                std::cerr << "Object is not triangulated.\n Finishing...";
                return false;
            }
            
            red = rand() % (int)(255 + 1);
            green = rand() % (int)(255 + 1);
            blue = rand() % (int)(255 + 1);
            
            
            primitives_.push_back(Primitive::PrimitiveUniquePtr( new Triangle{
                //color
                glm::vec3{red,green,blue},
                
                // a
                glm::vec3{
                    scene->mMeshes[mesh]->mVertices[scene->mMeshes[mesh]->mFaces[face].mIndices[0]].x,
                    scene->mMeshes[mesh]->mVertices[scene->mMeshes[mesh]->mFaces[face].mIndices[0]].y,
                    scene->mMeshes[mesh]->mVertices[scene->mMeshes[mesh]->mFaces[face].mIndices[0]].z
                },
                
                // b
                glm::vec3{
                    scene->mMeshes[mesh]->mVertices[scene->mMeshes[mesh]->mFaces[face].mIndices[1]].x,
                    scene->mMeshes[mesh]->mVertices[scene->mMeshes[mesh]->mFaces[face].mIndices[1]].y,
                    scene->mMeshes[mesh]->mVertices[scene->mMeshes[mesh]->mFaces[face].mIndices[1]].z
                },
                
                // c
                glm::vec3{
                    scene->mMeshes[mesh]->mVertices[scene->mMeshes[mesh]->mFaces[face].mIndices[2]].x,
                    scene->mMeshes[mesh]->mVertices[scene->mMeshes[mesh]->mFaces[face].mIndices[2]].y,
                    scene->mMeshes[mesh]->mVertices[scene->mMeshes[mesh]->mFaces[face].mIndices[2]].z
                },
            } ) );
            
        }
    }
    
    return true;
}

