# Cocos2DPerlinNoiseOcean

Cocos2DPerlinNoiseOcean is a test I did for an interview.  The test was to create a 3D ocean within Cocos2D-x so I've generated the ocean mesh with OpenGL and written some shaders to make it looks nicer.  

Ocean was animated by moving vertices in **vertex shader** with **perlin noise** and **sin waves**, but later changed to use **simplex noise** instead.  So the repo name is actually inaccurate now and should be called SimplexNoiseOcean.  

As this is done with a simple mesh and a simple shader, performance cost is relatively cheap and this should run in low end mobile devices that supports **OpenGLES 2.0**.

![Alt text](README/OceanGif.gif?raw=true "Title")


## Cocos2D-x Version

This project is created using *cocos2d-x 3.4*.  Please include dependency library accordingly.  


## Platform Support

This project supports iOS and Android

### iOS Project
Path: `proj.ios_mac/Cocos2DDynamicBokeh.xcodeproj/project.pbxproj`

### Android Build
Path: `proj.android/build_native.py`
