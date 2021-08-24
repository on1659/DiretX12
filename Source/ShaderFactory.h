#pragma once

#include <memory>


//KYT '16.01.22 plus 
/* 
	GSCubeShader �� �����ϸ� �ν��� ����. 
	���� �ذ�� ���
	-> ���� �ذ��. ������ Factoryó�� ��
*/

class CMatrixShader;
class CInstancedObjectsShader;
class CBlendShader;

//---------------------------------------------- InstanceShader ---------------------
class CInstancedShaderFactoryAbstract
{
public:
	std::shared_ptr<CInstancedObjectsShader> Create(){ return ShaderCreate(); }

protected:
	virtual std::shared_ptr<CInstancedObjectsShader> ShaderCreate() = 0;
};

template<class ShaderFactory>
class CShaderFactory : public CInstancedShaderFactoryAbstract
{
private:
	virtual std::shared_ptr<CInstancedObjectsShader> ShaderCreate() { return std::make_shared<ShaderFactory>(); }
};
//---------------------------------------------- InstanceShader ---------------------



//---------------------------------------------- MatrixShader ---------------------

class CMatrixhaderFactoryAbstract
{
public:
	std::shared_ptr<CMatrixShader> Create(){ return ShaderCreate(); }

protected:
	virtual std::shared_ptr<CMatrixShader> ShaderCreate() = 0;
};

template<class ShaderFactory>
class CMatrixShaderFactory : public CMatrixhaderFactoryAbstract
{
private:
	virtual std::shared_ptr<CMatrixShader> ShaderCreate(){ return std::make_shared<ShaderFactory>(); }
};

//---------------------------------------------- MatrixShader ---------------------




//---------------------------------------------- BlendShader ---------------------

class CBlendFactoryAbstract
{
public:
	std::shared_ptr<CBlendShader> Create() { return ShaderCreate(); }

protected:
	virtual std::shared_ptr<CBlendShader> ShaderCreate() = 0;
};

template<class ShaderFactory>
class CBlendShaderShaderFactory : public CBlendFactoryAbstract
{
private:
	virtual std::shared_ptr<CBlendShader> ShaderCreate() { return std::make_shared<ShaderFactory>(); }
};

//---------------------------------------------- BlendShader ---------------------
