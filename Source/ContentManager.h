#pragma once


//KYT '16.01.20 plus

//Default Object �� �����Ѵ�. 
//���⼭ ��� Shader, Material, Texture�� ����
// GameObject, Mesh�� ����������

/*
	! ReadMe

	���⿡�� Mesh,Instanced Shader, Texture, Material, MaterialColor, GameObject�� ����� ��Ȱ���Ѵ�.
	Default Setting
	1. Mesh       : fDefaultMeshSize ũ���� ���簢�� ť��޽ø� �����.
	2. Texture    : DEFAULT_TEXTURE_PATH ��ο� �ִ� �ؽ��ĸ� �ε��Ѵ�.
	3. Material   : �Ͼ���� �⺻ �ؽ��ĸ� ���� Material�̴�.
	4. GameObject : 0,0,0�� �ִ� �⺻ Object�̴�. Componet�� ����.
	5  Shader     : Instanced Shader�ν� ���� �������� �ʴ´ٸ� �⺻ ���� ���� �͵��� ���ִ�.

	//KYT '16.01.21 plus
	-> ����
	Mesh, Texture�� ���� InstanceShader, Material ���� �Ǿ������Ƿ� ���� ������ ����
	��� Texture, Shader�� vContainer �� ����ְ�, 0���� Default��.

	-> �߰�
	Default Shader�� Dynamic��

*/


//KYT '16.01.21 plus
enum SHADER_TYPE
{
	SHADER_TYPE_STATIC = 0,
	SHADER_TYPE_DYNAMIC = 1,
	SHADER_TYPE_INPUT = 2,
};


// Defulat Texture Path Need
// "../Assets/03_Image/Miscellaneous/Wood01.jpg"
//#define DEFAULT_TEXTURE_PATH "../Assets/Voxel/DefaultImage.png"
#define DEFAULT_TEXTURE_PATH "../Assets/untitled.png"
#define DEFAULT_NORMA_TEXTURE_PATH "../Assets/normal.png"


class CMesh;
class CMaterialColor;
class CTexture;
class CMaterial;
class CMaterialColors;
class CGameObject;
class CInstancedObjectsShader;
class CMatrixShader;
class CUIShader;
class CBlendShader;

class CContentManager : public CSingleTonBase<CContentManager>
{
private:
	bool										m_bLoadingInit;

private:
	ID3D11SamplerState*							m_pd3dSamplerState;
	std::vector<ID3D11SamplerState*>			m_vSamplerState;
	float									    m_fDefaultMeshSize;


	//KYT '16.01.18 minus 
	/*
	��Ƽ �������� �Ҷ��� Device�� �� �����忡 �°� �����ؾ� ������
	�� �̿ܿ��� �̰ŷ� ����� �� �ֵ�.
	*/
	ID3D11Device*							    m_pd3dDevice;

private:
	//KYT '16.01.19 minus 
	/* Mesh�� �ϳ��� ��� �ϹǷ� */
	//std::vector<std::shared_ptr<CMesh>>	   vpMesh;

	//KYT '16.01.19 minus 
	/* Į��� �ϳ��� ����Ұ� ���� */
	std::shared_ptr<CMaterialColors>		   m_pSharedColor;

	//KYT '16.01.21 minus 
	/* Texture�� Material�� 1:1������*/
	//std::vector<
	//	std::shared_ptr<CTexture>>			   m_vTexture;

	//KYT '16.01.21 plus 
	/* ������ �������� map���� ��ȯ�Ѵ� */
	std::vector<
		std::shared_ptr<CMaterial >> m_vMaterial;

	//KYT '16.01.21 minus 
	/* Default Shader�� vShader�� 0���� */
	//std::shared_ptr<CInstancedObjectsShader>  m_pSharedShader;


	//KYT '16.01.20 plus 
	/*
	�ν��Ͻ� ���̴��� ������ �����.
	���⿡ static, dynamic shader �߰��Ѵ�.
	*/
	
	std::vector<
		std::shared_ptr<CInstancedObjectsShader >> m_vStaticShader;

	std::vector<
		std::shared_ptr<CInstancedObjectsShader >> m_vDynamicShader;

	std::vector<
		std::shared_ptr<CBlendShader>> m_vBlendShader;

	std::vector<
		std::shared_ptr<CUIShader >> m_vUIShdaer;

	std::vector<
		std::shared_ptr<CMatrixShader>> m_vMatrixShdaer;

