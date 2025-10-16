#include "EarthRenderer.h"
#include "Constants.h"
#include "rlgl.h"
#include <iostream>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


// This method sucks but I havent found a better way to do it and make it work properly
EarthRenderer::EarthRenderer() 
    : textureLoaded(false), rotationEnabled(true), rotationAngle(0.0f) {}

EarthRenderer::~EarthRenderer() {
    unload();
}

// Generate sphere with proper UV mapping for equirectangular textures
Mesh GenerateSphereWithCorrectUVs(float radius, int rings, int slices) {
    Mesh mesh = { 0 };

    int vertexCount = (rings + 1) * (slices + 1);
    int triangleCount = rings * slices * 2;

    mesh.vertexCount = vertexCount;
    mesh.triangleCount = triangleCount;
    
    mesh.vertices = (float *)malloc(vertexCount * 3 * sizeof(float));
    mesh.texcoords = (float *)malloc(vertexCount * 2 * sizeof(float));
    mesh.normals = (float *)malloc(vertexCount * 3 * sizeof(float));
    mesh.indices = (unsigned short *)malloc(triangleCount * 3 * sizeof(unsigned short));

    int vertexIndex = 0;
    
    // Generate vertices, normals, and UVs
    for (int ring = 0; ring <= rings; ring++) {
        float v = (float)ring / (float)rings;  // 0 to 1 from top to bottom
        float phi = v * M_PI;  // 0 to PI
        
        for (int slice = 0; slice <= slices; slice++) {
            float u = (float)slice / (float)slices;  // 0 to 1 around
            float theta = u * 2.0f * M_PI;  // 0 to 2*PI
            
            // Position
            float x = radius * sinf(phi) * cosf(theta);
            float y = radius * cosf(phi);
            float z = radius * sinf(phi) * sinf(theta);
            
            mesh.vertices[vertexIndex * 3 + 0] = x;
            mesh.vertices[vertexIndex * 3 + 1] = y;
            mesh.vertices[vertexIndex * 3 + 2] = z;
            
            // Normal (normalized position for a sphere)
            mesh.normals[vertexIndex * 3 + 0] = x / radius;
            mesh.normals[vertexIndex * 3 + 1] = y / radius;
            mesh.normals[vertexIndex * 3 + 2] = z / radius;
            
            // UV coordinates (corrected for equirectangular projection)
            mesh.texcoords[vertexIndex * 2 + 0] = u;
            mesh.texcoords[vertexIndex * 2 + 1] = v;
            
            vertexIndex++;
        }
    }

    // Generate indices
    int indexIndex = 0;
    for (int ring = 0; ring < rings; ring++) {
        for (int slice = 0; slice < slices; slice++) {
            int current = ring * (slices + 1) + slice;
            int next = current + slices + 1;
            
            // First triangle
            mesh.indices[indexIndex++] = current;
            mesh.indices[indexIndex++] = next;
            mesh.indices[indexIndex++] = current + 1;
            
            // Second triangle
            mesh.indices[indexIndex++] = current + 1;
            mesh.indices[indexIndex++] = next;
            mesh.indices[indexIndex++] = next + 1;
        }
    }

    UploadMesh(&mesh, false);
    return mesh;
}

void EarthRenderer::load() {
    std::cout << "Loading Earth...\n";
    
    // Generate sphere with proper UV mapping for equirectangular textures
    Mesh sphereMesh = GenerateSphereWithCorrectUVs(EARTH_RADIUS * SCALE, 64, 64);
    earthModel = LoadModelFromMesh(sphereMesh);
    
    earthTexture = LoadTexture("../assets/textures/earth.png");
    if (earthTexture.id == 0) {
        earthTexture = LoadTexture("../assets/textures/earth.jpg"); 
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
            rlRotatef(23.5f, 0, 0, 1);
            rlRotatef(-90.0f, 0, 1, 0);
            rlRotatef(rotationAngle, 0, 1, 0);
            DrawModel(earthModel, Vector3{0, 0, 0}, 1.0f, WHITE);
        rlPopMatrix();
    } else {
        rlPushMatrix();
            rlRotatef(23.5f, 0, 0, 1);
            rlRotatef(-90.0f, 0, 1, 0);
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