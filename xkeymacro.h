#ifndef XKEYMACRO_H
#define XKEYMACRO_H

#include <stdbool.h>
#include <X11/Xlib.h>

#define XKEYMACRO_SHORTCUT_DELIMITER "+"

struct XKeyMacro {
	KeySym symbol;
	KeyCode code;
	unsigned int modifiers;
};

struct XKeyMacroNode {
	struct XKeyMacro *macro;
	bool grabbed;
	struct XKeyMacroNode *prev;
	struct XKeyMacroNode *next;
};

struct XKeyMacroInstance {
	Display *display;
	Window grab_window;
	struct XKeyMacroNode *latest_node;
};

struct XKeyMacroInstance *xkeymacro_new_instance(Display *display);
void xkeymacro_set_display(struct XKeyMacroInstance *instance, char *display_name);
struct XKeyMacro *xkeymacro_parse(const char *shortcut, struct XKeyMacroInstance *instance);
struct XKeyMacroNode *xkeymacro_add(struct XKeyMacroInstance *instance, struct XKeyMacro *macro, bool grab);
bool xkeymacro_remove(struct XKeyMacroInstance *instance, struct XKeyMacro *macro, bool ungrab);
struct XKeyMacroNode *xkeymacro_find(struct XKeyMacroInstance *instance, struct XKeyMacro *macro);
struct XKeyMacro *xkeymacro_next_event(struct XKeyMacroInstance *instance);

#endif
