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

struct XKeyMacroInstance {
	Display *display;
};

void xkeymacro_set_display(struct XKeyMacroInstance *instance, char *display_name);
bool xkeymacro_parse(const char *shortcut, struct XKeyMacro *macro, struct XKeyMacroInstance *instance);

#endif
