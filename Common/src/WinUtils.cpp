#include "WinUtils.h"

using namespace std;

namespace OracleFormsUtils {

	string getWindowTitle(HWND _handle) {
		if (_handle == NULL)
			_handle = GetForegroundWindow();

		constexpr int buffer_size = 256;
		char buffer[buffer_size];
		GetWindowTextA(_handle, buffer, buffer_size);

		return string(buffer);
	}

	string getWindowClass(HWND _handle) {
		if (_handle == NULL)
			_handle = GetForegroundWindow();

		constexpr int buffer_size = 256;
		char buffer[buffer_size];
		GetClassNameA(_handle, buffer, buffer_size);

		return string(buffer);
	}

	SCROLLINFO getScrollInfo(HWND _handle) {
		SCROLLINFO si;
		si.cbSize = sizeof(SCROLLINFO);
		si.fMask = SIF_ALL;
		si.nPage = si.nMin = si.nMax = si.nTrackPos = si.nPos = 0;
		GetScrollInfo(_handle, SB_CTL, &si);

		return si;
	}

	void changeScrollbarPos(HWND _handle, int _sc_event, int _scroll_shift) {
		SCROLLINFO si = getScrollInfo(_handle);

		si.nPos += (_sc_event * _scroll_shift);
		si.nPos = si.nPos * (si.nPos > si.nMin && si.nPos < si.nMax) // nowa poz. tylko jeœli nie wychodzimy poza zakres
			+ (si.nPos >= si.nMax) * si.nMax; // max jeœli wychodzimy z góry ; wyjœcia z do³u nie trzeba bo da nam to 0

		SendMessage(_handle, WM_VSCROLL, (WPARAM)((si.nPos << 16) + SB_THUMBPOSITION), (LPARAM)_handle);
		SetScrollPos(_handle, SB_CTL, si.nPos, TRUE);
	}

	bool isScrollableWindow(string _title) {
		return _title == "Property Palette"
			|| _title == "Object Navigator"
			|| _title == "Paleta W³aœciwoœci"
			|| _title == "Nawigator obiektów";
	}

	bool isShiftPressed() {
		return ((1 << 15) & GetAsyncKeyState(VK_SHIFT));
	}

	bool isCtrlPressed() {
		return ((1 << 15) & GetAsyncKeyState(VK_CONTROL));
	}

	bool isSpacePressed(KBDLLHOOKSTRUCT * _kbd_struct) {
		return _kbd_struct->vkCode == VK_SPACE;
		//return ((1 << 15) & GetAsyncKeyState(VK_SPACE));
	}

	bool isLetterPressed(char _letter, KBDLLHOOKSTRUCT * _kbd_struct) {
		_letter += (('A' - 'a') * (_letter >= 'a' && _letter <= 'z'));

		return _kbd_struct->vkCode == (int)_letter;
	}

	RECT getCaretPos() {
		DWORD tid = GetWindowThreadProcessId(GetForegroundWindow(), 0);
		GUITHREADINFO guiThreadInfoObject;
		guiThreadInfoObject.cbSize = sizeof(GUITHREADINFO);
		GetGUIThreadInfo(tid, &guiThreadInfoObject);
		
		return guiThreadInfoObject.rcCaret;
	}

	HWND getFocusWindow() {
		DWORD tid = GetWindowThreadProcessId(GetForegroundWindow(), 0);
		GUITHREADINFO guiThreadInfoObject;
		guiThreadInfoObject.cbSize = sizeof(GUITHREADINFO);
		GetGUIThreadInfo(tid, &guiThreadInfoObject);
		
		return guiThreadInfoObject.hwndFocus;
	}

	RECT getFocusWindowPos() {
		HWND hwnd = getFocusWindow();
		RECT r;
		GetWindowRect(hwnd, &r);

		return r;
	}

	void setWindowFocus(HWND _handle) {
		//set up a generic keyboard event
		INPUT keyInput;
		keyInput.type = INPUT_KEYBOARD;
		keyInput.ki.wScan = 0; //hardware scan code for key
		keyInput.ki.time = 0;
		keyInput.ki.dwExtraInfo = 0;

		//set focus to the hWnd (sending Alt allows to bypass limitation)
		keyInput.ki.wVk = VK_MENU;
		keyInput.ki.dwFlags = 0;   //0 for key press
		SendInput(1, &keyInput, sizeof(INPUT));

		SetForegroundWindow(_handle); //sets the focus

		keyInput.ki.wVk = VK_MENU;
		keyInput.ki.dwFlags = KEYEVENTF_KEYUP;  //for key release
		SendInput(1, &keyInput, sizeof(INPUT));
	}
}