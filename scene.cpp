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
    
    primitives_.push_back( Primitive::PrimitiveUniquePtr( new Sphere{ new LightSource{ glm::vec3{ 30, 30, 30 }},
        glm::vec3{ -0.35f, -0.5f, 0.00f }, 0.1f } ) );
    primitives_.push_back( Primitive::PrimitiveUniquePtr( new Sphere{ new LightSource{ glm::vec3{ 30, 30, 30 }},
        glm::vec3{ 0.35f, -0.5f, 0.00f }, 0.1f } ) );
    
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
    
    for (unsigned int mesh = 0 ; mesh < scene->mNumMeshes; mesh++) {
        
        //Setting diffuseColor and emissiveColor initially to black
        aiColor3D diffuseColor = {0,0,0};
        aiColor3D emissiveColor = {0,0,0};
        
        if (scene->mMaterials) {
            // Setting up the diffuse and emissive color used by the mesh
            scene->mMaterials[scene->mMeshes[mesh]->mMaterialIndex]->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
            scene->mMaterials[scene->mMeshes[mesh]->mMaterialIndex]->Get(AI_MATKEY_COLOR_EMISSIVE, emissiveColor);
        }
        
        aiString name;
        scene->mMaterials[scene->mMeshes[mesh]->mMaterialIndex]->Get(AI_MATKEY_NAME,name);
        
        for (unsigned int face = 0; face < scene->mMeshes[mesh]->mNumFaces; face++) {
            
            if (scene->mMeshes[mesh]->mFaces[face].mNumIndices != 3) {
                std::cerr << "Object is not triangulated.\n Finishing...";
                return false;
            }
            
            if(std::string(name.data) == "ceiling")
                primitives_.push_back(Primitive::PrimitiveUniquePtr( new Triangle{
                    //material
                    new Mirror(),
                    
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
            else
                primitives_.push_back(Primitive::PrimitiveUniquePtr( new Triangle{
                    //material
                    new GenericMaterial(glm::vec3 {emissiveColor.r, emissiveColor.g, emissiveColor.b},
                                        glm::vec3 {diffuseColor.r, diffuseColor.g, diffuseColor.b}),
                    
                    
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

