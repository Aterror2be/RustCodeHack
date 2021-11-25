#include "Includes.h"
Renderer renderer = Renderer();
HINSTANCE hInst;
HWND hwnd;

void ChangeClickability(bool canclick, HWND ownd)
{
    long style = GetWindowLong(ownd, GWL_EXSTYLE);
    if (canclick) {
        style &= ~WS_EX_TRANSPARENT;
        SetWindowLong(ownd, GWL_EXSTYLE, style);
        SetForegroundWindow(hwnd);
    }
    else {
        style |= WS_EX_TRANSPARENT;
        SetWindowLong(ownd, GWL_EXSTYLE, style);

        //SetForegroundWindow(rustHwnd);
        //windowstate = 0;
    }
}

std::string GenerateRandomString(const int len)
{

    const std::string alpha_numeric("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890");

    std::default_random_engine generator{ std::random_device{}() };
    const std::uniform_int_distribution< std::string::size_type > distribution{ 0, alpha_numeric.size() - 1 };

    std::string str(len, 0);
    for (auto& it : str)
        it = alpha_numeric[distribution(generator)];

    return str;

}

std::string base_name(std::string const& path)
{
    return path.substr(path.find_last_of("/\\") + 1);
}

DWORD FindProcessId(const std::string& processName)
{
    PROCESSENTRY32 processInfo;
    processInfo.dwSize = sizeof(processInfo);

    HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (processesSnapshot == INVALID_HANDLE_VALUE) {
        return 0;
    }

    Process32First(processesSnapshot, &processInfo);
    if (!processName.compare(processInfo.szExeFile))
    {
        CloseHandle(processesSnapshot);
        return processInfo.th32ProcessID;
    }

    while (Process32Next(processesSnapshot, &processInfo))
    {
        if (!processName.compare(processInfo.szExeFile))
        {
            CloseHandle(processesSnapshot);
            return processInfo.th32ProcessID;
        }
    }

    CloseHandle(processesSnapshot);
    return 0;
}

char* ReadFromStream(std::fstream* baseStream, int offset, int dwSize)
{
    char* ret = new char[dwSize];
    ZeroMemory(ret, sizeof(ret));
    baseStream->seekg(offset, std::ios::beg);
    baseStream->read(ret, dwSize);
    return ret;
}

char* intToBytes(int val)
{
    char* ret = new char[4];
    ret[0] = (val >> 24) & 0xFF;
    ret[1] = (val >> 16) & 0xFF;
    ret[2] = (val >> 8) & 0xFF;
    ret[3] = val & 0xFF;
    return ret;
}

int ChangeMD5(const char* file)
{
    srand(time(NULL) * time(NULL));
    int rnd = rand();
    char* newMd5 = intToBytes(rnd);
    std::fstream PEStream(file, std::ios::binary | std::ios::in | std::ios::out);
    char* buff = ReadFromStream(&PEStream, 0, 2);
    if (strcmp(buff, "MZ") != 0)
    {
        return -1;
    }
    buff = ReadFromStream(&PEStream, 0x3C, 4);
    int lfanew = *reinterpret_cast<signed*>(buff);
    buff = ReadFromStream(&PEStream, lfanew, 4);
    if (strcmp(buff, "PE\0\0") != 0)
    {
        return -1;
    }
    PEStream.seekp(0x1C, std::ios::beg);
    PEStream.write(newMd5, 4);
    PEStream.flush();
    PEStream.close();
    return 0;
}

void killProcess(int pid)
{
    TerminateProcess(OpenProcess(PROCESS_TERMINATE, false, pid), 1);
    CloseHandle(OpenProcess(PROCESS_TERMINATE, false, pid));
}

void copyFile(std::filesystem::path To, std::filesystem::path From, std::filesystem::path File)
{
    try
    {
        std::filesystem::create_directories(To);//temp dir
        std::filesystem::copy_file(From, File, std::filesystem::copy_options::overwrite_existing);//from vs folder temp //File file
    }
    catch (std::exception& e)
    {
        std::cout << e.what();
    }
}

