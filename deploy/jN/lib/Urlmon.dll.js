require("Library.js");

Urlmon = new Library("urlmon.dll");

Urlmon.Define("URLDownloadToFileW", "DWORD", "LPCTSTR", "LPCTSTR", "DWORD", "DWORD");

/*
HRESULT URLDownloadToFile(
             LPUNKNOWN            pCaller,
             LPCTSTR              szURL,
             LPCTSTR              szFileName,
  _Reserved_ DWORD                dwReserved,
             LPBINDSTATUSCALLBACK lpfnCB
);


Urlmon.URLDownloadToFileW(0,"https://assets.pixolum.com/blog/wp-content/uploads/2019/09/Blumen-Fotografieren-50mm-800x533.webp","d:/temp/1.webp",0,0);
*/