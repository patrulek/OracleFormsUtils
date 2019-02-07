#ifndef AUTOCOMPLETER_H
#define AUTOCOMPLETER_H

#include <QQuickWindow>

class Autocompleter
{
public:
    static Autocompleter * instance();

    void setWindow(QQuickWindow * _wnd);
    void toggle();
private:
    Autocompleter();
    static Autocompleter * s_instance;

    QQuickWindow * wnd;
};

#endif // AUTOCOMPLETER_H
