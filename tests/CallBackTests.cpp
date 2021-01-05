#include "gtest/gtest.h"

#include "windows.h"

#include "../common/CallBack.h"

namespace CallBackTests{

class TestCfg : public IDispatchEx{
public:
    BSTR Stack = NULL;
    size_t Result = 0;
    TestCfg(size_t result = 0){
        Result = result;
    }
    ~TestCfg(){
        if (Stack)
            SysFreeString(Stack);
    }
    // Inherited via IDispatchEx
    virtual HRESULT __stdcall QueryInterface(REFIID riid, void** ppvObject) override
    {
        return E_NOTIMPL;
    }
    virtual ULONG __stdcall AddRef(void) override
    {
        return 0;
    }
    virtual ULONG __stdcall Release(void) override
    {
        return 0;
    }
    virtual HRESULT __stdcall GetTypeInfoCount(UINT* pctinfo) override
    {
        return E_NOTIMPL;
    }
    virtual HRESULT __stdcall GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo** ppTInfo) override
    {
        return E_NOTIMPL;
    }
    virtual HRESULT __stdcall GetIDsOfNames(REFIID riid, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId) override
    {
        return E_NOTIMPL;
    }
    virtual HRESULT __stdcall Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr) override
    {
        return E_NOTIMPL;
    }
    virtual HRESULT __stdcall GetDispID(BSTR bstrName, DWORD grfdex, DISPID* pid) override
    {
        *pid = 1;
        return S_OK;
    }
    virtual HRESULT __stdcall InvokeEx(DISPID id, LCID lcid, WORD wFlags, DISPPARAMS* pdp, VARIANT* pvarRes, EXCEPINFO* pei, IServiceProvider* pspCaller) override
    {
        Stack = pdp->rgvarg[1].bstrVal;

       
        pvarRes->vt = VT_UI4;
        pvarRes->lVal = Result;
       
        return S_OK;
    }
    virtual HRESULT __stdcall DeleteMemberByName(BSTR bstrName, DWORD grfdex) override
    {
        return E_NOTIMPL;
    }
    virtual HRESULT __stdcall DeleteMemberByDispID(DISPID id) override
    {
        return E_NOTIMPL;
    }
    virtual HRESULT __stdcall GetMemberProperties(DISPID id, DWORD grfdexFetch, DWORD* pgrfdex) override
    {
        return E_NOTIMPL;
    }
    virtual HRESULT __stdcall GetMemberName(DISPID id, BSTR* pbstrName) override
    {
        return E_NOTIMPL;
    }
    virtual HRESULT __stdcall GetNextDispID(DWORD grfdex, DISPID id, DISPID* pid) override
    {
        return E_NOTIMPL;
    }
    virtual HRESULT __stdcall GetNameSpaceParent(IUnknown** ppunk) override
    {
        return E_NOTIMPL;
    }
};


TEST(CallBackTests, Without_parameter_has_stack_len_0) {
  TestCfg cfg;

  LocRef<CallBack> callBack = CallBack::RegisterCallBack(&cfg, 0);
  
  callBack->Proc()();

  EXPECT_EQ(SysStringByteLen(cfg.Stack), 0);
}

typedef unsigned int (CALLBACK *fn)(int i, int k);

TEST(CallBackTests, With_parameter_has_corresponding_stack_len_and_result) {
  TestCfg cfg(543);

  LocRef<CallBack> callBack = CallBack::RegisterCallBack(&cfg, 2*(sizeof(size_t)));

  auto result = ((fn)(callBack->Proc()))(123, 456);

  EXPECT_EQ(SysStringByteLen(cfg.Stack), 2*(sizeof(size_t)));
  EXPECT_EQ(result, cfg.Result);
  
  auto stack = (size_t*)cfg.Stack;

  EXPECT_EQ(*(int*)stack, 123);
  EXPECT_EQ(*(int*)(stack+1), 456);
}
}