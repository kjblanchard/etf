namespace Supergoon {
// Initialize EscapeTheFateSystems
void InitializeSystems();
// Start EscapeTheFateSystems (happens on each level load)
void StartSystems();
// Update EscapeTheFateSystems each tick
void UpdateSystems();
// Draw EscapeTheFateSystems each draw
void DrawSystems();
// Draw EscapeTheFateSystems debug widgets if imgui is enabled for debugging
void DrawSystemsDebugWidgets();
} // namespace Supergoon
