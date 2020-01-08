//#include "PlayerControllerModual.h"

#include "ExternalStructModual.h"
#include "RenderModual.h"

void RegisterStuff() {
    RegisterExternalStructModualComponents();
    RegisterRenderComponents();
    RegisterRenderSystems();
}
int main()
{
    //init
    InitWindow(300, 200, "HSTK");
    gameState = ecs_init();
    RegisterStuff();
    ecs_entity_t gamePiece1 = ecs_new_entity(gameState, "gamePiece1", "Model");
    ECS_ENTITY(gameState, mainCamera, cubeMapCamera, Transform);
    ecs_set(gameState, mainCamera, Transform, { {0,0,0}, QuaternionIdentity(), {1,1,1} });
    Model mdl = LoadModel("Assets/arena.obj");
    ecs_set_ref(gameState, gamePiece1, Model, mdl);
    //ecs_set_ref(gameState, gamePiece1, Model, mdl);
    Model kube = LoadModelFromMesh(GenMeshCube(1, 1, 1));
    while (!WindowShouldClose()) {
        ClearBackground(GRAY);
        ecs_progress(gameState,GetFrameTime());
        //update
       
        //draw (calls manual draw systems to maintain clearcut order of the render pipeline)
        BeginDrawing();
        for (int i = 0; i < 6; i++) {
            cubeMapCamera camera = ecs_get(gameState, mainCamera, cubeMapCamera);
            BeginTextureMode(camera.textures[i]); //TODO put real render texture
            ClearBackground(GRAY);
            BeginMode3D(camera.cameras[i]); //TODO put real camera here
            
            //ecs_run(gameState, FrenderModels, GetFrameTime(), nullptr);
            DrawModel(kube, { 1,0,0 },  1, RED);
            DrawModel(kube, { 0,1,0 },  1, GREEN);
            DrawModel(kube, { 0,0,1 },  1, BLUE);
            DrawModel(kube, { -1,0,0 }, 1, RED);
            DrawModel(kube, { 0,-1,0 }, 1, GREEN);
            DrawModel(kube, { 0,0,-1 }, 1, BLUE);
            
            //DrawCircle(50, 50, 50, RED);
            EndMode3D();
            EndTextureMode();
            
            
            DrawTexture(camera.textures[i].texture,100*(i%3), 100*(i/3),WHITE);
        }
        
        //DrawModel(mdl, { 0,0,0 }, 1, WHITE);
        
#ifdef EDITOR 
        //draw GUI
#endif // EDITOR 
        EndDrawing();
    }
}