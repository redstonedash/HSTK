#pragma once
Shader ortho;
Model cone;

#pragma endregion
#define SYSTEMS SYSTEM(Select, EcsManual, BoundingBox);\
SYSTEM(Deselect, EcsManual, BoundingBox);\
SYSTEM(DrawOrthoWindow, EcsManual, Model, Transform, BoundingBox, ?Selected);\
SYSTEM(Gizmos, EcsManual, Transform, Selected);\
SYSTEM(Move, EcsManual, Transform, Selected);\


#pragma region SystemDeclares

#define SYSTEM(id, kind, ...) ecs_entity_t F##id;\

SYSTEMS

#define SYSTEM(id, kind, ...) ecs_type_t ecs_type(id);

SYSTEMS

#pragma endregion

void DrawGizmo(Vector3 position, float length) { //TODO make system
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

void DrawOrthoWindow(ecs_rows_t* rows) {
	ECS_COLUMN(rows, Model, mdl, 1);
	ECS_COLUMN(rows, Transform, trs, 2);
	ECS_COLUMN(rows, BoundingBox, aabb, 3);
	ECS_COLUMN(rows, Selected, s, 4);
	Shader ortho = *((Shader*)rows->param);
	for (int i = 0; i < rows->count; i++) {
		Color aabbColor = ORANGE;
		Color modelColor = SKYBLUE;
		if (s) { //the system is ran twice, once for entities with (model, tranform, boundingbox, selected), and one for (model, tranform, boundingbox)
			aabbColor = { 255,180,100,255 };
			modelColor = { 97, 255, 144, 255 };
		}
		DrawBoundingBox(aabb[i], aabbColor);
		Shader temp = mdl[i].materials[0].shader;
		mdl[i].materials[0].shader = ortho;
		DrawModelWires(mdl[i], trs[i].translation, 1, modelColor);
		mdl[i].materials[0].shader = temp;
	}
}

void Deselect(ecs_rows_t* rows) {
	for (int i = 0; i < rows->count; i++) {
		ecs_remove(gameState, rows->entities[i], Selected);
	}
}

void Move(ecs_rows_t* rows) {
	ECS_COLUMN(rows, Transform, trs, 1);
	Vector3 moveDelta = *((Vector3 *)rows->param);
	for (int i = 0; i < rows->count; i++) {
		trs[i].translation = Vector3Add(trs[i].translation, moveDelta);
	}
}

void Gizmos(ecs_rows_t* rows) {
	Camera c = *((Camera*)rows->param);
	rows->param = (((Camera*)(rows->param)) + 1);
	Rectangle window = *((Rectangle*)rows->param); 
	rows->param = (((Rectangle*)(rows->param)) + 1);
	char dir = *((char*)rows->param); 
	rows->param = (((char*)(rows->param)) + 1);
	float zoom = *((float*)rows->param);
	ECS_COLUMN(rows, Transform, trs, 1);
	zoom *= 0.25f;
	float scale = zoom * 0.125;
	Vector3 position = { 0,0,0 };
		position = (trs[0]).translation;
		if (dir & 0b00000001) { //x
			DrawModelEx(cone, Vector3Add(position, { zoom,0,0 }), { 0,0,1 }, -90, { scale, scale, scale }, RED);
			if (CheckCollisionPointCircle(Vector2Subtract(inputManager.lastMousePosition, { window.x,window.y }), GetWorldToTexture({ zoom,0,0 }, c, window.width, window.height), 50.0f) //TODO check aabb of cone
				&& inputManager.action) {
				printf("gizmo\n");
				Vector3 delta = {(GetMousePosition().x-inputManager.lastMousePosition.x*zoom)/window.height,0,0 };
				ecs_run(gameState, FMove, -1, &delta);
				inputManager.action = false;
			}
		}
		if (dir & 0b00000010) { //y
			DrawModelEx(cone, Vector3Add(position, { 0,zoom,0 }), { 0,0,1 }, 0, { scale, scale, scale }, GREEN);
		}
		if (dir & 0b00000100) { //z
			DrawModelEx(cone, Vector3Add(position, { 0,0,zoom }), { 1,0,0 }, 90, { scale, scale, scale }, BLUE);
		}
	DrawGizmo(position, zoom);
}

void Select(ecs_rows_t* rows) {
	ECS_COLUMN(rows, BoundingBox, aabb, 1);
	Camera c = *((Camera*)(rows->param));
	rows->param = (((Camera*)(rows->param)) + 1);
	Rectangle window = *((Rectangle*)(rows->param));
	rows->param = (((Rectangle*)(rows->param)) + 1);
	for (int i = 0; i < rows->count; i++) {
		Vector2 min = GetWorldToTexture(aabb[i].min, c, window.width, window.height);
		Vector2 max = GetWorldToTexture(aabb[i].max, c, window.width, window.height);
		if (min.x > max.x) {
			std::swap(min.x, max.x);
		}
		if (min.y > max.y) {
			std::swap(min.y, max.y);
		}
		//printf("RECTANGLE x:%f y:%f width:%f height:%f", min.x, min.y, max.x - min.x, max.y - min.y);
		//DrawPixel(Vector2Subtract(GetMousePosition(), { window.x, window.y }).x, Vector2Subtract(GetMousePosition(), { window.x, window.y }).y, RED);
		//DrawRectangle(min.x, min.y, max.x - min.x, max.y - min.y, WHITE);
		if (IsKeyDown(KEY_LEFT_SHIFT)) {
			if (CheckCollisionPointRec(Vector2Subtract(GetMousePosition(), { window.x, window.y }), { min.x, min.y, max.x - min.x, max.y - min.y })) {
				ecs_add(gameState, rows->entities[i], Selected);
			}
		}
		else {
			ecs_run(gameState, FDeselect, -1.0f, nullptr); // manual system to remove selected from all object
			if (CheckCollisionPointRec(Vector2Subtract(GetMousePosition(), { window.x, window.y }), { min.x, min.y, max.x - min.x, max.y - min.y })) {
				ecs_add(gameState, rows->entities[i], Selected);
				printf("%s", "did thing");
				break;
			}
		}
	}
}

#pragma region RegisterSystemsAndComponenets
#define SYSTEM(id, kind, ...) F##id = ecs_new_system(gameState, #id, kind, #__VA_ARGS__, id);\
    ecs_type(id) = ecs_type_from_entity(gameState, F##id);\

#define COMPONENT(id) ecs_entity(id) = ecs_new_component(gameState, #id, sizeof(id));\
    ecs_type(id) = ecs_type_from_entity(gameState, ecs_entity(id));\
    (void)ecs_entity(id);\
    (void)ecs_type(id);\

void RegisterEditorModualComponents() {
    COMPONENTS;
}
void RegisterEditorModualSystems() {
    SYSTEMS
}
#pragma endregion

#pragma once