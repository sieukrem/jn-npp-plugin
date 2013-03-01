/*
This file is part of jN, a plugin for Notepad++
Copyright (C)2013 Eugen Kremer <eugen DOT kremer AT gmail DOT com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "Library.h"
#include "Windows.h"
#include "MyActiveSite.h"
#include "..\Interfaces.h"

#include "LastError.h"

#include "System.h"

Library::Library(BSTR* path):CComDispatch()
{
	m_Handle = LoadLibrary(*path);

	AddRef();
}

HMODULE Library::handle(){
	return m_Handle;
}

Library::~Library()
{
	if (m_Handle)
		FreeLibrary(m_Handle);
}

HRESULT STDMETHODCALLTYPE Library::alloc(unsigned int number, BSTR* result){
	*result = SysAllocStringLen(NULL, number);
	return S_OK;
}

FARPROC Library::GetProc(BSTR functionName) const {
	char* _functionName;

	int newlen = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)functionName, SysStringLen(functionName), NULL,0, NULL, NULL);
	_functionName = new char[newlen+1];
	int res = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)functionName, SysStringLen(functionName), _functionName, newlen+1, NULL,NULL);
	_functionName[res] = 0;

	FARPROC proc = GetProcAddress(m_Handle, (LPCSTR)_functionName);

	return proc;
}

HRESULT STDMETHODCALLTYPE Library::call(BSTR functionName, BSTR params, int* result){
#ifndef _M_X64
	FARPROC proc = GetProc(functionName);

	if (proc == NULL){
		LastError errMsg;
		MyActiveSite::getInstance()->Throw(errMsg, __uuidof(ILibrary) );		
		
		return E_FAIL;
	}



	int paramlen = SysStringByteLen(params);
	
	/*
		copy params to stack
	*/

	char* buf;		// pointer to the buffer on stack
	void* v_esp;	// old stack pointer
	DWORD v_res;

	// reserve memory on stack
	__asm {
		mov v_esp, esp
		mov eax, paramlen
		sub esp, eax
		mov buf, esp
	}

	memcpy(buf, params, paramlen);

	/*
		make a call to the function
	*/

    __asm {
        call    proc
		mov v_res, eax
        mov eax, paramlen
        mov esp, v_esp
    }
 
	*result = v_res;
#else
#pragma message(": warning: Library::call not available")
#endif

	return S_OK;
}

template<typename T>
HRESULT write(BSTR dest,unsigned int pos,VARIANT value){
	T* val = (T*)(((char*)dest)+pos);

	switch(value.vt){
		case VT_BOOL:	*val = (T)*(unsigned short*)&value.boolVal; break;
		case VT_UI1:	*val = (T)*(unsigned char*)&value.bVal; break;
		case VT_I1:		*val = (T)*(unsigned char*)&value.cVal; break;
		case VT_I2:     *val = (T)*(unsigned short*)&value.iVal; break;
		case VT_I4:     *val = (T)*(unsigned long*)&value.lVal; break;
		case VT_I8:     *val = (T)*(unsigned long long*)&value.llVal; break;
		case VT_INT:    *val = (T)*(unsigned int*)&value.intVal;  break; 
		case VT_R4:     *val = (T)*(unsigned int*)&value.fltVal;  break; 
		case VT_R8:     *val = (T)*(unsigned long long*)&value.dblVal;  break; 
		case VT_BSTR:   *val = (T)*(unsigned __int32*)&value.bstrVal;  break; 
		case VT_DISPATCH:{
			void* pint = NULL; 
			value.pdispVal->QueryInterface(__uuidof(ICallBack), &pint);
			if (pint!=NULL){
				*val = (T)(static_cast<CallBack*>(value.pdispVal))->Proc();
				value.pdispVal->Release();
			}else
				*val = (T)value.pdispVal;
		}
	}

	return S_OK;
}

