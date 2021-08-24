#pragma once

class CScreenShader;

#ifdef _MULIT_THREAD_LOADING_

	extern std::shared_ptr<CScreenShader> gpTextureToScreenShader;

	UINT WINAPI LoadingThreadLoad(LPVOID lpParam);

	void WINAPI PrepareManagerLoading(LPVOID lpParam);

	void WINAPI FinalManagerLoading(LPVOID lpParam);

	void ProgramingExit();

	void WINAPI ReleaseManager(LPVOID lpParam);

	void DimensionsChange(YT_GameMode mode, float delay = 0.0f);

	void RecycleThreadLoad(ID3D11Device* pd3dDevice);

#endif // _MULIT_THREAD_LOADING_
