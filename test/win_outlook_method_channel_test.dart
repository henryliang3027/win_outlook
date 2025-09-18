import 'package:flutter/services.dart';
import 'package:flutter_test/flutter_test.dart';
import 'package:win_outlook/win_outlook_method_channel.dart';

void main() {
  TestWidgetsFlutterBinding.ensureInitialized();

  MethodChannelWinOutlook platform = MethodChannelWinOutlook();
  const MethodChannel channel = MethodChannel('win_outlook');

  setUp(() {
    TestDefaultBinaryMessengerBinding.instance.defaultBinaryMessenger
        .setMockMethodCallHandler(
      channel,
      (MethodCall methodCall) async {
        return '';
      },
    );
  });

  tearDown(() {
    TestDefaultBinaryMessengerBinding.instance.defaultBinaryMessenger
        .setMockMethodCallHandler(channel, null);
  });
}
