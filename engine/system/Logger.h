#pragma once
#include <Windows.h>
#include <string>
#include <StringUtility.h>

namespace Kouro
{
	namespace Logger
	{

		/**
		* \brief  ログ出力関数
		* \param  message 出力するメッセージ
		*/
		void Log(const std::string& message);

		/**
		* \brief  ワイド文字ログ出力関数
		* \param  message 出力するメッセージ
		*/
		void wLog(const std::wstring& message);

	}
}

