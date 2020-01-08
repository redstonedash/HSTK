#include "boilerPlateIncludes.h"
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
ecs_entity_t selected;

struct intVector2 {
	int x;
	int y;
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

#pragma endregion


enum direction { top, right, front }; // xz, yz, xy



void HandleOrthoWindowInput(float*windowZoom, Vector2*windowOffset, Rectangle window, Camera c) {
	if (CheckCollisionPointRec(GetMousePosition(), window)) { //in window
		*windowZoom = std::fmax(*windowZoom - ((float)GetMouseWheelMove() * 0.1f), 0.1f);	
		if (inputManager.action) {
			byte buf[sizeof(c) + sizeof(window)];
			memcpy(buf, &c, sizeof(c));
			memcpy(buf + sizeof(c), &window, sizeof(window));
			ecs_run(gameState, FSelect, -1, &buf);
			inputManager.action = false;
		}
	}
	if (CheckCollisionPointRec(inputManager.mousePositionOnLastClick,window)) {
		if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
			*windowOffset = Vector2Add(*windowOffset, Vector2Scale(Vector2Divide(Vector2Subtract(GetMousePosition(), inputManager.lastMousePosition), window.height), std::pow(*windowZoom, 5.0f)));
		}
	}
}


void topWindow(int i) {
	Vector2 windowResolution = { (float) windows[i].width, (float) windows[i].height };
	Camera c = { {topWindowOffset.x,1,topWindowOffset.y}, {topWindowOffset.x,0,topWindowOffset.y}, {0,0,1}, std::pow(topWindowZoom,5.0f), CAMERA_ORTHOGRAPHIC };
	BeginTextureMode(renderTextures[i]);
		DrawText("Top", 0, 0, 12, LIME);
		ClearBackground({ (unsigned char)5,(unsigned char)5,(unsigned char)40,(unsigned char)255 });
		BeginMode3D({ {topWindowOffset.x,1,topWindowOffset.y}, {topWindowOffset.x,0,topWindowOffset.y}, {0,0,1}, std::pow(topWindowZoom,5.0f), CAMERA_ORTHOGRAPHIC });
			DrawGridTop(10, 1);
			ecs_run(gameState, FDrawOrthoWindow, -1, &ortho);
			byte buf[sizeof(Camera) + sizeof(Rectangle) + 1 + sizeof(float)];
			memcpy(buf, &c, sizeof(Camera));
			memcpy(buf + sizeof(Camera), &windows[i], sizeof(Rectangle));
			*(buf + sizeof(Camera) + sizeof(Rectangle)) = 0b00000101;
			*((float *)(buf + sizeof(Camera) + sizeof(Rectangle) + 1)) = std::pow(topWindowZoom, 5.0f); //this looks a little confusing but we are just building up a byte array to pass to the system
			ecs_run(gameState, FGizmos, -1, buf);
			//Gizmos(c, windows[i], 0b00000101, std::pow(topWindowZoom, 5.0f));
		EndMode3D();
	EndTextureMode();
	HandleOrthoWindowInput(&topWindowZoom, &topWindowOffset, windows[i], c);
}
void frontWindow(int i) {
	Vector2 windowResolution = { (float)windows[i].width, (float)windows[i].height };
	Camera c = { {frontWindowOffset.x,frontWindowOffset.y,-1}, {frontWindowOffset.x,frontWindowOffset.y,0}, {0,1,0}, std::pow(frontWindowZoom,5.0f), CAMERA_ORTHOGRAPHIC };
	BeginTextureMode(renderTextures[i]);
	DrawText("Front", 0, 0, 12, LIME);
	ClearBackground({ (unsigned char)5,(unsigned char)5,(unsigned char)40,(unsigned char)255 });
	BeginMode3D({ {frontWindowOffset.x,frontWindowOffset.y,-1}, {frontWindowOffset.x,frontWindowOffset.y,0}, {0,1,0}, std::pow(frontWindowZoom,5.0f), CAMERA_ORTHOGRAPHIC });
	DrawGridFront(10, 1);
	ecs_run(gameState, FDrawOrthoWindow, -1, &ortho);
	byte buf[sizeof(Camera) + sizeof(Rectangle) + 1 + sizeof(float)];
	memcpy(buf, &c, sizeof(Camera));
	memcpy(buf + sizeof(Camera), &windows[i], sizeof(Rectangle));
	*(buf + sizeof(Camera) + sizeof(Rectangle)) = 0b00000011;
	*((float*)(buf + sizeof(Camera) + sizeof(Rectangle) + 1)) = std::pow(frontWindowZoom, 5.0f); //this looks a little confusing but we are just building up a byte array to pass to the system
	ecs_run(gameState, FGizmos, -1, buf);
	//Gizmos(c, windows[i], 0b00000011, std::pow(frontWindowZoom, 5.0f));
	EndMode3D();
	EndTextureMode();
	HandleOrthoWindowInput(&frontWindowZoom, &frontWindowOffset, windows[i], c);
}
void rightWindow(int i) {
	Vector2 windowResolution = { (float)windows[i].width, (float)windows[i].height };
	Camera c = { {1,rightWindowOffset.y,rightWindowOffset.x}, {0,rightWindowOffset.y,rightWindowOffset.x}, {0,1,0}, std::pow(rightWindowZoom,5.0f), CAMERA_ORTHOGRAPHIC };
	BeginTextureMode(renderTextures[i]);
	DrawText("Right", 0, 0, 12, LIME);
	ClearBackground({ (unsigned char)5,(unsigned char)5,(unsigned char)40,(unsigned char)255 });
	BeginMode3D(c);
	DrawGridRight(10, 1);
	ecs_run(gameState, FDrawOrthoWindow, -1, &ortho);
	byte buf[sizeof(Camera) + sizeof(Rectangle) + 1 + sizeof(float)];
	memcpy(buf, &c, sizeof(Camera));
	memcpy(buf + sizeof(Camera), &windows[i], sizeof(Rectangle));
	*(buf + sizeof(Camera) + sizeof(Rectangle)) = 0b00000110;
	*((float*)(buf + sizeof(Camera) + sizeof(Rectangle) + 1)) = std::pow(rightWindowZoom, 5.0f); //this looks a little confusing but we are just building up a byte array to pass to the system
	ecs_run(gameState, FGizmos, -1, buf);
	//Gizmos(c, windows[i], 0b00000110, std::pow(rightWindowZoom, 5.0f));
	EndMode3D();
	EndTextureMode();
	HandleOrthoWindowInput(&rightWindowZoom, &rightWindowOffset, windows[i], c);
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
	gameState = ecs_init();
	RegisterExternalStructModualComponents();
	RegisterEditorModualComponents();
	RegisterEditorModualSystems();
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
		inputManager.action = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
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