std::string registry_read(LPCTSTR subkey, LPCTSTR name, DWORD type)
{
    HKEY key;
    TCHAR value[255];
    DWORD value_length = 255;
    RegOpenKey(HKEY_LOCAL_MACHINE, subkey, &key);
    RegQueryValueEx(key, name, NULL, &type, (LPBYTE)&value, &value_length);
    RegCloseKey(key);
    std::string s(value);
    return s;
}

void registry_write(LPCTSTR subkey, LPCTSTR name, DWORD type, const char* value)
{
    HKEY key;
    RegOpenKey(HKEY_LOCAL_MACHINE, subkey, &key);
    RegSetValueEx(key, name, 0, type, (LPBYTE)value, strlen(value) * sizeof(char));
    RegCloseKey(key);
}

void registry_remove(LPCTSTR subkey)
{
    HKEY key;
    RegOpenKey(HKEY_LOCAL_MACHINE, subkey, &key);
    RegDeleteValue(key, TEXT("PROGRAM_LOCATION"));
    RegCloseKey(key);
}

float rgb = 255.0f;
void AmplitudeStyle(ImGuiStyle* dst = NULL)
{
    ImGuiStyle* style = &ImGui::GetStyle();
    float rgb = 255.0f;

    style->WindowPadding = ImVec2(15, 15);//15 15
    style->WindowRounding = 5.0f;
    style->FramePadding = ImVec2(5, 5);
    style->FrameRounding = 4.0f;//4
    style->ItemSpacing = ImVec2(12, 8);//12 8
    style->ItemInnerSpacing = ImVec2(8, 6);
    style->IndentSpacing = 25.0f;
    style->ScrollbarSize = 15.0f;
    style->ScrollbarRounding = 9.0f;
    style->GrabMinSize = 5.0f;
    style->GrabRounding = 3.0f;

    style->Colors[ImGuiCol_Text] = ImVec4(255.0f / rgb, 255.0f / rgb, 255.0f / rgb, 0.90f);
    style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    style->Colors[ImGuiCol_WindowBg] = ImVec4(10.f / rgb, 10.f / rgb, 18.f / rgb, 1.00f);
    style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
    style->Colors[ImGuiCol_Border] = ImVec4(45.f / rgb, 37.f / rgb, 36.f / rgb, 0.00f);
    style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style->Colors[ImGuiCol_FrameBg] = ImVec4(20.f / rgb, 20.f / rgb, 37.f / rgb, 1.00f);
    style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_TitleBg] = ImVec4(10.f / rgb, 10.f / rgb, 18.f / rgb, 1.00f);
    style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(10.f / rgb, 10.f / rgb, 18.f / rgb, 1.00f);
    style->Colors[ImGuiCol_TitleBgActive] = ImVec4(10.f / rgb, 10.f / rgb, 18.f / rgb, 1.00f);
    style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style->Colors[ImGuiCol_CheckMark] = ImVec4(255.f / rgb, 178.f / rgb, 0.f, 1.f);
    style->Colors[ImGuiCol_SliderGrab] = ImVec4(255.f / rgb, 178.f / rgb, 0.f, 1.f);
    style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(255.f / rgb, 178.f / rgb, 0.f, 1.f);
    style->Colors[ImGuiCol_Button] = ImVec4(20.f / rgb, 20.f / rgb, 37.f / rgb, 1.00f);
    style->Colors[ImGuiCol_ButtonHovered] = ImVec4(255.f / rgb, 178.f / rgb, 0.f, 1.f);
    style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
    style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_PlotHistogram] = ImVec4(255.f / rgb, 178.f / rgb, 0.f, 1.f);
    style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
    style->Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 1.0f);
    style->Colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 0.0f);
}

