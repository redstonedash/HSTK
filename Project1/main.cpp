#include <vector>
#include <stdio.h>
#include <random>
#include <iterator>
#include <algorithm>
#include <cmath>
#include "rlgl.h"
#include "ExternalStructModual.h"
#include "RenderModual.h"
#include "EditorModual.h"
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
Shader ortho;
Model cone;
ecs_entity_t selected;

struct intVector2 {
	int x;
	int y;
};

struct input {
	Vector2 mousePositionOnLastClick;
	Vector2 lastMousePosition;
};



#pragma region gridDirection

void DrawGridTop(int slices, float spacing)
{
	int halfSlices = slices / 2;

	if (rlCheckBufferLimit(slices * 4)) rlglDraw();

	rlBegin(RL_LINES);
	for (int i = -halfSlices; i <= halfSlices; i++)
	{
		if (i == 0)
		{
			rlColor3f(0.5f, 0.5f, 0.5f);
			rlColor3f(0.5f, 0.5f, 0.5f);
			rlColor3f(0.5f, 0.5f, 0.5f);
			rlColor3f(0.5f, 0.5f, 0.5f);
		}
		else
		{
			rlColor3f(0.75f, 0.75f, 0.75f);
			rlColor3f(0.75f, 0.75f, 0.75f);
			rlColor3f(0.75f, 0.75f, 0.75f);
			rlColor3f(0.75f, 0.75f, 0.75f);
		}

		rlVertex3f((float)i * spacing, 0.0f, (float)-halfSlices * spacing);
		rlVertex3f((float)i * spacing, 0.0f, (float)halfSlices * spacing);

		rlVertex3f((float)-halfSlices * spacing, 0.0f, (float)i * spacing);
		rlVertex3f((float)halfSlices * spacing, 0.0f, (float)i * spacing);
	}
	rlEnd();
}

void DrawGridRight(int slices, float spacing)
{
	int halfSlices = slices / 2;

	if (rlCheckBufferLimit(slices * 4)) rlglDraw();

	rlBegin(RL_LINES);
	for (int i = -halfSlices; i <= halfSlices; i++)
	{
		if (i == 0)
		{
			rlColor3f(0.5f, 0.5f, 0.5f);
			rlColor3f(0.5f, 0.5f, 0.5f);
			rlColor3f(0.5f, 0.5f, 0.5f);
			rlColor3f(0.5f, 0.5f, 0.5f);
		}
		else
		{
			rlColor3f(0.75f, 0.75f, 0.75f);
			rlColor3f(0.75f, 0.75f, 0.75f);
			rlColor3f(0.75f, 0.75f, 0.75f);
			rlColor3f(0.75f, 0.75f, 0.75f);
		}

		rlVertex3f(0.0f, (float)i * spacing, (float)-halfSlices * spacing);
		rlVertex3f(0.0f, (float)i * spacing, (float)halfSlices * spacing);

		rlVertex3f(0.0f, (float)-halfSlices * spacing, (float)i * spacing);
		rlVertex3f(0.0f, (float)halfSlices * spacing, (float)i * spacing);
	}
	rlEnd();
}

void DrawGridFront(int slices, float spacing)
{
	int halfSlices = slices / 2;

	if (rlCheckBufferLimit(slices * 4)) rlglDraw();

	rlBegin(RL_LINES);
	for (int i = -halfSlices; i <= halfSlices; i++)
	{
		if (i == 0)
		{
			rlColor3f(0.5f, 0.5f, 0.5f);
			rlColor3f(0.5f, 0.5f, 0.5f);
			rlColor3f(0.5f, 0.5f, 0.5f);
			rlColor3f(0.5f, 0.5f, 0.5f);
		}
		else
		{
			rlColor3f(0.75f, 0.75f, 0.75f);
			rlColor3f(0.75f, 0.75f, 0.75f);
			rlColor3f(0.75f, 0.75f, 0.75f);
			rlColor3f(0.75f, 0.75f, 0.75f);
		}

		rlVertex3f((float)i * spacing, (float)-halfSlices * spacing, 0.0f);
		rlVertex3f((float)i * spacing, (float)halfSlices * spacing, 0.0f);

		rlVertex3f((float)-halfSlices * spacing, (float)i * spacing, 0.0f);
		rlVertex3f((float)halfSlices * spacing, (float)i * spacing, 0.0f);
	}
	rlEnd();
}

