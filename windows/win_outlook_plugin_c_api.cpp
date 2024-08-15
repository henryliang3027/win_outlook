#include "include/win_outlook/win_outlook_plugin_c_api.h"

#include <flutter/plugin_registrar_windows.h>

#include "win_outlook_plugin.h"

void WinOutlookPluginCApiRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar) {
  win_outlook::WinOutlookPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}
