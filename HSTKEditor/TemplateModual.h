#pragma once
#pragma region ComponenetDeclares
#define COMPONENTS\

#define COMPONENT(id) ecs_entity_t ecs_entity(id);
COMPONENTS;
#define COMPONENT(id) ecs_type_t ecs_type(id);
COMPONENTS;
#pragma endregion
#define SYSTEMS \

#pragma region SystemDeclares

#define SYSTEM(id, kind, ...) ecs_entity_t F##id;\

SYSTEMS

#define SYSTEM(id, kind, ...) ecs_type_t ecs_type(id);

SYSTEMS

#pragma endregion

#pragma region RegisterSystemsAndComponenets
#define SYSTEM(id, kind, ...) F##id = ecs_new_system(gameState, #id, kind, #__VA_ARGS__, id);\
    ecs_type(id) = ecs_type_from_entity(gameState, F##id);\

#define COMPONENT(id) ecs_entity(id) = ecs_new_component(gameState, #id, sizeof(id));\
    ecs_type(id) = ecs_type_from_entity(gameState, ecs_entity(id));\
    (void)ecs_entity(id);\
    (void)ecs_type(id);\

void RegisterModualComponents() {
    COMPONENTS;
}
void RegisterModualSystems() {
    SYSTEMS
}
#pragma endregion

#pragma once