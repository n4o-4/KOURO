﻿#pragma once
#include <windows.h>
#include <string>

namespace StringUtility
{
	std::wstring ConvertString(const std::string& str);

	std::string ConvertString(const std::wstring& str);
}