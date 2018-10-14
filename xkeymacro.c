#include <stdlib.h>
#include <string.h>
#include "xkeymacro.h"

struct XKeyMacroInstance *xkeymacro_new_instance(Display *display) {
	struct XKeyMacroInstance *instance = malloc(sizeof(struct XKeyMacroInstance));
	if (instance == NULL) return NULL;
	instance->display = display;
	instance->grab_window = DefaultRootWindow(display);
	instance->latest_node = NULL;
	return instance;
}

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

struct XKeyMacroNode *xkeymacro_add(struct XKeyMacroInstance *instance, struct XKeyMacro *macro, bool grab) {
	struct XKeyMacroNode *new_node = malloc(sizeof(struct XKeyMacroNode));
	if (!new_node) return NULL;
	if (grab) {
		XGrabKey(instance->display, macro->code, macro->modifiers, instance->grab_window, true, GrabModeAsync, GrabModeAsync);
		new_node->grabbed = true;
	} else new_node->grabbed = false;
	new_node->macro = macro;
	new_node->next = NULL;
	new_node->prev = instance->latest_node;
	if (instance->latest_node) instance->latest_node->next = new_node;
	instance->latest_node = new_node;
	return new_node;
}

bool xkeymacro_remove(struct XKeyMacroInstance *instance, struct XKeyMacro *macro, bool ungrab) {
	struct XKeyMacroNode *target_node = xkeymacro_find(instance, macro);
	if (!target_node) return false;
	if (ungrab && target_node->grabbed) XUngrabKey(instance->display, macro->code, macro->modifiers, instance->grab_window);
	if (target_node->next) target_node->next->prev = target_node->prev;
	if (target_node->prev) target_node->prev->next = target_node->next;
	if (target_node == instance->latest_node) instance->latest_node = target_node->next;
	free(target_node);
	return true;
}

struct XKeyMacroNode *xkeymacro_find(struct XKeyMacroInstance *instance, struct XKeyMacro *macro) {
	struct XKeyMacroNode *target_node = instance->latest_node;
	while (target_node) {
		if (target_node->macro == macro) return target_node;
		target_node = target_node->prev;
	}
	return NULL;
}

struct XKeyMacro *xkeymacro_next_event(struct XKeyMacroInstance *instance) {
	XEvent event;
	struct XKeyMacroNode *target_node = instance->latest_node;
	while (true) {
		XNextEvent(instance->display, &event);
		if (event.type != KeyPress) continue;
		while (target_node) {
			if (
				target_node->macro->code == event.xkey.keycode
				&&
				target_node->macro->modifiers == event.xkey.state
			) return target_node->macro;
			target_node = target_node->prev;
		}
	}
}
