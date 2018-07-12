#include <Windows.h>
#include <stdlib.h>

// UTF16をSJISに変換
char* utf16_to_sjis(const wchar_t* utf16) {
	// ShiftJISへ変換後の文字列長を得る
	const int length = WideCharToMultiByte(CP_ACP, 0, utf16, wcslen(utf16), NULL, 0, NULL, NULL);
	// 必要な分だけShiftJIS文字列のバッファを確保
	char* result = malloc(length + 1);
	if (result != NULL) {
		result[length] = '\0';
		// UnicodeからShiftJISへ変換
		WideCharToMultiByte(CP_ACP, 0, utf16, wcslen(utf16), result, length, NULL, NULL);
	}
	return result;
}

// SJISをUTF16に変換
wchar_t* sjis_to_utf16(const char* sjis) {
	// Shift-JIS文字列からUTF-16に変換したときの文字列長を求める
	const int length = MultiByteToWideChar(CP_ACP, 0, sjis, strlen(sjis), NULL, 0);
	// UTF-16文字列の領域を確保する。
	wchar_t* result = malloc(length * 2 + 2);
	if (result != NULL) {
		// \0を代入
		result[length] = '\0';
		// Shift-JIS文字列からUTF-16に変換する。
		MultiByteToWideChar(CP_ACP, 0, sjis, strlen(sjis), result, length);
	}
	return result;
}
