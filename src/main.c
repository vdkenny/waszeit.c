#include "database.h"
#include "state.h"

static State *state = NULL;

LRESULT CALLBACK LowLevelMouseProc(int code, WPARAM wparam, LPARAM lparam) {
  if (code < 0 || wparam == WM_MOUSEMOVE) {
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
  sqlite3 *database = database_init();
  if (database == NULL) {
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
  sqlite3_close(database);
}
