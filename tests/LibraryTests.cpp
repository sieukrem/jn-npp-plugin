
#include "gtest/gtest.h"

#include "windows.h"

#include "../common/Library.h"
#include "../common/CallFrame.h"

#ifdef _M_X64
#define isx64 1
#else
#define isx64 0
#endif

TEST(LibraryTests, Call_winapi_from_library_works) {
  BSTR module = SysAllocString(TEXT("Kernel32.dll"));

  Library library(&module);
  CallFrame callFrame;
  
  VARIANT value;
  value.llVal = 0;
  value.vt = isx64? VT_I8 : VT_I4;
  
  if(isx64)
    callFrame.pushLWORD(value);
  else
    callFrame.pushDWORD(value);

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