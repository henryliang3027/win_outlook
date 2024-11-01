class EmailStatus {
  final bool isSuccessful;
  final String message;

  EmailStatus({required this.isSuccessful, required this.message});

  // Factory constructor to create EmailStatus from a map
  factory EmailStatus.fromMap(Map<dynamic, dynamic> map) {
    return EmailStatus(
      isSuccessful: map['isSuccessful'] as bool,
      message: map['message'] as String,
    );
  }
}
