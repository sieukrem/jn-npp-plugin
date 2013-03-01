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

#include <dispex.h>

#include "common\combase.h"
#include "common\ForegroundIdleHook.h"

#include "Interfaces.h"

#include "npp\PluginDefinition.h"

#include "SCIView.h"
#include "IndicatorPanel.h"

#define m_Handle (((m_Id-1)==0)? nppData._scintillaMainHandle : nppData._scintillaSecondHandle)


extern const TCHAR* langs[];

class CEditor;

class CEditorView : public CComDispatch<IView>,public ForegroundIdleHook::IdleHandler, public SCIView
{
public:
	IDispatchEx* m_Listener;

	CEditorView(int id);
	~CEditorView(void){
	
	};

	LRESULT OnMessage(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	int m_Id;

	CEditor* m_Editor;
	IndicatorPanel m_IndPanel;

	void DoMessage(SCNotification* eventArgs);


#pragma region Helper methods
	BSTR adjustFromCodePage(char* buf, int len);
	char* adjustToCodePage(BSTR str);

	int posToBytePos(int value);
	int bytePosToPos(int pos);

private:

	int getBufferId();
	void setIndicatorLinesUpdater(int begin, int end);

#pragma endregion



#pragma region IView implementation
public : 
		virtual HRESULT STDMETHODCALLTYPE get_text( BSTR *result);

        virtual HRESULT STDMETHODCALLTYPE put_text(BSTR *value);
        
        virtual HRESULT STDMETHODCALLTYPE get_files( long *value);

        virtual HRESULT STDMETHODCALLTYPE get_selection( BSTR *value);
        
        virtual HRESULT STDMETHODCALLTYPE put_selection(BSTR *value);
        
        virtual HRESULT STDMETHODCALLTYPE get_codepage(int *value);
        
        virtual HRESULT STDMETHODCALLTYPE put_codepage(int value);

        virtual HRESULT STDMETHODCALLTYPE get_files(IDispatch **result);
        
        virtual HRESULT STDMETHODCALLTYPE get_file(int *value);
        
        virtual  HRESULT STDMETHODCALLTYPE put_file( int value);

		virtual HRESULT STDMETHODCALLTYPE get_lang( int *result);

        virtual HRESULT STDMETHODCALLTYPE put_lang( int value);

        virtual HRESULT STDMETHODCALLTYPE get_line( int *result);
        
        virtual HRESULT STDMETHODCALLTYPE put_line( int value);
        
        virtual HRESULT STDMETHODCALLTYPE get_pos( int *result);

        virtual HRESULT STDMETHODCALLTYPE put_pos( int value);

        virtual HRESULT STDMETHODCALLTYPE get_bytePos( int *result);
        
        virtual HRESULT STDMETHODCALLTYPE put_bytePos( int value);

        virtual HRESULT STDMETHODCALLTYPE get_column( int *result);

        virtual HRESULT STDMETHODCALLTYPE get_anchor( int *result);
        
        virtual HRESULT STDMETHODCALLTYPE put_anchor(int value);

		virtual HRESULT STDMETHODCALLTYPE get_byteAnchor( int *result);
        
        virtual HRESULT STDMETHODCALLTYPE put_byteAnchor(int value);


        virtual HRESULT STDMETHODCALLTYPE get_lineCount(int* line);

        virtual HRESULT STDMETHODCALLTYPE get_lines(IViewLines** result);

        virtual HRESULT STDMETHODCALLTYPE get_navBarHidden( VARIANT_BOOL *result);
        
        virtual HRESULT STDMETHODCALLTYPE put_navBarHidden( VARIANT_BOOL value);

		virtual HRESULT STDMETHODCALLTYPE get_handle(int *result);

#pragma endregion

};