	/*
		ContentManager���� ���� ������ ������ �ִ� Shader��
	*/
	//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	//KYT '16.04.30
	/*
		StaticTerrainObject
	*/
	std::vector<
		std::shared_ptr<CInstancedObjectsShader >> m_vStaticTerrainShader;

	//KYT '16.01.22 plus 
	/*
		Input �����и� �޴� Shader�� ���� ������ �Ѵ�.
		���� �� Shader�� ������ �׷�����.
		������Ʈ�� ������ ���� �ʴ� ������ Componet ��Ģ�� ���� �ʱ� ���ؼ��̴�.
		 ex) Player 
	*/
	//std::vector<
	//	std::shared_ptr<CUIShader >> m_vInputShader;


public:

	CContentManager();

	~CContentManager();

	void ReleseInstance();

	void Load(ID3D11Device *pd3dDevice);

	void FinalLoad(ID3D11Device *pd3dDevice);

	void UILoad(ID3D11Device *pd3dDevice);

	void ReSetShader(ID3D11Device *pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext);

	//Get Default

	//-------------------------------------------------------------------------------
	std::unique_ptr<CMesh> GetGetDefaultShaderMesh(float fSize = 1);

	std::unique_ptr<CMesh> GetGetDefaultShaderMesh(ID3D11Device *pd3dDevice, float fSize = 0);

	std::shared_ptr<CMaterialColors> GetColor();

	//KYT '16.01.21 plus 
	/* Default�� 0���̴ϱ� */
	std::shared_ptr<CMaterial> GetDefaultMaterial();

	//KYT '16.01.18 plus 
	/*�⺻ Object�� ���� ���� �����Ѵ�.. */
	//�׋� �׋� �����Ѵ�.
	std::shared_ptr<CGameObject> GetDefaultGameObject();

	//KYT '16.01.20 plus 
	/*�⺻ Material�� Return�Ѵ�. */
	std::shared_ptr<CInstancedObjectsShader> GetDefaultShader();

	//-------------------------------------------------------------------------------



	//KYT '16.01.20 plus 
	/*Shader ���͸� �����Ѵ�. �� ���ʹ� Animate �� �� ���ȴ�... */
	std::vector<std::shared_ptr<CInstancedObjectsShader>>
		GetShaderContainer(SHADER_TYPE type);

	std::vector<std::shared_ptr<CInstancedObjectsShader>>
		GetStaticShaderContainer(){ return m_vStaticShader; }

	std::vector<std::shared_ptr<CInstancedObjectsShader>>
		GetDynamicShaderContainer(){ return m_vDynamicShader; }

	std::vector<std::shared_ptr<CBlendShader>>
		GetBlendShaderContainer() { return m_vBlendShader; }

	std::vector<std::shared_ptr<CInstancedObjectsShader>>
		GetAnimateObjectShaderContainer(int i)
	{
		if (i == 0)  return m_vDynamicShader;
		else		return m_vStaticShader;
	}

	//KYT '16.02.24 
	/* UI Shader */
	std::vector<std::shared_ptr<CUIShader>>
		GetUIShaderContainer(){ return m_vUIShdaer; }

	//KYT '16.02.27 
	/*
	  Matrix Shader 
	 */
	std::vector<std::shared_ptr<CMatrixShader>>
		GetMatrixShaderContainer(){ return m_vMatrixShdaer; }


	std::vector<std::shared_ptr<CInstancedObjectsShader>>
		GetStaticTerrainShaderContainer() { return m_vStaticTerrainShader; }

	//KYT '16.01.21 plus & '16.03.05
	/*
	Material  �����Ѵ�. �� ���׸����� move�� �Ѱ��ش�.
	& SamplerState�� ���� �� �����ϰ� ����ϱ� ���� �����Ͽ���.
	*/
	std::shared_ptr<CMaterial>
		GetMaterial
		(
		ID3D11Device* pd3dDevice,					//Device
		std::string path,							//���, vPath�� �� �� �ִ�.
		int nStartSampler = PS_SLOT_SAMPLER,        //StartSampler Number, Defautl�� �ִ�.
		int nCountSampler = 1,					   //Sampler�� ����
		YT_SAMPLER eSampler = SAMPLESTATE_DEFAULT  //Sampleer Cotainer���� ������ ���� ����. Default�� �ִ�.
		);

