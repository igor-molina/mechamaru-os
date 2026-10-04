#include <Arduino.h>
#include <M5Cardputer.h>

#include "App.h"
#include "LauncherApp.h"
#include "DiagnosticsApp.h"

LauncherApp launcherApp;
DiagnosticsApp diagnosticsApp;

App* activeApp = &launcherApp;

void switchTo(App* nextApp) {
  if (nextApp == nullptr || nextApp == activeApp) {
    return;
  }

  activeApp->onExit();
  activeApp = nextApp;
  activeApp->onEnter();

  Serial.println("[system] application switched");
}

void showBootScreen() {
  auto& display = M5Cardputer.Display;

  display.fillScreen(BLACK);
  display.setTextSize(1);

  display.setTextColor(CYAN, BLACK);
  display.setCursor(12, 24);
  display.println("MECHAMARU OS");

  display.setTextColor(WHITE, BLACK);
  display.setCursor(12, 42);
  display.println("booting...");

  display.setTextColor(DARKGREY, BLACK);
  display.setCursor(12, 60);
  display.println("Cardputer-Adv / ESP32-S3");
}

void printKeyboardState(
    Keyboard_Class::KeysState keys,
    bool pressed) {

  Serial.printf(
      "[kbd] pressed=%d enter=%d del=%d tab=%d "
      "fn=%d shift=%d ctrl=%d alt=%d word=\"",
      pressed,
      keys.enter,
      keys.del,
      keys.tab,
      keys.fn,
      keys.esc,
      keys.shift,
      keys.ctrl,
      keys.alt
  );

  for (const auto& key : keys.word) {
    if (key >= 32 && key <= 126) {
      Serial.print(key);
    } else {
      Serial.print('?');
    }
  }

  Serial.println("\"");
}

void setup() {
  auto config = M5.config();

  // O segundo argumento habilita o teclado do Cardputer.
  M5Cardputer.begin(config, true);

  Serial.begin(115200);

  M5Cardputer.Display.setRotation(1);

  showBootScreen();
  delay(500);

  launcherApp.setOpenDiagnosticsCallback([]() {
    switchTo(&diagnosticsApp);
  });

  diagnosticsApp.setExitCallback([]() {
    switchTo(&launcherApp);
  });

  activeApp->onEnter();

  Serial.println("[system] Mechamaru OS v0.1.0 started");
  Serial.println("[system] keyboard diagnostic enabled");
}

void loop() {
  // Atualiza teclado, energia e periféricos.
  M5Cardputer.update();

  // Deixa o app ativo atualizar estado periódico.
  activeApp->tick();

  // isChange() ocorre tanto ao pressionar quanto ao soltar.
  if (M5Cardputer.Keyboard.isChange()) {
    const auto keys = M5Cardputer.Keyboard.keysState();
    const bool pressed = M5Cardputer.Keyboard.isPressed();

    printKeyboardState(keys, pressed);

    // Encaminhamos apenas o momento de pressionamento ao app ativo.
    if (pressed) {
      // W, S, letras, números e demais caracteres normais.
      for (const auto& key : keys.word) {
        Serial.printf("[kbd] character='%c' code=%d\n",
                      key,
                      static_cast<int>(key));

        activeApp->onKey(key);
      }

      // A tecla marcada como OK deve ativar keys.enter.
      if (keys.enter) {
        Serial.println("[kbd] ENTER/OK detected");
        activeApp->onKey('\n');
      }

      // Delete/Backspace.
      if (keys.del) {
        Serial.println("[kbd] DELETE detected");
        activeApp->onKey('\b');
      }

      // Tab.
      if (keys.tab) {
        Serial.println("[kbd] TAB detected");
        activeApp->onKey('\t');
      }

      if (keys.esc) {
        Serial.println("[kbd] ESC detected");
        activeApp->onKey(27);
      }
    }
  }

  // Redesenha somente se o app pediu uma atualização visual.
  if (activeApp->needsRender()) {
    activeApp->render();
    activeApp->clearInvalidation();
  }

  delay(10);
}