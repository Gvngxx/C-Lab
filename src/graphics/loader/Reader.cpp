#include "../../Models.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>

DMalla Reader::Import(const std::string& ruta) {
    DMalla resultado;
    Assimp::Importer importer;
    
    const aiScene* scene = importer.ReadFile(
    ruta,
    aiProcess_Triangulate |
    aiProcess_FlipUVs |
    aiProcess_GenSmoothNormals
    );
    
    if (!scene || !scene->mRootNode) {
        std::cerr << "Error Assimp: " << importer.GetErrorString() << std::endl;
        return resultado;
    }

    // Unimos todas las mallas del archivo en un solo buffer plano (Ultra Easy)
    for (unsigned int m = 0; m < scene->mNumMeshes; m++) {
        aiMesh* mallaAssimp = scene->mMeshes[m];
        unsigned int desplazarIndices = resultado.vertices.size();

        // 1. Copiar Vértices
        for (unsigned int i = 0; i < mallaAssimp->mNumVertices; i++) {
            Vertice v;
            v.posicion[0] = mallaAssimp->mVertices[i].x;
            v.posicion[1] = mallaAssimp->mVertices[i].y;
            v.posicion[2] = mallaAssimp->mVertices[i].z;

            v.normal[0] = mallaAssimp->mNormals[i].x;
            v.normal[1] = mallaAssimp->mNormals[i].y;
            v.normal[2] = mallaAssimp->mNormals[i].z;

            if (mallaAssimp->mTextureCoords[0]) {
                v.coordsTextura[0] = mallaAssimp->mTextureCoords[0][i].x;
                v.coordsTextura[1] = mallaAssimp->mTextureCoords[0][i].y;
            } else {
                v.coordsTextura[0] = 0.0f; v.coordsTextura[1] = 0.0f;
            }
            resultado.vertices.push_back(v);
        }

        // 2. Copiar Índices
        for (unsigned int i = 0; i < mallaAssimp->mNumFaces; i++) {
            aiFace cara = mallaAssimp->mFaces[i];
            for (unsigned int j = 0; j < cara.mNumIndices; j++) {
                resultado.indices.push_back(desplazarIndices + cara.mIndices[j]);
            }
        }
    }
    return resultado;
}