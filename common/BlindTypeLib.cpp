#include "BlindTypeLib.h"

UINT __stdcall BlindTypeLib::GetTypeInfoCount(void)
{
	return 0;
}

HRESULT __stdcall BlindTypeLib::GetTypeInfo(UINT index, ITypeInfo** ppTInfo)
{
	return E_NOTIMPL;
}

HRESULT __stdcall BlindTypeLib::GetTypeInfoType(UINT index, TYPEKIND* pTKind)
{
	return E_NOTIMPL;
}

HRESULT __stdcall BlindTypeLib::GetTypeInfoOfGuid(REFGUID guid, ITypeInfo** ppTinfo)
{
	return E_NOTIMPL;
}

HRESULT __stdcall BlindTypeLib::GetLibAttr(TLIBATTR** ppTLibAttr)
{
	return E_NOTIMPL;
}

HRESULT __stdcall BlindTypeLib::GetTypeComp(ITypeComp** ppTComp)
{
	return E_NOTIMPL;
}

HRESULT __stdcall BlindTypeLib::GetDocumentation(INT index, BSTR* pBstrName, BSTR* pBstrDocString, DWORD* pdwHelpContext, BSTR* pBstrHelpFile)
{
	return E_NOTIMPL;
}

HRESULT __stdcall BlindTypeLib::IsName(LPOLESTR szNameBuf, ULONG lHashVal, BOOL* pfName)
{
	return E_NOTIMPL;
}

HRESULT __stdcall BlindTypeLib::FindName(LPOLESTR szNameBuf, ULONG lHashVal, ITypeInfo** ppTInfo, MEMBERID* rgMemId, USHORT* pcFound)
{
	return E_NOTIMPL;
}

void __stdcall BlindTypeLib::ReleaseTLibAttr(TLIBATTR* pTLibAttr)
{
}