	//KYT '16.02.28 
	/*Material �������� �ؽ��ĸ� ���� */
	std::shared_ptr<CMaterial>
		GetMaterial(ID3D11Device* pd3dDevice, std::vector<std::string> vPath, int nStartSampler = PS_SLOT_SAMPLER, int nCountSampler = 1, YT_SAMPLER eSampler = SAMPLESTATE_DEFAULT);

	std::shared_ptr<CMaterial> 
		GetUIMaterial(ID3D11Device * pd3dDevice, std::string path, int nStartSampler = PS_SLOT_SAMPLER, int nCountSampler = 1, YT_SAMPLER eSampler = SAMPLESTATE_DEFAULT);

	//�̰� �� Ŭ���� �������� ����ϴ� private �Լ�
private:
	//KYT '16.01.21 change 
	/*Textur�� Material�� 1:1�� �����Ǿ��ִ�.*/
	std::shared_ptr<CTexture> DefaultGetTexture(ID3D11Device* pd3dDevice, std::string name = "Default");

	//KYT '16.01.21 change 
	std::shared_ptr<CMaterial> MakeMaterial(ID3D11Device* pd3dDevice, std::string name, int nStartSampler = PS_SLOT_SAMPLER, int nCountSampler = 1, YT_SAMPLER eSampler = SAMPLESTATE_DEFAULT);

	//KYT '16.02.28 
	/* Material �������� �ؽ��ĸ� ���� */
	std::shared_ptr<CMaterial> MakeMaterial(ID3D11Device* pd3dDevice, std::vector<std::string> vPath, int nStartSampler = PS_SLOT_SAMPLER, int nCountSampler = 1, YT_SAMPLER eSampler = SAMPLESTATE_DEFAULT);

	//KYT '16.07.23 change 
	std::shared_ptr<CMaterial> MakeUIMaterial(ID3D11Device* pd3dDevice, std::string name, std::string sPath, int nStartSampler = PS_SLOT_SAMPLER, int nCountSampler = 1, YT_SAMPLER eSampler = SAMPLESTATE_DEFAULT);



	//template Return
public:
	//KYT '16.01.20 plus 
	/*Material  �����Ѵ�. �� ���׸����� move�� �Ѱ��ش�. */
	template<class Mesh>
	std::unique_ptr<Mesh> MakeMetaMesh(ID3D11Device* pd3dDevice, float fSize)
	{
		if (fSize != 0) m_fDefaultMeshSize = fSize;
		return move(std::make_unique<Mesh>(pd3dDevice, m_fDefaultMeshSize, m_fDefaultMeshSize, m_fDefaultMeshSize));
	}

	//KYT '16.02.07 plus 
	/*Material  �����Ѵ�. �� ���׸����� move�� �Ѱ��ش�. */
	template<class Mesh>
	std::unique_ptr<Mesh> MakeMetaMesh(ID3D11Device* pd3dDevice, float xSize = 1.0f, float ySize = 1.0f, float zSize = 1.0f)
	{
		if (fSize != 0) m_fDefaultMeshSize = fSize;
		return move(std::make_unique<Mesh>(pd3dDevice, xSize, ySize, zSize));
	}


	template<class StaticShader>
	std::shared_ptr<CInstancedObjectsShader> MakeStaticShaderFactoty(ID3D11Device* pd3dDevice, std::string name)
	{
		for (auto &pShader : m_vStaticShader)
		{
			if (pShader->name == name)
				return pShader;
		}

		CShaderFactory<StaticShader> pShaderCreator;
		auto pStaticShdaer = pShaderCreator.Create();
		pStaticShdaer->CreateShader(pd3dDevice);
		pStaticShdaer->name = name;
		m_vStaticShader.push_back(pStaticShdaer);

		return move(pStaticShdaer);
	}

	template<class DynamicShader>
	std::shared_ptr<CInstancedObjectsShader> MakeDynamicShaderFactoty(ID3D11Device* pd3dDevice, std::string name)
	{
		for (auto &pShader : m_vDynamicShader)
		{
			if (pShader->name == name)
				return pShader;
		}
		CShaderFactory<DynamicShader> pShaderCreator;
		auto pDynamicShader = pShaderCreator.Create();
		pDynamicShader->CreateShader(pd3dDevice);
		pDynamicShader->name = name;

		m_vDynamicShader.push_back(pDynamicShader);

		return (move(pDynamicShader));
	}

