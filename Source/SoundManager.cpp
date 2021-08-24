
#include "stdafx.h"
#include "SoundManager.h"

Sound3DList::Sound3DList() 
{
	m_list.clear(); 
}


Sound3DList::~Sound3DList()
{
	for (auto &sound : m_list)
	{
		if (sound)delete sound;
		sound = nullptr;
	}
	m_list.clear();
}

void Sound3DList::addSound(Sound3D* newSound)
{
	m_list.push_front(newSound);
}

void Sound3DList::update3DSound(XMFLOAT3 listenerPos)
{
	//for (auto &sound : m_list)
	//	sound->update(listenerPos);
	//
	//waterfall->updateWater(listenerPos);
	//
	//m_list.remove_if
	//(
	//	[](auto sound)
	//	{
	//		return (sound->isPlaying);
	//	}
	//);
}


//Create & Destory
SoundManager::SoundManager()
{
	System_Create(&g_pSystem);
	g_pSystem->init(64, FMOD_INIT_NORMAL, 0);
}

SoundManager::~SoundManager()
{
	g_pSystem->release();
	g_pSystem->close();
}

//Load
void SoundManager::LoadSound()
{
	m_fVolum = 0.05f;
	sound3dList = std::make_unique<Sound3DList>();

	return;

	//배경음 Stream
	AddStrem("../Assets/04_Sound/BGM/Timeless.mp3", "배경");
	AddStrem("../Assets/04_Sound/BGM/bgm01.mp3", "배경소리");
	AddStrem("../Assets/04_Sound/BGM/Lobby.mp3", "로비");
	AddStrem("../Assets/04_Sound/BGM/GamePlay.mp3", "게임플레이");
	AddStrem("../Assets/04_Sound/BGM/Result.mp3", "결과");

	//효과음 Sound
	AddSound("../Assets/04_Sound/Effect/getCoin.wav", "코인");
	AddSound("../Assets/04_Sound/Effect/loding33.wav", "로딩");
	AddSound("../Assets/04_Sound/Effect/start.wav", "스타트");
	AddSound("../Assets/04_Sound/Effect/DebugBlend.wav", "디버깅블랜드");
	AddSound("../Assets/04_Sound/Effect/footstep.wav", "발소리");
	AddSound("../Assets/04_Sound/Effect/GetHit.mp3", "맞음");
	AddSound("../Assets/04_Sound/Effect/Trapped.mp3", "덫_걸림");
	AddSound("../Assets/04_Sound/Effect/FlashBang.wav", "플래시뱅");
	AddSound("../Assets/04_Sound/Effect/Hide.mp3", "은신");
	AddSound("../Assets/04_Sound/Effect/Miss.mp3", "회피");
	AddSound("../Assets/04_Sound/Effect/Throw.wav", "투척");
	AddSound("../Assets/04_Sound/Effect/Teleport.mp3", "텔레포트");
	AddSound("../Assets/04_Sound/Effect/WaterFall.mp3", "폭포");
	
	PlayWaterSound();

}
//배경음을 추가할 떄 사용한다
void SoundManager::AddStrem(char* filePath, std::string name)
{
	auto pFindSound = SoundManager::FindSound(name);

	if (pFindSound != NULL)
		return;

	else
	{
		FMOD::Sound* sound;
		g_pSystem->createStream(filePath, FMOD_LOOP_NORMAL, 0, &sound);
		if (sound) m_pSound.insert(std::make_pair(name, sound));
		else ::GMessageBox(filePath, "경로가 잘못되었습니다");

	}
}

//효과음을 추가할 떄 사용한다.
void SoundManager::AddSound(char* filePath, std::string name)
{
	auto pFindSound = SoundManager::FindSound(name);

	if (pFindSound != NULL)
		return;

	else
	{
		FMOD::Sound* sound;
		g_pSystem->createStream(filePath, FMOD_HARDWARE, 0, &sound);
		if (sound) m_pSound.insert(std::make_pair(name, sound));
		else ::GMessageBox(filePath, "경로가 잘못되었습니다");
	}
}

//Find
FMOD::Sound* SoundManager::FindSound(std::string name)
{
	std::map<std::string, FMOD::Sound*>::iterator iter = m_pSound.find(name);

	if (iter == m_pSound.end())
	{
		//MessageBox(g_hWnd,name.c_str(),"사운드가 없당",NULL);
		return nullptr;
	}
	else
		return iter->second;
}


//Update
void SoundManager::OnUpdate(XMFLOAT3 listenerPos)
{
	 sound3dList->update3DSound(listenerPos);
	g_pSystem->update();
}

void SoundManager::Play(std::string name, XMFLOAT3 pos, FMOD::Channel* channel)
{
	g_pSystem->playSound(FMOD_CHANNEL_FREE, FindSound(name), false, &channel);
	channel->setVolume(m_fVolum * 2.0f);
	sound3dList->addSound(new Sound3D(pos, channel));
}

void SoundManager::Play(std::string name, XMFLOAT3 pos)
{
	g_pSystem->playSound(FMOD_CHANNEL_FREE, FindSound(name), false, &g_pChannel[SOUND_EFFECT]);
	g_pChannel[SOUND_EFFECT]->setVolume(m_fVolum * 2.0f);
	sound3dList->addSound(new Sound3D(pos, g_pChannel[SOUND_EFFECT]));
}

//Play

void SoundManager::PlayBGM(std::string name)
{
	g_pChannel[SOUND_BGM]->stop();
	g_pSystem->playSound(FMOD_CHANNEL_FREE, FindSound(name), false, &g_pChannel[SOUND_BGM]);
	g_pChannel[SOUND_BGM]->setVolume(m_fVolum);
}

void SoundManager::Play(std::string name)
{
	g_pSystem->playSound(FMOD_CHANNEL_FREE, FindSound(name), false, &g_pChannel[SOUND_EFFECT]);
	g_pChannel[SOUND_EFFECT]->setVolume(m_fVolum*2.0f);
}

//Stop
void SoundManager::Stop()
{
	g_pChannel[SOUND_EFFECT]->stop();
	g_pChannel[SOUND_BGM]->stop();
}


//Volum
void SoundManager::VolumSet(float vol)
{
	g_pChannel[SOUND_EFFECT]->setVolume(vol);
	   g_pChannel[SOUND_BGM]->setVolume(vol);
	//g_pSystem->playSound(FMOD_CHANNEL_FREE, g_pSound[esound], false, &g_pChannel[SOUND_EFFECT]);
}

void SoundManager::VolumUp(float vol)
{
	m_fVolum += vol;
	g_pChannel[SOUND_EFFECT]->setVolume(m_fVolum);
	g_pChannel[SOUND_BGM]->setVolume(m_fVolum);
	cout << (float)m_fVolum << endl;
}

void SoundManager::VolumDown(float vol)
{
	m_fVolum -= vol;
	g_pChannel[SOUND_EFFECT]->setVolume(m_fVolum);
	g_pChannel[SOUND_BGM]->setVolume(m_fVolum);
	cout << (float)m_fVolum << endl;
}


void SoundManager::PlayWaterSound()
{
	g_pSystem->playSound(FMOD_CHANNEL_FREE, FindSound("폭포"), false, &g_pWaterFall);
	g_pWaterFall->setVolume(m_fVolum * 2.0f);
	sound3dList->waterfall = new Sound3D(XMFLOAT3(138, 1, 139), g_pWaterFall);
	sound3dList->update3DSound(XMFLOAT3(999, 999, 999));
}