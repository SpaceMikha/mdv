#include "EarthRenderer.h"
#include "Constants.h"
#include "rlgl.h"
#include <iostream>

EarthRenderer::EarthRenderer() 
    : textureLoaded(false), rotationEnabled(true), rotationAngle(0.0f) {}

EarthRenderer::~EarthRenderer() {
    unload();
}

void EarthRenderer::load() {
    std::cout << "Loading Earth...\n";
    
    Mesh sphereMesh = GenMeshSphere(EARTH_RADIUS * SCALE, 64, 64);
    earthModel = LoadModelFromMesh(sphereMesh);
    
    earthTexture = LoadTexture("../assets/textures/planet.png");
    if (earthTexture.id == 0) {
        earthTexture = LoadTexture("../assets/textures/planet.jpg"); // for now using 'planet' texture as earth one is not working
    }
    
    if (earthTexture.id != 0) {
        std::cout << "  Earth texture loaded!\n";
        
        SetTextureWrap(earthTexture, TEXTURE_WRAP_REPEAT);
        SetTextureFilter(earthTexture, TEXTURE_FILTER_BILINEAR);
        
        SetMaterialTexture(&earthModel.materials[0], MATERIAL_MAP_DIFFUSE, earthTexture);
        textureLoaded = true;
    } else {
        std::cout << "  No texture found, using default blue sphere\n";
        textureLoaded = false;
    }
}

void EarthRenderer::update(float deltaTime) {
    if (rotationEnabled) {
        rotationAngle += deltaTime * 6.0f;
        if (rotationAngle >= 360.0f) rotationAngle -= 360.0f;
    }
}

void EarthRenderer::draw() const {
    if (textureLoaded) {
        rlPushMatrix();
            rlRotatef(180.0f, 1, 0, 0);
            rlRotatef(180.0f, 0, 1, 0);
            rlRotatef(-23.5f, 1, 0, 0);
            rlRotatef(rotationAngle, 0, 1, 0);
            DrawModel(earthModel, Vector3{0, 0, 0}, 1.0f, WHITE);
        rlPopMatrix();
    } else {
        rlPushMatrix();
            rlRotatef(23.5f, 1, 0, 0);
            rlRotatef(rotationAngle, 0, 1, 0);
            DrawSphere(Vector3{0, 0, 0}, EARTH_RADIUS * SCALE, BLUE);
        rlPopMatrix();
    }
}

void EarthRenderer::unload() {
    if (textureLoaded) {
        UnloadTexture(earthTexture);
        textureLoaded = false;
    }
    UnloadModel(earthModel);
}