void DrawGizmo(Vector3 position, float length) {
	rlPushMatrix();
	rlTranslatef(position.x, position.y, position.z);
	rlScalef(length, length, length);

	rlBegin(RL_LINES);
	rlColor3f(1.0f, 0.0f, 0.0f); rlVertex3f(0.0f, 0.0f, 0.0f);
	rlColor3f(1.0f, 0.0f, 0.0f); rlVertex3f(1.0f, 0.0f, 0.0f);

	rlColor3f(0.0f, 1.0f, 0.0f); rlVertex3f(0.0f, 0.0f, 0.0f);
	rlColor3f(0.0f, 1.0f, 0.0f); rlVertex3f(0.0f, 1.0f, 0.0f);

	rlColor3f(0.0f, 0.0f, 1.0f); rlVertex3f(0.0f, 0.0f, 0.0f);
	rlColor3f(0.0f, 0.0f, 1.0f); rlVertex3f(0.0f, 0.0f, 1.0f);
	rlEnd();
	rlPopMatrix();
}

#pragma endregion


input inputManager = {};

enum direction { top, right, front }; // xz, yz, xy



void HandleOrthoWindowInput(float*windowZoom, Vector2*windowOffset, Rectangle window, Camera c) {
	if (CheckCollisionPointRec(GetMousePosition(), window)) { //in window
		*windowZoom = std::fmax(*windowZoom - ((float)GetMouseWheelMove() * 0.1f), 0.1f);	
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			ecs_run(gameState, FSelect, -1, nullptr);
		}
	}
	if (CheckCollisionPointRec(inputManager.mousePositionOnLastClick,window)) {
		if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
			*windowOffset = Vector2Add(*windowOffset, Vector2Scale(Vector2Divide(Vector2Subtract(GetMousePosition(), inputManager.lastMousePosition), window.height), std::pow(*windowZoom, 5.0f)));
		}
	}
}
//bool Gizmos(Camera c, Rectangle window, char dir, float zoom) {
//	bool clicked = false;
//	zoom *= 0.25f;
//	float scale = zoom * 0.125;
//	Vector3 position = { 0,0,0 };
//	if (!selectedGameObjects.empty()) {
//		position = (*selectedGameObjects.back()).transform.position;
//		if (dir & 0b00000001) { //x
//			DrawModelEx(cone, Vector3Add(position, { zoom,0,0 }), { 0,0,1 }, -90, { scale, scale, scale }, RED);
//			if (CheckCollisionPointCircle(Vector2Subtract(inputManager.lastMousePosition, { window.x,window.y }), GetWorldToTexture({ zoom,0,0 }, c, window.width, window.height),25.0f) //TODO check aabb of cone
//				&&  IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
//				clicked = true;
//
//			}
//		}
//		if (dir & 0b00000010) { //y
//			DrawModelEx(cone, Vector3Add(position, { 0,zoom,0 }), { 0,0,1 }, 0, { scale, scale, scale }, GREEN);
//		}
//		if (dir & 0b00000100) { //z
//			DrawModelEx(cone, Vector3Add(position, { 0,0,zoom }), { 1,0,0 }, 90, { scale, scale, scale }, BLUE);
//		}
//	}
//	DrawGizmo(position, zoom);
//	return clicked;
//}

