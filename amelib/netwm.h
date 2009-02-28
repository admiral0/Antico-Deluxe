#ifndef __AMENETWM_H
#define __AMENETWM_H

#include <Global>
#include <QtGui/QWidgetList> // for WId
#include <QList>
#include <X11/Xlib.h>
#include <X11/Xatom.h>

class AME_EXPORT AmeWinInfo
{
public:

	AmeWinInfo(WId w);
	AmeWinInfo();
	~AmeWinInfo();
protected:
	WId id;
};

class AME_EXPORT AmeNETWm
{

public:

	enum {
    	COMPOSITING_MANAGER = 0,
    	KDE_NET_WM_SYSTEM_TRAY_WINDOW_FOR,
    	KWM_WIN_ICON,
    	MOTIF_WM_HINTS,
    	MOTIF_WM_INFO,
    	NET_ACTIVE_WINDOW,
    	NET_CLIENT_LIST,
    	NET_CLIENT_LIST_STACKING,
    	NET_CLOSE_WINDOW,
	    NET_CURRENT_DESKTOP,
	    NET_DESKTOP_GEOMETRY,
	    NET_DESKTOP_LAYOUT,
	    NET_DESKTOP_NAMES,
	    NET_DESKTOP_VIEWPORT,
	    NET_FRAME_EXTENTS,
	    NET_MOVERESIZE_WINDOW,
	    NET_NUMBER_OF_DESKTOPS,
	    NET_REQUEST_FRAME_EXTENTS,
	    NET_SHOWING_DESKTOP,
	    NET_STARTUP_ID,
	    NET_SUPPORTED,
	    NET_SUPPORTING_WM_CHECK,
	    NET_SYSTEM_TRAY_OPCODE,
	    NET_WM_ACTION_ABOVE,
	    NET_WM_ACTION_BELOW,
	    NET_WM_ACTION_CHANGE_DESKTOP,
	    NET_WM_ACTION_CLOSE,
	    NET_WM_ACTION_FULLSCREEN,
	    NET_WM_ACTION_MAXIMIZE_HORZ,
	    NET_WM_ACTION_MAXIMIZE_VERT,
	    NET_WM_ACTION_MINIMIZE,
	    NET_WM_ACTION_MOVE,
	    NET_WM_ACTION_RESIZE,
	    NET_WM_ACTION_SHADE,
	    NET_WM_ACTION_STICK,
	    NET_WM_ALLOWED_ACTIONS,
	    NET_WM_CONTEXT_HELP,
	    NET_WM_DESKTOP,
	    NET_WM_ICON,
	    NET_WM_ICON_GEOMETRY,
	    NET_WM_ICON_NAME,
	    NET_WM_MOVERESIZE,
	    NET_WM_NAME,
	    NET_WM_PID,
	    NET_WM_PING,
	    NET_WM_WINDOW_OPACITY,
	    NET_WM_WINDOW_OPACITY_LOCKED,
	    NET_WM_STATE,
	    NET_WM_STATE_ABOVE,
	    NET_WM_STATE_BELOW,
	    NET_WM_STATE_DEMANDS_ATTENTION,
	    NET_WM_STATE_FULLSCREEN,
	    NET_WM_STATE_HIDDEN,
	    NET_WM_STATE_MAXIMIZED_HORZ,
	    NET_WM_STATE_MAXIMIZED_VERT,
	    NET_WM_STATE_MODAL,
	    NET_WM_STATE_SHADED,
	    NET_WM_STATE_SKIP_PAGER,
	    NET_WM_STATE_SKIP_TASKBAR,
	    NET_WM_STATE_STICKY,
	    NET_WM_STRUT,
	    NET_WM_STRUT_PARTIAL,
	    NET_WM_SYNC_REQUEST,
		NET_WM_SYNC_REQUEST_COUNTER,
	    NET_WM_USER_TIME,
	    NET_WM_USER_TIME_WINDOW,
	    NET_WM_WINDOW_TYPE,
	    NET_WM_WINDOW_TYPE_DESKTOP,
	    NET_WM_WINDOW_TYPE_DIALOG,
	    NET_WM_WINDOW_TYPE_DOCK,
	    NET_WM_WINDOW_TYPE_MENU,
	    NET_WM_WINDOW_TYPE_NORMAL,
	    NET_WM_WINDOW_TYPE_SPLASH,
	    NET_WM_WINDOW_TYPE_TOOLBAR,
	    NET_WM_WINDOW_TYPE_UTILITY,
	    NET_WORKAREA,
	    SM_CLIENT_ID,
	    UTF8_STRING,
	    WM_CHANGE_STATE,
	    WM_CLIENT_LEADER,
	    WM_CLIENT_MACHINE,
	    WM_COLORMAP_WINDOWS,
	    WM_DELETE_WINDOW,
	    WM_HINTS,
		WM_NORMAL_HINTS,
	    WM_PROTOCOLS,
	    WM_STATE,
	    WM_TAKE_FOCUS,
	    WM_TRANSIENT_FOR,
	    WM_WINDOW_ROLE,
	    XROOTPMAP_ID,
	    XSETROOT_ID,
	    ATOM_COUNT
	};

	enum WindowType {
		Unknown = -1,
		Normal = 0,
		Desktop = 1,
		Dock = 2,
		Toolbar = 3,
		Menu = 4,
		Dialog = 5,
		Override = 6,
		TopMenu = 7,
		Utility = 8,
		Splash = 9,
		DropdownMenu = 10,
		PopupMenu = 11,
		Tooltip = 12,
		Notification = 13,
		ComboBox = 14,
		DNDIcon = 15
	};

	AmeNETWm(Display *display, int screen = -1);
	virtual ~AmeNETWm();	

	static AmeNETWm *global();

	QList<WId> windows();
	uint windowDesktop(WId);
	int windowType(WId);
	uint currentDesktop();
	void mapWindow(WId);
	void iconifyWindow(WId);

	Atom atom(int i);

protected:
	Display *m_display;
	Window m_window, m_root;	
	int m_screen;

	void *get_xaproperty(Window win, Atom prop, Atom type, int *nitems);

};

#endif /* __AMENETWM_H */

