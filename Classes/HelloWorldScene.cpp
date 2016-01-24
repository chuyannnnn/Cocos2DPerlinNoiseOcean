#include "HelloWorldScene.h"

USING_NS_CC;


Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto center = Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);
    
    // Link Shaders
    auto program = new GLProgram;
    program->initWithFilenames("ocean.vsh", "ocean.fsh");
    program->link();
    program->updateUniforms();
    this->setGLProgram(program);
    
    glClearColor(0.95, 1, 1, 1);
    
    // Schedule Update
    this->schedule( schedule_selector(HelloWorld::update) );
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    // Init
    time = 0;
    oceanCol = 30;
    oceanRow = 10;
    oceanWidth = 15.f;
    oceanHeight = 5.f;
    
    // Load Textures
    oceanColorTextureId = Director::getInstance()->getTextureCache()->addImage("oceanTex.jpg")->getName();
    
    // Generate Geometries
    VertexData vData = generateOceanVertices(oceanCol, oceanRow, oceanWidth, oceanHeight);
    Vertex* vertices = vData.vertices;
    GLushort* indices = vData.indices;
    
    glGenBuffers(1, &vertexVBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
    glBufferData(GL_ARRAY_BUFFER, oceanCol * oceanRow * 9 * 4 * 4, vertices, GL_STATIC_DRAW);
    
    GLuint positionLocation = glGetAttribLocation(program->getProgram(), "a_position");
    glEnableVertexAttribArray(positionLocation);
    
    //    CCLOG("Sizeof verts: %d", (int)sizeof(vertices));
    
    glVertexAttribPointer(positionLocation,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          9 * 4,
                          (GLvoid*)offsetof(Vertex,Position));
    
    GLuint colorLocation = glGetAttribLocation(program->getProgram(), "a_color");
    glEnableVertexAttribArray(colorLocation);
    glVertexAttribPointer(colorLocation,
                          4,
                          GL_FLOAT,
                          GL_FALSE,
                          9 * 4,
                          (GLvoid*)offsetof(Vertex,Color));
    
    GLuint texCoordLocation = glGetAttribLocation(program->getProgram(), "a_texcoord");
    glEnableVertexAttribArray(texCoordLocation);
    glVertexAttribPointer(texCoordLocation,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          9 * 4,
                          (GLvoid*)offsetof(Vertex,TexCoord));
    
    GLuint indexVBO;
    glGenBuffers(1, &indexVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, oceanCol * oceanRow * 6 * 2 , indices, GL_STATIC_DRAW);
    
    program->autorelease();
    
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    return true;
}

void HelloWorld::update(float dt)
{
    time += dt;
}

void HelloWorld::visit(cocos2d::Renderer *renderer, const Mat4 &transform, uint32_t transformUpdated)
{
    Layer::draw(renderer, transform, transformUpdated);
    
    _customCommand.init(_globalZOrder);
    _customCommand.func = CC_CALLBACK_0(HelloWorld::onDraw, this);
    renderer->addCommand(&_customCommand);
    
}

void HelloWorld::onDraw()
{
    Director::getInstance()->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    Director::getInstance()->loadIdentityMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    Director::getInstance()->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
    Director::getInstance()->loadIdentityMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
    
    auto size = Director::getInstance()->getWinSize();
    
    Mat4 modelViewMatrix;
    Mat4::createLookAt(Vec3(0,0,1), Vec3(0,0,0), Vec3(0,1,0), &modelViewMatrix);
    
    modelViewMatrix.translate(-oceanWidth/2.f, 0.5, -5);
    
    static float rotation = 20;
    modelViewMatrix.rotate(Vec3(1,0,0),CC_DEGREES_TO_RADIANS(rotation));
    
    
    Mat4 projectionMatrix;
    Mat4::createPerspective(60, size.width/size.height, 1.0, 42, &projectionMatrix);
    Director::getInstance()->multiplyMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION, projectionMatrix);
    Director::getInstance()->multiplyMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, modelViewMatrix);
    
    
    auto glProgram = getGLProgram();
    glProgram->use();
    glProgram->setUniformsForBuiltins();
    
    glBindVertexArray(vao);
    
    // Setup Wave
    GLuint uWaveTimeLocation = glGetUniformLocation(glProgram->getProgram(), "u_waveTime");
    glUniform1f(uWaveTimeLocation, time);
    
    GLuint uWaveHeightLocation = glGetUniformLocation(glProgram->getProgram(), "u_waveHeight");
    glUniform1f(uWaveHeightLocation, .3f);
    
    GLuint uWaveWidthLocation = glGetUniformLocation(glProgram->getProgram(), "u_waveWidth");
    glUniform1f(uWaveWidthLocation, .9f);
    
    GLuint uWaveScreenWidth = glGetUniformLocation(glProgram->getProgram(), "u_screenWidth");
    glUniform1f(uWaveScreenWidth, size.width);
    
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,  GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    
    GL::bindTexture2D(oceanColorTextureId);
    
    glDrawElements(GL_TRIANGLES, oceanRow * oceanCol * 6, GL_UNSIGNED_SHORT,(GLvoid*)0);
    
    glBindVertexArray(0);
    
    CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, oceanRow * oceanCol * 6);
    CHECK_GL_ERROR_DEBUG();
    
    Director::getInstance()->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
    
    Director::getInstance()->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}

HelloWorld::VertexData HelloWorld::generateOceanVertices(int col, int row, float width, float height)
{
    VertexData vertsData;
    Vertex* verts = new Vertex[col * row * 4];
    GLushort* indices = new GLushort[col * row * 6];
    
    float texTileU = 9;
    float texTileV = 3;
    
    for (int j=0; j<row; j++) {
        for (int i=0; i<col; i++) {
            int vIdx = (j*col*4) + i*4;
            int iIdx = (j*col*6) + i*6;
            float color = 1.f;
            
            // Set Vertices
            verts[vIdx] = {{
                i*(width/(float)col),
                0,
                j*(height/(float)row)},
                {color,color,color,1},
                {i*(1.f/(float)col) * texTileU,j*(1.f/(float)row) * texTileV}};
            
            verts[vIdx+1] = {{
                (i+1)*(width/(float)col),
                0,
                j*(height/(float)row)},
                {color,color,color,1},
                {(i+1)*(1.f/(float)col) * texTileU,j*(1.f/(float)row) * texTileV}};
            
            verts[vIdx+2] = {{
                i*(width/(float)col),
                0,
                (j+1)*(height/(float)row)},
                {color,color,color,1},
                {i*(1.f/(float)col) * texTileU,(j+1)*(1.f/(float)row) * texTileV}};
            
            verts[vIdx+3] = {{
                (i+1)*(width/(float)col),
                0,
                (j+1)*(height/(float)row)},
                {color,color,color,1},
                {(i+1)*(1.f/(float)col) * texTileU,(j+1)*(1.f/(float)row) * texTileV}};
            
            // Set Indices
            indices[iIdx] = vIdx;
            indices[iIdx+1] = vIdx+1;
            indices[iIdx+2] = vIdx+2;
            indices[iIdx+3] = vIdx+2;
            indices[iIdx+4] = vIdx+1;
            indices[iIdx+5] = vIdx+3;
        }
    }
    
    vertsData.vertices = verts;
    vertsData.indices = indices;
    
    return vertsData;
}