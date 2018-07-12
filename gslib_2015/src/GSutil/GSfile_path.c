#include "GSfile_path.h"
#include "GSstring_converter.h"
#include <stdlib.h>
#include <string.h>

// ファイルパスの分解
void gsSplitPath(const char* path, char* drive, char* dir, char* fname, char* ext) {
	// SJISでのsplitpathが全角文字の一部で動作しないためUTF16で分解する
	wchar_t w_drive[_MAX_DRIVE];
	wchar_t w_dir[_MAX_DIR];
	wchar_t w_fname[_MAX_FNAME];
	wchar_t w_ext[_MAX_EXT];
	wchar_t* wchar_path = sjis_to_utf16(path);
	_wsplitpath_s(wchar_path, w_drive, _MAX_DRIVE, w_dir, _MAX_DIR, w_fname, _MAX_FNAME, w_ext, _MAX_EXT);
	free(wchar_path);
	if (drive != NULL) {
		char* mb_drive = utf16_to_sjis(w_drive);
		strcpy(drive, mb_drive);
		free(mb_drive);
	}
	if (dir != NULL) {
		char* mb_dir = utf16_to_sjis(w_dir);
		strcpy(dir, mb_dir);
		free(mb_dir);
	}
	if (fname != NULL) {
		char* mb_fname = utf16_to_sjis(w_fname);
		strcpy(fname, mb_fname);
		free(mb_fname);
	}
	if (ext != NULL) {
		char* mb_ext = utf16_to_sjis(w_ext);
		strcpy(ext, mb_ext);
		free(mb_ext);
	}
}

// ファイルパスの組み立て
void gsMakePath(char* path, const char* drive, const char* dir, const char* fname, const char* ext) {
	// makepathはそのまま使用
	_makepath(path, drive, dir, fname, ext);
}

