#include <M5Cardputer.h>
#include "LauncherApp.h"

void LauncherApp::setOpenDiagnosticsCallback(
    OpenDiagnosticsCallback callback) {
        openDiagnostics_ = callback;
    }

void LauncherApp::onEnter() {
    selected_ = 0;
    invalidate();
}

void LauncherApp::onKey(char key) {
    Serial.printf("[launcher] key='%c' code=%d\n", key, key);

    if (key == 's' || key == 'S') {
        selected_ = (selected_ + 1) % itemCount_;
        invalidate();
        return;
    }

    if (key == 'w' || key == 'W') {
        selected_ = (selected_ + itemCount_ - 1) % itemCount_;
        invalidate();
        return;
    }

    if (key == '\n' || key == '\r') {
        if (selected_ == 0 && openDiagnostics_ != nullptr) {
            openDiagnostics_();
        }
    }
}

void LauncherApp::render() {
    auto& display = M5Cardputer.Display;

    display.fillScreen(BLACK);
    display.setTextSize(1);

    display.setTextColor(CYAN, BLACK);
    display.setCursor(8, 6);
    display.println("MechamaruOS");

    display.setTextColor(DARKGREY, BLACK);
    display.setCursor(8, 17);
    display.println("launcher / v0.1.0");

    display.drawFastHLine(8, 28, 224, DARKGREY);

    drawItem(0, "Diagnostics");
    drawItem(1, "Terminal - em breve");
    drawItem(2, "Files - em breve");
    drawItem(3, "Settings - em breve");

    display.setTextColor(DARKGREY, BLACK);
    display.setCursor(8, 120);
    display.print("W/S: navegar  Enter: abrir");
}

void LauncherApp::drawItem(int index, const char* label) {
    auto& display = M5Cardputer.Display;

    const int y = 36 + (index * 18);
    const bool isSelected = index == selected_;

    if (isSelected) {
        display.fillRoundRect(6, y - 2, 228, 15, 3, DARKCYAN);
    }

    display.setTextColor(
        isSelected ? WHITE : LIGHTGREY,
        isSelected ? DARKCYAN : BLACK);
        
    display.setCursor(12, y + 1);
    display.printf("%c %s", isSelected ? '>' : ' ', label);
}