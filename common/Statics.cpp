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
#include "Statics.h"
#include "LastError.h"


Statics::Statics(){
	hInstance = 0;
	hWindow   = 0;
}

ITypeLib* Statics::GetTypeLib(){

	TCHAR dllpath[MAX_PATH];
	GetModuleFileName(hInstance, dllpath, MAX_PATH);

	ITypeLib* result = NULL;
	HRESULT hr = LoadTypeLib(dllpath, &result);

	if (!SUCCEEDED(hr)){
		LastError err;
		throw err.staticmessage();
	}

	return result;
}

Statics& Statics::instance(){
	static Statics s_instance;

	return s_instance;
}
