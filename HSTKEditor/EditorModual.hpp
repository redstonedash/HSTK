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
enum Cardinals{top, right, forward};
struct Ortho {
	Cardinals dir; //what direction is this ortho windo?
};
void registerComponents() {
	flecs::component<Input>(gameState, "Input");
	flecs::component<Selected>(gameState, "Selected");
	flecs::component<Window>(gameState, "Window");
}
void registerSystems() {
	auto DrawWindows = flecs::system<Window, Ortho>(gameState, "DrawWindows").kind(flecs::Manual).action(
		[](flecs::rows& rows,
			flecs::column<Window> win,
			flecs::column<Ortho> ortho) {
		for (auto row : rows) {
			//make camera
			//call draw models
			//call draw AABB
			//call drawGizmos
		}
	});
	
	auto DrawOrthoWindows = flecs::system<Window>(gameState, "DrawWindows").kind(flecs::Manual).action(
		[](flecs::rows& rows,
			flecs::column<Window> win) {
		for (auto row : rows) {
			DrawTextureRec(win[row].windowTexture.texture, { 0,0,win[row].width, -win[row].height }, { win[row].height, win[row].width }, WHITE);
		}
	});

	auto DrawAABBs = flecs::system<BoundingBox>(gameState, "DrawAABBs").kind(flecs::Manual).action(
		[](flecs::rows& rows,
			flecs::column<BoundingBox> aabb) {
		for (auto row : rows) {
			
		}
	});
}
void initEditor() {

}