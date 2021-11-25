#include "Includes.h"
#include "Codes.h"
extern Renderer renderer;
extern std::string CodeList[];

int getKeyCode(int _val){
	if (_val == 0){
		return 0x30;
	}
	else if (_val == 1){
		return 0x31;
	}
	else if (_val == 2){
		return 0x32;
	}
	else if (_val == 3){
		return 0x33;
	}
	else if (_val == 4){
		return 0x34;
	}
	else if (_val == 5){
		return 0x35;
	}
	else if (_val == 6){
		return 0x36;
	}
	else if (_val == 7){
		return 0x37;
	}
	else if (_val == 8){
		return 0x38;
	}
	else if (_val == 9){
		return 0x39;
	}
	else{
		std::cout << _val << " Invalid Code: FAILED!\n";
		return -1;
	}
}

void typeCode(std::string codeList){
	keybd_event(getKeyCode(std::stoi(codeList)), 0, KEYEVENTF_EXTENDEDKEY, 0);
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	keybd_event(getKeyCode(std::stoi(codeList)), 0, KEYEVENTF_KEYUP, 0);
	std::this_thread::sleep_for(std::chrono::milliseconds(30));
}

void CalcNumbers()
{
	renderer.currentCode = 10000 / renderer.teamSize * renderer.yourSection - 10000 / renderer.teamSize;
	renderer.remainder = 10000 % renderer.teamSize;

	renderer.start = 10000 / renderer.teamSize * renderer.yourSection - 10000 / renderer.teamSize;
	renderer.end = 10000 / renderer.teamSize * renderer.yourSection;
	renderer.lenght = 10000 / renderer.teamSize;
	if (renderer.yourSection == renderer.teamSize)
	{
		//dump all the shit on the last section because im lazy and dont feel like doing the better alternative
		renderer.end += renderer.remainder;
		renderer.lenght += renderer.remainder;
	}
}

int Program(){
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_LOWEST);
	while (true)
	{
		if (renderer.debugToConsole){
			AllocConsole();
			FILE* f;
			freopen_s(&f, "CONOUT$", "w", stdout);
		}
		else{
			FreeConsole();
		}

		if (renderer.yourSection > renderer.teamSize){
			renderer.yourSection = renderer.teamSize;
		}
		if (renderer.currentCode > 9999){
			renderer.currentCode = 0;
		}
		if (renderer.currentCode < 0){
			renderer.currentCode = 9999;
		}
		if (GetAsyncKeyState(VK_END) & 1){

			if (renderer.debugToConsole){
				std::cout << renderer.currentCode << ".| " << CodeList[renderer.currentCode] << " |" << "\n";
			}
			for (int x = 0; x < 4; x++){
				typeCode(CodeList[renderer.currentCode].substr(x, 1));
			}
			if (renderer.currentCode == renderer.end - 1)
			{
				MessageBox(NULL, "You Are Done With Your Section!", "Warning", MB_OK);
			}
			else
			{
				renderer.currentCode++;
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	return 0;
}