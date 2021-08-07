#include "Listener.h"
#include <Windows.h>
#include <iostream>

HHOOK MouseHook;
HHOOK KeyboardHook;

LRESULT CALLBACK MouseHookProcedure(int nCode, WPARAM wParam, LPARAM lParam)
{
    MSLLHOOKSTRUCT* p = (MSLLHOOKSTRUCT*)lParam;
    BOOLEAN injected = p->flags & LLMHF_INJECTED || p->flags & LLMHF_LOWER_IL_INJECTED; // Checks if click was injected and not from a mouse
    if (nCode == HC_ACTION && !injected)
    {
        if (wParam == WM_LBUTTONDOWN) instance->onLMB_Down();
        else if (wParam == WM_LBUTTONUP) instance->onLMB_Up();
        else if (wParam == WM_RBUTTONDOWN) instance->onRMB_Down();
        else if (wParam == WM_RBUTTONUP) instance->onRMB_Up();
    }
    return CallNextHookEx(nullptr, nCode, wParam, lParam);
}

void Listener::setupMouseHook() {
    std::cout << "[*] Starting Mouse Hook" << std::endl;
    MouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseHookProcedure, GetModuleHandle(nullptr), NULL);
    if (!MouseHook) std::cout << "[!] Failed to get handle from SetWindowsHookEx() - M" << std::endl;
    else {
        std::cout << "[*] Mouse Hook Ready" << std::endl;
        //A Now process all the input
        MSG Msg;
        while (GetMessage(&Msg, nullptr, 0, 0) > 0)
        {
            TranslateMessage(&Msg);
            DispatchMessage(&Msg);
        }
        //A
    }
    if (MouseHook) { UnhookWindowsHookEx(MouseHook); }
}

// ########################################################################################################################################################

LRESULT CALLBACK KeyboardHookProcedure(int nCode, WPARAM wParam, LPARAM lParam)
{
    // WH_KEYBOARD_LL uses the LowLevelKeyboardProc Call Back
    // wParam and lParam parameters contain information about the message.
    auto *p = (KBDLLHOOKSTRUCT *)lParam;
    if (nCode == HC_ACTION)
    {
        if (wParam == WM_SYSKEYDOWN || wParam == WM_KEYDOWN)
        {
            if (p->vkCode == instance->scanKey) {
                instance->callback();
            }
        }
    }
    return CallNextHookEx(nullptr, nCode, wParam, lParam);
}

void Listener::setupKeyboardHook() {
    std::cout << "[*] Starting Keyboard Hook" << std::endl;
    KeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardHookProcedure, GetModuleHandle(nullptr), NULL);
    if (!KeyboardHook) std::cout << "[!] Failed to get handle from SetWindowsHookEx() - KB" << std::endl;
    else {
        std::cout << "[*] Keyboard Hook Ready" << std::endl;
        //A Now process all the input
        MSG Msg;
        while (GetMessage(&Msg, nullptr, 0, 0) > 0)
        {
            TranslateMessage(&Msg);
            DispatchMessage(&Msg);
        }
        //A
    }
    if (KeyboardHook) { UnhookWindowsHookEx(KeyboardHook); }
}