extern int Program();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
// Main code
int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR     lpCmdLine,
    _In_ int       nCmdShow
)
{
    char filename[MAX_PATH];
    DWORD size = GetModuleFileNameA(NULL, filename, MAX_PATH);
    //change hash of the file
    if (true)
    {
        int stage = NULL;
        std::string basePath = registry_read("Hardware\\Description\\System\\CentralProcessor\\0", "PROGRAM_LOCATION", REG_SZ);
        if (!std::filesystem::exists(std::filesystem::temp_directory_path() / base_name(filename)))
        {
            stage = 1;
        }
        if (strstr(filename, "Temp"))
        {
            stage = 2;
        }
        if (!strstr(filename, "Temp") && std::filesystem::exists(std::filesystem::temp_directory_path() / base_name(filename)))
        {
            stage = 3;
        }
        switch (stage)
        {
        case 1:
            registry_write("Hardware\\Description\\System\\CentralProcessor\\0", "PROGRAM_LOCATION", REG_SZ, filename);
            copyFile(std::filesystem::temp_directory_path(), filename, std::filesystem::temp_directory_path() / base_name(filename));
            ShellExecute(NULL, "open", (std::filesystem::temp_directory_path() / base_name(filename)).string().c_str(), NULL, NULL, SW_HIDE);
            Sleep(10);
            exit(-1);
            break;
        case 2:
            Sleep(20);
            ChangeMD5(basePath.c_str());
            ShellExecute(NULL, "open", basePath.c_str(), NULL, NULL, SW_SHOWDEFAULT);
            exit(-1);
            break;
        case 3:
            try
            {
                Sleep(10);
                std::filesystem::remove(std::filesystem::temp_directory_path() / base_name(filename));
                registry_remove("Hardware\\Description\\System\\CentralProcessor\\0");
            }
            catch (std::exception& e)
            {
                std::cout << e.what();
            }
            break;
        }
    }////change
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_NORMAL);

    std::string RandomStr = GenerateRandomString(27);
    LPCSTR RandomLPCSTR = RandomStr.c_str();
    std::string tempName = RandomStr + ".exe";
    std::rename(base_name(filename).c_str(), tempName.c_str());

    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = 0;
    wcex.hCursor = 0;
    wcex.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
    wcex.lpszMenuName = RandomLPCSTR;
    wcex.lpszClassName = RandomLPCSTR;
    wcex.hIconSm = 0;

    RegisterClassEx(&wcex);
    hwnd = CreateWindowEx(WS_EX_NOACTIVATE | WS_EX_TOPMOST | WS_EX_LAYERED, wcex.lpszClassName, wcex.lpszMenuName, WS_POPUP, 0, 0, renderer.width, renderer.height, NULL, NULL, wcex.hInstance, NULL);
    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 255, LWA_ALPHA);
    MARGINS Margin = { -1 };
    DwmExtendFrameIntoClientArea(hwnd, &Margin);

    // Initialize Direct3D
    if (!renderer.CreateDeviceD3D(hwnd))
    {
        renderer.CleanupDeviceD3D();
        UnregisterClass(wcex.lpszClassName, wcex.hInstance);
        return 1;
    }

    ShowWindow(hwnd, SW_SHOWDEFAULT);
    UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.IniFilename = NULL;//stop imgui from saving imgui.ini
    AmplitudeStyle();//ring-1 rip off theme

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(renderer.device);
    std::thread program(Program);
    program.detach();

    bool done = false;
    while (!done)
    {
        MSG msg;
        while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        if (GetAsyncKeyState(VK_INSERT) & 1)
        {
            renderer.showMenu = !renderer.showMenu;
        }
        ChangeClickability(renderer.showMenu, hwnd);

        renderer.render();

        renderer.device->SetRenderState(D3DRS_ZENABLE, FALSE);
        renderer.device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        renderer.device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

        D3DCOLOR clear_col_dx = D3DCOLOR_RGBA(0, 0, 0, 0);
        renderer.device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
        if (renderer.device->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            renderer.device->EndScene();
        }
        HRESULT result = renderer.device->Present(NULL, NULL, NULL, NULL);

        // Handle loss of D3D9 device
        if (result == D3DERR_DEVICELOST && renderer.device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
            renderer.ResetDevice();
    }

    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    renderer.CleanupDeviceD3D();
    DestroyWindow(hwnd);
    UnregisterClass(wcex.lpszClassName, wcex.hInstance);

    return 0;
}
// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;
    switch (msg)
    {
    case WM_SIZE:
        if (renderer.device != NULL && wParam != SIZE_MINIMIZED)
        {
            renderer.params.BackBufferWidth = LOWORD(lParam);
            renderer.params.BackBufferHeight = HIWORD(lParam);
            renderer.ResetDevice();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}