void topWindow(int i) {
	Vector2 windowResolution = { (float) windows[i].width, (float) windows[i].height };
	Camera c = { {topWindowOffset.x,1,topWindowOffset.y}, {topWindowOffset.x,0,topWindowOffset.y}, {0,0,1}, std::pow(topWindowZoom,5.0f), CAMERA_ORTHOGRAPHIC };
	HandleOrthoWindowInput(&topWindowZoom, &topWindowOffset, windows[i], c);
	BeginTextureMode(renderTextures[i]);
		DrawText("Top", 0, 0, 12, LIME);
		ClearBackground({ (unsigned char)5,(unsigned char)5,(unsigned char)40,(unsigned char)255 });
		BeginMode3D({ {topWindowOffset.x,1,topWindowOffset.y}, {topWindowOffset.x,0,topWindowOffset.y}, {0,0,1}, std::pow(topWindowZoom,5.0f), CAMERA_ORTHOGRAPHIC });
			DrawGridTop(10, 1);
			
			//Gizmos(c, windows[i], 0b00000101, std::pow(topWindowZoom, 5.0f));
		EndMode3D();
	EndTextureMode();
}
void frontWindow(int i) {
	Vector2 windowResolution = { (float)windows[i].width, (float)windows[i].height };
	Camera c = { {frontWindowOffset.x,frontWindowOffset.y,-1}, {frontWindowOffset.x,frontWindowOffset.y,0}, {0,1,0}, std::pow(frontWindowZoom,5.0f), CAMERA_ORTHOGRAPHIC };
	HandleOrthoWindowInput(&frontWindowZoom, &frontWindowOffset, windows[i], c);
	BeginTextureMode(renderTextures[i]);
	DrawText("Front", 0, 0, 12, LIME);
	ClearBackground({ (unsigned char)5,(unsigned char)5,(unsigned char)40,(unsigned char)255 });
	BeginMode3D({ {frontWindowOffset.x,frontWindowOffset.y,-1}, {frontWindowOffset.x,frontWindowOffset.y,0}, {0,1,0}, std::pow(frontWindowZoom,5.0f), CAMERA_ORTHOGRAPHIC });
	DrawGridFront(10, 1);
	ecs_run(gameState, FDrawOrthoWindow, -1, nullptr);
	//Gizmos(c, windows[i], 0b00000011, std::pow(frontWindowZoom, 5.0f));
	EndMode3D();
	EndTextureMode();
}
void rightWindow(int i) {
	Vector2 windowResolution = { (float)windows[i].width, (float)windows[i].height };
	Camera c = { {1,rightWindowOffset.y,rightWindowOffset.x}, {0,rightWindowOffset.y,rightWindowOffset.x}, {0,1,0}, std::pow(rightWindowZoom,5.0f), CAMERA_ORTHOGRAPHIC };
	HandleOrthoWindowInput(&rightWindowZoom, &rightWindowOffset, windows[i], c);
	BeginTextureMode(renderTextures[i]);
	DrawText("Right", 0, 0, 12, LIME);
	ClearBackground({ (unsigned char)5,(unsigned char)5,(unsigned char)40,(unsigned char)255 });
	BeginMode3D(c);
	DrawGridRight(10, 1);
	ecs_run(gameState, FDrawOrthoWindow, -1, nullptr);
	//Gizmos(c, windows[i], 0b00000110, std::pow(rightWindowZoom, 5.0f));
	EndMode3D();
	EndTextureMode();
}
void todo(int i) {
	srand(i);
	Color randomColor = { ((char)rand()),((char)rand()),((char)rand()),255 };
	BeginTextureMode(renderTextures[i]);
	ClearBackground(randomColor);
	EndTextureMode();
	DrawTextureRec(renderTextures[i].texture, { 0,0,windows[i].width,windows[i].height }, { windows[i].x, windows[i].y }, WHITE);
}

void (*windowBehaviour[])(int) = { todo, rightWindow, topWindow, todo, frontWindow, todo, todo, todo };

int main() {
	InitWindow(width, height, "HSTK - Editor");
	SetConfigFlags(FLAG_MSAA_4X_HINT);
	
	ortho = LoadShader("resources/shaders/glsl330/ortho.vs", "resources/shaders/glsl330/base.fs");
	cone = LoadModel("resources/models/cone.obj");
	cone.materials[0].shader = ortho;
	ECS_ENTITY(gameState, testObject1, Model, Transform, BoundingBox);
	ECS_ENTITY(gameState, testObject2, Model, Transform, BoundingBox);
	Model knot = LoadModelFromMesh(GenMeshKnot(1.0f, 2.0f, 16, 128));
	ecs_set_ref(gameState, testObject1, Model, knot);
	ecs_set_ref(gameState, testObject1, BoundingBox, MeshBoundingBox(knot.meshes[0]));
	Model cylinder = LoadModelFromMesh(GenMeshCylinder(2, 1, 32));
	ecs_set_ref(gameState, testObject2, Model, cylinder);
	ecs_set_ref(gameState, testObject2, BoundingBox, MeshBoundingBox(cylinder.meshes[0]));;
	
	
	for (int i = 0; i < sizeof(windows) / sizeof(Rectangle); i++) {
		renderTextures[i] = LoadRenderTexture(windows[i].width, windows[i].height);
		int intVec[] = { 0,0 };
	}
	while(!WindowShouldClose()) {
		if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
			inputManager.mousePositionOnLastClick = GetMousePosition();
		}
		BeginDrawing();
		ClearBackground(WHITE);
		for (int i = 0; i < sizeof(windows)/sizeof(Rectangle); i++) {
			windowBehaviour[i](i);
			DrawTextureRec(renderTextures[i].texture, { 0,0,windows[i].width,-windows[i].height }, { windows[i].x, windows[i].y }, WHITE);
		}
		inputManager.lastMousePosition = GetMousePosition();
		EndDrawing();
	}
	return 0;
}