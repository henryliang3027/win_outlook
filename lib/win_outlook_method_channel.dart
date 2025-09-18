import 'dart:io';

import 'package:flutter/foundation.dart';
import 'package:flutter/services.dart';
import 'package:win_outlook/email_status.dart';

import 'win_outlook_platform_interface.dart';

/// An implementation of [WinOutlookPlatform] that uses method channels.
class MethodChannelWinOutlook extends WinOutlookPlatform {
  /// The method channel used to interact with the native platform.
  @visibleForTesting
  final methodChannel = const MethodChannel('win_outlook');

  @override
  Future<EmailStatus> openEmail({
    required String subject,
    required String body,
    required String attachmentPath,
    String recipient = '',
  }) async {
    try {
      final Map<dynamic, dynamic>? result =
          await methodChannel.invokeMethod<Map<dynamic, dynamic>>('openEmail', {
        'recipient': recipient,
        'subject': subject,
        'body': body,
        'attachmentPath': attachmentPath,
      });

      if (result != null) {
        return EmailStatus.fromMap(result);
      } else {
        return EmailStatus(
          isSuccessful: false,
          message: "invoked openEmail with no response",
        );
      }
    } catch (e) {
      // Handle PlatformException specifically and other exceptions
      if (e is PlatformException) {
        return EmailStatus(
          isSuccessful: false,
          message: e.message ?? "Platform exception occurred",
        );
      } else {
        // Handle unexpected exceptions
        return EmailStatus(
          isSuccessful: false,
          message: "An unexpected error occurred: $e",
        );
      }
    }
  }
}
