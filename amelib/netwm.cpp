#include <QtGui/qx11info_x11.h>
#include <Netwm>

static Atom atoms[AmeNETWm::ATOM_COUNT];
static bool atoms_created = FALSE;

static bool create_netwm_atoms(Display *d)
{
    static const char *atom_names[] = {
        "COMPOSITING_MANAGER",
        "_KDE_NET_WM_SYSTEM_TRAY_WINDOW_FOR",
        "KWM_WIN_ICON",
        "_MOTIF_WM_HINTS",
        "_MOTIF_WM_INFO",
        "_NET_ACTIVE_WINDOW",
        "_NET_CLIENT_LIST",
        "_NET_CLIENT_LIST_STACKING",
        "_NET_CLOSE_WINDOW",
        "_NET_CURRENT_DESKTOP",
        "_NET_DESKTOP_GEOMETRY",
        "_NET_DESKTOP_LAYOUT",
        "_NET_DESKTOP_NAMES",
        "_NET_DESKTOP_VIEWPORT",
        "_NET_FRAME_EXTENTS",
        "_NET_MOVERESIZE_WINDOW",
        "_NET_NUMBER_OF_DESKTOPS",
        "_NET_REQUEST_FRAME_EXTENTS",
        "_NET_SHOWING_DESKTOP",
        "_NET_STARTUP_ID",
        "_NET_SUPPORTED",
        "_NET_SUPPORTING_WM_CHECK",
        "_NET_SYSTEM_TRAY_OPCODE",
        "_NET_WM_ACTION_ABOVE",
        "_NET_WM_ACTION_BELOW",
        "_NET_WM_ACTION_CHANGE_DESKTOP",
        "_NET_WM_ACTION_CLOSE",
        "_NET_WM_ACTION_FULLSCREEN",
        "_NET_WM_ACTION_MAXIMIZE_HORZ",
        "_NET_WM_ACTION_MAXIMIZE_VERT",
        "_NET_WM_ACTION_MINIMIZE",
        "_NET_WM_ACTION_MOVE",
        "_NET_WM_ACTION_RESIZE",
        "_NET_WM_ACTION_SHADE",
        "_NET_WM_ACTION_STICK",
        "_NET_WM_ALLOWED_ACTIONS",
        "_NET_WM_CONTEXT_HELP",
        "_NET_WM_DESKTOP",
        "_NET_WM_ICON",
        "_NET_WM_ICON_GEOMETRY",
        "_NET_WM_ICON_NAME",
        "_NET_WM_MOVERESIZE",
        "_NET_WM_NAME",
        "_NET_WM_PID",
        "_NET_WM_PING",
        "_NET_WM_WINDOW_OPACITY",
        "_NET_WM_WINDOW_OPACITY_LOCKED",
        "_NET_WM_STATE",
        "_NET_WM_STATE_ABOVE",
        "_NET_WM_STATE_BELOW",
        "_NET_WM_STATE_DEMANDS_ATTENTION",
        "_NET_WM_STATE_FULLSCREEN",
        "_NET_WM_STATE_HIDDEN",
        "_NET_WM_STATE_MAXIMIZED_HORZ",
        "_NET_WM_STATE_MAXIMIZED_VERT",
        "_NET_WM_STATE_MODAL",
        "_NET_WM_STATE_SHADED",
        "_NET_WM_STATE_SKIP_PAGER",
        "_NET_WM_STATE_SKIP_TASKBAR",
        "_NET_WM_STATE_STICKY",
        "_NET_WM_STRUT",
        "_NET_WM_STRUT_PARTIAL",
        "_NET_WM_SYNC_REQUEST",
        "_NET_WM_SYNC_REQUEST_COUNTER",
        "_NET_WM_USER_TIME",
        "_NET_WM_USER_TIME_WINDOW",
        "_NET_WM_WINDOW_TYPE",
        "_NET_WM_WINDOW_TYPE_DESKTOP",
        "_NET_WM_WINDOW_TYPE_DIALOG",
        "_NET_WM_WINDOW_TYPE_DOCK",
        "_NET_WM_WINDOW_TYPE_MENU",
        "_NET_WM_WINDOW_TYPE_NORMAL",
        "_NET_WM_WINDOW_TYPE_SPLASH",
        "_NET_WM_WINDOW_TYPE_TOOLBAR",
        "_NET_WM_WINDOW_TYPE_UTILITY",
        "_NET_WORKAREA",
        "SM_CLIENT_ID",
        "UTF8_STRING",
        "WM_CHANGE_STATE",
        "WM_CLIENT_LEADER",
        "WM_CLIENT_MACHINE",
        "WM_COLORMAP_WINDOWS",
        "WM_DELETE_WINDOW",
        "WM_HINTS",
		"WM_NORMAL_HINTS",
        "WM_PROTOCOLS",
        "WM_STATE",
        "WM_TAKE_FOCUS",
        "WM_TRANSIENT_FOR",
        "WM_WINDOW_ROLE",
        "XFWM4_COMPOSITING_MANAGER",
        "XFWM4_TIMESTAMP_PROP",
        "_XROOTPMAP_ID",
        "_XSETROOT_ID"
    };

    return (XInternAtoms (d, (char **) atom_names, AmeNETWm::ATOM_COUNT, FALSE, atoms) != 0);
}

