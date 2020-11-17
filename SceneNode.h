#pragma once
#include "utils.h"
class Geometry;
class Material;
class Camera;
class LinkedList{
public:
    LinkedList*mNext;
    LinkedList():mNext(nullptr){}
    void Append(LinkedList*node){
        if(mNext== nullptr){
            mNext=node;
        }else{
            mNext->Append(node);
        }
    }
    template<typename T>
    T* Next(){
        return (T*)mNext;
    }
};
class SceneNode : public LinkedList{
public:
    glm::mat4 mModelMatrix;
	glm::mat4 mScaleMatrix;
    Geometry * mGeometry;
    Material * mMaterial;
    void Update(float delta);
    void Render(Camera*camera);
};