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

#include "..\Interfaces.h"

#include "combase.h"
#include "windows.h"
#include <dispex.h>

class Library:  public CComDispatch<ILibrary>{
private:
	HMODULE m_Handle;

	FARPROC GetProc(BSTR functionName) const;

public:
	Library(BSTR* path);
	~Library(void);

	HMODULE handle();

	HRESULT STDMETHODCALLTYPE alloc(unsigned int number, BSTR* dest);

	HRESULT STDMETHODCALLTYPE call(BSTR functionName, BSTR params, int* result);

    HRESULT STDMETHODCALLTYPE writeByte(BSTR dest,unsigned int pos,VARIANT value);
    
    HRESULT STDMETHODCALLTYPE writeWord(BSTR dest,unsigned int pos,VARIANT value);
    
    HRESULT STDMETHODCALLTYPE writeDWord(BSTR dest,unsigned int pos,VARIANT value);
    
    HRESULT STDMETHODCALLTYPE writeInt64(BSTR dest,unsigned int pos,VARIANT value);
    
    HRESULT STDMETHODCALLTYPE copy( BSTR dest,unsigned int destPos,BSTR src,unsigned int srcPos,unsigned int size);

	HRESULT STDMETHODCALLTYPE readByte( BSTR src,unsigned int pos, VARIANT *value);
	HRESULT STDMETHODCALLTYPE readWord( BSTR src,unsigned int pos, VARIANT *value);
	HRESULT STDMETHODCALLTYPE readDWord( BSTR src,unsigned int pos,VARIANT *value);
	HRESULT STDMETHODCALLTYPE readInt64( BSTR src,unsigned int pos,VARIANT *value);
	HRESULT STDMETHODCALLTYPE readBSTR( BSTR src,unsigned int pos, BSTR* value);
        
};
