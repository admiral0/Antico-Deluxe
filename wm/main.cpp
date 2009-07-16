
#include "defs.h"
#include "adx.h"

extern Adx *deluxe;

int main(int argc, char **argv)
{
        Adx a(argc, argv);

        deluxe = &a;
        (void) QApplication::desktop();

        prev_x11_event_filter = qApp->setEventFilter(Adx::x11_event_filter);

        XSelectInput(display(), rootWindow(), KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask |
                 KeymapStateMask | ButtonMotionMask | PointerMotionMask | EnterWindowMask | LeaveWindowMask | FocusChangeMask |
                 ExposureMask | StructureNotifyMask | SubstructureRedirectMask | SubstructureNotifyMask);

        XClearWindow(display(), rootWindow());


        XSync(QX11Info::display(), FALSE);

    return a.exec();
}

