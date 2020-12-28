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
public:
	uint64_t m_Registers[4+4];
	
	uint64_t* m_DoubleFloatRegister;
	uint64_t* m_IntegerRegister;

    std::vector<uint8_t> m_Buffer;

private:
	int m_Position = 0;
	const int c_MaxPosition = (sizeof(m_Registers) / sizeof(m_Registers[0]) / 2) - 1;
  
    template<typename T>
	void insert(T val){
		if (m_Position > c_MaxPosition){
			m_Buffer.insert(m_Buffer.end(), (uint8_t*)&val, (uint8_t*)&val+sizeof(T));
		}else{
			insertRegister(val);
			m_Position++;
		}
	}

	template<typename T>
	void insertRegister(T val){
		m_IntegerRegister[m_Position] = (uint64_t)(UINT64_MAX & val);
	}

	template<>
	void insertRegister(double val){
		m_DoubleFloatRegister[m_Position] = (uint64_t)val;
	}

	template<>
	void insertRegister(float val){
		m_DoubleFloatRegister[m_Position] = (uint64_t)val;
	}

    template<typename T>
	HRESULT write(VARIANT value){
		switch(value.vt){
			case VT_BOOL:	insert<T>((T)*(unsigned short*)&value.boolVal); break;
			case VT_UI1:	insert<T>((T)*(unsigned char*)&value.bVal); break;
			case VT_I1:		insert<T>((T)*(unsigned char*)&value.cVal); break;
			case VT_I2:     insert<T>((T)*(unsigned short*)&value.iVal); break;
			case VT_I4:     insert<T>((T)*(unsigned long*)&value.lVal); break;
			case VT_I8:     insert<T>((T)*(unsigned long long*)&value.llVal); break;
			case VT_INT:    insert<T>((T)*(unsigned int*)&value.intVal);  break; 
			case VT_R4:     insert<T>((T)*(uint32_t*)&value.fltVal);  break; 
			case VT_R8:     insert<T>((T)*(uint64_t*)&value.dblVal);  break; 
			case VT_BSTR:  
			case VT_DISPATCH:
				return E_FAIL;
		}
		
		return S_OK;
	}
	
	template<typename T>
	HRESULT writePtr( VARIANT value){
		switch(value.vt){
			case VT_BOOL:
			case VT_UI1:
			case VT_I1:	
			case VT_I2:  
			case VT_I4:  
			case VT_I8:  
			case VT_INT: 
			case VT_R4:  
			case VT_R8:  
				return  E_FAIL;

			case VT_BSTR: insert<T>((T)value.bstrVal);  break; 
			case VT_DISPATCH:{
				#pragma warning( push )
				#pragma warning(disable:4311 4302)
				void* pint = NULL; 
				value.pdispVal->QueryInterface(__uuidof(ICallBack), &pint);
				if (pint!=NULL){
					insert<T>((T)(static_cast<CallBack*>(value.pdispVal))->Proc());
					value.pdispVal->Release();
				}else
					insert<T>((T)value.pdispVal);
				#pragma warning( pop )
				}
		}

		return S_OK;
	}

public:
	CallFrame();
	~CallFrame(void);

	size_t Call(FARPROC fn);

    HRESULT STDMETHODCALLTYPE pushByte(VARIANT value);
        
    HRESULT STDMETHODCALLTYPE pushWORD( 
        VARIANT value);
        
    HRESULT STDMETHODCALLTYPE pushDWORD( 
        VARIANT value);
        
    HRESULT STDMETHODCALLTYPE pushLWORD( 
        VARIANT value);
        
    HRESULT STDMETHODCALLTYPE pushPtr( 
        VARIANT value);

    HRESULT STDMETHODCALLTYPE pushFloat( 
        VARIANT value);

    HRESULT STDMETHODCALLTYPE pushDouble( 
		VARIANT value);
};
