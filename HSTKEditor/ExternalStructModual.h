#pragma once
#pragma region ComponenetDeclares

#define COMPONENTS COMPONENT(Transform)\
COMPONENT(Model);\
COMPONENT(BoundingBox);\

#define COMPONENT(id) ecs_entity_t ecs_entity(id);
COMPONENTS;
#define COMPONENT(id) ecs_type_t ecs_type(id);
COMPONENTS;
#pragma endregion

#define COMPONENT(id) ecs_entity(id) = ecs_new_component(gameState, #id, sizeof(id));\
    ecs_type(id) = ecs_type_from_entity(gameState, ecs_entity(id));\
    (void)ecs_entity(id);\
    (void)ecs_type(id);\

void RegisterExternalStructModualComponents() {
    COMPONENTS;
}

#pragma once