AmeWinInfo::AmeWinInfo(WId w)
{
	id = w;
}
 
AmeWinInfo::AmeWinInfo()
{
}

AmeWinInfo::~AmeWinInfo()
{
}

AmeNETWm::AmeNETWm(Display *display, int screen)
{
	m_display = display;
	
	if (screen != -1) {
		m_screen = screen;
	} else {
		m_screen = DefaultScreen(m_display);
	}
	m_root = RootWindow(m_display, m_screen);

	if (!atoms_created) 
		{
			create_netwm_atoms(display);
			atoms_created = TRUE;
		}	
}


AmeNETWm::~AmeNETWm()
{
}


QList<WId> AmeNETWm::windows()
{
	int i, num;
	Window *win = NULL;
	QList<WId> list;

	win = (Window *) get_xaproperty(QX11Info::appRootWindow(), atoms[NET_CLIENT_LIST], XA_WINDOW, &num);
	if (!win) return list;
	if (!num)
	{
		XFree(win);
		return list;
	}
	for (i=0; i<num; i++)
	{
		list.append(win[i]);
	}
	return list;
}

uint AmeNETWm::windowDesktop(WId w)
{
	uint desk = 0;
	uint *data;
	
	data = (uint *) get_xaproperty(w, atoms[NET_WM_DESKTOP], XA_CARDINAL, 0);
	if (data)
	{
		desk = *data;
		XFree(data);
	}
	return desk;
}

uint AmeNETWm::currentDesktop()
{
	uint desk;
	quint32 *data;

	data = (quint32*) get_xaproperty(QX11Info::appRootWindow(), atoms[NET_CURRENT_DESKTOP], XA_CARDINAL, 0);
	if (!data) return 0;
	desk = *data;
	XFree(data);
	return desk;
}

void AmeNETWm::iconifyWindow(WId w)
{
	QX11Info inf; // FIXME: more elegant way to get the current screen
	XIconifyWindow(QX11Info::display(), w, inf.screen());	
}

Atom AmeNETWm::atom(int a)
{
	if ((a >= 0) || (a <= ATOM_COUNT)) 
		return atoms[a];
	else
		return None;
}

void AmeNETWm::mapWindow(WId w)
{
	XMapWindow(QX11Info::display(), w);
}

int AmeNETWm::windowType(WId w)
{
	Atom *state;
	int num3;
	int t = AmeNETWm::Unknown;
	
	if (!(state = (Atom *) get_xaproperty(w, atoms[NET_WM_WINDOW_TYPE], XA_ATOM, &num3)))
		return t;
	while (--num3 >= 0) {
		if (state[num3] == atoms[NET_WM_WINDOW_TYPE_DESKTOP]) t = AmeNETWm::Desktop;
		else if (state[num3] == atoms[NET_WM_WINDOW_TYPE_DOCK]) t = AmeNETWm::Dock;
		else if (state[num3] == atoms[NET_WM_WINDOW_TYPE_TOOLBAR]) t = AmeNETWm::Toolbar;
		else if (state[num3] == atoms[NET_WM_WINDOW_TYPE_MENU]) t = AmeNETWm::Menu;
		else if (state[num3] == atoms[NET_WM_WINDOW_TYPE_UTILITY]) t = AmeNETWm::Utility;
		else if (state[num3] == atoms[NET_WM_WINDOW_TYPE_SPLASH]) t = AmeNETWm::Splash;
		else if (state[num3] == atoms[NET_WM_WINDOW_TYPE_DIALOG]) t = AmeNETWm::Dialog;
		else if (state[num3] == atoms[NET_WM_WINDOW_TYPE_NORMAL]) t = AmeNETWm::Normal;
	}
	XFree(state);
	return t;
}

void *AmeNETWm::get_xaproperty(Window win, Atom prop, Atom type, int *nitems)
{
	Atom type_ret;
	int format_ret;
	unsigned long items_ret;
	unsigned long after_ret;
	unsigned char *prop_data;

	prop_data = NULL;

	if (XGetWindowProperty(m_display, win, prop, 0, 0x7fffffff, False, 
			type, &type_ret, &format_ret, &items_ret,
			&after_ret, &prop_data) != Success)
		return NULL;
	
	if (nitems)
		*nitems = items_ret;
	return prop_data;
}


AME_GLOBAL_STATIC_WITH_ARGS(AmeNETWm, gAmeNETWm,
	(QX11Info::display(), -1))


AmeNETWm *AmeNETWm::global()
{
	return gAmeNETWm;
}

