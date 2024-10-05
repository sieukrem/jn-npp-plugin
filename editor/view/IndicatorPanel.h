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
#include <map>

#include "common\ForegroundIdleHook.h"
#include "SCIView.h"

class IndicatorPanel
{
	typedef std::map<size_t, DWORD> LineToMaskMap;
	LineToMaskMap m_Indicators;

	typedef void (IndicatorPanel::* UpdateFunction)(void);
	class IndicatorUpdater : public ForegroundIdleHook::IdleHandler {
		IndicatorPanel& m_IndPanel;
		UpdateFunction m_UpdateFn;
	public:
		IndicatorUpdater(IndicatorPanel& indPanel, UpdateFunction fn) :m_IndPanel(indPanel), m_UpdateFn(fn) {
		};

		void execute() {
			std::invoke(m_UpdateFn, &m_IndPanel);
		}
	};

	IndicatorUpdater	m_PixelsUpdateAction;
	IndicatorUpdater	m_LinesUpdateAction;

	RECT	m_PanelRect;
	RECT	m_UnderScroll;

	SCIView& m_View;

	DWORD* m_PixelIndicators;
	int  m_PixelIndicatorsLen;

	static bool hasStyle(HWND hwnd, int style);

	DWORD m_IndicatorMask; 
	bool  m_Disabled;

	int m_Begin = 0;
	int m_End = 0;

	void OnUpdatePixels();
	void OnUpdateLines();

	void ClearIndicators(int begin, int end);
	bool GetIndicatorLines();
	void GetIndicatorPixels();

	void paintIndicators();
	void paintIndicators(HDC hdc);
	COLORREF getColorForMask(DWORD mask);
	void RedrawIndicatorPanel();

public:
	LRESULT OnNCCalcSize(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT OnNCPaint(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	DWORD GetIndicatorMask();
	void SetIndicatorMask(DWORD value);

	void SetDisabled(bool value);
	bool GetDisabled();

	IndicatorPanel(SCIView& m_View);
	~IndicatorPanel(void);

	void updateLines(int begin, int end);
	void updatePixels();
};
