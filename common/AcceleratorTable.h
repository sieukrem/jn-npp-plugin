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

#include "AcceleratorHook.h"

class CAcceleratorTable: public AcceleratorHook::AcceleratorHandler{
public:

	class Accelerator{
	public:
		virtual int execute(){
			return 0;
		}	
	};

	WORD m_StartId;
	
	typedef std::vector<ACCEL> ACCELS;
	ACCELS m_HotKeyArray;

	typedef std::vector<Accelerator*> Accelerators;
	Accelerators m_Accelerators;

	HACCEL m_HotKeyTable;
	HWND m_Window;

	CAcceleratorTable(WORD startId, HWND window){
		m_StartId = startId;
		m_HotKeyTable = NULL;
		m_Window = window;
	}

	~CAcceleratorTable(){
		destroyTable();

		for(int i=0, c=m_Accelerators.size(); i<c; i++){
			delete m_Accelerators[i];
		}
	}

	void destroyTable(){
		if (m_HotKeyTable){
			DestroyAcceleratorTable(m_HotKeyTable);
			m_HotKeyTable = NULL;
		}
	}

	void add(ACCEL accel, Accelerator* handler){
		int c = m_HotKeyArray.size();
		
		m_HotKeyArray.push_back(accel);
		m_Accelerators.push_back(handler);

		destroyTable();

		m_HotKeyTable = CreateAcceleratorTable(&m_HotKeyArray[0], c);
	}

	int execute(MSG* msg){
		HWND foregroundW = GetForegroundWindow();
		if (m_Window != foregroundW)
			return 0;
		
		if (!m_HotKeyTable)
			return 0;

		return TranslateAccelerator(m_Window, m_HotKeyTable, msg);
	}

	// returns 0 if no handler called
	int CallHandler(WORD command){
		//WORD lw = LOWORD(msg->wParam);

		int cmdId = command - m_StartId;

		if (cmdId < 0 || cmdId >= (int)m_Accelerators.size())
			return 0;
			
		int result = m_Accelerators[cmdId]->execute();

		return result;
	}
};
