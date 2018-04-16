#include <overkill/ModelSystem.hpp>

#include <overkill/MaterialSystem.hpp>

namespace overkill
{

std::vector<Model> ModelSystem::m_models;
std::unordered_map<C::Tag, C::ID> ModelSystem::m_mapModelID;

auto ModelSystem::getIdByTag(const C::Tag& tag) -> C::ID
{
    return m_mapModelID[tag];
}

auto ModelSystem::getByTag(const C::Tag& tag) -> const Model&
{
    return ModelSystem::m_models[m_mapModelID[tag]];
}

auto ModelSystem::getById(C::ID modelID) -> const Model&
{
    return ModelSystem::m_models[modelID];
}


void ModelSystem::load()
{
    auto vertices = std::vector<Vertex> {

        // Cube 1
        {  -0.5f, -0.5f,  0.5f,  1,   0, 0 ,  255,     255-96,  255-192, 255 },
        {   0.5f, -0.5f,  0.5f,  1,   1, 0 ,  255-16,  255-112, 255-208, 255 },
        {   0.5f,  0.5f,  0.5f,  1,   1, 1 ,  255-32,  255-128, 255-224, 255 },
        {  -0.5f,  0.5f,  0.5f,  1,   0, 1 ,  255-48,  255-144, 255-240, 255 },

        {  -0.5f, -0.5f, -0.5f,  1,   1, 0 ,  255-64,  255-160, 255-255, 255 },
        {  -0.5f,  0.5f, -0.5f,  1,   1, 1 ,  255-80,  255-176, 255, 255 },

        {   0.5f, -0.5f, -0.5f,  1,   0, 0 ,  255-96,  255-192, 255-16, 255 },
        {   0.5f,  0.5f, -0.5f,  1,   0, 1 ,  255-112, 255-208, 255-32, 255 },

        {  -0.5f, -0.5f, -0.5f,  1,   0, 1 ,  255-128, 255-224, 255-48, 255 },
        {   0.5f, -0.5f, -0.5f,  1,   1, 1 ,  255-144, 255-240, 255-64, 255 },

        {  -0.5f,  0.5f, -0.5f,  1,   0, 0 ,  255-160, 255-255,  255-80, 255 },
        {   0.5f,  0.5f, -0.5f,  1,   1, 0 ,  255-176, 255,      255-96, 255 },

        {  -0.5f,  0.5f,  0.5f,  1,   0, 1 ,  255-192, 255-16,   255-112, 255 },
        {   0.5f,  0.5f,  0.5f,  1,   1, 1 ,  255-208, 255-32,   255-128, 255 },


        // Cube 2
        {  -1.0f, -1.0f,  1.0f,  1,   0, 0 ,  255, 255, 255, 80 },
        {   1.0f, -1.0f,  1.0f,  1,   1, 0 ,  255, 255, 255, 80 },
        {   1.0f,  1.0f,  1.0f,  1,   1, 1 ,  255, 255, 255, 80 },
        {  -1.0f,  1.0f,  1.0f,  1,   0, 1 ,  255, 255, 255, 80 },

        {  -1.0f, -1.0f, -1.0f,  1,   1, 0 ,  255, 255, 255, 80 },
        {  -1.0f,  1.0f, -1.0f,  1,   1, 1 ,  255, 255, 255, 80 },

        {   1.0f, -1.0f, -1.0f,  1,   0, 0 ,  255, 255, 255, 80 },
        {   1.0f,  1.0f, -1.0f,  1,   0, 1 ,  255, 255, 255, 80 },

        {  -1.0f, -1.0f, -1.0f,  1,   0, 1 ,  255, 255, 255, 80 },
        {   1.0f, -1.0f, -1.0f,  1,   1, 1 ,  255, 255, 255, 80 },

        {  -1.0f,  1.0f, -1.0f,  1,   0, 0 ,  255, 255, 255, 80 },
        {   1.0f,  1.0f, -1.0f,  1,   1, 0 ,  255, 255, 255, 80 },

        {  -1.0f,  1.0f,  1.0f,  1,   0, 1 ,  255, 255, 255, 80 },
        {   1.0f,  1.0f,  1.0f,  1,   1, 1 ,  255, 255, 255, 80 },

    };

    std::vector<GLuint> indicies {

        // Cube 1
        0, 1, 2,
        2, 3, 0,

        4, 0, 3,
        3, 5, 4,

        1, 6, 7,
        7, 2, 1,

        8, 9, 1,
        1, 0, 8,

        10, 11, 9,
        9,  8,  10,

        12, 13, 11,
        11, 10, 12,


        // Cube 2
        14, 15, 16,
        16, 17, 14,

        18, 14, 3+14,
        3 + 14, 5 + 14, 4 + 14,

        1+14, 6+14, 7+14,
        7+14, 2+14, 1+14,

        8+14, 9+14, 1+14,
        1+14, 0+14, 8+14,

        10 + 14, 11 + 14, 9 + 14,
        9 + 14,  8 + 14,  10 + 14,

        12+14, 13+14, 11+14,
        11+14, 10+14, 12+14,
    };

    Model model{vertices};
    m_mapModelID["cube"] = m_models.size();
    
    Mesh mesh { 
        indicies,
        MaterialSystem::getIdByTag("brick"),
        ShaderSystem::copyByTag("base")
    };

    auto meshID = model.m_meshes.size();
    model.m_meshes.emplace_back(mesh);

    MaterialSystem::bindOnUpdate(
        "brick",
        m_mapModelID["cube"],
        meshID,
        [](C::ID materialID, C::ID modelID, C::ID meshID) {

            auto model = ModelSystem::getById(modelID);
            auto mesh  = model.m_meshes[meshID];

            mesh.m_materialID = materialID;
            mesh.m_shaderProgram.setMaterial(MaterialSystem::getById(materialID));
        }
    );


    m_models.push_back(model);
}

}
