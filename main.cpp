#define _WIN32_WINNT 0x0500

#include <windows.h>
#include <iostream>

//https://docs.microsoft.com/en-us/windows/desktop/inputdev/virtual-key-codes
#define VK_T 0x54
#define VK_Y 0x59
#define VK_G 0x47
#define VK_H 0x48
#define VK_B 0x42
#define VK_N 0x4E

HHOOK kHook;
HHOOK mHook;

INPUT getInput();
bool isWarcraftWindow(HWND handle);
LRESULT CALLBACK keyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK mouseProc(int nCode, WPARAM wParam, LPARAM lParam);

int main() {

    std::cout << "Hooking keyboard... ";
    kHook = SetWindowsHookEx(WH_KEYBOARD_LL, keyboardProc, 0, 0);
    std::cout << kHook << std::endl;
    std::cout << "Hooking mouse... ";
    mHook = SetWindowsHookEx(WH_MOUSE_LL, mouseProc, 0, 0);
    std::cout << mHook << std::endl;

    MSG msg{0};
    while (GetMessage(&msg, NULL, 0, 0) != 0);
    UnhookWindowsHookEx(kHook);
    UnhookWindowsHookEx(mHook);

    return 0;
}

LRESULT CALLBACK keyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {

    HWND focusWindow = GetForegroundWindow();
    PKBDLLHOOKSTRUCT key = (PKBDLLHOOKSTRUCT)lParam;

    if (isWarcraftWindow(focusWindow) && wParam == WM_KEYDOWN && nCode == HC_ACTION) {

        INPUT in = getInput();

        switch (key->vkCode) {
            case VK_T:
                in.ki.wVk = VK_NUMPAD7;
                SendInput(1, &in, sizeof(INPUT));
                return 1;
            case VK_Y:
                in.ki.wVk = VK_NUMPAD8;
                SendInput(1, &in, sizeof(INPUT));
                return 1;
            case VK_G:
                in.ki.wVk = VK_NUMPAD4;
                SendInput(1, &in, sizeof(INPUT));
                return 1;
            case VK_H:
                in.ki.wVk = VK_NUMPAD5;
                SendInput(1, &in, sizeof(INPUT));
                return 1;
            case VK_B:
                in.ki.wVk = VK_NUMPAD2;
                SendInput(1, &in, sizeof(INPUT));
                return 1;
            case VK_N:
                in.ki.wVk = VK_NUMPAD3;
                SendInput(1, &in, sizeof(INPUT));
                return 1;
        }
    }
    return CallNextHookEx(kHook, nCode, wParam, lParam);
}

LRESULT CALLBACK mouseProc(int nCode, WPARAM wParam, LPARAM lParam) {

    HWND focusWindow = GetForegroundWindow();
    MSLLHOOKSTRUCT *mouse = (MSLLHOOKSTRUCT *)lParam;

    if (isWarcraftWindow(focusWindow) && wParam == WM_XBUTTONDOWN && nCode == HC_ACTION) {

        INPUT in = getInput();

        switch (HIWORD(mouse->mouseData)) {
            case XBUTTON1:
                in.ki.wVk = VK_NUMPAD5;
                SendInput(1, &in, sizeof(INPUT));
                return 1;
            case XBUTTON2:
                in.ki.wVk = VK_NUMPAD7;
                SendInput(1, &in, sizeof(INPUT));
                return 1;
        }
    }
    return CallNextHookEx(mHook, nCode, wParam, lParam);
}

INPUT getInput() {
    INPUT in;
    in.type = INPUT_KEYBOARD;
    in.ki.wScan = 0;
    in.ki.time = 0;
    in.ki.dwExtraInfo = 0;
    in.ki.dwFlags = 0;
    return in;
}

bool isWarcraftWindow(HWND handle) {
    char title[256];
    GetWindowText(handle, title, sizeof(title));
    return strcmp(title, "Warcraft III") == 0;
}
