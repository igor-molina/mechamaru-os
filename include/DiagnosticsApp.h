#pragma once

#include "App.h"

class DiagnosticsApp : public App {
 public:
  using ExitCallback = void (*)();

  void setExitCallback(ExitCallback callback);

  void onEnter() override;
  void tick() override;
  void onKey(char key) override;
  void render() override;

 private:
  char lastKey_ = '-';
  unsigned long lastRefreshMs_ = 0;
  ExitCallback exitCallback_ = nullptr;
};