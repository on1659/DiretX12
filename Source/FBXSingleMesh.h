#pragma once

class Mesh;
class FBXModelLoader;
class FBXAnimationModelLoader;

class CFBXSingleMeshTexturedIlluminated : public CMeshTexturedIlluminated
{
public:
	CFBXSingleMeshTexturedIlluminated(ID3D11Device *pd3dDevice, std::string path, float modelscale = 1.0f, bool isVoxel = false);

	virtual ~CFBXSingleMeshTexturedIlluminated();

private:
	FBXModelLoader* m_pfbxModel;
};