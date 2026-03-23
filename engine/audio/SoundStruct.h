#pragma once
#include <string>

#include <xaudio2.h>

#pragma comment(lib,"xaudio2.lib")

#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>

#pragma comment(lib, "Mf.lib")
#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "Mfreadwrite.lib")
#pragma comment(lib, "mfuuid.lib")
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
	std::unique_ptr<BYTE[]> pBuffer;

	// バッファのサイズ
	unsigned int bufferSize;
};