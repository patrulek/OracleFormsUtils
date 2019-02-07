#include "hookhandler.h"
#include <thread>

#include "FAPILogger.h"
#include "WinUtils.h"
#include "Utils.h"
#include "modulemanager.h"
#include "autocompleter.h"

using namespace OracleFormsUtils;
using namespace CPPFAPIWrapper;

HookHandler * HookHandler::s_instance = nullptr;

HookHandler * HookHandler::instance() {
    if( !s_instance ) {
        s_instance = new HookHandler();
        connect(s_instance, SIGNAL(mouseAction(int, LPARAM, WPARAM)), s_instance, SLOT(onMouseAction(int, LPARAM, WPARAM)));
        connect(s_instance, SIGNAL(keyboardAction(int, LPARAM, WPARAM)), s_instance, SLOT(onKeyboardAction(int, LPARAM, WPARAM)));
    }

    return s_instance;
}
HookHandler::~HookHandler() {
    releaseHooks();
}

void HookHandler::releaseHooks() {
    UnhookWindowsHookEx(mouse_hook);
    UnhookWindowsHookEx(keyboard_hook);
}

void HookHandler::setHooks() {
    setMouseHook();
    setKeyboardHook();
}

LRESULT CALLBACK HookHandler::mouseHookProc(int _ncode, WPARAM _wparam, LPARAM _lparam) {
    emit s_instance->mouseAction(_ncode, _lparam, _wparam);

    return CallNextHookEx(s_instance->mouse_hook, _ncode, _wparam, _lparam);
}

LRESULT CALLBACK HookHandler::keyboardHookProc(int _ncode, WPARAM _wparam, LPARAM _lparam) {
    emit s_instance->keyboardAction(_ncode, _lparam, _wparam);

    return CallNextHookEx(s_instance->keyboard_hook, _ncode, _wparam, _lparam);
}

void HookHandler::setMouseHook() {
    HINSTANCE hInstance = GetModuleHandle(nullptr);
    mouse_hook = SetWindowsHookEx(WH_MOUSE_LL, mouseHookProc, hInstance, NULL);
}

void HookHandler::setKeyboardHook() {
    HINSTANCE hInstance = GetModuleHandle(nullptr);
    keyboard_hook = SetWindowsHookEx(WH_KEYBOARD_LL, keyboardHookProc, hInstance, NULL);
}


void HookHandler::onMouseAction(int _msg, LPARAM _lparam, WPARAM _wparam) {
    MSLLHOOKSTRUCT * pMouseStruct = reinterpret_cast<MSLLHOOKSTRUCT *>(_lparam);

    // mouse_data < 0: scrollujemy w dół; > 0 scrollujemy w górę
    short mouse_data = static_cast<short>(HIWORD(pMouseStruct->mouseData));
    int sc_event = (static_cast<int>(_wparam) == static_cast<int>(WM_MOUSEWHEEL)) * ((mouse_data < 0) - (mouse_data > 0));

    if (sc_event == 0)
        return;

    HWND window = WindowFromPoint(pMouseStruct->pt);
    std::string str_title = getWindowTitle(window);

    if (isScrollableWindow(str_title)) {
        EnumChildWindows(window, enumProc, static_cast<LPARAM>(sc_event));
        return;
    }

    window = GetParent(window);

    while (window) {
        str_title = getWindowTitle(window);

        if ( isScrollableWindow(str_title) ) {
            EnumChildWindows(window, enumProc, static_cast<LPARAM>(sc_event));
            break;
        }

        window = GetParent(window);
    }
}

void HookHandler::onKeyboardAction(int _msg, LPARAM _lparam, WPARAM _wparam) {
    if( _wparam != WM_KEYDOWN )
        return;

    HWND hwnd = GetForegroundWindow();
    std::string str_title = getWindowTitle(hwnd);

    if (!isOracleFormsWindow(str_title))
        return;

    // lParam is the pointer to the struct containing the data needed, so cast and assign it to kdbStruct.
    KBDLLHOOKSTRUCT * kbd_struct = reinterpret_cast<KBDLLHOOKSTRUCT*>(_lparam);

    menuSearch(kbd_struct, hwnd);
    reloadModule(kbd_struct, hwnd);
    openAutocomplete(kbd_struct, hwnd);
}


void HookHandler::menuSearch(KBDLLHOOKSTRUCT * _kbd_struct, HWND _hwnd) {
    bool searching = isShiftPressed() && isCtrlPressed() && isLetterPressed('F', _kbd_struct);

    if ( searching )
        PostMessage(_hwnd, WM_COMMAND, 179, 0);
}

void HookHandler::reloadModule(KBDLLHOOKSTRUCT * _kbd_struct, HWND _hwnd) {
    bool compiling = isCtrlPressed() && (isLetterPressed('T', _kbd_struct) || (isShiftPressed() && (isLetterPressed('K', _kbd_struct))));
    bool saving = isCtrlPressed() && isLetterPressed('S', _kbd_struct);

    if( !ModuleManager::instance()->isReloading() && (compiling || saving) )
        ModuleManager::instance()->reloadModule();
}

void HookHandler::openAutocomplete(KBDLLHOOKSTRUCT * _kbd_struct, HWND _hwnd) {
    bool completing = isCtrlPressed() && isSpacePressed(_kbd_struct);

    if( completing )
        Autocompleter::instance()->toggle();
}


BOOL CALLBACK HookHandler::enumProc(HWND _hwnd, LPARAM _lparam) {
    std::string class_str = getWindowClass(_hwnd);

    if (class_str.find("ScrollBar") != std::string::npos) {
        RECT r;
        GetWindowRect(_hwnd, &r);
        int sc_event = static_cast<int>(_lparam);

        // uwaga, być może ta liczba pobierana jest z rejestru, W10:HKCU/Control Panel/Desktop/Metrics
        if (r.right - r.left == 17) {
            changeScrollbarPos(_hwnd, sc_event, 96);
            return false;
        }
    }

    return true;
}
