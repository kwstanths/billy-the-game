#include "GraphicsObject.hpp"
#include "Renderer.hpp"

#include "cal3d/error.h"
#include "cal3d/mixer.h"
#include "cal3d/renderer.h"

#include "game_engine/ErrorCodes.hpp"
#include "game_engine/math/Matrices.hpp"

#include "debug_tools/CodeReminder.hpp"

#include "AssetManager.hpp"
#include "AssimpHelp.hpp"

namespace math = game_engine::math;

namespace game_engine {
namespace graphics {

    GraphicsObject::GraphicsObject() {

        translation_matrix_ = math::GetTranslateMatrix(0, 0, 0);

        is_inited_ = false;
    }

    int GraphicsObject::Init(Real_t x, Real_t y, Real_t z, std::string model_file_path, Type type) {

        translation_matrix_ = math::GetTranslateMatrix(x, y, z);
        scale_matrix_ = math::GetScaleMatrix(1.0f, 1.0f, 1.0f);

        type_ = type;
        if (type_ == GraphicsObject::ASSIMP) {
            /* If model already loaded, then use the same */
            AssetManager& asset_manager = AssetManager::GetInstance();
            model_ = asset_manager.FindModel(model_file_path);
            if (model_ == nullptr) {
                /* Else load it, and insert it in the asset manager */
                model_ = new Model();
                model_->Init(model_file_path);
                asset_manager.InsertModel(model_file_path, model_);
            }

            mesh_queries_ = std::vector<opengl::OpenGLQuery>(model_->GetNumberOfMeshes());
            for (size_t i = 0; i < model_->GetNumberOfMeshes(); i++)
                mesh_queries_[i].Init();

            is_inited_ = true;
            return 0;
        } else {

            m_calCoreModel = new cal3d::CalCoreModel("dummy");

            m_state = ANIM_STATE_IDLE;
            m_motionBlend[0] = 0.6f;
            m_motionBlend[1] = 0.1f;
            m_motionBlend[2] = 0.3f;
            m_animationCount = 0;
            m_meshCount = 0;
            m_renderScale = 1.0f;
            m_lodLevel = 1.0f;

            // open the model configuration file
            std::ifstream file;
            file.open(model_file_path.c_str(), std::ios::in | std::ios::binary);
            if (!file) {
                std::cerr << "Failed to open model configuration file '" << model_file_path << "'." << std::endl;
                return false;
            }

            // initialize the data path
            std::string strPath = m_path;

            // initialize the animation count
            int animationCount;
            animationCount = 0;

            // parse all lines from the model configuration file
            int line;
            for (line = 1; ; line++)
            {
                // read the next model configuration line
                std::string strBuffer;
                std::getline(file, strBuffer);

                // stop if we reached the end of file
                if (file.eof()) break;

                // check if an error happened while reading from the file
                if (!file) {
                    std::cerr << "Error while reading from the model configuration file '" << model_file_path << "'." << std::endl;
                    return false;
                }

                // find the first non-whitespace character
                std::string::size_type pos;
                pos = strBuffer.find_first_not_of(" \t");

                // check for empty lines
                if ((pos == std::string::npos) || (strBuffer[pos] == '\n') || (strBuffer[pos] == '\r') || (strBuffer[pos] == 0)) continue;

                // check for comment lines
                if (strBuffer[pos] == '#') continue;

                // get the key
                std::string strKey;
                strKey = strBuffer.substr(pos, strBuffer.find_first_of(" =\t\n\r", pos) - pos);
                pos += strKey.size();

                // get the '=' character
                pos = strBuffer.find_first_not_of(" \t", pos);
                if ((pos == std::string::npos) || (strBuffer[pos] != '='))
                {
                    std::cerr << model_file_path << "(" << line << "): Invalid syntax." << std::endl;
                    return false;
                }

                // find the first non-whitespace character after the '=' character
                pos = strBuffer.find_first_not_of(" \t", pos + 1);

                // get the data
                std::string strData;
                strData = strBuffer.substr(pos, strBuffer.find_first_of("\n\r", pos) - pos);

                // handle the model creation
                if (strKey == "scale")
                {
                    // set rendering scale factor
                    m_renderScale = atof(strData.c_str());
                }
                else if (strKey == "path")
                {
                    // set the new path for the data files if one hasn't been set already
                    if (m_path == "") strPath = strData;
                }
                else if (strKey == "skeleton")
                {
                    // load core skeleton
                    std::cout << "Loading skeleton '" << strData << "'..." << std::endl;
                    if (!m_calCoreModel->loadCoreSkeleton(strPath + strData)) {
                        cal3d::CalError::printLastError();
                        return false;
                    }
                }
                else if (strKey == "animation")
                {
                    // load core animation
                    std::cout << "Loading animation '" << strData << "'..." << std::endl;
                    m_animationId[animationCount] = m_calCoreModel->loadCoreAnimation(strPath + strData);
                    if (m_animationId[animationCount] == -1)
                    {
                        cal3d::CalError::printLastError();
                        return false;
                    }

                    animationCount++;
                }
                else if (strKey == "mesh")
                {
                    // load core mesh
                    std::cout << "Loading mesh '" << strData << "'..." << std::endl;
                    if (m_calCoreModel->loadCoreMesh(strPath + strData) == -1)
                    {
                        cal3d::CalError::printLastError();
                        return false;
                    }
                }
                else if (strKey == "material")
                {
                    // load core material
                    std::cout << "Loading material '" << strData << "'..." << std::endl;
                    if (m_calCoreModel->loadCoreMaterial(strPath + strData) == -1)
                    {
                        cal3d::CalError::printLastError();
                        return false;
                    }
                }
                else
                {
                    std::cerr << model_file_path << "(" << line << "): Invalid syntax." << std::endl;
                    return false;
                }
            }

            // explicitely close the file
            file.close();

            // load all textures and store the opengl texture id in the corresponding map in the material
            int materialId;
            for (materialId = 0; materialId < m_calCoreModel->getCoreMaterialCount(); materialId++)
            {
                // get the core material
                cal3d::CalCoreMaterial *pCoreMaterial;
                pCoreMaterial = m_calCoreModel->getCoreMaterial(materialId);

                // loop through all maps of the core material
                int mapId;
                for (mapId = 0; mapId < pCoreMaterial->getMapCount(); mapId++)
                {
                    // get the filename of the texture
                    std::string strFilename;
                    strFilename = pCoreMaterial->getMapFilename(mapId);

                    // load the texture from the file
                    opengl::OpenGLTexture * tmp = new opengl::OpenGLTexture();
                    tmp->Init(strPath + strFilename, GAME_ENGINE_TEXTURE_TYPE_DIFFUSE_MAP);
                    textures_.push_back(tmp);

                    // store the opengl texture id in the user data of the map
                    pCoreMaterial->setMapUserData(mapId, (Cal::UserData)tmp->GetID());
                }
            }

            // make one material thread for each material
            // NOTE: this is not the right way to do it, but this viewer can't do the right
            // mapping without further information on the model etc.
            for (materialId = 0; materialId < m_calCoreModel->getCoreMaterialCount(); materialId++)
            {
                // create the a material thread
                m_calCoreModel->createCoreMaterialThread(materialId);

                // initialize the material thread
                m_calCoreModel->setCoreMaterialId(materialId, 0, materialId);
            }

            // Calculate Bounding Boxes

            m_calCoreModel->getCoreSkeleton()->calculateBoundingBoxes(m_calCoreModel);

            m_calModel = new cal3d::CalModel(m_calCoreModel);

            // attach all meshes to the model
            int meshId;
            for (meshId = 0; meshId < m_calCoreModel->getCoreMeshCount(); meshId++)
            {
                m_calModel->attachMesh(meshId);
            }

            // set the material set of the whole model
            m_calModel->setMaterialSet(0);

            // set initial animation state
            m_state = ANIM_STATE_MOTION;
            m_calModel->getMixer()->blendCycle(m_animationId[ANIM_STATE_MOTION], m_motionBlend[0], 0.0f);
            m_calModel->getMixer()->blendCycle(m_animationId[ANIM_STATE_MOTION + 1], m_motionBlend[1], 0.0f);
            m_calModel->getMixer()->blendCycle(m_animationId[ANIM_STATE_MOTION + 2], m_motionBlend[2], 0.0f);

            // get the renderer of the model
            cal3d::CalRenderer *pCalRenderer;
            pCalRenderer = m_calModel->getRenderer();

            // begin the rendering loop
            if (!pCalRenderer->beginRendering()) return -1;

            // get the number of meshes
            int meshCount;
            meshCount = pCalRenderer->getMeshCount();

            // render all meshes of the model
            for (meshId = 0; meshId < meshCount; meshId++)
            {
                // get the number of submeshes
                int submeshCount;
                submeshCount = pCalRenderer->getSubmeshCount(meshId);

                // render all submeshes of the mesh
                int submeshId;
                for (submeshId = 0; submeshId < submeshCount; submeshId++)
                {
                    // select mesh and submesh for further data access
                    if (pCalRenderer->selectMeshSubmesh(meshId, submeshId))
                    {
                        // get the vertices of the submesh
                        static float meshVertices[30000][3];
                        int vertexCount;
                        vertexCount = pCalRenderer->getVertices(&meshVertices[0][0]);

                        // get the normals of the submesh
                        static float meshNormals[30000][3];
                        pCalRenderer->getNormals(&meshNormals[0][0]);

                        // get the coordinates of the submesh
                        static float meshTextureCoordinates[30000][2];
                        int textureCoordinateCount;
                        textureCoordinateCount = pCalRenderer->getTextureCoordinates(0, &meshTextureCoordinates[0][0]);

                        // get the faces of the submesh
                        static CalIndex meshFaces[50000][3];
                        int faceCount;
                        faceCount = pCalRenderer->getFaces(&meshFaces[0][0]);

                        std::vector<Vertex_t> vertices;
                        std::vector<unsigned int> indices;
                        for (size_t vertex = 0; vertex < vertexCount; vertex++) {
                            vertices.push_back(Vertex_t(glm::vec3(meshVertices[vertex][0], meshVertices[vertex][1], meshVertices[vertex][2]),
                                glm::vec3(meshNormals[vertex][0], meshNormals[vertex][1], meshNormals[vertex][2]),
                                glm::vec2(meshTextureCoordinates[vertex][0], meshTextureCoordinates[vertex][1])));
                        }
                        for (size_t face = 0; face < faceCount; face++) {
                            indices.push_back(meshFaces[face][0]);
                            indices.push_back(meshFaces[face][1]);
                            indices.push_back(meshFaces[face][2]);
                        }
                        opengl::OpenGLObject * temp = new opengl::OpenGLObject();
                        temp->Init(vertices, indices, false);
                        objects_.push_back(temp);
                    }
                }
            }

            // end the rendering
            pCalRenderer->endRendering();
        }

        is_inited_ = true;
        return 0;
    }

