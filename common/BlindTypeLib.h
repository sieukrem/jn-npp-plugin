#pragma once
#include <OAIdl.h>
#include "combase.h"


class BlindTypeLib : public CComBase<ITypeLib>{
	// Inherited via CComBase
	virtual UINT __stdcall GetTypeInfoCount(void) override;
	virtual HRESULT __stdcall GetTypeInfo(UINT index, ITypeInfo** ppTInfo) override;
	virtual HRESULT __stdcall GetTypeInfoType(UINT index, TYPEKIND* pTKind) override;
	virtual HRESULT __stdcall GetTypeInfoOfGuid(REFGUID guid, ITypeInfo** ppTinfo) override;
	virtual HRESULT __stdcall GetLibAttr(TLIBATTR** ppTLibAttr) override;
	virtual HRESULT __stdcall GetTypeComp(ITypeComp** ppTComp) override;
	virtual HRESULT __stdcall GetDocumentation(INT index, BSTR* pBstrName, BSTR* pBstrDocString, DWORD* pdwHelpContext, BSTR* pBstrHelpFile) override;
	virtual HRESULT __stdcall IsName(LPOLESTR szNameBuf, ULONG lHashVal, BOOL* pfName) override;
	virtual HRESULT __stdcall FindName(LPOLESTR szNameBuf, ULONG lHashVal, ITypeInfo** ppTInfo, MEMBERID* rgMemId, USHORT* pcFound) override;
	virtual void __stdcall ReleaseTLibAttr(TLIBATTR* pTLibAttr) override;
};