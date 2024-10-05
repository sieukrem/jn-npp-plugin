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
#include "npp\Scintilla.h"

#include "IndicatorPanel.h"

#pragma warning (disable : 4355)
IndicatorPanel::IndicatorPanel(SCIView& view ): m_View(view), m_PixelsUpdateAction(*this, &IndicatorPanel::OnUpdatePixels), m_LinesUpdateAction(*this, &IndicatorPanel::OnUpdateLines)
{
	m_IndicatorMask = (~0); // All of indicators are enabled
	m_Disabled = true;

	m_PixelIndicators = 0;

	// let window calculate nc region again, to be able to draw panel first time
	SetWindowPos(view.m_Handle, NULL,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
}

IndicatorPanel::~IndicatorPanel(void)
{
	ForegroundIdleHook::getInstance()->remove(&m_LinesUpdateAction);
	ForegroundIdleHook::getInstance()->remove(&m_PixelsUpdateAction);

	if (m_PixelIndicators)
		delete[] m_PixelIndicators;
}

void IndicatorPanel::updateLines(int begin, int end)
{
	// remove pixels updater when scheduled
	ForegroundIdleHook::getInstance()->remove(&m_LinesUpdateAction);
	ForegroundIdleHook::getInstance()->remove(&m_PixelsUpdateAction);

	// shift m_Begin to the left when required
	if (m_Begin > begin || (m_End == 0 /* first time */))
		m_Begin = begin;

	// shift m_End to the right when required
	if (m_End != -1 && (end == -1 || m_End < end))
		m_End = end;

	// shedule lines update
	ForegroundIdleHook::getInstance()->add(&m_LinesUpdateAction);
}

void IndicatorPanel::updatePixels()
{
	ForegroundIdleHook::getInstance()->remove(&m_PixelsUpdateAction);
	ForegroundIdleHook::getInstance()->add(&m_PixelsUpdateAction);
}

void IndicatorPanel::OnUpdatePixels()
{
	if (m_Disabled)
		return;

	GetIndicatorPixels();
	if (m_PixelIndicatorsLen > 0)
		RedrawIndicatorPanel(); //m_View->paintIndicators();
}

void IndicatorPanel::OnUpdateLines()
{
	if (m_Disabled)
		return;

	if (GetIndicatorLines()) {
		ForegroundIdleHook::getInstance()->add(&m_PixelsUpdateAction);
		
		// set to 0, to indicate finished update
		m_Begin = 0; 
		m_End = 0;
	}
	else {
		ForegroundIdleHook::getInstance()->add(&m_LinesUpdateAction);
	}
}

void IndicatorPanel::ClearIndicators(int begin, int end)
{
	if (m_Indicators.empty())
		return;

	// document line
	unsigned int beginLine = (begin <= 0) ? 0 : m_View.sci(SCI_LINEFROMPOSITION, begin, 0);
	unsigned int endLine = (end < 0) ? m_View.sci(SCI_GETLINECOUNT, 0, 0) : m_View.sci(SCI_LINEFROMPOSITION, end, 0);

	// view line
	auto beginViewPos = m_View.sci(SCI_VISIBLEFROMDOCLINE, beginLine, 0);
	auto endViewPos = m_View.sci(SCI_VISIBLEFROMDOCLINE, endLine, 0);

	if (end < 0)
		endViewPos = max(endViewPos, m_Indicators.rbegin()->first);

	m_Indicators.erase(
		m_Indicators.emplace(beginViewPos, 0).first,
		m_Indicators.emplace(endViewPos, 0).first
	);
}

bool IndicatorPanel::GetIndicatorLines() {

	auto stopTime = GetTickCount64() + 200; // 200ms

	ClearIndicators(m_Begin, m_End);

	if (m_Begin < 0)
		m_Begin = 0;

	if (m_End < 0)
		m_End = m_View.sci(SCI_GETLENGTH, 0, 0);

	auto line = m_View.sci(SCI_LINEFROMPOSITION, m_Begin, 0);
	auto lineEnd = m_View.sci(SCI_GETLINEENDPOSITION, line, 0);
	auto nextSavePosition = min(lineEnd, m_End - 1);

	DWORD	mask = 0;

	for (; m_Begin < m_End; ++m_Begin) {

		// add indicator mask for the line to the list of masks
		// if either end of line or end of update range is reached
		if (m_Begin >= nextSavePosition) {

			// dont add empty masks
			if (mask) {
				// get position of line in the view
				auto viewPos = m_View.sci(SCI_VISIBLEFROMDOCLINE, line, 0);

				if (viewPos >= 0) {
					auto existingPos = m_Indicators.find(viewPos);
					if (existingPos != m_Indicators.end())
						existingPos->second = existingPos->second | mask;
					else
						m_Indicators.emplace(viewPos, mask);
				}
			}

			line++;
			lineEnd = m_View.sci(SCI_GETLINEENDPOSITION, line, 0);
			nextSavePosition = min(lineEnd, m_End - 1);

			mask = 0;

			if (GetTickCount64() > stopTime)
				return false;
		}
		//
		DWORD tmp = static_cast<DWORD>(m_View.sci(SCI_INDICATORALLONFOR, m_Begin, 0));
		mask = mask | tmp;
	}

	return true;
}

void IndicatorPanel::GetIndicatorPixels() {
	m_PixelIndicatorsLen = (m_PanelRect.bottom - m_PanelRect.top);

	int indicatorThickness = ((m_PanelRect.right - m_PanelRect.left) + 1) / 2;

	if (m_PixelIndicators) {
		delete[] m_PixelIndicators;
		m_PixelIndicators = NULL;
	}

	int scrollHHeight = GetSystemMetrics(SM_CYHSCROLL);
	if (hasStyle(m_View.m_Handle, WS_VSCROLL))
		m_PixelIndicatorsLen -= 2 * scrollHHeight;
	if (hasStyle(m_View.m_Handle, WS_HSCROLL))
		m_PixelIndicatorsLen -= scrollHHeight;

	if (m_PixelIndicatorsLen <= 0)
		return; // not sufficient place

	auto lines = m_View.sci(SCI_GETLINECOUNT, 0, 0);

	auto lineHeight = m_View.sci(SCI_TEXTHEIGHT, 0, 0);

	if (!(lineHeight && lines))
		return; // avoid division by 0

	auto visibleLines = m_View.sci(SCI_VISIBLEFROMDOCLINE, lines - 1, 0) + 1;
	auto linesOnPage = (m_PixelIndicatorsLen) / lineHeight;

	// maximum pixel per line on panel line height
	float pixelPerLineOnPanel = (linesOnPage > visibleLines) ? lineHeight : (float)(m_PixelIndicatorsLen - indicatorThickness) / visibleLines;

	m_PixelIndicators = new DWORD[m_PixelIndicatorsLen];

	memset(m_PixelIndicators, 0, sizeof(DWORD) * m_PixelIndicatorsLen);

	// setup mask
	for (auto it = m_Indicators.begin(); it != m_Indicators.end(); ++it) {
		int y = (int)(pixelPerLineOnPanel * (float)(it->first));

		for (int t = y; t < (y + indicatorThickness) && t < m_PixelIndicatorsLen; t++) {
			m_PixelIndicators[t] |= it->second;
		}
	}
}

void IndicatorPanel::RedrawIndicatorPanel(){
	BOOL res;
	res = 0;
	res = RedrawWindow(m_View.m_Handle, &m_PanelRect, NULL, RDW_INVALIDATE | RDW_FRAME);
	res = res;
};

LRESULT IndicatorPanel::OnNCCalcSize(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){
	if (wParam && !m_Disabled){
		NCCALCSIZE_PARAMS* ncp = (NCCALCSIZE_PARAMS*)lParam;
		int borderWidth = GetSystemMetrics(SM_CXEDGE);
		int hScrollHeight = GetSystemMetrics(SM_CYVTHUMB);
		int vScrollWidth = GetSystemMetrics(SM_CXHTHUMB);
		int panelWidth = (vScrollWidth + 1) / 2;


		m_PanelRect = ncp->rgrc[0];

		m_PanelRect.bottom -= (m_PanelRect.top + borderWidth); 
		m_PanelRect.top = borderWidth;

		m_PanelRect.right -= (m_PanelRect.left + borderWidth); 
		m_PanelRect.left = m_PanelRect.right - panelWidth - borderWidth;

		ncp->rgrc[0].right -= (panelWidth + borderWidth);

		m_UnderScroll.bottom	= m_PanelRect.bottom;
		m_UnderScroll.right		= m_PanelRect.left;
		m_UnderScroll.top		= m_UnderScroll.bottom	- hScrollHeight;
		m_UnderScroll.left		= m_UnderScroll.right	- vScrollWidth;

		ForegroundIdleHook::getInstance()->add( &m_PixelsUpdateAction);
	}
	return m_View.CallOldWndProc(message,wParam,lParam);
}

LRESULT IndicatorPanel::OnNCPaint(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){

	if (m_Disabled)
		return m_View.CallOldWndProc(message,wParam,lParam);

	int borderWidth		= GetSystemMetrics(SM_CXFOCUSBORDER);
	int scrollWidth		= GetSystemMetrics(SM_CXVSCROLL);
	int scrollHHeight	= GetSystemMetrics(SM_CXHSCROLL);

	HDC hdc;
	HBRUSH hbr3DFace; 
	int res;

	hbr3DFace = (HBRUSH)GetSysColorBrush(COLOR_3DFACE); 

	HRGN prRG = CreateRectRgnIndirect(&m_PanelRect);

	hdc = GetWindowDC(hwnd);

	bool vscroll = hasStyle(hwnd, WS_VSCROLL);
	bool hscroll = hasStyle(hwnd, WS_HSCROLL);

	if (vscroll && hscroll){
		// fill small rectangle under vertical scrollbar and panel with 
		// frame color
		HRGN combinedRG	= CreateRectRgnIndirect(&m_UnderScroll);
		HRGN underRG	= CreateRectRgnIndirect(&m_UnderScroll);
		res = CombineRgn(combinedRG, underRG, prRG, RGN_OR);

		res = FillRgn(hdc, combinedRG, hbr3DFace);

		DeleteObject(combinedRG);
		DeleteObject(underRG);

	}else{
		res = FillRgn(hdc, prRG, hbr3DFace);
	}

	if (m_PixelIndicators){
		paintIndicators(hdc);
	}

	DeleteObject(prRG);

	ReleaseDC(hwnd, hdc);

	return m_View.CallOldWndProc(message,wParam,lParam);
}

void IndicatorPanel::paintIndicators(){
	HDC hdc = GetWindowDC(m_View.m_Handle);

	paintIndicators(hdc);

	ReleaseDC(m_View.m_Handle, hdc);
}

void IndicatorPanel::paintIndicators(HDC hdc){
	
	if (!m_PixelIndicators || m_Disabled)
		return;

	bool vscroll = hasStyle(m_View.m_Handle, WS_VSCROLL);
	int scrollHHeight	= GetSystemMetrics(SM_CXHSCROLL);
	HBRUSH hbr3DFace = (HBRUSH)GetSysColorBrush(COLOR_3DFACE); 
	
	int topOffset = vscroll ? m_PanelRect.top + scrollHHeight : m_PanelRect.top;

	BOOL res = FillRect(hdc, &m_PanelRect, hbr3DFace);

	int indicatorPadding = (m_PanelRect.right - m_PanelRect.left) / 4;
	int indicatorLeftPos = m_PanelRect.left + indicatorPadding;
	int indicatorRightPos = m_PanelRect.right - indicatorPadding;

	for (int i=0; i<m_PixelIndicatorsLen; i++){
		DWORD mask = m_PixelIndicators[i];
		DWORD maskToPaint = m_IndicatorMask & mask;
		if (maskToPaint){
			COLORREF color = getColorForMask(maskToPaint);		
			HPEN hpen = CreatePen(PS_SOLID, 1, color);
			HPEN oldPen = (HPEN)SelectObject(hdc, hpen);
			int curOffset = i + topOffset;
			res = MoveToEx(hdc, indicatorLeftPos, curOffset, NULL);
			res = LineTo(hdc, indicatorRightPos, curOffset);
			SelectObject(hdc, oldPen);
			DeleteObject(hpen);
		}
	}
}

bool IndicatorPanel::hasStyle(HWND hwnd, int style) {
	return (GetWindowLongPtr(hwnd, GWL_STYLE) & style) != 0;
}

COLORREF IndicatorPanel::getColorForMask(DWORD mask){
	int indic = 0;
	int r,g,b;
	r=g=b=0;
	int n=0;

	while(mask){
		if (mask & 0x1){
			COLORREF c = static_cast<COLORREF>(m_View.sci(SCI_INDICGETFORE, indic, 0));
			r += GetRValue(c);
			g += GetGValue(c);
			b += GetBValue(c);
			n++;
		}
		indic++;
		mask >>=1;
	}
	if (n)
		return RGB(r / n, g / n, b / n);
	else
		return RGB(0,0,0);
}

void IndicatorPanel::SetDisabled(bool value){
	if (value == m_Disabled)
		return;

	m_Disabled = value;

	// let window calculate nc region again, to be able to draw panel first time
	SetWindowPos(m_View.m_Handle, NULL,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
}

bool IndicatorPanel::GetDisabled(){
	return m_Disabled;
}

DWORD IndicatorPanel::GetIndicatorMask(){
	return m_IndicatorMask;
}

void  IndicatorPanel::SetIndicatorMask(DWORD value){
	m_IndicatorMask = value;

	paintIndicators();
}
