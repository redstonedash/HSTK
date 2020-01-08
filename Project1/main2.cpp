//#include "PlayerControllerModual.h"
#include "raylib.h"
#include "raymath.h"
struct cubeMapCamera {
    Camera3D cameras[6];
    RenderTexture textures[6];
};
int main()
{

    //init
    InitWindow(300, 200, "HSTK");
    cubeMapCamera mainCamera = cubeMapCamera();
    Transform mainCameraTransform = { {0,0,0},QuaternionIdentity(),{1,1,1} };

    mainCamera.textures[0] = LoadRenderTexture(100, 100);
    mainCamera.textures[1] = LoadRenderTexture(100, 100);
    mainCamera.textures[2] = LoadRenderTexture(100, 100);
    mainCamera.textures[3] = LoadRenderTexture(100, 100);
    mainCamera.textures[4] = LoadRenderTexture(100, 100);
    mainCamera.textures[5] = LoadRenderTexture(100, 100);
    mainCamera.cameras[0].fovy = 90;
    mainCamera.cameras[1].fovy = 90;
    mainCamera.cameras[2].fovy = 90;
    mainCamera.cameras[3].fovy = 90;
    mainCamera.cameras[4].fovy = 90;
    mainCamera.cameras[5].fovy = 90;
    mainCamera.cameras[0].type = CAMERA_PERSPECTIVE;
    mainCamera.cameras[1].type = CAMERA_PERSPECTIVE;
    mainCamera.cameras[2].type = CAMERA_PERSPECTIVE;
    mainCamera.cameras[3].type = CAMERA_PERSPECTIVE;
    mainCamera.cameras[4].type = CAMERA_PERSPECTIVE;
    mainCamera.cameras[5].type = CAMERA_PERSPECTIVE;
    auto rot = QuaternionToMatrix(mainCameraTransform.rotation); //should be in update but nothing can change the transform so it's fine
    auto trans = MatrixTranslate(mainCameraTransform.translation.x, mainCameraTransform.translation.y, mainCameraTransform.translation.z);
    auto scale = MatrixScale(mainCameraTransform.scale.x, mainCameraTransform.scale.y, mainCameraTransform.scale.z);

    Matrix trsMatrix = MatrixMultiply(MatrixMultiply(scale, rot), trans);
    const Vector3 cardinals[] = { {1,0,0}, {0,1,0}, {0,0,1}, {-1,0,0}, {0,-1,0}, {0,0,-1} }; //forward, up, right, back, own, left
    for (int i = 0; i < 6; i++) {
        mainCamera.cameras[i].position = Vector3Transform({ 0,0,0 }, trsMatrix);
        mainCamera.cameras[i].target = Vector3Transform(cardinals[i], trsMatrix);
        mainCamera.cameras[i].up = Vector3Transform(cardinals[(i + 1) % 6], trsMatrix);
    }

    //ecs_set_ref(gameState, gamePiece1, Model, mdl);
    Model kube = LoadModelFromMesh(GenMeshCube(1, 1, 1));
    while (!WindowShouldClose()) {
        ClearBackground(GRAY);
        //ecs_progress(gameState,GetFrameTime());
        //update
       
        //draw (calls manual draw systems to maintain clearcut order of the render pipeline)
        BeginDrawing();
        for (int i = 0; i < 6; i++) {
            BeginTextureMode(mainCamera.textures[i]); //TODO put real render texture
            BeginMode3D(mainCamera.cameras[i]); //TODO put real camera here
            
            //ecs_run(gameState, FrenderModels, GetFrameTime(), nullptr);
            DrawModelWires(kube, { 1,0,0 },  1, RED);
            DrawModelWires(kube, { 0,1,0 },  1, GREEN);
            DrawModelWires(kube, { 0,0,1 },  1, BLUE);
            DrawModelWires(kube, { -1,0,0 }, 1, RED);
            DrawModelWires(kube, { 0,-1,0 }, 1, GREEN);
            DrawModelWires(kube, { 0,0,-1 }, 1, BLUE);
            
            //DrawCircle(50, 50, 50, RED);
            EndMode3D();
            EndTextureMode();
            
            
            DrawTexture(mainCamera.textures[i].texture,100*(i%3), 100*(i/3),WHITE);
        }
        
        //DrawModel(mdl, { 0,0,0 }, 1, WHITE);
        
#ifdef EDITOR 
        //draw GUI
#endif // EDITOR 
        EndDrawing();
    }
}