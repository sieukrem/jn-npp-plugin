require("lib/Library.js");

Kernel32 = new Library("Kernel32.dll");


/*
int WideCharToMultiByte(
  _In_       UINT CodePage,
  _In_       DWORD dwFlags,
  _In_       LPCWSTR lpWideCharStr,
  _In_       int cchWideChar,
  _Out_opt_  LPSTR lpMultiByteStr,
  _In_       int cbMultiByte,
  _In_opt_   LPCSTR lpDefaultChar,
  _Out_opt_  LPBOOL lpUsedDefaultChar
);
*/
Kernel32.Define("WideCharToMultiByte", "UINT", "DWORD", "LPCWSTR", "int", "LPSTR", "int", "LPCSTR", "LPBOOL");
