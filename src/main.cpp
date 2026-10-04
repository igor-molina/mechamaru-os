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