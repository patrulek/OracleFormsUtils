#ifndef WINUTILS_H
#define WINUTILS_H

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <string>

namespace OracleFormsUtils {
	std::string getWindowTitle(HWND _handle);

	std::string getWindowClass(HWND _handle);

	bool isScrollableWindow(std::string _title);

	void changeScrollbarPos(HWND _handle, int _sc_event, int _scroll_shift);

	SCROLLINFO getScrollInfo(HWND _handle);

	bool isShiftPressed();

	bool isCtrlPressed();

	bool isSpacePressed(KBDLLHOOKSTRUCT * _kbd_struct);

	bool isLetterPressed(char _letter, KBDLLHOOKSTRUCT * _kbd_struct);

	RECT getCaretPos();

	HWND getFocusWindow();

	RECT getFocusWindowPos();

	void setWindowFocus(HWND _handle);
}

#endif // WINUTILS_H