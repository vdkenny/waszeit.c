#include "state.h"
#include <minwindef.h>

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
