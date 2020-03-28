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
#pragma once

#include <vector>

#include "..\Interfaces.h"

#include "combase.h"
#include "windows.h"
#include <dispex.h>

#include "CallBack.h"

class CallFrame:  public CComDispatch<ICallFrame>{
private:
    std::vector<uint8_t> m_Buffer;

    template<typename T>
	HRESULT write( VARIANT value){
		T val;

		switch(value.vt){
			case VT_BOOL:	val = (T)*(unsigned short*)&value.boolVal; break;
			case VT_UI1:	val = (T)*(unsigned char*)&value.bVal; break;
			case VT_I1:		val = (T)*(unsigned char*)&value.cVal; break;
			case VT_I2:     val = (T)*(unsigned short*)&value.iVal; break;
			case VT_I4:     val = (T)*(unsigned long*)&value.lVal; break;
			case VT_I8:     val = (T)*(unsigned long long*)&value.llVal; break;
			case VT_INT:    val = (T)*(unsigned int*)&value.intVal;  break; 
			case VT_R4:     val = (T)*(unsigned int*)&value.fltVal;  break; 
			case VT_R8:     val = (T)*(unsigned long long*)&value.dblVal;  break; 
			case VT_BSTR:   val = (T)*(unsigned __int32*)&value.bstrVal;  break; 
			case VT_DISPATCH:{
				#pragma warning( push )
#pragma warning(disable:4311 4302)
				void* pint = NULL; 
				value.pdispVal->QueryInterface(__uuidof(ICallBack), &pint);
				if (pint!=NULL){
					val = (T)(static_cast<CallBack*>(value.pdispVal))->Proc();
					value.pdispVal->Release();
				}else
					val = (T)value.pdispVal;
				#pragma warning( pop )
			}
		}

		m_Buffer.insert(m_Buffer.end(), (uint8_t*)&val, (uint8_t*)&val+sizeof(T));

		return S_OK;
	}

public:
	CallFrame();
	~CallFrame(void);

    HRESULT STDMETHODCALLTYPE pushByte(VARIANT value);
        
    HRESULT STDMETHODCALLTYPE pushWORD( 
        VARIANT value);
        
    HRESULT STDMETHODCALLTYPE pushDWORD( 
        VARIANT value);
        
    HRESULT STDMETHODCALLTYPE pushLWORD( 
        VARIANT value);
        
    HRESULT STDMETHODCALLTYPE pushPtr( 
        VARIANT value);
};
