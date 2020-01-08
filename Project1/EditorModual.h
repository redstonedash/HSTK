#pragma once
#pragma region ComponenetDeclares
struct Selected {
};
#define COMPONENTS COMPONENT(Selected)\

#define COMPONENT(id) ecs_entity_t ecs_entity(id);
COMPONENTS;
#define COMPONENT(id) ecs_type_t ecs_type(id);
COMPONENTS;
#pragma endregion
#define SYSTEMS SYSTEM(Select, EcsManual, BoundingBox ! Selected);\
SYSTEM(Deselect, EcsManual, BoundingBox);\
SYSTEM(DrawOrthoWindow, EcsManual, Model, Transform, BoundingBox, ?Selected);\


#pragma region SystemDeclares

#define SYSTEM(id, kind, ...) ecs_entity_t F##id;\

SYSTEMS

#define SYSTEM(id, kind, ...) ecs_type_t ecs_type(id);

SYSTEMS

#pragma endregion

void DrawOrthoWindow(ecs_rows_t* rows) {
	ECS_COLUMN(rows, Model, mdl, 1);
	ECS_COLUMN(rows, Transform, trs, 2);
	ECS_COLUMN(rows, BoundingBox, aabb, 3);
	ECS_COLUMN(rows, Selected, s, 4);
	Shader ortho = *((Shader*)rows->param);
	for (int i = 0; i < rows->count; i++) {
		Color aabbColor = ORANGE;
		Color modelColor = SKYBLUE;
		if (s) {
			aabbColor = { 255,180,100,255 };
			modelColor = { 97, 255, 144, 255 };
		}
		DrawBoundingBox(aabb[i], aabbColor);
		Shader temp = mdl[i].materials[0].shader;
		mdl[i].materials[0].shader = ortho;
		DrawModelWires(mdl[i], { 0,0,0 }, 1, modelColor);
		mdl[i].materials[0].shader = temp;

	}
}

void Deselect(ecs_rows_t* rows) {
	for (int i = 0; i < rows->count; i++) {
		ecs_remove(gameState, rows->entities[i], Selected);
	}
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