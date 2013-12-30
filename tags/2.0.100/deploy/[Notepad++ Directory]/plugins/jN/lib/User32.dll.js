require("lib/Library.js");

User32 = new Library("User32.dll");

User32.Define("SendMessageW", "HWND", "UINT", "WPARAM", "LPARAM");
User32.Define("MessageBoxW", "HWND", "LPCTSTR", "LPCTSTR", "UINT");
User32.Define("SetParent", "HWND", "HWND");
User32.Define("GetWindowLongW", "HWND", "int");
User32.Define("SetWindowLongW", "HWND", "int", "LONG_PTR");
User32.Define("SetWindowPos", "HWND", "HWND", "int","int","int","int","UINT");
