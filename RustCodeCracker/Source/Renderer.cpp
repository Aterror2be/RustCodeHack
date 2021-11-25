#include "Includes.h"
extern Renderer renderer;
extern std::string CodeList[];
extern void CalcNumbers();
std::string overlayDebug()
{
	const std::string& prefix = "Application average ", delay = "ms/frame", fpsStr = "(Fps:", suffix = ")";
	return prefix + std::to_string(1000 / (int)ImGui::GetIO().Framerate) + delay + fpsStr + std::to_string((int)ImGui::GetIO().Framerate) + suffix;
}

void Renderer::menu()
{

}
void Renderer::render()
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	auto draw = ImGui::GetBackgroundDrawList();

	draw->AddText(ImVec2(0 + 1, renderer.height - 15 + 1), IM_COL32(1, 1, 1, 255), overlayDebug().c_str());
	draw->AddText(ImVec2(0, renderer.height - 15), IM_COL32(0, 255, 0, 255), overlayDebug().c_str());

	if (renderer.showMenu)
	{
		ImGui::SetNextWindowSize(ImVec2(600, 400), NULL);
		ImGui::Begin("Code Cracker -Aterror2be#3778", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		ImGui::SliderInt("Team Size", &renderer.teamSize, 1, 50);
		ImGui::SliderInt("Your Section", &renderer.yourSection, 1, renderer.teamSize);
		ImGui::Text("Section Start: %i", renderer.start);
		ImGui::Text("Section End: %i", renderer.end);
		ImGui::Text("Section Lenght: %i", renderer.lenght);
		ImGui::Separator();
		ImGui::Checkbox("Show OverlayWindow", &renderer.showOverlayWindow);
		ImGui::Checkbox("Debug To Console", &renderer.debugToConsole);
		ImGui::Separator();
		if (ImGui::Button("Calculate Codes", ImVec2(150, 50))){
			std::thread program(CalcNumbers);
			program.join();
		}
		ImGui::SameLine();
		ImGui::TextWrapped("Note: Pressing this button while guessing codes will set it back to the start of your section!");
		ImGui::SliderInt("Current Code", &renderer.currentCode, 0, 9999);
		ImGui::Dummy(ImVec2(0, 25));
		if (ImGui::Button("Exit", ImVec2(100, 30)))
		{
			exit(-1);
		}
		ImGui::End();
	}
	if (renderer.showOverlayWindow){
		ImGui::SetNextWindowSize(ImVec2(220, 400), NULL);
		ImGui::Begin("Code Cracker Overlay", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
		ImGui::Text("Progress");
		ImGui::ProgressBar((float)(renderer.currentCode - ((10000 / renderer.teamSize) * renderer.yourSection - 10000 / renderer.teamSize)) / (10000 / renderer.teamSize), ImVec2(150, 20));
		ImGui::Text("Last 5 Codes");
		ImGui::Separator();
		ImGui::Text("Code Type |  Pin  | Number");

		if (renderer.currentCode){
			ImGui::Text("Current:  |"); ImGui::SameLine(); ImGui::Text(CodeList[renderer.currentCode].c_str()); ImGui::SameLine(); ImGui::Text("| [%i]",renderer.currentCode);
		}
		if (renderer.currentCode > 1){
			ImGui::Text("Last  1:  |"); ImGui::SameLine(); ImGui::Text(CodeList[renderer.currentCode - 1].c_str()); ImGui::SameLine(); ImGui::Text("| [%i]", renderer.currentCode - 1);
		}
		if (renderer.currentCode > 2){
			ImGui::Text("Last  2:  |"); ImGui::SameLine(); ImGui::Text(CodeList[renderer.currentCode - 2].c_str()); ImGui::SameLine(); ImGui::Text("| [%i]", renderer.currentCode - 2);
		}
		if (renderer.currentCode > 3){
			ImGui::Text("Last  3:  |"); ImGui::SameLine(); ImGui::Text(CodeList[renderer.currentCode - 3].c_str()); ImGui::SameLine(); ImGui::Text("| [%i]", renderer.currentCode - 3);
		}
		if (renderer.currentCode > 4){
			ImGui::Text("Last  4:  |"); ImGui::SameLine(); ImGui::Text(CodeList[renderer.currentCode - 4].c_str()); ImGui::SameLine(); ImGui::Text("| [%i]", renderer.currentCode - 4);
		}
		if (renderer.currentCode > 5){
			ImGui::Text("Last  5:  |"); ImGui::SameLine(); ImGui::Text(CodeList[renderer.currentCode - 5].c_str()); ImGui::SameLine(); ImGui::Text("| [%i]", renderer.currentCode - 5);
		}
		ImGui::End();
	}
	ImGui::EndFrame();
}

bool Renderer::CreateDeviceD3D(HWND hWnd)
{
	if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &object)))
	{
		return false;
	}
	// Create the D3DDevice
	ZeroMemory(&params, sizeof(params));

	params.BackBufferWidth = renderer.width;
	params.BackBufferHeight = renderer.height;
	params.Windowed = true;
	params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	params.MultiSampleQuality = D3DMULTISAMPLE_NONE;
	params.BackBufferFormat = D3DFMT_A8R8G8B8;
	params.EnableAutoDepthStencil = TRUE;
	params.AutoDepthStencilFormat = D3DFMT_D16;
	params.PresentationInterval = D3DPRESENT_INTERVAL_ONE;           // Present with vsync
	//params.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // Present without vsync, maximum unthrottled framerate

	if (object->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &params, 0, &device) < 0)
		return false;

	return true;
}

void Renderer::CleanupDeviceD3D()
{
	if (device) { device->Release(); device = NULL; }
	if (object) { object->Release(); object = NULL; }
}

void Renderer::ResetDevice()
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
	HRESULT hr = device->Reset(&params);
	if (hr == D3DERR_INVALIDCALL)
		IM_ASSERT(0);
	ImGui_ImplDX9_CreateDeviceObjects();
}

Renderer::Renderer()
{
}

Renderer::Renderer(HWND hWnd, HWND targetHWND, int width, int height)
{
	this->width = width;
	this->height = height;
}

