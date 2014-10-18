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

#include "oleauto.h"
#include <strsafe.h>

class SysStr{
private:
	BSTR m_Str;
	void Free(){
		if (m_Str){
			SysFreeString(m_Str);
			m_Str = NULL;
		}
	}

public:
	BSTR Clone(){
		if (!m_Str)
			return NULL;

		return SysAllocString(m_Str);
	}
	
	int Length(){
		if (!m_Str)
			return -1;

		return SysStringLen(m_Str);
	}

	SysStr& Append(BSTR value){
		if (!m_Str){
			Set(value);
			return *this;
		}

		UINT len = SysStringLen(value);
		UINT newLen = len + Length();

		SysReAllocStringLen(&m_Str, m_Str, newLen);

		StringCchCat(m_Str, newLen+1, value);
		
		return *this;
	}

	void Set(const TCHAR* value){
		Free();

		if (value)
			m_Str = SysAllocString(value);
	}

	SysStr(const TCHAR* value){
		m_Str = NULL;
		
		Set(value);
	}

	~SysStr(){
		Free();
	}

	operator const BSTR(){
		return m_Str;
	}
};