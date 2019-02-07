#include "autocompleter.h"

#include "FAPILogger.h"
#include "WinUtils.h"
#include <qplatformdefs.h>


using namespace CPPFAPIWrapper;
using namespace OracleFormsUtils;

Autocompleter * Autocompleter::s_instance = nullptr;

Autocompleter * Autocompleter::instance() {
    if( !s_instance )
        s_instance = new Autocompleter();

    return s_instance;
}

Autocompleter::Autocompleter()
{

}

void Autocompleter::setWindow(QQuickWindow * _wnd) {
    wnd = _wnd;
}

void Autocompleter::toggle() {
    if( !wnd->isVisible() ) {
        RECT caret = getCaretPos();
        RECT focus = getFocusWindowPos();
        wnd->setPosition(focus.left + caret.left, focus.top + caret.bottom + 8 /*todo offset*/);
        wnd->show();
        setWindowFocus(reinterpret_cast<HWND>(wnd->winId()));
    }
    else
        wnd->hide();
}
