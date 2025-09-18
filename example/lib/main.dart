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
  final _winOutlookPlugin = WinOutlook();

  @override
  void initState() {
    super.initState();
  }

  @override
  Widget build(BuildContext context) {
    void sendEmail() async {
      final EmailStatus result = await _winOutlookPlugin.openEmail(
        recipient: '',
        subject: 'Hello from Flutter',
        body: 'Please find the attachment 請參考附件',
        attachmentPath:
            'C:/Users/henry.liang/Documents/ACI+/QR_Code_20241101225836.png',
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
