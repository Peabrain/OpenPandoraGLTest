#pragma once
#include "Model.h"

namespace lib3D
{
	class CONVERT
	{
	public:
		CONVERT()
		{
		}
		virtual ~CONVERT()
		{
		}

		virtual MODEL* Load(const char* szPath) = 0;
	private:
	};
}
