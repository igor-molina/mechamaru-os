#pragma once

enum class Key {
  None,
  Up,
  Down,
  Left,
  Right,
  Enter,
  Escape,
  Backspace,
  Character
};

struct InputEvent {
  Key key = Key::None;
  char character = '\0';
};