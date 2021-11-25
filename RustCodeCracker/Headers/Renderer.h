#include "Includes.h"

class Renderer
{
private:

public:
	int width = GetSystemMetrics(SM_CXSCREEN), height = GetSystemMetrics(SM_CYSCREEN);
	IDirect3D9Ex* object = NULL;
	IDirect3DDevice9Ex* device = NULL;
	D3DPRESENT_PARAMETERS params;
	LPDIRECT3DVERTEXBUFFER9 vertexBuffer;
	bool showMenu = true;
	bool showOverlayWindow = false;
	bool debugToConsole = false;
	int currentCode = 0;
	int teamSize = 1;
	int yourSection = 1;
	int remainder = 0;
	int start = 0;
	int end = 10000;
	int lenght = 10000;

	void render();
	void menu();
	bool CreateDeviceD3D(HWND hWnd);
	void CleanupDeviceD3D();
	void ResetDevice();
	Renderer();
	Renderer(HWND hWnd, HWND targetHWND, int width, int height);
};

