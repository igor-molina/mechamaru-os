#pragma once

#include "App.h"

class LauncherApp : public App {
 public:
  using OpenDiagnosticsCallback = void (*)();

  void setOpenDiagnosticsCallback(OpenDiagnosticsCallback callback);

  void onEnter() override;
  void onKey(char key) override;
  void render() override;

 private:
  static constexpr int itemCount_ = 4;

  int selected_ = 0;
  OpenDiagnosticsCallback openDiagnostics_ = nullptr;

  void drawItem(int index, const char* label);
};