#pragma once
#include <cmath>
#include "raylib.h"
#include "rlgl.h"
#include "flecs.h"
#include "flecs/flecs.hpp"
#include "raymath.h"
template<typename T> T* addrOf(T&& v) { return &v; }
#undef ecs_set

#define ecs_set(world, entity, component, ...)\
    _ecs_set_ptr(world, entity, ecs_entity(component), sizeof(component), addrOf(component __VA_ARGS__))

#define ecs_set_ref(world, entity, component, ...)\
    _ecs_set_ptr(world, entity, ecs_entity(component), sizeof(component), &__VA_ARGS__)

_declspec(selectany) flecs::world gameState;

Vector2 GetWorldToTexture(Vector3 position, Camera camera, int screenWidth, int screenHeight)
{
	// Calculate projection matrix (from perspective instead of frustum
	Matrix matProj = MatrixIdentity();

	if (camera.type == CAMERA_PERSPECTIVE)
	{
		// Calculate projection matrix from perspective
		matProj = MatrixPerspective(camera.fovy * DEG2RAD, ((double)screenWidth / (double)screenHeight), 0.01, 1000.0);
	}
	else if (camera.type == CAMERA_ORTHOGRAPHIC)
	{
		float aspect = (float)screenWidth / (float)screenHeight;
		double top = camera.fovy / 2.0;
		double right = top * aspect;

		// Calculate projection matrix from orthographic
		matProj = MatrixOrtho(-right, right, -top, top, 0.01, 1000.0);
	}

	// Calculate view matrix from camera look at (and transpose it)
	Matrix matView = MatrixLookAt(camera.position, camera.target, camera.up);

	// Convert world position vector to quaternion
	Quaternion worldPos = { position.x, position.y, position.z, 1.0f };

	// Transform world position to view
	worldPos = QuaternionTransform(worldPos, matView);

	// Transform result to projection (clip space position)
	worldPos = QuaternionTransform(worldPos, matProj);

	// Calculate normalized device coordinates (inverted y)
	Vector3 ndcPos = { worldPos.x / worldPos.w, -worldPos.y / worldPos.w, worldPos.z / worldPos.w };

	// Calculate 2d screen position vector
	Vector2 screenPosition = { (ndcPos.x + 1.0f) / 2.0f * (float)screenWidth, (ndcPos.y + 1.0f) / 2.0f * (float)screenHeight };

	return screenPosition;
}