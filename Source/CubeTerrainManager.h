#pragma once

struct LIGHTS;
class CTerrainCubeShader;
class CInstancedObjectsShader;
class CInstancedSpriteShader;

class CCubeTerrainManager : public CSingleTonBase<CCubeTerrainManager>
{
private:
	std::vector<std::shared_ptr<CInstancedObjectsShader>>	m_vPlaneShader;
	std::shared_ptr<CGameObject> m_pWaterObject;
	std::vector<std::shared_ptr<CInstancedObjectsShader>>	m_vPumpkin;

	std::unique_ptr<CInstancedSpriteShader> m_pWaterFallShader;

public:
	CCubeTerrainManager();
	
	~CCubeTerrainManager();
	
	void ReleseInstance();

	void Load(ID3D11Device *pd3dDevice);

	void Render(ID3D11DeviceContext *pd3dDeviceContext);

	void Animate(float fTimeElasped);

private:
	std::vector<std::string> m_vMapPath;

public:
	std::vector<std::string> GetMapPath() {return m_vMapPath; }

};

