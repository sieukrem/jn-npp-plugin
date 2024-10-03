/*
This file is part of jN, a plugin for Notepad++
Copyright (C)2017 Eugen Kremer <eugen DOT kremer AT gmail DOT com>

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

#ifndef VERSION_MA
#define VERSION_MA 2
#endif

#ifndef VERSION_MI
#define VERSION_MI 2
#endif

#ifndef BUILD_NUMBER
// increment BUILD_NUMBER in case of modification
#define BUILD_NUMBER 185
#endif

#ifndef VERSION_REVISION
#define VERSION_REVISION 0
#endif

#define xstr(s) str(s)
#define str(s) #s
#define _VERSION_STR VERSION_MA ## . ## VERSION_MI ## . ## BUILD_NUMBER  ## . ## VERSION_REVISION

