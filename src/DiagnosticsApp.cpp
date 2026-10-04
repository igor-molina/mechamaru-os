#include <Arduino.h>
#include <M5Cardputer.h>
#include "DiagnosticsApp.h"

void DiagnosticsApp::setExitCallback(ExitCallback callback) {
    exitCallback_ = callback;
}

void DiagnosticsApp::onEnter() {
    lastKey_ = '-';
    lastRefreshMs_ = 0;
    invalidate();
}

void DiagnosticsApp::tick() {
    const unsigned long now = millis();

    if (now - lastRefreshMs_ >= 1000) {
        lastRefreshMs_ = now;
        invalidate();
    }
}

void DiagnosticsApp::onKey(char key) {
    Serial.printf("[diagnostics] key='%c' code=%d\n", key, key);

    lastKey_ = key;

    if (key ==27 && exitCallback_ != nullptr) {
        exitCallback_();
        return;
    }

    invalidate();
}

void DiagnosticsApp::render() {
    auto& display = M5Cardputer.Display;

    const unsigned long seconds = millis() / 1000;
    const unsigned long minutes = seconds / 60;
    const unsigned long remainingSeconds = seconds % 60;

    display.fillScreen(BLACK);
    display.setTextSize(1);

    display.setTextColor(YELLOW, BLACK);
    display.setCursor(8, 6);
    display.println("Diagnostics");

    display.drawFastHLine(8, 18, 224, DARKGREY);

    display.setTextColor(WHITE, BLACK);

    display.setCursor(8, 30);
    display.printf("Battery: %d%%",
                    M5Cardputer.Power.getBatteryLevel());

    display.setCursor(8, 45);
    display.printf("Voltage: %.2f V",
                    M5Cardputer.Power.getBatteryVoltage() / 1000.0f);

    display.setCursor(8, 60);
    display.printf("Uptime: %lum %02lus",
                    minutes,
                    remainingSeconds);

    display.setCursor(8, 75);
    display.printf("Heap free: %u bytes",
                    ESP.getFreeHeap());

    display.setCursor(8, 90);
    display.printf("Last key: %c (%d)",
                    lastKey_,
                    static_cast<int>(lastKey_));

    display.setTextColor(DARKGREY, BLACK);
    display.setCursor(8, 120);
    display.print("ESC: voltar");
}