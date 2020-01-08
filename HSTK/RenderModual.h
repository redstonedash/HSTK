#pragma once
#pragma region ComponenetDeclares

struct cubeMapCamera {
    Camera3D cameras[6];
    RenderTexture textures[6];
};

#define COMPONENTS \
COMPONENT(cubeMapCamera);\

#define COMPONENT(id) ecs_entity_t ecs_entity(id);
COMPONENTS;
#define COMPONENT(id) ecs_type_t ecs_type(id);
COMPONENTS;
#pragma endregion
#define SYSTEMS SYSTEM(renderModels, EcsManual, Model);\
SYSTEM(updateCamera, EcsPostUpdate, cubeMapCamera, Transform);\
SYSTEM(initCamera, EcsOnAdd, cubeMapCamera);\

#pragma region SystemDeclares

#define SYSTEM(id, kind, ...) ecs_entity_t F##id;\

SYSTEMS

#define SYSTEM(id, kind, ...) ecs_type_t ecs_type(id);

SYSTEMS

#pragma endregion

void renderModels(ecs_rows_t * row) {
    ECS_COLUMN(row, Model, mdl, 1);
    for (int i = 0; i<row->count; i++) {
        DrawModel(mdl[i], { 0,0,0 }, 1, WHITE);
    } 
}

void updateCamera(ecs_rows_t* row) {
    ECS_COLUMN(row, cubeMapCamera, cmc, 1);
    ECS_COLUMN(row, Transform, trs, 2);
    for (int entity = 0; entity < row->count; entity++) {
        auto rot = QuaternionToMatrix(trs[entity].rotation);
        auto trans = MatrixTranslate(trs[entity].position.x, trs[entity].position.y, trs[entity].position.z);
        auto scale = MatrixScale(trs[entity].scale.x, trs[entity].scale.y, trs[entity].scale.z);

        Matrix trsMatrix = MatrixMultiply(MatrixMultiply(scale, rot), trans);
        const Vector3 cardinals[] = { {1,0,0}, {0,1,0}, {0,0,1}, {-1,0,0}, {0,-1,0}, {0,0,-1}}; //forward, up, right, back, own, left
        for (int i = 0; i < 6; i++) {
            cmc[entity].cameras[i].position = Vector3Transform({ 0,0,0 }, trsMatrix);
            cmc[entity].cameras[i].target = Vector3Transform(cardinals[i], trsMatrix);
            cmc[entity].cameras[i].up = Vector3Transform(cardinals[(i+1)%6], trsMatrix);
        }
    }
}
const int size = 100; //width and height are equal
void initCamera(ecs_rows_t* row) {
    ECS_COLUMN(row, cubeMapCamera, cmc, 1);
    for (int entity = 0; entity < row->count; entity++) {
        //not using a for loop because the sides on a cube map will never change
        cmc[entity].textures[0] = LoadRenderTexture(size, size);
        cmc[entity].textures[1] = LoadRenderTexture(size, size);
        cmc[entity].textures[2] = LoadRenderTexture(size, size);
        cmc[entity].textures[3] = LoadRenderTexture(size, size);
        cmc[entity].textures[4] = LoadRenderTexture(size, size);
        cmc[entity].textures[5] = LoadRenderTexture(size, size);
        cmc[entity].cameras[0].fovy = 90;
        cmc[entity].cameras[1].fovy = 90;
        cmc[entity].cameras[2].fovy = 90;
        cmc[entity].cameras[3].fovy = 90;
        cmc[entity].cameras[4].fovy = 90;
        cmc[entity].cameras[5].fovy = 90;
        cmc[entity].cameras[0].type = CAMERA_PERSPECTIVE;
        cmc[entity].cameras[1].type = CAMERA_PERSPECTIVE;
        cmc[entity].cameras[2].type = CAMERA_PERSPECTIVE;
        cmc[entity].cameras[3].type = CAMERA_PERSPECTIVE;
        cmc[entity].cameras[4].type = CAMERA_PERSPECTIVE;
        cmc[entity].cameras[5].type = CAMERA_PERSPECTIVE;
    }
}

#pragma region RegisterSystemsAndComponenets

#define SYSTEM(id, kind, ...) F##id = ecs_new_system(gameState, #id, kind, #__VA_ARGS__, id);\
    ecs_type(id) = ecs_type_from_entity(gameState, F##id);\

#define COMPONENT(id) ecs_entity(id) = ecs_new_component(gameState, #id, sizeof(id));\
    ecs_type(id) = ecs_type_from_entity(gameState, ecs_entity(id));\
    (void)ecs_entity(id);\
    (void)ecs_type(id);\

void RegisterRenderComponents() {
    COMPONENTS;
}

void RegisterRenderSystems() {
    SYSTEMS
}
#pragma endregion