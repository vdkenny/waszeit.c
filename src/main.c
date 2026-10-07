#include <minwindef.h>
#include <stdio.h>
#include <stdlib.h>
#include <windef.h>
#include <windows.h>

typedef struct State {
  MSLLHOOKSTRUCT *mouse;
  KBDLLHOOKSTRUCT *keyboard;
} State;

static State *state = NULL;

LRESULT CALLBACK LowLevelMouseProc(int code, WPARAM wparam, LPARAM lparam) {
  if (code < 0) {
    return CallNextHookEx(NULL, code, wparam, lparam);
  }

  // MSLLHOOKSTRUCT *ev = (MSLLHOOKSTRUCT *)lparam;

  return CallNextHookEx(NULL, code, wparam, lparam);
}

LRESULT CALLBACK LowLevelKeyboardProc(int code, WPARAM wparam, LPARAM lparam) {
  if (code < 0) {
    return CallNextHookEx(NULL, code, wparam, lparam);
  }

  KBDLLHOOKSTRUCT *ev = (KBDLLHOOKSTRUCT *)lparam;
  printf("%ld : %ld\n", ev->vkCode, ev->time);

  return CallNextHookEx(NULL, code, wparam, lparam);
}

int main() {
  HHOOK mouse_hook = SetWindowsHookExW(WH_MOUSE_LL, LowLevelMouseProc, 0, 0);
  HHOOK keyboard_hook =
      SetWindowsHookExW(WH_KEYBOARD_LL, LowLevelKeyboardProc, 0, 0);

  if (mouse_hook == NULL || keyboard_hook == NULL) {
    return -1;
  }

  state = malloc(sizeof(State)); // todo: periodically save on disk to sqlite
  state->mouse = malloc(100'000 * sizeof(MSLLHOOKSTRUCT));
  state->keyboard = malloc(10'000 * sizeof(KBDLLHOOKSTRUCT));

  while (GetMessage(NULL, NULL, 0, 0) != 0) {
  }

  UnhookWindowsHookEx(mouse_hook);
  UnhookWindowsHookEx(keyboard_hook);
}
