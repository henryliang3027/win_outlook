import 'package:flutter_test/flutter_test.dart';
import 'package:win_outlook/email_status.dart';
import 'package:win_outlook/win_outlook.dart';
import 'package:win_outlook/win_outlook_platform_interface.dart';
import 'package:win_outlook/win_outlook_method_channel.dart';
import 'package:plugin_platform_interface/plugin_platform_interface.dart';

class MockWinOutlookPlatform
    with MockPlatformInterfaceMixin
    implements WinOutlookPlatform {
  @override
  Future<EmailStatus> openEmail(
      {required String subject,
      required String body,
      required String attachmentPath,
      String recipient = ''}) {
    // TODO: implement openEmail
    throw UnimplementedError();
  }
}

void main() {
  final WinOutlookPlatform initialPlatform = WinOutlookPlatform.instance;

  test('$MethodChannelWinOutlook is the default instance', () {
    expect(initialPlatform, isInstanceOf<MethodChannelWinOutlook>());
  });

  test('getPlatformVersion', () async {
    WinOutlook winOutlookPlugin = WinOutlook();
    MockWinOutlookPlatform fakePlatform = MockWinOutlookPlatform();
    WinOutlookPlatform.instance = fakePlatform;
  });
}
