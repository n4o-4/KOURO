#pragma once
#include <windows.h>
#include <string>

namespace StringUtility
{
	/**
	* \brief  stringからwstringに変換する
	* \param  str 変換元のstring
	*/
	std::wstring ConvertString(const std::string& str);

	/**
	* \brief  wstringからstringに変換する
	* \param  str 変換元のwstring
	*/
	std::string ConvertString(const std::wstring& str);
}