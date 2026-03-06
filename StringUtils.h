#pragma once
#include <string>

namespace Kouro::StringUtils
{
	/**
	* \brief  文字列変換関数
	* \param  str 変換する文字列
	* \return 変換された文字列
	*/
	std::wstring ToWString(const std::string& str);

	/**
	* \brief  文字列変換関数
	* \param  wstr 変換する文字列
	* \return 変換された文字列
	*/
	std::string ToString(const std::wstring& wstr);
}