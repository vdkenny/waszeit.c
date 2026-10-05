#include <stdio.h>
#include <windows.h>

LRESULT CALLBACK GlobalHookProc(int code, WPARAM wparam, LPARAM lparam) {
  printf("code: %d\n", code);

  return CallNextHookEx(NULL, code, wparam, lparam);
}

int main() {
  HHOOK hook = SetWindowsHookExA(WH_KEYBOARD_LL, GlobalHookProc, 0, 0);

  if (hook == NULL) {
    return -1;
  }

  while (GetMessage(NULL, NULL, 0, 0)) {
  }

  UnhookWindowsHookEx(hook);
}
