#include "win_outlook_plugin.h"

// This must be included before many other Windows headers.
#include <windows.h>

// For getPlatformVersion; remove unless needed for your plugin implementation.
#include <VersionHelpers.h>

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>

#include <memory>
#include <sstream>
#include <MAPI.h>

#pragma comment(lib, "MAPI32.lib")
namespace win_outlook
{

  void OpenEmailWithAttachment(const std::string &recipient,
                               const std::string &subject,
                               const std::string &body,
                               const std::string &attachmentPath)
  {

    LPSTR lpstrRecipient = new char[recipient.size() + 1];
    strcpy_s(lpstrRecipient, recipient.size() + 1, recipient.c_str());

    LPSTR lpstrAttachmentPath = new char[attachmentPath.size() + 1];
    strcpy_s(lpstrAttachmentPath, attachmentPath.size() + 1, attachmentPath.c_str());

    LPSTR lpstrSubject = new char[subject.size() + 1];
    strcpy_s(lpstrSubject, subject.size() + 1, subject.c_str());

    LPSTR lpstrBody = new char[body.size() + 1];
    strcpy_s(lpstrBody, body.size() + 1, body.c_str());

    // Define the attachment
    MapiFileDesc attachment = {};
    attachment.nPosition = (ULONG)-1; // Attach at the end of the message
    attachment.lpszPathName = lpstrAttachmentPath;
    attachment.lpszFileName = ""; // Optional: the name to display in Outlook

    // Define the message
    MapiMessage message = {};
    message.lpszSubject = lpstrSubject;
    message.lpszNoteText = lpstrBody;

    message.nFileCount = 1;
    message.lpFiles = &attachment;

    if (!recipient.empty())
    {

      MapiRecipDesc recipDesc = {};
      recipDesc.ulRecipClass = MAPI_TO;
      recipDesc.lpszName = lpstrRecipient;
      recipDesc.lpszAddress = ""; // Use "SMTP:" prefix for email address

      message.nRecipCount = 1;
      message.lpRecips = &recipDesc;
    }

    // Initialize MAPI
    HINSTANCE hMapi = ::LoadLibraryA("MAPI32.DLL");
    if (hMapi)
    {
      LPMAPISENDMAIL lpfnMAPISendMail = (LPMAPISENDMAIL)::GetProcAddress(hMapi, "MAPISendMail");
      if (lpfnMAPISendMail)
      {
        lpfnMAPISendMail(0, 0, &message, MAPI_DIALOG, 0);
      }
      ::FreeLibrary(hMapi);
      delete[] lpstrRecipient;
      delete[] lpstrAttachmentPath;
      delete[] lpstrSubject;
      // delete[] lpstrBody;
    }
  }

  // static
  void WinOutlookPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarWindows *registrar)
  {
    auto channel =
        std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
            registrar->messenger(), "win_outlook",
            &flutter::StandardMethodCodec::GetInstance());

    auto plugin = std::make_unique<WinOutlookPlugin>();

    channel->SetMethodCallHandler(
        [plugin_pointer = plugin.get()](const auto &call, auto result)
        {
          plugin_pointer->HandleMethodCall(call, std::move(result));
        });

    registrar->AddPlugin(std::move(plugin));
  }

  WinOutlookPlugin::WinOutlookPlugin() {}

  WinOutlookPlugin::~WinOutlookPlugin() {}

  void WinOutlookPlugin::HandleMethodCall(
      const flutter::MethodCall<flutter::EncodableValue> &method_call,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
  {
    if (method_call.method_name().compare("getPlatformVersion") == 0)
    {
      std::ostringstream version_stream;
      version_stream << "Windows ";
      if (IsWindows10OrGreater())
      {
        version_stream << "10+";
      }
      else if (IsWindows8OrGreater())
      {
        version_stream << "8";
      }
      else if (IsWindows7OrGreater())
      {
        version_stream << "7";
      }
      result->Success(flutter::EncodableValue(version_stream.str()));
    }
    else if (method_call.method_name().compare("openEmail") == 0)
    {

      // Parse arguments and call the function
      const auto *args = std::get_if<flutter::EncodableMap>(method_call.arguments());
      if (args)
      {
        std::string recipient = std::get<std::string>(args->at(flutter::EncodableValue("recipient"))).c_str();
        std::string subject = std::get<std::string>(args->at(flutter::EncodableValue("subject"))).c_str();
        std::string body = std::get<std::string>(args->at(flutter::EncodableValue("body"))).c_str();
        std::string attachmentPath = std::get<std::string>(args->at(flutter::EncodableValue("attachmentPath"))).c_str();
        OpenEmailWithAttachment(recipient, subject, body, attachmentPath);

        std::string mailto = "/m \"" + recipient + "?subject=" + subject + "&body=" + body + "\"";
        result->Success(flutter::EncodableValue(mailto));
      }
      else
      {
        result->Error("INVALID_ARGUMENTS", "Invalid arguments provided to openEmail");
      }
    }
    else
    {
      result->NotImplemented();
    }
  }

} // namespace win_outlook
