
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>

#define TARGET_NICE -15
#define RESET_NICE +5

// Helper to get a long property (like a Window ID or PID) from a window
unsigned long get_long_prop(Display *d, Window w, char *prop_name) {
    Atom actual_type, filter_atom;
    int actual_format;
    unsigned long nitems, bytes_after;
    unsigned char *prop;
    unsigned long result = 0;

    filter_atom = XInternAtom(d, prop_name, True);
    if (XGetWindowProperty(d, w, filter_atom, 0, 1, False, AnyPropertyType,
                           &actual_type, &actual_format, &nitems, &bytes_after, &prop) == Success && prop) {
        result = *(unsigned long *)prop;
        XFree(prop);
    }
    return result;
}

int main() {
    Display *d = XOpenDisplay(NULL);
    if (!d) return (fprintf(stderr, "Cannot open display\n"), 1);

    if (geteuid() != 0) {
        printf("Warning: Must run as root (sudo) to set priority to %d\n", TARGET_NICE);
        return 1;
    }

    Window root = DefaultRootWindow(d);
    Atom net_active_window = XInternAtom(d, "_NET_ACTIVE_WINDOW", False);
    
    // Select PropertyChangeMask on the root window to hear about focus changes
    XSelectInput(d, root, PropertyChangeMask);

    unsigned long last_pid = 0;
    printf("Renicer active. Target: %d. Monitoring focus...\n", TARGET_NICE);

    XEvent ev;
    while (1) {
        XNextEvent(d, &ev);
        
        // Only trigger when the focused window property changes
        if (ev.type == PropertyNotify && ev.xproperty.atom == net_active_window) {
            Window active_win = get_long_prop(d, root, "_NET_ACTIVE_WINDOW");
            if (active_win == None) continue;

            unsigned long pid = get_long_prop(d, active_win, "_NET_WM_PID");
            
            if (pid > 0 && pid != last_pid) {
                // Restore old window
                if (last_pid > 0) {
                    setpriority(PRIO_PROCESS, last_pid, RESET_NICE);
                    printf("Restored PID %lu to nice %d\n", last_pid, RESET_NICE);
                }

                // Boost new window
                if (setpriority(PRIO_PROCESS, pid, TARGET_NICE) == 0) {
                    printf("BOOSTED PID %lu to nice %d\n", pid, TARGET_NICE);
                    last_pid = pid;
                }
            }
        }
    }

    XCloseDisplay(d);
    return 0;
}
