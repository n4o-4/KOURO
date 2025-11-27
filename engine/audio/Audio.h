#pragma once
#include <fstream>
#include <wrl.h>
#include <xaudio2.h>
#include <memory>

#include <unordered_map>

#pragma comment(lib,"xaudio2.lib")

#include <AudioManager.h>

// \brief Audio
// 音声再生を管理するクラス。WASAPI/XAudio2を使用して、BGMや効果音の再生・停止・音量調整が可能。

class Audio {


public:

private:

public:

	void Initialize();

	void Finalize();
	
	// 音声再生

	void SoundPlay(const char* filename,int loopCount);

	void SoundStop(const char* filename);

	void SetVolume(float volume);

private:

	Microsoft::WRL::ComPtr<IXAudio2> xAudio2;

	IXAudio2MasteringVoice* masterVoice;

	IXAudio2SourceVoice* pSourceVoice = nullptr;

};