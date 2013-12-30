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
#include "LastError.h"
#include "Strsafe.h"


LastError::LastError(){
	DWORD err = GetLastError();

	m_Message = NULL;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		0 ,
		err, 
		0,
		(LPWSTR)&m_Message,
		0,
		NULL
	);
}

LastError::~LastError(){
	if (m_Message)
		LocalFree(m_Message);
}

TCHAR* LastError::message(){
	return m_Message;
}

TCHAR* LastError::staticmessage() {
	static TCHAR buf[1024];
	buf[0] = 0;

	if (m_Message)
		StringCchCopy(buf, 1024, m_Message);

	return buf;
}