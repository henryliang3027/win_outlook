import 'package:plugin_platform_interface/plugin_platform_interface.dart';
import 'package:win_outlook/email_status.dart';

import 'win_outlook_method_channel.dart';

abstract class WinOutlookPlatform extends PlatformInterface {
  /// Constructs a WinOutlookPlatform.
  WinOutlookPlatform() : super(token: _token);

  static final Object _token = Object();

  static WinOutlookPlatform _instance = MethodChannelWinOutlook();

  /// The default instance of [WinOutlookPlatform] to use.
  ///
  /// Defaults to [MethodChannelWinOutlook].
  static WinOutlookPlatform get instance => _instance;

  /// Platform-specific implementations should set this with their own
  /// platform-specific class that extends [WinOutlookPlatform] when
  /// they register themselves.
  static set instance(WinOutlookPlatform instance) {
    PlatformInterface.verifyToken(instance, _token);
    _instance = instance;
  }

  Future<EmailStatus> openEmail({
    required String subject,
    required String body,
    required String attachmentPath,
    String recipient = '',
  }) {
    throw UnimplementedError('openEmail() has not been implemented.');
  }
}
