import 'package:flutter/foundation.dart';
import 'package:flutter/services.dart';

import 'win_outlook_platform_interface.dart';

/// An implementation of [WinOutlookPlatform] that uses method channels.
class MethodChannelWinOutlook extends WinOutlookPlatform {
  /// The method channel used to interact with the native platform.
  @visibleForTesting
  final methodChannel = const MethodChannel('win_outlook');

  @override
  Future<String?> getPlatformVersion() async {
    final version =
        await methodChannel.invokeMethod<String>('getPlatformVersion');
    return version;
  }

  @override
  Future<String?> openEmail({
    required String subject,
    required String body,
    required String attachmentPath,
    String recipient = '',
  }) async {
    final result = await methodChannel.invokeMethod<String>('openEmail', {
      'recipient': recipient,
      'subject': subject,
      'body': body,
      'attachmentPath': attachmentPath,
    });
    return result;
  }
}