	template<class BlendShader>
	std::shared_ptr<CBlendShader> MakeBlendShaderFactoty(ID3D11Device* pd3dDevice, std::string name)
	{
		for (auto &pShader : m_vBlendShader)
		{
			if (pShader->name == name)
				return pShader;
		}
		CBlendShaderShaderFactory<BlendShader> pShaderCreator;
		auto pBlendShader = pShaderCreator.Create();
		pBlendShader->CreateShader(pd3dDevice);
		pBlendShader->name = name;
		m_vBlendShader.push_back(pBlendShader);

		return (move(pBlendShader));
	}

	template<class MatrixShader>
	std::shared_ptr<CMatrixShader> MakeMatrixShaderFactoty(ID3D11Device* pd3dDevice, std::string name)
	{
		for (auto &pShader : m_vMatrixShdaer)
		{
			if (pShader->name == name)
				return pShader;
		}
		CMatrixShaderFactory<MatrixShader> pShaderCreator;
		auto pMatrixShader = pShaderCreator.Create();
		pMatrixShader->CreateShader(pd3dDevice);
		pMatrixShader->name = name;
		m_vMatrixShdaer.push_back(pMatrixShader);
		return (move(pMatrixShader));
	}

	template<class StaticTerrainShader>
	std::shared_ptr<CInstancedObjectsShader> MakeStaticTerrainShaderFactoty(ID3D11Device* pd3dDevice, std::string name)
	{
		for (auto &pShader : m_vStaticTerrainShader)
		{
			if (pShader->name == name)
				return pShader;
		}
		CShaderFactory<StaticTerrainShader> pShaderCreator;
		auto pStaticTerrainShader = pShaderCreator.Create();
		pStaticTerrainShader->CreateShader(pd3dDevice);
		pStaticTerrainShader->name = name;
		m_vStaticTerrainShader.push_back(pStaticTerrainShader);
		return (move(pStaticTerrainShader));
	}

	std::shared_ptr<CUIShader> MakeUIShaderFactoty(ID3D11Device* pd3dDevice);

	private:
		template<class VariousShader, class meshType>
		void MakeVariousShaderFactoty(ID3D11Device* pd3dDevice, std::string name, std::string meshPath, float meshScale = 0.01f, float fbxType = SH_FBXTYPE::FBX_VOXEL)
		{
			for (auto &pShader : m_mVariousShader)
			{
				if (pShader.first == name)
					return;
			}
			CShaderFactory<VariousShader> pShaderCreator;
			auto pShader = pShaderCreator.Create();
			pShader->CreateShader(pd3dDevice);
			pShader->SetMesh(move(std::make_unique<meshType>(pd3dDevice, name, meshScale, fbxType)));
			pShader->name = name;
			m_mVariousShader.insert(std::make_pair(name, std::move(pShader)));
		}


	//KYT '16.04.03 plus
	/*
	CSV Map Data
	*/
private:
	int**							m_pMapArrary;

public:
	int** GetMapArrary()			{ return m_pMapArrary; }

	int GetMapArrary(int x, int z)	{ return m_pMapArrary[z][x]; }

	//KYT '16.07.22 plus
	/*
		BlendState 
	*/
	ID3D11BlendState*							m_pd3dBlendingBlend;

	std::map<std::string,ID3D11BlendState*>		m_mapBlendingBlend;

	void LoadBlendState(ID3D11Device* pd3dDevice);

	void AddBlendState
	(
			ID3D11Device*	pd3dDevice
		,	std::string		name
		,	bool			AlphaToCoverageEnable
		,	bool			IndependentBlendEnable
		,	bool			BlendEnable
		,	UINT			SrcBlend
		,	UINT			DestBlend
		,	UINT			BlendOp
		,	UINT			SrcBlendAlpha
		,	UINT			DestBlendAlpha
		,	UINT			BlendOpAlpha
		,	UINT			RenderTargetWriteMask
	);

	ID3D11BlendState* FindBlendState(std::string str);

	void OMSetBlendingState(ID3D11DeviceContext *pd3dDeviceContext, std::string name);

	void ModifyBlendState();

	void DebugginBlendingState(ID3D11DeviceContext *pd3dDeviceContext, bool start = true);

//private:
//	std::vector
//		<std::shared_ptr<CInstancedObjectsShader>>	m_vMonsterShader;
//
//public:
//	CInstancedObjectsShader* GetMonsterShader(int n) const { return m_vMonsterShader[n].get(); }
//
//	std::vector
//		<std::shared_ptr<CInstancedObjectsShader>> GetMonsterShader() const { return m_vMonsterShader; }



};