    int GraphicsObject::InitTextureAtlas(std::string file_name) {

        AssetManager& asset_manager = AssetManager::GetInstance();
        std::string file_path = file_name.substr(0, file_name.find_last_of("."));

        std::vector<Mesh *> meshes;
        int ret = ProcessTextureAtlas(file_name, meshes);

        for (size_t i = 0; i < meshes.size(); i++) {
            Model * new_model = new Model();
            new_model->Init({ meshes[i] });
            asset_manager.InsertModel(file_path + "_" + std::to_string(i) + ".obj", new_model);
        }

        return 0;
    }

    int GraphicsObject::Destroy() {

        /* DONT Destroy or delete model_, It stil might be used by other objects */

        is_inited_ = false;
        return 0;
    }

    bool GraphicsObject::IsInited() {
        return is_inited_;
    }

    void GraphicsObject::Draw(Renderer * renderer) {
        if (type_ == ASSIMP) {
            renderer->Draw(this);
        } else {

        }
    }

    void GraphicsObject::SetPosition(Real_t x, Real_t y, Real_t z) {
        /* Set the translation matrix */
        translation_matrix_ = math::GetTranslateMatrix(x, y, z);
    }

    void GraphicsObject::Scale(Real_t scale_x, Real_t scale_y, Real_t scale_z) {
        /* Set the scaling matrix */
        scale_matrix_ = math::GetScaleMatrix(scale_x, scale_y, scale_z);
    }

    void GraphicsObject::SetRotation(glm::mat4 rotation) {
        rotation_matrix_ = rotation;
    }

    void GraphicsObject::Rotate(Real_t angle, glm::vec3 axis) {
        rotation_matrix_ *= math::GetRotateMatrix(angle, axis.x, axis.y, axis.z);
    }

    void GraphicsObject::UpdateAnimation(float delta) {
        if (type_ == ANIMATED) {
            m_calModel->update(delta);
        }
    }

    void GraphicsObject::ExecuteAction() {
        if (type_ == ANIMATED) {
            m_calModel->getMixer()->executeAction(m_animationId[6], 0.0f, 0.0f);
        }
    }

    void GraphicsObject::StopAction() {
        m_calModel->getMixer()->stopCycle(m_animationId[6]);
    }

    void GraphicsObject::SetModelMatrix() {
        model_matrix_ = translation_matrix_ * rotation_matrix_ * scale_matrix_;
    }


}
}