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

// FUNCTIONS *****************************

bool decode::FileExist(const wchar_t* file_name)
{
	bool exist{ false };

	std::wifstream check(file_name);
	exist = check.is_open();
	check.close();

	return exist;
}

size_t decode::Decode(const wchar_t* file_name, BAG<wchar_t>& Bag)
{
	size_t ret{ 0 };

	Bag.clear();

	if (FileExist(file_name))
	{
		std::wifstream save(file_name);

		int code{ 0 };
		size_t text_size{ 0 };

		save >> text_size;
		save >> code;

		for (size_t count = 0; count < text_size; ++count)
		{
			int letter{ 0 };

			save >> letter;
			letter -= code;

			Bag.push_back(static_cast<wchar_t>(letter));

			++ret;
		}

		save.close();
	}

	return ret;
}