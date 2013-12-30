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
#include "..\ComBase.h"

class Dialog;

class MOleClientSite : public CComBase<IOleClientSite>{
public:
	Dialog* m_Dialog;

	virtual ULONG STDMETHODCALLTYPE AddRef();

	virtual ULONG STDMETHODCALLTYPE Release();

	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, LPVOID *ppv);

	virtual HRESULT STDMETHODCALLTYPE SaveObject();

	virtual HRESULT STDMETHODCALLTYPE GetMoniker( DWORD dwAssign,    DWORD dwWhichMoniker,    IMoniker **ppmk);

	virtual HRESULT STDMETHODCALLTYPE GetContainer( IOleContainer **ppContainer);

	virtual HRESULT STDMETHODCALLTYPE ShowObject();

	virtual HRESULT STDMETHODCALLTYPE OnShowWindow(BOOL fShow);

	virtual HRESULT STDMETHODCALLTYPE RequestNewObjectLayout();
};



