#include "Audio.h"

#include "assert.h"

void Audio::Initialize()
{
	HRESULT result;

	// XAudioエンジンのインスタンスを生成
	result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);

	// マスターボイスを生成
	result = xAudio2->CreateMasteringVoice(&masterVoice);

}
void Audio::Finalize()
{
	pSourceVoice->Stop();
}

void Audio::SoundPlay(const char* filename,int loopCount)
{
	HRESULT result;

	// 波系フォーマットを元にSourceVoiceの生成
	
	auto soundDatas = AudioManager::GetInstance()->GetSoundData();

	if (soundDatas.find(filename) != soundDatas.end())
	{

		result = xAudio2->CreateSourceVoice(&pSourceVoice, &soundDatas.find(filename)->second.wfex);
		assert(SUCCEEDED(result));

		// 再生する波形データの設定
		XAUDIO2_BUFFER buf{};
		buf.pAudioData = soundDatas.find(filename)->second.pBuffer;
		buf.AudioBytes = soundDatas.find(filename)->second.bufferSize;
		buf.Flags = XAUDIO2_END_OF_STREAM;

		if (loopCount != 0)
		{
			buf.LoopCount = loopCount;
		}

		if (loopCount >= 9999)
		{
			buf.LoopCount = XAUDIO2_LOOP_INFINITE;
		}

		// 波形データの再生
		result = pSourceVoice->SubmitSourceBuffer(&buf);
		result = pSourceVoice->Start();

		return;
	}

	assert(0);
}

void Audio::SoundStop(const char* filename)
{

	auto soundDatas = AudioManager::GetInstance()->GetSoundData();

	if (soundDatas.find(filename) != soundDatas.end())
	{

		XAUDIO2_BUFFER buf{};
		buf.pAudioData = soundDatas.find(filename)->second.pBuffer;
		buf.AudioBytes = soundDatas.find(filename)->second.bufferSize;
		buf.Flags = XAUDIO2_END_OF_STREAM;

		// 波形データの再生
		pSourceVoice->SubmitSourceBuffer(&buf);
		pSourceVoice->Stop();

		return;
	}


}

void Audio::SetVolume(float volume)
{

	pSourceVoice->SetVolume(volume);

}
