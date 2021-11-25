#pragma once
/*Menu Shit*/
#include <d3d9.h>
#include <d3dx9.h>
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma warning(disable : 4244) // disable loss of data conversion
#pragma warning(disable : 26495) // disable loss of data conversion

/*Main Includes*/
#include <random>
#include <dwmapi.h>
#include <fstream>
#include <filesystem>
#include <string>
#include <iostream>
#include <thread>
#include <windows.h>
#include <TlHelp32.h>
#include "Renderer.h"