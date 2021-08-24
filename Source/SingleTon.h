#pragma once

/*
---------------------------------------------------------------------------------------
ΩÃ±€≈Ê ≈€«√∏¥ ≈¨∑πΩ∫
---------------------------------------------------------------------------------------
*/

template <class ClassName>
class CSingleTonBase : public Object
{
private:
	static ClassName* m_sInstance;
	

protected:
	std::string m_name;
	

	CSingleTonBase(){}

	~CSingleTonBase(){};


public:
	static ClassName* GetInstance()
	{
		if (nullptr == m_sInstance)
			return m_sInstance = new ClassName;

		return m_sInstance;
	}

	void ReleseInstance()
	{
		if (m_sInstance)
		{
			delete m_sInstance;
			m_sInstance = nullptr;
		}
	}
};

//ΩÃ±€≈œ ¿ŒΩ∫≈œΩ∫ √ ±‚»≠
template <class ClassName>
ClassName* CSingleTonBase<ClassName>::m_sInstance = nullptr;
