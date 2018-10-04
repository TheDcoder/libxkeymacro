#include <stdlib.h>
#include <string.h>
#include "xkeymacro.h"

void xkeymacro_set_display(struct XKeyMacroInstance *instance, char *display_name) {
	instance->display = XOpenDisplay(display_name);
}

bool xkeymacro_parse(const char *shortcut, struct XKeyMacro *macro, struct XKeyMacroInstance *instance) {
	// Reset the macro modifiers
	macro->modifiers = 0;
	// Copy shortcut into a temporary string
	char *string = malloc(strlen(shortcut) + 1);
	if (string == NULL) return false;
	strcpy(string, shortcut);
	// Parse the string
	char *token = strtok(string, XKEYMACRO_SHORTCUT_DELIMITER);
	while (token) {
		if (strcasecmp(token, "Ctrl") == 0) {
			macro->modifiers |= ControlMask;
		} else if (strcasecmp(token, "Alt") == 0) {
			macro->modifiers |= Mod1Mask;
		} else if (strcasecmp(token, "Shift") == 0) {
			macro->modifiers |= ShiftMask;
		} else {
			macro->symbol = XStringToKeysym(token);
			if (instance) macro->code = XKeysymToKeycode(instance->display, macro->symbol);
			break;
		}
		token = strtok(NULL, XKEYMACRO_SHORTCUT_DELIMITER);
	}
	// Free the string
	free(string);
	return true;
}
