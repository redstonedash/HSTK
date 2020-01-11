#pragma once
struct Input {
	Vector2 mousePositionOnLastClick;
	Vector2 lastMousePosition;
	bool action;
};
struct Selected {
};
struct Window {
	int x;
	int y;
	int width;
	int height;
	RenderTexture windowTexture;
};
void registerComponents() {
	flecs::component<Input>(gameState, "Input");
	flecs::component<Selected>(gameState, "Selected");
	flecs::component<Window>(gameState, "Window");
}
void registerSystems() {
	auto DrawWindows = flecs::system<Window>(gameState).kind(flecs::Manual).action(
		[](flecs::rows& rows) {
		for (auto row : rows) {
			
		}
	});

}
void initEditor() {

}