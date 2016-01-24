#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

using namespace cocos2d;

class HelloWorld : public cocos2d::Layer
{
    typedef struct {
        float Position[3];
        float Color[4];
        float TexCoord[2];
    } Vertex;
    
    typedef struct {
        Vertex* vertices;
        GLushort* indices;
    } VertexData;
    
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    virtual void visit(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t transformUpdated) override;
    //we call our actual opengl commands here
    void onDraw();
    
    void update(float dt);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
private:
    CustomCommand _customCommand;
    
    GLuint vao;
    GLuint vertexVBO;
    GLuint colorVBO;
    
    GLuint oceanColorTextureId;
    GLuint oceanNormalTextureId;
    
    float time;
    int oceanCol, oceanRow, oceanWidth, oceanHeight;
    
    VertexData generateOceanVertices(int col, int row, float width, float height);
};

#endif // __HELLOWORLD_SCENE_H__
