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

#include "common\ForegroundIdleHook.h"
#include "SCIView.h"

class IndicatorPanel
{
	struct LineMask{
		DWORD line;
		DWORD mask;
	};
	
	typedef std::vector<LineMask> LineMasks;

	LineMasks m_Indicators;
	LineMasks m_PixelIndicators;

	RECT	m_PanelRect;
	RECT	m_UnderScroll;

	SCIView* m_View;

	const static int m_PanelWidth = 8;

	void ClearIndicators(int begin, int end);

public:
	DWORD* pixelIndicators;
	int  m_PixelIndicatorsLen;

	void GetIndicatorLines(int begin, int end);

	void GetIndicatorPixels();

	void paintIndicators();

	void paintIndicators(HDC hdc);
	
	COLORREF getColorForMask(DWORD mask);

	static bool hasStyle(HWND hwnd, int style);

	LRESULT OnNCCalcSize(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT OnNCPaint(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
	bool m_Disabled;

	void SetDisabled(bool value);
	bool GetDisabled();

	IndicatorPanel(SCIView* m_View);
	~IndicatorPanel(void);
	
	class IndicatorLinesUpdater : public ForegroundIdleHook::IdleHandler{
	public:
		IndicatorPanel* m_IndPanel;
		IndicatorLinesUpdater(IndicatorPanel* indPanel){
			m_IndPanel = indPanel;
			m_Begin = 0;
			m_End = 0;
		};

		int m_Begin;
		int m_End;

		void execute(){
			if (m_IndPanel->m_Disabled)
				return;

			m_IndPanel->GetIndicatorLines(m_Begin, m_End);
			ForegroundIdleHook::getInstance()->add( &m_IndPanel->m_IndicPixelsUp);

			m_Begin = 0;
			m_End = 0;
		};
	};

	class IndicatorPixelsUpdater : public ForegroundIdleHook::IdleHandler{
		public:
		IndicatorPanel* m_IndPanel;
		IndicatorPixelsUpdater(IndicatorPanel* indPanel){m_IndPanel = indPanel;};
		void execute(){
			if (m_IndPanel->m_Disabled)
				return;

			m_IndPanel->GetIndicatorPixels();
			if (m_IndPanel->m_PixelIndicatorsLen > 0)
				m_IndPanel->RedrawIndicatorPanel(); //m_View->paintIndicators();
		};
	};


	IndicatorPixelsUpdater	m_IndicPixelsUp;
	IndicatorLinesUpdater	m_IndicLinesUp;

	void RedrawIndicatorPanel();

};
