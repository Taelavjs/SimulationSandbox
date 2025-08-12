#include "Chunk.hpp"

Chunk::Chunk(Vector2D chunkGlobalCoords, std::vector<std::vector<Pixel *>> chunkVec)
:globalCoords(chunkGlobalCoords), vec(chunkVec)
{

}

Chunk::Chunk()
:globalCoords(Vector2D(999,999)), vec()
{

}
Chunk::~Chunk(){
    for(std::vector<Pixel *> pixs : vec){
        for(Pixel* pix : pixs){
            if(pix != nullptr){
                //delete pix;
            }
        }
    }
}

Vector2D Chunk::getGlobalCoords(){
    return globalCoords;
}

