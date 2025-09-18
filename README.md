# Win Outlook Flutter Plugin

這個套件專為 Windows 平台而設計，使用 C++ 調用 Windows MAPI 來開啟 Microsoft Outlook 並創建帶有附件的郵件。

## 快速開始

### 1. 安裝套件

在您的 Flutter 專案中的 `pubspec.yaml` 檔案中添加：

方法一：使用 Git Repository

```yaml
win_outlook:
  git:
    url: https://github.com/your-repo/win_outlook.git # git repository 路徑
    ref: main
```

方法二：使用本地端路徑

```yaml
win_outlook:
  path: /path/to/win_outlook # 本地端路徑
```

然後執行：

```bash
flutter pub get
```

### 2. 基本使用示例

```dart
import 'package:win_outlook/win_outlook.dart';
import 'package:win_outlook/email_status.dart';

// 創建 WinOutlook 實例
final winOutlook = WinOutlook();

// 開啟 Outlook 並創建郵件
Future<void> sendEmail() async {
  EmailStatus result = await winOutlook.openEmail(
    recipient: 'example@email.com',
    subject: 'Hello from Flutter',
    body: '這是從 Flutter 應用程式發送的郵件內容',
    attachmentPath: 'C:/path/to/your/attachment.pdf',
  );

  if (result.isSuccessful) {
    print('郵件視窗已成功開啟');
  } else {
    print('開啟郵件視窗失敗: ${result.message}');
  }
}
```

完整使用範例請參考：`example/lib/main.dart`

## API 函數說明

### WinOutlook 類別

#### 實例方法

**`Future<EmailStatus> openEmail({required String recipient, required String subject, required String body, required String attachmentPath})`**

- **功能**：開啟 Microsoft Outlook 並創建一個新的郵件視窗，包含指定的收件人、主旨、內容和附件
- **參數**：
  - `recipient` (String, required) - 收件人郵箱地址，可為空字串
  - `subject` (String, required) - 郵件主旨
  - `body` (String, required) - 郵件內容
  - `attachmentPath` (String, required) - 附件檔案的完整路徑
- **回傳**：`EmailStatus` 物件，包含操作結果

### EmailStatus 類別

**屬性**：

- `isSuccessful` (bool) - 操作是否成功
- `message` (String) - 操作結果訊息

## 平台支援

目前僅支援 Windows 平台：

- **Windows 10** 及以上版本