HRESULT STDMETHODCALLTYPE Library::writeByte(BSTR dest,unsigned int pos,VARIANT value){
	return write<unsigned __int8>(dest, pos, value);
}

HRESULT STDMETHODCALLTYPE Library::writeWord(BSTR dest,unsigned int pos,VARIANT value){
	return write<unsigned __int16>(dest, pos, value);
}

HRESULT STDMETHODCALLTYPE Library::writeDWord(BSTR dest,unsigned int pos,VARIANT value){
	return write<unsigned __int32>(dest, pos, value);
}

HRESULT STDMETHODCALLTYPE Library::writeInt64(BSTR dest,unsigned int pos,VARIANT value){
	return write<unsigned __int64>(dest, pos, value);
}

bool HasEnough(BSTR src,unsigned int srcPos,unsigned int size){
	int actualSize = SysStringByteLen(src);

	bool result = ((actualSize - srcPos) - size) >= 0;

	return result;
}

HRESULT STDMETHODCALLTYPE Library::copy( BSTR dest,unsigned int destPos,BSTR src,unsigned int srcPos,unsigned int size){
	if (!HasEnough(dest, destPos, size)){
		MyActiveSite::getInstance()->Throw(TEXT("Not enough bytes in destination array"), __uuidof(ILibrary) );	
		return E_FAIL;
	}

	if (!HasEnough(src, srcPos, size)){
		MyActiveSite::getInstance()->Throw(TEXT("Not enough bytes in source array"), __uuidof(ILibrary) );	
		return E_FAIL;
	}
	
	memcpy(((char*)dest) + destPos, ((char*)src) + srcPos, size); 

	return S_OK;
}

HRESULT STDMETHODCALLTYPE Library::readByte( BSTR src,unsigned int pos, VARIANT *value){
	if (!HasEnough(src, pos, 1)){
		MyActiveSite::getInstance()->Throw(TEXT("Not enough bytes in source array"), __uuidof(ILibrary) );	
		return E_FAIL;
	}

	value->vt    = VT_UI1;
	value->bVal  = *((BYTE *)(((char*)src)+pos));
	
	return S_OK;
}

HRESULT STDMETHODCALLTYPE Library::readWord( BSTR src,unsigned int pos, VARIANT *value){
	if (!HasEnough(src, pos, 2)){
		MyActiveSite::getInstance()->Throw(TEXT("Not enough bytes in source array"), __uuidof(ILibrary) );	
		return E_FAIL;
	}

	value->vt    = VT_UI2;
	value->uiVal = *((USHORT *)(((char*)src)+pos));
	
	return S_OK;
}

HRESULT STDMETHODCALLTYPE Library::readDWord( BSTR src,unsigned int pos,VARIANT *value){
	if (!HasEnough(src, pos, 4)){
		MyActiveSite::getInstance()->Throw(TEXT("Not enough bytes in source array"), __uuidof(ILibrary) );	
		return E_FAIL;
	}

	value->vt    = VT_UI4;
	value->ulVal = *((ULONG *)(((char*)src)+pos));
	
	return S_OK;
}

HRESULT STDMETHODCALLTYPE Library::readInt64( BSTR src,unsigned int pos,VARIANT *value){
	if (!HasEnough(src, pos, 8)){
		MyActiveSite::getInstance()->Throw(TEXT("Not enough bytes in source array"), __uuidof(ILibrary) );	
		return E_FAIL;
	}

	value->vt    = VT_UI8;
	value->ullVal = *((ULONGLONG *)(((char*)src)+pos));
	
	return S_OK;
}

HRESULT STDMETHODCALLTYPE Library::readBSTR( BSTR src,unsigned int pos, BSTR* value){
	if (!HasEnough(src, pos, 4)){
		MyActiveSite::getInstance()->Throw(TEXT("Not enough bytes in source array"), __uuidof(ILibrary) );	
		return E_FAIL;
	}

	*value = *((BSTR*)(((char*)src)+pos));
	
	return S_OK;
}

