#include "scene.h"

Scene::Scene( void )
{}

Scene::~Scene( void )
{}

bool Scene::intersect( const Ray &ray,
                      IntersectionRecord &intersection_record ) const
{
    switch (method_) {
        case BVH:
            //intersect using BVH
            return this->bvh->intersect(ray, intersection_record);
            break;
            
        case NONE:
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
            break;
    }
    
    return false;
}

void Scene::setMethod(const BVH::SplitMethod* splitMethod){
    
    if (!splitMethod)
        splitMethod = new BVH::SplitMethod(BVH::SplitMethod::SAH);
    
    switch (method_) {
        case BVH:
            bvh = new class BVH(primitives_);
            bvh->constructTree(*splitMethod);
            break;
        case NONE: // Set bvh pointer to nullptr
        default:
            bvh = nullptr;
            break;
    }
}
void Scene::load(const AcelerationMethod* method, const BVH::SplitMethod* splitMethod)
{
    
    primitives_.push_back( Primitive::PrimitiveUniquePtr( new Sphere{ new RoughConductor(0.8, glm::vec3{1.00, 0.71, 0.29}),
     glm::vec3{ 0.0f, 0.6f, 0.0f }, 0.3f } ) );
     primitives_.push_back( Primitive::PrimitiveUniquePtr( new Sphere{ new Diffuse(glm::vec3{0,191,255}),
     glm::vec3{ 0.5f, 0.3f, 0.3f }, 0.3f } ) );
     primitives_.push_back( Primitive::PrimitiveUniquePtr( new Sphere{ new Diffuse(glm::vec3{1,1,0}),
     glm::vec3{ -0.5f, 0.3f, 0.3f }, 0.3f } ) );
    
    /*
     primitives_.push_back( Primitive::PrimitiveUniquePtr( new Sphere{ new SmoothDieletric(),
     glm::vec3{ -0.3f, 0.31f, -0.3f}, 0.3f} ) );
     primitives_.push_back( Primitive::PrimitiveUniquePtr( new Sphere{ new SmoothDieletric(),
     glm::vec3{ 0.5f, 0.31f, 0.1f}, 0.3f} ) );
     
     */
    
     
    if (method_){
        this->method_ = *method;
        if (splitMethod)
            setMethod(splitMethod);
        else
            setMethod(nullptr);
    }else
        setMethod(nullptr);
}

bool Scene::load( std::vector<Object*> &objects, const AcelerationMethod* method, const BVH::SplitMethod* splitMethod)
{
    
    // Create an instance of the Importer class
    Assimp::Importer importer;
    
    for (auto &&object : objects) {
        const aiScene* scene = importer.ReadFile( object->getFileName(),
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
            
            if (object->getMaterialFromMTL) {
                //Setting diffuseColor and emissiveColor initially to black
                aiColor3D diffuseColor = {0,0,0};
                aiColor3D emissiveColor = {0,0,0};
                
                if (scene->mMaterials) {
                    // Setting up the diffuse and emissive color used by the mesh
                    scene->mMaterials[scene->mMeshes[mesh]->mMaterialIndex]->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
                    scene->mMaterials[scene->mMeshes[mesh]->mMaterialIndex]->Get(AI_MATKEY_COLOR_EMISSIVE, emissiveColor);
                }
                /*
                aiString name;
                scene->mMaterials[scene->mMeshes[mesh]->mMaterialIndex]->Get(AI_MATKEY_NAME,name);
                
                if (std::string(name.data) == "backWall") {
                    object->material_ = new Mirror();
                }else*/
                    object->material_ = new GenericMaterial(glm::vec3 {emissiveColor.r, emissiveColor.g, emissiveColor.b},
                                                            glm::vec3 {diffuseColor.r, diffuseColor.g, diffuseColor.b});
            }
            
            
            
            
            for (unsigned int face = 0; face < scene->mMeshes[mesh]->mNumFaces; face++) {
                
                if (scene->mMeshes[mesh]->mFaces[face].mNumIndices != 3) {
                    std::cerr << "Object is not triangulated.\n Finishing...";
                    return false;
                }
                
                primitives_.push_back(Primitive::PrimitiveUniquePtr( new Triangle{
                    //material
                    object->material_,
                    
                    // a
                    object->transformations_->ApplyTransformations(glm::vec3{
                        scene->mMeshes[mesh]->mVertices[scene->mMeshes[mesh]->mFaces[face].mIndices[0]].x,
                        scene->mMeshes[mesh]->mVertices[scene->mMeshes[mesh]->mFaces[face].mIndices[0]].y,
                        scene->mMeshes[mesh]->mVertices[scene->mMeshes[mesh]->mFaces[face].mIndices[0]].z
                    }),
                    
                    // b
                    object->transformations_->ApplyTransformations(glm::vec3{
                        scene->mMeshes[mesh]->mVertices[scene->mMeshes[mesh]->mFaces[face].mIndices[1]].x,
                        scene->mMeshes[mesh]->mVertices[scene->mMeshes[mesh]->mFaces[face].mIndices[1]].y,
                        scene->mMeshes[mesh]->mVertices[scene->mMeshes[mesh]->mFaces[face].mIndices[1]].z
                    }) ,
                    
                    // c
                    object->transformations_->ApplyTransformations(glm::vec3{
                        scene->mMeshes[mesh]->mVertices[scene->mMeshes[mesh]->mFaces[face].mIndices[2]].x,
                        scene->mMeshes[mesh]->mVertices[scene->mMeshes[mesh]->mFaces[face].mIndices[2]].y,
                        scene->mMeshes[mesh]->mVertices[scene->mMeshes[mesh]->mFaces[face].mIndices[2]].z
                    }) ,
                } ) );
                
            }
        }
    }
    
    if (method){
        this->method_ = *method;
        if (splitMethod)
            setMethod(splitMethod);
        else
            setMethod(nullptr);
    }else
        setMethod(nullptr);
    
    std::cout << "Número de primitivas: " << primitives_.size() << std::endl;
    
    return true;
}
