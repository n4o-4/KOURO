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

	/// \brief  Initialize 初期化
	void Initialize();

	/// \brief  Finalize 終了処理
	void Finalize();
	
	/**
	* \brief  SoundPlay 音声再生
	* \param  filename 音声ファイル名
	* \param  loopCount ループ回数
	*/
	void SoundPlay(const char* filename,int loopCount);

	/**
	* \brief  SoundStop 音声停止
	* \param  filename 音声ファイル名
	*/
	void SoundStop(const char* filename);

	/**
	* \brief  SetVolume 音量設定
	* \param  volume 音量
	*/
	void SetVolume(float volume);

private:

	Microsoft::WRL::ComPtr<IXAudio2> xAudio2;

	IXAudio2MasteringVoice* masterVoice;

	IXAudio2SourceVoice* pSourceVoice = nullptr;

};