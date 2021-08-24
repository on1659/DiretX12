#pragma once

//Loadµîµî

class CUIShader;

class CUIManager : public CSingleTonBase<CUIManager>
{

private:
	/*
		class CUIGameObject : public CGameObject
	{
	private:
		CUIShader*				

	public: 
		
		CUIGameObject(int nMeshes = 0) : CGameObject(nMeshes)
		{

		}
		CUIGameObject(std::string name, UINT tag = 0) : CGameObject(name, tag)
		{

		}
		virtual ~CUIGameObject()
		{

		}
	};
	*/

	struct UIShaderStruct
	{
		std::string name;
		std::string path;
		int			left;
		int			top;
		int			w;
		int			h;
		bool		blend;

		UIShaderStruct(std::string name, std::string path, int left, int top, int width, int height, bool blend = false) : name(name), path(path), left(left), top(top), w(width), h(height), blend(blend) 
		{
		}
	};


private:
	std::multimap<std::string, std::shared_ptr<CUIShader>>	m_mUIShader;
	std::multimap<std::string, std::shared_ptr<CUIShader>>	m_mVisibleShader;
	std::multimap<std::string, std::shared_ptr<CUIShader>>	m_mBlendingShader;
	ID3D11Device*											m_pd3dDevice;
	
	void AddShader(bool active, UIShaderStruct ui, std::string strBlendDesc = "");

public:
	CUIManager();
	~CUIManager();

	void ReleseInstance();

	void Load(ID3D11Device* pd3dDevice);
	void FinalLoad();

	template<class TemplateComponet>
	void AddItemShader(bool active, UIShaderStruct ui, int slot, std::unique_ptr<TemplateComponet> componet)
	{
		auto pUIShader = std::make_shared<CUIShader>();
		pUIShader->CreateShader(m_pd3dDevice);
		auto pMesh = std::make_unique<CUIMesh>(m_pd3dDevice, ui.w, ui.h);
		pUIShader->SetMesh(move(pMesh));
		pUIShader->SetMaterial(m_pd3dDevice, ui.path);

		pUIShader->SetPosition(ui.left, ui.top, ui.w, ui.h);
		pUIShader->SetUIShader(pUIShader);

		pUIShader->SetComponet(move(componet));

		pUIShader->BuildObjects(m_pd3dDevice);

		
		string str = "0";
		str += (slot +48);
		str.append(ui.name);
		ui.name = str;
		
		pUIShader->name.append(ui.name);

		if (active)
			m_mVisibleShader.insert(make_pair(ui.name, pUIShader));
		else
			m_mUIShader.insert(make_pair(ui.name, pUIShader));
	}
	
	bool SetComponet(std::string name, std::unique_ptr<Component> componet);

	void Render(ID3D11DeviceContext* pd3dDeivceContext);

	void RenderTransparent(ID3D11DeviceContext* pd3dDeivceContext);

	void Animate(float fTimeElapsed);

	CUIShader* UseShader(std::string uiName);

	void EndUseShader(std::string uiName);


	CUIShader* UseItemUiShader(int slot, std::string uiName);

	void EndUseItemUiShader(int slot, std::string uiName);

	//void AddShader(bool active, UIShaderStruct ui, std::unique_ptr<Component> ...);
};

