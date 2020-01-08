#include "DistLine3Triangle3.h"
#pragma region ComponenetDeclares
struct inputManager {
    Vector3 localWishDir;
    Matrix lookDelta;
    bool primaryFire;
    bool secondaryFire;
    bool jump;
    bool crouch;
    char playerNumber;
};
struct linearVelocity {
    float x;
    float y;
    float z;
};
struct line {
    Vector3 begin;
    Vector3 end;
};
struct CapsulCollider {
    gte::Segment<3,float> l; //cancerous templates
    float r;
};
struct meshCollider {
    size_t TriangleCount;
    gte::Triangle3<float>* Triangles;
};
struct spawnPoint {
    Vector3 point;
};
enum maxSpeedCapType{redirect = 0, source, proper};
enum keyState{noInput, release, hold, tap};
struct playerController {
    bool disableBhop; //when bhop is disabled the player recives friction before handeling wheather you jump or not
    bool autoHop; //holding jump will cause you to jump the frame you touch the ground
    bool maxSpeedCapType; //source: allows max speed bug\
                            redirect: consideres your current speed your max speed\
                            proper: can not break max speed, while going above max speed you must slow down to change your direction
    //bool punishAutoHop; //causes friction to be applied as if disableBhop = true but only if you did not press jump that frame
    bool grounded;
    float maxVelocity;
    float accel;
    float maxAirVelocity;
    float airAccel;
};
#define COMPONENTS \
COMPONENT(meshCollider);\
COMPONENT(ModelAnimation);\
COMPONENT(inputManager);\
COMPONENT(linearVelocity);\
COMPONENT(spawnPoint);\
COMPONENT(Camera);

#define COMPONENT(id) ecs_entity_t ecs_entity(id);
COMPONENTS;
#define COMPONENT(id) ecs_type_t ecs_type(id);
COMPONENTS;
#pragma endregion

#define SYSTEMS \
SYSTEM(UpdateInput, EcsOnUpdate, inputManager)\
SYSTEM(ClipVelocity, EcsOnUpdate, Transform, playerController, linearVelocity, CapsulCollider)\
SYSTEM(PlayerUpdate, EcsOnUpdate, inputManager, playerController, Transform, linearVelocity)\
SYSTEM(RespawnPlayer, EcsManual, playerController, Transform || linearVelocity)\

#pragma region SystemDeclares

#define SYSTEM(id, kind, ...) ecs_entity_t F##id;\

SYSTEMS

#define SYSTEM(id, kind, ...) ecs_type_t ecs_type(id);

SYSTEMS

#pragma endregion

keyState GetButtonState(KeyboardKey key) {
    if (IsKeyDown(key)) {
        if (IsKeyPressed(key)) {
            return tap;
        } else {
            return hold;
        }
    }
    else {
        if (IsKeyReleased(key)) {
            return release;
        }
        else {
            return noInput;
        }
    }
}

keyState GetButtonState(MouseButton key) {
    if (IsMouseButtonDown(key)) {
        if (IsMouseButtonPressed(key)) {
            return tap;
        }
        else {
            return hold;
        }
    }
    else {
        if (IsMouseButtonReleased(key)) {
            return release;
        }
        else {
            return noInput;
        }
    }
}
void UpdateInput(ecs_rows_t* row) {
    ECS_COLUMN(row, inputManager, input, 1);
    input->jump = GetButtonState(KEY_SPACE);
    //input->lookDelta 
    input->primaryFire = GetButtonState(MOUSE_LEFT_BUTTON);
    input->primaryFire = GetButtonState(MOUSE_RIGHT_BUTTON);
    input->jump = GetButtonState(KEY_LEFT_CONTROL);
    input->localWishDir = { (float)IsKeyDown(KEY_W) - (float)IsKeyDown(KEY_S), (float)IsKeyDown(KEY_A) - (float)IsKeyDown(KEY_D) };
}

void ClipVelocity(ecs_rows_t* row) {
    
}

void PlayerUpdate(ecs_rows_t* row) {
    ECS_COLUMN(row, inputManager, im, 1);
    ECS_COLUMN(row, playerController, pc, 2);
    ECS_COLUMN(row, Transform, trs, 3);
    ECS_COLUMN(row, linearVelocity, velocity, 4);
}

void RespawnPlayer(ecs_rows_t* row) {

}

#pragma region RegisterSystemsAndComponenets
#define SYSTEM(id, kind, ...) F##id = ecs_new_system(gameState, #id, kind, #__VA_ARGS__, id);\
    ecs_type(id) = ecs_type_from_entity(gameState, F##id);\

#define COMPONENT(id) ecs_entity(id) = ecs_new_component(gameState, #id, sizeof(id));\
    ecs_type(id) = ecs_type_from_entity(gameState, ecs_entity(id));\
    (void)ecs_entity(id);\
    (void)ecs_type(id);\

void RegisterPlayerController() {
    COMPONENTS;
    SYSTEMS
}
#pragma endregion

#pragma once