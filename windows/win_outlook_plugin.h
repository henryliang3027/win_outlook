#ifndef FLUTTER_PLUGIN_WIN_OUTLOOK_PLUGIN_H_
#define FLUTTER_PLUGIN_WIN_OUTLOOK_PLUGIN_H_

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>

#include <memory>

namespace win_outlook {

class WinOutlookPlugin : public flutter::Plugin {
 public:
  static void RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar);

  WinOutlookPlugin();

  virtual ~WinOutlookPlugin();

  // Disallow copy and assign.
  WinOutlookPlugin(const WinOutlookPlugin&) = delete;
  WinOutlookPlugin& operator=(const WinOutlookPlugin&) = delete;

  // Called when a method is called on this plugin's channel from Dart.
  void HandleMethodCall(
      const flutter::MethodCall<flutter::EncodableValue> &method_call,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
};

}  // namespace win_outlook

#endif  // FLUTTER_PLUGIN_WIN_OUTLOOK_PLUGIN_H_
