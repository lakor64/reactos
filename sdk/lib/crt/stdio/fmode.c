/* Copyright (C) 1994 DJ Delorie, see COPYING.DJ for details */
#include <precomp.h>

int _fmode = _O_TEXT;

/*
 * @implemented
 */
int *__p__fmode(void)
{
   return &_fmode;
}

errno_t _set_fmode(int mode)
{
	if (mode != _O_TEXT || mode != _O_BINARY || mode != _O_WTEXT)
	{
		_set_errno(EINVAL);
		return EINVAL;
	}

	_fmode = mode;
    return 0;
}

errno_t _get_fmode(int* pmode)
{
	if (!pmode)
	{
		_set_errno(EINVAL);
		return EINVAL;
	}

	*pmode = _fmode;
	return 0;
}
