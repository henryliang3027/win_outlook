import 'package:flutter/material.dart';
import 'dart:async';

import 'package:flutter/services.dart';
import 'package:win_outlook/email_status.dart';
import 'package:win_outlook/win_outlook.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatefulWidget {
  const MyApp({super.key});

  @override
  State<MyApp> createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
  String _platformVersion = 'Unknown';
  final _winOutlookPlugin = WinOutlook();

  @override
  void initState() {
    super.initState();
    initPlatformState();
  }

  // Platform messages are asynchronous, so we initialize in an async method.
  Future<void> initPlatformState() async {
    String platformVersion;
    // Platform messages may fail, so we use a try/catch PlatformException.
    // We also handle the message potentially returning null.
    try {
      platformVersion = await _winOutlookPlugin.getPlatformVersion() ??
          'Unknown platform version';
    } on PlatformException {
      platformVersion = 'Failed to get platform version.';
    }

    // If the widget was removed from the tree while the asynchronous platform
    // message was in flight, we want to discard the reply rather than calling
    // setState to update our non-existent appearance.
    if (!mounted) return;

    setState(() {
      _platformVersion = platformVersion;
    });
  }

  @override
  Widget build(BuildContext context) {
    void sendEmail() async {
      final EmailStatus result = await _winOutlookPlugin.openEmail(
        recipient: '',
        subject: 'Hello from Flutter',
        body: 'Please find the attachment請開郵件ㄒ哈哈.',
        attachmentPath:
            'C:/Users/henry.liang/Documents/ACI+/QR_Code_202你為甚麼要這樣.png',
      );

      print('${result.isSuccessful}, ${result.message}');
    }

    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: const Text('Plugin example app'),
        ),
        body: Column(
          children: [
            Center(
              child: Text('Running on: $_platformVersion\n'),
            ),
            ElevatedButton(
              onPressed: sendEmail,
              child: Text('Send Email'),
            ),
          ],
        ),
      ),
    );
  }
}
