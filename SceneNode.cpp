#include "SceneNode.h"
#include "Geometry.h"
#include "Material.h"
#include "shader.h"
#include "utils.h"

void SceneNode::Update(float delta) {
    //update self
    mGeometry->Update(delta);
    if(mNext!= nullptr){
        Next<SceneNode>()->Update(delta);
    }
}
void SceneNode::Render(Camera *camera) {
    if(mMaterial->mShader->mProgram==0){
        return ;
    }
    mGeometry->BeginDraw();
    mMaterial->BeginDraw(camera,mModelMatrix);
    mGeometry->Draw();
    mGeometry->EndDraw();
    mMaterial->EndDraw();
}