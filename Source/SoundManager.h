#pragma once

#include "../Assets/import/FMOD/inc/fmod.h"
#include "../Assets/import/FMOD/inc/fmod.hpp"
#include "../Assets/import/FMOD/inc/fmod_errors.h"

#pragma comment (lib, "../Assets/import/FMOD/lib/fmodex64_vc")


using namespace FMOD;


#define MAX_SOUND 30

#define FRONT_MAX_DISTANCE 40
#define BACK_MAX_DISTANCE 20


enum SoundType 
{
	SOUND_BGM,
	SOUND_EFFECT,
	SOUND_SelectEnd,
};

class Sound3D
{
public:
	bool isPlaying;
	XMFLOAT3 pos;
	XMFLOAT3 look;
	float nowSpeed;
	float addSpeed;
	float maxVolume;
	Channel* channel;
	
	Sound3D() 
	{
		isPlaying = true;
	}

	Sound3D(XMFLOAT3 pos,Channel* channel, XMFLOAT3 look = XMFLOAT3(0,0,0), float nowSpeed = 1.0f, float addSpeed = 0.0f, float maxVolume = 1.0f)
	{
		Sound3D::pos = pos;
		Sound3D::look = look;
		Sound3D::nowSpeed = nowSpeed;
		Sound3D::addSpeed = addSpeed;
		Sound3D::channel = channel;
		Sound3D::maxVolume = maxVolume;
		isPlaying = true;
	}

	void update(XMFLOAT3 listenerPos)
	{
		if (abs(pos.z - listenerPos.z) < 20) //단순히 z축 거리만 비교해서 50이상인 경우 중지.
		{
			//거리를 구한다.
			float maxDistance = FRONT_MAX_DISTANCE;
			float volume = 1.0f;

			XMFLOAT3 distance = XMFLOAT3(pos.x - listenerPos.x, 0, pos.z - listenerPos.z);

			if (distance.z < 0)
				maxDistance = BACK_MAX_DISTANCE;

			XMStoreFloat3(&distance, XMVector3Length(XMLoadFloat3(&distance)));
			volume = ((maxDistance - distance.x) / maxDistance) * maxVolume;
			if (volume <= 0)
				channel->stop();
			else
				channel->setVolume(volume);
		}

		else
		{
			channel->stop();
			isPlaying = false;
		}
	}


	void updateWater(XMFLOAT3 listenerPos)
	{
		if (abs(pos.z - listenerPos.z) < 100) //단순히 z축 거리만 비교해서 50이상인 경우 중지.
		{
			//거리를 구한다.
			float maxDistance = FRONT_MAX_DISTANCE * 2.0f;
			float volume = 1.0f;

			XMFLOAT3 distance = XMFLOAT3(pos.x - listenerPos.x, 0, pos.z - listenerPos.z);

			if (distance.z < 0)
				maxDistance = BACK_MAX_DISTANCE * 2.0f;

			XMStoreFloat3(&distance, XMVector3Length(XMLoadFloat3(&distance)));
			volume = ((maxDistance - distance.x) / maxDistance) * maxVolume;
			if (volume <= 0)
				channel->setVolume(0.0f);
			else
			{
				channel->setVolume(volume);
			}
		}

		else
		{
			channel->setVolume(0.0f);
			isPlaying = false;
		}
	}

};

class Sound3DList
{
private:
	std::list<Sound3D*> m_list;

public:
	Sound3DList();

	~Sound3DList();

	void addSound(Sound3D* newSound);

	void update3DSound(XMFLOAT3 listenerPos);

	Sound3D*		waterfall;
};

class SoundManager : public CSingleTonBase<SoundManager>
{

private:
	FMOD::System*				 g_pSystem;
	FMOD::Channel*				 g_pChannel[SOUND_SelectEnd];
	FMOD::Channel*		g_pWaterFall;

	std::unique_ptr<Sound3DList> sound3dList;

	std::map<std::string, FMOD::Sound*>			m_pSound;
	float										m_fVolum;

	//배경음을 추가할 떄 사용한다
	void AddStrem(char* filePath, std::string name);

	//효과음을 추가할 떄 사용한다.
	void AddSound(char* filePath, std::string name);

public:
	void LoadSound();

	//사운드의 원활한 재생을 위해 사용한다.
	void OnUpdate(XMFLOAT3 listenerPos);

	//따로안해도되네?
	void Play(std::string name, XMFLOAT3 pos, FMOD::Channel* channel);

	//따로안해도되네?
	void Play(std::string name, XMFLOAT3 pos);

	//따로안해도되네?
	void Play(std::string name);

	//bgm플레이는 따로 만들어줘야한다.
	void PlayBGM(std::string name);

	//스탑을 멈출 때
	void Stop();

	//소리조절
	void VolumSet(float vol);
	void VolumUp(float vol = 0.05f);
	void VolumDown(float vol = 0.05f);

	//노래를 찾을때
	FMOD::Sound*		FindSound(std::string name);
	
	void PlayWaterSound();
	



public:
	SoundManager();
	~SoundManager();

};



