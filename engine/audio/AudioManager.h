#pragma once
#include <string>
#include <fstream>
#include <wrl.h>
#include <memory>
#include <unordered_map>

#include "SoundStruct.h"

// \brief AudioManager
// 音声ファイルの読み込みと管理を行うシングルトンクラス。
// WAV/MP3形式に対応し、各音声データをバッファとして保持する。

namespace Kouro
{
	class Audio;

	class AudioManager
	{
	private:
		
	public:

		/// \brief インスタンス取得
		static AudioManager* GetInstance();

		/// \brief 初期化
		void Initialize();

		/// \brief 終了処理
		void Finalize();

		/**
		* \brief 音声ファイル読み込み
		* \param filename ファイル名
		*/
		void SoundLoadFile(const char* filename);

		/// \brief 音声データ解放
		void SoundUnload();

		/**
		* \brief 音声データ取得
		* \return soundDatas 音声データ群
		*/
		std::unordered_map < std::string, SoundData > & GetSoundData() { return soundDatas; }

	private:

		/**
		* \brief WAVファイル読み込み
		* \param filename ファイル名
		*/
		void SoundLoadWave(const char* filename);

		/**
		* \brief MP3ファイル読み込み
		* \param filename ファイル名
		*/
		void SoundLoadMP3(const char* filename);

	private:

		static std::unique_ptr<AudioManager> instance;

		friend std::unique_ptr<AudioManager> std::make_unique<AudioManager>();
		friend std::default_delete<AudioManager>;

		AudioManager() = default;
		~AudioManager() = default;
		AudioManager(AudioManager&) = delete;
		AudioManager& operator=(AudioManager&) = delete;

		const std::string filePath_ = "Resources/";

		std::unordered_map < std::string, SoundData> soundDatas;

	};
}