#include "pch.h"
#include "TextDecoder.h"

// EXCEPTION class **********************

decode::EXCEPTION::EXCEPTION(int what)
{
	err_code = what;
}
const wchar_t* decode::EXCEPTION::eGet() const
{
	switch (err_code)
	{
	case ERR_INDEX:
		return L"Bad index passed to BAG container !";

	case ERR_ITERATOR:
		return L"Bad iterator passed to BAG container !";

	case ERR_PARAM:
		return L"Bad parameter passed to BAG container !";

	case ERR_PTR:
		return L"Bad pointer passed to BAG container !";
	}

	return L"Unknown error occured in BAG container !";
}

//////////////////////////////////////////