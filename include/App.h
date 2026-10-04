#pragma once

class App {
 public:
  virtual ~App() = default;

  virtual void onEnter() {}
  virtual void onExit() {}
  virtual void tick() {}
  virtual void onKey(char key) = 0;
  virtual void render() = 0;

  void invalidate() {
    dirty_ = true;
  }

  bool needsRender() const {
    return dirty_;
  }

  void clearInvalidation() {
    dirty_ = false;
  }

 protected:
  bool dirty_ = true;
};