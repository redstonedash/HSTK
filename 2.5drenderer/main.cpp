#include <vector>
#include <stdio.h>
#include <random>
#include <iterator>
#include <algorithm>
#include <cmath>
#include "raylib.h"
#include "rlgl.h"

Rectangle windows[] = { {0,0,1453,28},{0,28,465,307}, {465,28,576,307}, {0,335,465,313}, {465, 335, 576,313 }, {1041,28,412,907}, {0,648,1041,43},{0,691,1041,245} };
RenderTexture2D renderTextures[sizeof(windows) / sizeof(Rectangle)];
	
int width = 1453;
int height = 935;
float topWindowZoom = 1;
Vector2 topWindowOffset = { 0,0 };
float rightWindowZoom = 1;
Vector2 rightWindowOffset = { 0,0 };
float frontWindowZoom = 1;
Vector2 frontWindowOffset = { 0,0 };
#pragma region gridDirection
Model plane;
Shader _2dLighting;
Texture tex;
int modelLoc;
int scaleLoc;
void reloadShader() {
 	_2dLighting = LoadShader("resources/2.5dLighting.vert", "resources/2.5dLighting.frag");
	if (_2dLighting.id == 0) {
		_2dLighting = LoadShader(0, 0);//default
	}
	else {
		modelLoc = GetShaderLocation(_2dLighting, "model");
		scaleLoc = GetShaderLocation(_2dLighting, "scale");
		
	}
	plane.materials[0].shader = _2dLighting;
	plane.materials[0].maps[0].texture = tex;
}

int main() {
	InitWindow(500, 500, "2.5d test");
	tex = LoadTexture("resources/Image0001.png");
	plane = LoadModelFromMesh(GenMeshPlane(1, 1, 1, 1));
	reloadShader();
	plane.materials[0].shader = _2dLighting;
	plane.materials[0].maps[0].texture = tex;
	while (!WindowShouldClose()) {
		
		if (IsKeyPressed(KEY_SPACE)) {
			reloadShader();
		}
		BeginDrawing();
		ClearBackground(BLACK);
		BeginMode3D({ {0,1,0},  {0,0,0}, {1,0,0}, 10, CAMERA_ORTHOGRAPHIC });
		//DrawCube({ 0,0,0 }, 1, 1, 1, RED);
		//DrawGrid(10, 1.0f);
		Ray r = GetMouseRay(GetMousePosition(), { {0,1,0},  {0,0,0}, {1,0,0}, 10, CAMERA_ORTHOGRAPHIC });
		Vector3 v = { r.position.x, 0, r.position.z };
		float s = 1;
		SetShaderValue(_2dLighting, modelLoc, &v, UNIFORM_VEC3);
		SetShaderValue(_2dLighting, scaleLoc, &s, UNIFORM_FLOAT);
		DrawModel(plane, v, s, WHITE);
		v = { 0, 0, 0 };
		//DrawModel(plane, v, 1, WHITE);
		EndMode3D();
		EndDrawing();
	}
	UnloadTexture(tex);
	UnloadShader(_2dLighting);
	return 0;
}