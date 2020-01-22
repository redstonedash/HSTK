void RegisterExternalStructModualComponents() {
	flecs::component<Transform>(gameState, "Transform");
	flecs::component<Model>(gameState, "Model");
	flecs::component<BoundingBox>(gameState, "BoundingBox");
}