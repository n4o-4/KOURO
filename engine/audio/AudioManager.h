#pragma once
#include <string>
#include <fstream>
#include <wrl.h>
#include <memory>
#include <unordered_map>
#include <xaudio2.h>

#pragma comment(lib,"xaudio2.lib")

#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>

#pragma comment(lib, "Mf.lib")
#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "Mfreadwrite.lib")
#pragma comment(lib, "mfuuid.lib")

// \brief AudioManager
// 音声ファイルの読み込みと管理を行うシングルトンクラス。
// WAV/MP3形式に対応し、各音声データをバッファとして保持する。

class AudioManager
{
private:
	// チャンクヘッダ
	struct ChunkHeader
	{
		char id[4];   // チャンク毎のID
		int32_t size; // チャンクサイズ
	};

	// RIFFヘッダチャンク
	struct RiffHeader
	{
		ChunkHeader chunk; // "RIFF"
		char type[4];      // WAVE"
	};

	// FMTチャンク
	struct FormatChunk
	{
		ChunkHeader chunk; // "fmt"
		WAVEFORMATEX fmt;  // 波系フォーマット
	};

	// 音声データ
	struct SoundData
	{
		// 波系フォーマット
		WAVEFORMATEX wfex;

		// バッファの先頭アドレス
		BYTE* pBuffer;

		// バッファのサイズ
		unsigned int bufferSize;
	};

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
	std::unordered_map<std::string, SoundData> GetSoundData() { return soundDatas; }

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

	SoundData soundData{};

	std::unordered_map<std::string, SoundData> soundDatas;

};