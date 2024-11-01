import 'package:win_outlook/email_status.dart';

import 'win_outlook_platform_interface.dart';

class WinOutlook {
  Future<String?> getPlatformVersion() {
    return WinOutlookPlatform.instance.getPlatformVersion();
  }

  Future<EmailStatus> openEmail({
    required String recipient,
    required String subject,
    required String body,
    required String attachmentPath,
  }) async {
    return WinOutlookPlatform.instance.openEmail(
      recipient: recipient,
      subject: subject,
      body: body,
      attachmentPath: attachmentPath,
    );
  }
}
