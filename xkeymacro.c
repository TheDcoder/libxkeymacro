#include <stdlib.h>
#include <string.h>
#include "keyboard_shortcuts.h"

bool xkeymacro_parse(const char *shortcut, KeySym *keysym, unsigned int *modifiers) {
	// Copy shortcut into a temporary string
	char *string = malloc(strlen(shortcut) + 1);
	if (string == NULL) return false;
	strcpy(string, shortcut);
	// Parse the string
	char *token = strtok(string, XKEYMACRO_SHORTCUT_DELIMITER);
	while (token) {
		if (strcasecmp(token, "Ctrl") == 0) {
			*modifiers |= ControlMask;
		} else if (strcasecmp(token, "Alt") == 0) {
			*modifiers |= Mod1Mask;
		} else if (strcasecmp(token, "Shift") == 0) {
			*modifiers |= ShiftMask;
		} else {
			*keysym = XStringToKeysym(token);
			break;
		}
		token = strtok(NULL, XKEYMACRO_SHORTCUT_DELIMITER);
	}
	// Free the string
	free(string);
	return true;
}
