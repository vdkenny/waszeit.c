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

State *state_init();
