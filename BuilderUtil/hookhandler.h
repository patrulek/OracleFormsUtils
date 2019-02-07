#ifndef HOOKHANDLER_H
#define HOOKHANDLER_H

#define _mm_prefetch _mm_prefetch_name_changed_to_avoid_linkage_error

#include <QObject>
#include "WinUtils.h"

class HookHandler : public QObject
{
    Q_OBJECT

    public:
        ~HookHandler();

        static HookHandler * instance();
        void setHooks();
        void releaseHooks();
    private:
        void setMouseHook();
        static LRESULT CALLBACK mouseHookProc(int _n_code, WPARAM _w_param, LPARAM _l_param);
        void setKeyboardHook();
        static LRESULT CALLBACK keyboardHookProc(int _n_code, WPARAM _w_param, LPARAM _l_param);

        static BOOL CALLBACK enumProc(HWND _hwnd, LPARAM _lparam);

        Q_SLOT void onMouseAction(int _msg, LPARAM _lparam, WPARAM _wparam);
        Q_SLOT void onKeyboardAction(int _msg, LPARAM _lparam, WPARAM _wparam);

        Q_SIGNAL void mouseAction(int _ncode, LPARAM _lparam, WPARAM _wparam);
        Q_SIGNAL void keyboardAction(int _ncode, LPARAM _lparam, WPARAM _wparam);

        HHOOK mouse_hook, keyboard_hook;
        static HookHandler * s_instance;

        void menuSearch(KBDLLHOOKSTRUCT * _kbd_struct, HWND _hwnd);
        void reloadModule(KBDLLHOOKSTRUCT * _kbd_struct, HWND _hwnd);
        void openAutocomplete(KBDLLHOOKSTRUCT * _kbd_struct, HWND _hwnd);

        HookHandler() = default;
};

#endif // HOOKHANDLER_H
