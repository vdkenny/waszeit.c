#include <minwindef.h>
#include <sqlite3.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <windef.h>
#include <windows.h>

#define EVENTS_MAX 10'000

typedef struct MouseInputEvent {
  DWORD time;
  WPARAM message;
} MouseInputEvent; // todo: store POINT from MSLLHOOKSTRUCT for heatmaps

typedef struct KeyboardInputEvent {
  DWORD vkCode;
  DWORD time;
  WPARAM message;
} KeyboardInputEvent;

typedef struct State {
  MouseInputEvent *mouse;
  size_t mouse_len;

  KeyboardInputEvent *keyboard;
  size_t keyboard_len;
} State;

State *state_init() {
  State *state = malloc(sizeof(State));
  if (state == NULL) {
    return NULL;
  }

  state->mouse = malloc(EVENTS_MAX * sizeof(MouseInputEvent));
  state->mouse_len = 0;
  if (state->mouse == NULL) {
    free(state);
    return NULL;
  }

  state->keyboard = malloc(EVENTS_MAX * sizeof(KeyboardInputEvent));
  state->keyboard_len = 0;
  if (state->keyboard == NULL) {
    free(state->mouse);
    free(state);

    return NULL;
  }

  return state;
}

static State *state = NULL;

LRESULT CALLBACK LowLevelMouseProc(int code, WPARAM wparam, LPARAM lparam) {
  if (code < 0) {
    return CallNextHookEx(NULL, code, wparam, lparam);
  }

  MSLLHOOKSTRUCT *ev = (MSLLHOOKSTRUCT *)lparam;
  state->mouse[state->mouse_len] =
      (MouseInputEvent){.time = ev->time, .message = wparam};
  state->mouse_len++;

  return CallNextHookEx(NULL, code, wparam, lparam);
}

LRESULT CALLBACK LowLevelKeyboardProc(int code, WPARAM wparam, LPARAM lparam) {
  if (code < 0) {
    return CallNextHookEx(NULL, code, wparam, lparam);
  }

  KBDLLHOOKSTRUCT *ev = (KBDLLHOOKSTRUCT *)lparam;
  state->keyboard[state->keyboard_len] = (KeyboardInputEvent){
      .vkCode = ev->vkCode, .time = ev->time, .message = wparam};
  state->keyboard_len++;

  return CallNextHookEx(NULL, code, wparam, lparam);
}

int main() {
  sqlite3 *database;
  if (sqlite3_open("events.db", &database) != SQLITE_OK) {
    return -1;
  }

  HHOOK mouse_hook = SetWindowsHookExW(WH_MOUSE_LL, LowLevelMouseProc, 0, 0);
  HHOOK keyboard_hook =
      SetWindowsHookExW(WH_KEYBOARD_LL, LowLevelKeyboardProc, 0, 0);

  if (mouse_hook == NULL || keyboard_hook == NULL) {
    return -1;
  }

  state = state_init();

  while (GetMessage(NULL, NULL, 0, 0) != 0) {
  }

  UnhookWindowsHookEx(mouse_hook);
  UnhookWindowsHookEx(keyboard_hook);
}
