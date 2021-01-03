
#include "gtest/gtest.h"

#include "windows.h"

#include "../common/Library.h"
#include "../common/CallFrame.h"

TEST(LibraryTests, Call_winapi_from_library_works) {
  BSTR module = SysAllocString(TEXT("Kernel32.dll"));

  Library library(&module);
  CallFrame callFrame;
  
  VARIANT value;
  value.llVal = 0;
  value.vt = VT_I8;
  
  callFrame.pushLWORD(value);

  wchar_t buffer[1024];

  value.bstrVal = buffer;
  value.vt = VT_BSTR;
  callFrame.pushPtr(value);
  

  value.lVal = sizeof(buffer);
  value.vt = VT_I4;
  callFrame.pushDWORD(value);

  size_t res = 0;
  library.callWithFrame(SysAllocString(L"GetModuleFileNameW"), &callFrame, &res);

  EXPECT_NE(0, res);

}