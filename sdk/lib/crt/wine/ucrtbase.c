 /*
 * msvcrt.dll dll data items
 *
 * Copyright 2000 Jon Griffiths
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

#include "config.h"
#include "wine/port.h"

#include <math.h>
#include "msvcrt.h"
#include <winnls.h>
#include "wine/debug.h"

/*********************************************************************
 *		_get_narrow_winmain_command_line (UCRTBASE.@)
 */
char* CDECL _get_narrow_winmain_command_line(void)
{
  static char *narrow_command_line;
  char *s;

  if (narrow_command_line)
      return narrow_command_line;

  s = GetCommandLineA();
  while (*s && *s != ' ' && *s != '\t')
  {
      if (*s++ == '"')
      {
          while (*s && *s++ != '"')
              ;
      }
  }

  while (*s == ' ' || *s == '\t')
      s++;

  return narrow_command_line = s;
}

/*********************************************************************
 *		_get_wide_winmain_command_line (UCRTBASE.@)
 */
MSVCRT_wchar_t* CDECL _get_wide_winmain_command_line(void)
{
  static MSVCRT_wchar_t *wide_command_line;
  MSVCRT_wchar_t *s;

  if (wide_command_line)
      return wide_command_line;

  s = GetCommandLineW();
  while (*s && *s != ' ' && *s != '\t')
  {
      if (*s++ == '"')
      {
          while (*s && *s++ != '"')
              ;
      }
  }

  while (*s == ' ' || *s == '\t')
      s++;

  return wide_command_line = s;
}
