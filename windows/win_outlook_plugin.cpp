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
  class EmailStatus
  {
  public:
    bool isSuccessful;
    std::string message;

    // Convert EmailStatus to flutter::EncodableMap
    flutter::EncodableValue toEncodableValue() const
    {
      flutter::EncodableMap map;
      map[flutter::EncodableValue("isSuccessful")] = flutter::EncodableValue(isSuccessful);
      map[flutter::EncodableValue("message")] = flutter::EncodableValue(message);
      return flutter::EncodableValue(map);
    }
  };

  PWSTR ConvertStringToPWSTR(const std::string &str)
  {

    // Step 1: Calculate the required size for the wide character buffer
    int wstr_size = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);

    // Step 2: Allocate memory for PWSTR
    PWSTR pwstr = new wchar_t[wstr_size];

    // Step 3: Perform the actual conversion
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, pwstr, wstr_size);

    return pwstr;
  }

  std::string getFilenameFromPath(const std::string &filepath)
  {
    // Find the last occurrence of '/' and '\\'
    size_t lastSlashPos = filepath.find_last_of("/\\");

    // If no slash is found, the entire string is the filename
    if (lastSlashPos == std::string::npos)
    {
      return filepath;
    }

    // Return the substring after the last slash
    return filepath.substr(lastSlashPos + 1);
  }

  EmailStatus OpenEmailWithAttachment(const std::string &recipient,
                                      const std::string &subject,
                                      const std::string &body,
                                      const std::string &attachmentPath)
  {
    // Allocate strings
    PWSTR pwstrRecipient = ConvertStringToPWSTR(recipient);
    PWSTR pwstrAttachmentPath = ConvertStringToPWSTR(attachmentPath);
    PWSTR pwstrSubject = ConvertStringToPWSTR(subject);
    PWSTR pwstrBody = ConvertStringToPWSTR(body);

    // Define the attachment structure
    MapiFileDescW attachment = {};
    attachment.nPosition = (ULONG)-1;              // Attach at the end of the message
    attachment.lpszPathName = pwstrAttachmentPath; // File path of the attachment
    std::string filename = getFilenameFromPath(attachmentPath);
    attachment.lpszFileName = ConvertStringToPWSTR(filename); // Display name of the attachment

    // Define the recipient
    MapiRecipDescW recipDesc = {};
    recipDesc.ulRecipClass = MAPI_TO;
    recipDesc.lpszName = pwstrRecipient;
    recipDesc.lpszAddress = pwstrRecipient; // Mail address

    // Define the message
    MapiMessageW message = {};
    message.lpszSubject = pwstrSubject;
    message.lpszNoteText = pwstrBody;
    if (!recipient.empty())
    {
      message.nRecipCount = 1;
      message.lpRecips = &recipDesc; // Set the recipient
    }

    message.nFileCount = 1;        // Number of files to attach
    message.lpFiles = &attachment; // Pointer to the attachments

    EmailStatus emailStatus;

    // Load MAPI and send email
    HINSTANCE hMapi = ::LoadLibraryA("MAPI32.DLL");
    if (hMapi)
    {
      LPMAPISENDMAILW lpfnMAPISendMail = (LPMAPISENDMAILW)::GetProcAddress(hMapi, "MAPISendMailW");
      if (lpfnMAPISendMail)
      {
        HRESULT hr = lpfnMAPISendMail(0, 0, &message, MAPI_DIALOG, 0);
        if (hr != S_OK)
        {
          emailStatus.isSuccessful = false;
          emailStatus.message = "MAPISendMail failed with error: " + hr;
        }
        else
        {
          emailStatus.isSuccessful = true;
          emailStatus.message = "Email sent successfully." + hr;
        }
      }
      else
      {
        emailStatus.isSuccessful = false;
        emailStatus.message = "Failed to get address of MAPISendMailW";
      }
      ::FreeLibrary(hMapi);
    }
    else
    {
      emailStatus.isSuccessful = false;
      emailStatus.message = "Failed to load MAPI32.DLL";
    }

    // Clean up
    delete[] pwstrRecipient;
    delete[] pwstrAttachmentPath;
    delete[] pwstrSubject;
    delete[] pwstrBody;

    return emailStatus;
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

        EmailStatus emailStatus = OpenEmailWithAttachment(recipient, subject, body, attachmentPath);

        flutter::EncodableValue encodableStatus = emailStatus.toEncodableValue();
        result->Success(encodableStatus);
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
