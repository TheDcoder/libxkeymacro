#ifndef XKEYMACRO_H
#define XKEYMACRO_H

#include <stdbool.h>
#include <X11/Xlib.h>

#define XKEYMACRO_SHORTCUT_DELIMITER "+"

bool xkeymacro_parse(const char *shortcut, KeySym *keysym, unsigned int *modifiers);

#endif
