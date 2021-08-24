////////////////////////////////////////////////////////////////////////////////
// Filename: FBXLoader.cpp
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "FBXLoader.h"

FBXModelLoader::FBXModelLoader(std::string filename, float modelScale, bool isVoxel)
	: m_isVoxel(isVoxel)
	, m_manager(nullptr)
{
	//m_pMeshNodeInfo = new FBXSDK_MESH_NODE();
	//m_pMeshNodeInfo->Re lease();

	m_pMeshNodeInfo.clear();

	CreateFBXScene(filename);

	ImportFBX(filename);

	//DirectX방식의 축으로 변환
	FbxAxisSystem OurAxisSystem = FbxAxisSystem::DirectX;

	FbxAxisSystem SceneAxisSystem = m_scene->GetGlobalSettings().GetAxisSystem();

	if (SceneAxisSystem != OurAxisSystem)
	{
		FbxAxisSystem::DirectX.ConvertScene(m_scene);
	}

	FbxSystemUnit SceneSystemUnit = m_scene->GetGlobalSettings().GetSystemUnit();
	if (SceneSystemUnit.GetScaleFactor() != 1.0)
	{
		FbxSystemUnit::cm.ConvertScene(m_scene);
	}

#ifdef FBX_GLOBAL_MANAGER
	FbxGeometryConverter GeomConverter(g_FBXManager);
#else
	FbxGeometryConverter GeomConverter(m_manager);
#endif
	GeomConverter.Triangulate(m_scene, true);

	SetModelScale(modelScale);

	m_fileName = filename;

	m_getfirstPos = false;
	m_highestX = 0.0f;
	m_lowestX = 0.0f;
	m_highestY = 0.0f;
	m_lowestY = 0.0f;
	m_highestZ = 0.0f;
	m_lowestZ = 0.0f;
}

FBXModelLoader::FBXModelLoader(FbxScene* scene, float modelScale, bool isVoxel)
	: m_isVoxel(isVoxel)
{
	m_scene = scene;


	SetModelScale(modelScale);

	m_fileName = nullptr;
}

FBXModelLoader::~FBXModelLoader()
{
	m_pMeshNodeInfo.clear();

	if (m_importer)
	{
		m_importer->Destroy();
		m_importer = nullptr;
	}

	if (m_scene)
	{
		m_scene->Destroy();
		m_scene = nullptr;
	}
#ifndef FBX_GLOBAL_MANAGER
	if (m_manager)
	{
		m_manager->Destroy();
		m_manager = nullptr;
	}
#endif
}

void FBXModelLoader::CreateFBXScene(std::string filename)
{
	#ifdef FBX_GLOBAL_MANAGER
		if (!bManagerSetting)
		{
			g_FBXManager = FbxManager::Create();
			g_FBXIOsettings = FbxIOSettings::Create(g_FBXManager, IOSROOT);
			g_FBXManager->SetIOSettings(g_FBXIOsettings);
			bManagerSetting = true;
		}
		m_scene = FbxScene::Create(g_FBXManager, filename.c_str());
	#else
	try {

		m_manager = FbxManager::Create();
	}
	catch (int e)
	{
		return;
	}
		m_ioSettings = FbxIOSettings::Create(m_manager, IOSROOT);

		m_manager->SetIOSettings(m_ioSettings);

		m_scene = FbxScene::Create(m_manager, "tempName");
	
	#endif
}

void FBXModelLoader::ImportFBX(std::string path)
{
	#ifdef FBX_GLOBAL_MANAGER
		m_importer = FbxImporter::Create(g_FBXManager, "");
		if (!m_importer->Initialize(path.c_str(), -1, g_FBXManager->GetIOSettings()))
		{
			std::string ErrorTitle = "FBX Import Error";
			GMessageBox(ErrorTitle, path);
		}
	#else
		m_importer = FbxImporter::Create(m_manager, "");
		if (!m_importer->Initialize(path.c_str(), -1, m_manager->GetIOSettings()))
		{
			std::string ErrorTitle = "FBX Import Error";
			GMessageBox(ErrorTitle, path);
		}
	#endif
		m_importer->Import(m_scene);
		m_importer->Destroy();
}

void FBXModelLoader::ProcessMeshInfo()
{
	FbxNode* rootNode = m_scene->GetRootNode();
	if (rootNode) { this->TraversalFBXTree_forMesh(rootNode); }
}

void FBXModelLoader::ProcessMeshInfo(FbxScene* scene)
{
	FbxNode* rootNode = scene->GetRootNode();
	if (rootNode) { this->TraversalFBXTree_forMesh(rootNode); }
}

void  FBXModelLoader::TraversalFBXTree_forMesh(FbxNode* Node)
{
	int numChild = Node->GetChildCount();
	FbxNode *childNode = nullptr;

	if (0 != m_pMeshNodeInfo.size())
	{
		m_pMeshNodeInfo.clear();
	}

	for (int i = 0; i < numChild; i++)
	{
		childNode = Node->GetChild(i);

		FbxMesh *childMesh = childNode->GetMesh();

		if (childMesh != nullptr)
		{

			FBXSDK_MESH_NODE* tempMeshNodeInfo = new FBXSDK_MESH_NODE();
			tempMeshNodeInfo->Release();

			tempMeshNodeInfo->m_polygonCount = childMesh->GetPolygonCount();
			//=================================================================================================================
			//=================================================================================================================

			//정점
			LoadVertexInformation(childMesh, tempMeshNodeInfo);

			//인덱스
			//주의!!!! 이 인덱스는 정점 순서를 표현하는 것이지 노멀&UV에 순서를 표현하는 것이 아님니다!!!!!!!
			//std::cout << "\n#인덱스 추출";
			tempMeshNodeInfo->m_numIndices = childMesh->GetPolygonVertexCount();
			tempMeshNodeInfo->m_indices = new int[tempMeshNodeInfo->m_numIndices];
			tempMeshNodeInfo->m_indices = childMesh->GetPolygonVertices();

			//=================================================================================================================
			//=================================================================================================================

			//std::cout << "\n" << "#노멀값";
			//노멀값
			LoadNormalInformation(childMesh, tempMeshNodeInfo);

			//=================================================================================================================
			//=================================================================================================================


			//std::cout << "\n" << "#텍스쳐UV";
			//텍스쳐 uv
			LoadUVInformation(childMesh, tempMeshNodeInfo);

			//=================================================================================================================
			//=================================================================================================================

			m_pMeshNodeInfo.push_back(*tempMeshNodeInfo);

			delete tempMeshNodeInfo;
		}
	}
}

void FBXModelLoader::LoadVertexInformation(FbxMesh* pMesh, FBXSDK_MESH_NODE* tempMeshNodeInfo)
{
	int numVerts = pMesh->GetControlPointsCount();

	for (unsigned int i = 0; i < numVerts; ++i)
	{
		FBX_DATATYPE3 currPosition;
		if (m_isVoxel)
		{
			currPosition.x = static_cast<float>(pMesh->GetControlPointAt(i).mData[0] * m_modelScale);
			currPosition.y = static_cast<float>(pMesh->GetControlPointAt(i).mData[2] * m_modelScale);
			currPosition.z = static_cast<float>(pMesh->GetControlPointAt(i).mData[1] * m_modelScale);
		}
		else
		{
			currPosition.x = static_cast<float>(pMesh->GetControlPointAt(i).mData[0] * m_modelScale);
			currPosition.y = static_cast<float>(pMesh->GetControlPointAt(i).mData[1] * m_modelScale);
			currPosition.z = static_cast<float>(pMesh->GetControlPointAt(i).mData[2] * m_modelScale);
		}

		if (m_getfirstPos)
		{
			if (m_highestX < currPosition.x) { m_highestX = currPosition.x; }
			if (m_lowestX > currPosition.x) { m_lowestX = currPosition.x; }
			if (m_highestY < currPosition.y) { m_highestY = currPosition.y; }
			if (m_lowestY > currPosition.y) { m_lowestY = currPosition.y; }
			if (m_highestZ < currPosition.z) { m_highestZ = currPosition.z; }
			if (m_lowestZ > currPosition.z) { m_lowestZ = currPosition.z; }
		}
		else {
			m_highestX = currPosition.x;
			m_lowestX = currPosition.x;

			m_highestY = currPosition.y;
			m_lowestY = currPosition.y;

			m_highestZ = currPosition.z;
			m_lowestZ = currPosition.z;
			m_getfirstPos = true;
		}
		

		tempMeshNodeInfo->m_positions.push_back(currPosition);
	}
}

void FBXModelLoader::LoadNormalInformation(FbxMesh* pMesh, FBXSDK_MESH_NODE* tempMeshNodeInfo)
{
	FbxGeometryElementNormal* normalElement = pMesh->GetElementNormal();
	if (normalElement)
	{
		if (FbxGeometryElement::eByControlPoint == normalElement->GetMappingMode())
		{
			for (int vertexIndex = 0; vertexIndex < pMesh->GetControlPointsCount(); vertexIndex++)
			{
				int normalIndex = 0;

				//reference mode is direct, the normal index is same as vertex index.
				//get normals by the index of control vertex
				if (normalElement->GetReferenceMode() == FbxGeometryElement::eDirect)
					normalIndex = vertexIndex;

				//reference mode is index-to-direct, get normals by the index-to-direct
				if (normalElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
					normalIndex = normalElement->GetIndexArray().GetAt(vertexIndex);

				//Got normals of each vertex.
				FbxVector4 getNormal = normalElement->GetDirectArray().GetAt(normalIndex);
				tempMeshNodeInfo->m_normals.push_back(FBX_DATATYPE3((float)getNormal[0], (float)getNormal[1], (float)getNormal[2]));
			}
		}
		else if (FbxGeometryElement::eByPolygonVertex == normalElement->GetMappingMode())
		{
			int indexByPolygonVertex = 0;

			//Let's get normals of each polygon, since the mapping mode of normal element is by polygon-vertex.
			for (int polygonIndex = 0; polygonIndex < tempMeshNodeInfo->m_polygonCount; polygonIndex++)
			{
				//get polygon size, you know how many vertices in current polygon.
				int polygonSize = pMesh->GetPolygonSize(polygonIndex);

				//retrieve each vertex of current polygon.
				for (int i = 0; i < polygonSize; i++)
				{
					int lNormalIndex = 0;
					//reference mode is direct, the normal index is same as indexByPolygonVertex.
					if (normalElement->GetReferenceMode() == FbxGeometryElement::eDirect)
						lNormalIndex = indexByPolygonVertex;

					//reference mode is index-to-direct, get normals by the index-to-direct
					if (normalElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
						lNormalIndex = normalElement->GetIndexArray().GetAt(indexByPolygonVertex);

					//Got normals of each polygon-vertex.
					FbxVector4 getNormal = normalElement->GetDirectArray().GetAt(lNormalIndex);
					tempMeshNodeInfo->m_normals.push_back(FBX_DATATYPE3((float)getNormal[0], (float)getNormal[1], (float)getNormal[2]));

					indexByPolygonVertex++;
				}
			}
		}
	}
}

void FBXModelLoader::LoadUVInformation(FbxMesh* pMesh, FBXSDK_MESH_NODE* tempMeshNodeInfo)
{
	//UV 이름 리스트를 가져온다. 이름은 UV Channel 1 식으로 나열.
	FbxStringList lUVSetNameList;
	pMesh->GetUVSetNames(lUVSetNameList);

	//iterating over all uv sets
	for (int lUVSetIndex = 0; lUVSetIndex < lUVSetNameList.GetCount(); lUVSetIndex++)
	{
		//get lUVSetIndex-th uv set
		const char* lUVSetName = lUVSetNameList.GetStringAt(lUVSetIndex);
		const FbxGeometryElementUV* lUVElement = pMesh->GetElementUV(lUVSetName);

		if (!lUVElement)
			continue;

		// only support mapping mode eByPolygonVertex and eByControlPoint
		if (lUVElement->GetMappingMode() != FbxGeometryElement::eByPolygonVertex &&
			lUVElement->GetMappingMode() != FbxGeometryElement::eByControlPoint)
			return;

		//index array, where holds the index referenced to the uv data
		const bool lUseIndex = lUVElement->GetReferenceMode() != FbxGeometryElement::eDirect;
		const int lIndexCount = (lUseIndex) ? lUVElement->GetIndexArray().GetCount() : 0;

		//iterating through the data by polygon
		const int lPolyCount = pMesh->GetPolygonCount();

		if (lUVElement->GetMappingMode() == FbxGeometryElement::eByControlPoint)
		{
			for (int lPolyIndex = 0; lPolyIndex < lPolyCount; ++lPolyIndex)
			{
				// build the max index array that we need to pass into MakePoly
				const int lPolySize = pMesh->GetPolygonSize(lPolyIndex);
				for (int lVertIndex = 0; lVertIndex < lPolySize; ++lVertIndex)
				{
					FbxVector2 lUVValue;

					//get the index of the current vertex in control points array
					int lPolyVertIndex = pMesh->GetPolygonVertex(lPolyIndex, lVertIndex);

					//the UV index depends on the reference mode
					int lUVIndex = lUseIndex ? lUVElement->GetIndexArray().GetAt(lPolyVertIndex) : lPolyVertIndex;

					lUVValue = lUVElement->GetDirectArray().GetAt(lUVIndex);
					
					float dataU = 0, dataV = 0;

					if ((float)lUVValue.mData[0] > 1.0f)
					{
						dataU = (float)lUVValue.mData[0] - (int)lUVValue.mData[0];
					}
					else if ((float)lUVValue.mData[0] < 0.0f)
					{
						dataU = 1.0f - ((float)lUVValue.mData[0] - (int)lUVValue.mData[0]);
					}
					else
					{
						dataU = (float)lUVValue.mData[0];
					}

					if ((float)lUVValue.mData[1] > 1.0f)
					{
						dataV = (float)lUVValue.mData[1] - (int)lUVValue.mData[1];
					}
					else if ((float)lUVValue.mData[1] < 0.0f)
					{
						dataV = 1.0f - ((float)lUVValue.mData[1] - (int)lUVValue.mData[1]);
					}
					else
					{
						dataV = (float)lUVValue.mData[1];
					}

					tempMeshNodeInfo->m_textureCoords.push_back(FBX_DATATYPE2(dataU, dataV));
				}
			}
		}
		else if (lUVElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
		{
			int lPolyIndexCounter = 0;
			for (int lPolyIndex = 0; lPolyIndex < lPolyCount; ++lPolyIndex)
			{
				// build the max index array that we need to pass into MakePoly
				const int lPolySize = pMesh->GetPolygonSize(lPolyIndex);
				for (int lVertIndex = 0; lVertIndex < lPolySize; ++lVertIndex)
				{
					if (lPolyIndexCounter < lIndexCount)
					{
						FbxVector2 lUVValue;

						//the UV index depends on the reference mode
						int lUVIndex = lUseIndex ? lUVElement->GetIndexArray().GetAt(lPolyIndexCounter) : lPolyIndexCounter;

						lUVValue = lUVElement->GetDirectArray().GetAt(lUVIndex);

						float dataU = 0, dataV = 0;

						if ((float)lUVValue.mData[0] > 1.0f)
						{
							dataU = (float)lUVValue.mData[0] - (int)lUVValue.mData[0];
						}
						else if((float)lUVValue.mData[0] < 0.0f)
						{
							dataU = 1.0f + ((float)lUVValue.mData[0] - (int)lUVValue.mData[0]);
						}
						else
						{
							dataU = (float)lUVValue.mData[0];
						}

						if ((float)lUVValue.mData[1] > 1.0f)
						{
							dataV = (float)lUVValue.mData[1] - (int)lUVValue.mData[1];
						}
						else if ((float)lUVValue.mData[1] < 0.0f)
						{
							dataV = 1.0f + ((float)lUVValue.mData[1] - (int)lUVValue.mData[1]);
						}
						else
						{
							dataV = (float)lUVValue.mData[1];
						}

						tempMeshNodeInfo->m_textureCoords.push_back(FBX_DATATYPE2((float)dataU, (float)dataV));

						lPolyIndexCounter++;
					}
				}
			}
		}
	}
}




FBXAnimationModelLoader::FBXAnimationModelLoader(std::string filename, float modelScale, bool isVoxel)
	: FBXModelLoader(filename, modelScale, isVoxel)
{
	//애니메이션 스택명 저장
	m_scene->FillAnimStackNameArray(m_animStackNameArray);

	//AnimationLayerData animdata;

	FbxAnimStack* currAnimStack = m_scene->GetSrcObject<FbxAnimStack>(0);
	FbxString animStackName = currAnimStack->GetName();
	FbxTakeInfo* takeInfo = m_scene->GetTakeInfo(animStackName);
	m_animTimeStart = takeInfo->mLocalTimeSpan.GetStart();
	m_animTimeEnd = takeInfo->mLocalTimeSpan.GetStop();
	m_animTimeLength = m_animTimeEnd.GetFrameCount(FBX_FPS)
		- (m_animTimeStart.GetFrameCount(FBX_FPS) + 1);

	m_path = filename;
}

FBXAnimationModelLoader::~FBXAnimationModelLoader()
{
	FBXModelLoader::~FBXModelLoader();
}

void FBXAnimationModelLoader::ProcessAnimationInfo()
{
	// 본(Bone) 계층구조
	ProcessSkeletonHierarchy(m_scene->GetRootNode());

	FbxNode* rootNode = m_scene->GetRootNode();
	if (rootNode) {
		this->TraversalFBXTree_forAnimation(rootNode);
	}
}

void FBXAnimationModelLoader::ProcessSkeletonHierarchy(FbxNode* inRootNode)
{
	for (int childIndex = 0; childIndex < inRootNode->GetChildCount(); ++childIndex)
	{
		FbxNode* currNode = inRootNode->GetChild(childIndex);
	
		// 본(Bone)의 최상위의 인덱스는 -1 단계별로 뻗어갈 때보다 +1
		ProcessSkeletonHierarchyRecursively(currNode, 0, 0, -1);
	}
	
#pragma region[플레이어 캐릭터(여성) 본체계 재설정 160423]
	if (m_fileName == "../Assets/02_ModelData/01_CHARACTER_MEI/Jeni.fbx"
		|| m_fileName == "../Assets/02_ModelData/01_CHARACTER_MEI/Jeni_idle.fbx"
		|| m_fileName == "../Assets/02_ModelData/01_CHARACTER_MEI/Jeni_running.fbx"
		|| m_fileName == "../Assets/02_ModelData/01_CHARACTER_MEI/Jeni_left_strafe.fbx"
		|| m_fileName == "../Assets/02_ModelData/01_CHARACTER_MEI/Jeni_right_strafe.fbx"
		|| m_fileName == "../Assets/02_ModelData/01_CHARACTER_MEI/Jeni_left_forward_diagonal.fbx"
		|| m_fileName == "../Assets/02_ModelData/01_CHARACTER_MEI/Jeni_right_forward_diagonal.fbx"
		|| m_fileName == "../Assets/02_ModelData/01_CHARACTER_MEI/Jeni_smash.fbx"
		|| m_fileName == "../Assets/02_ModelData/01_CHARACTER_MEI/Jeni_running_backward.fbx"
		|| m_fileName == "../Assets/02_ModelData/01_CHARACTER_MEI/Jeni_standing_react_death_forward.FBX")
	{
		std::vector<Joint> tmpJoints;
		
		tmpJoints = m_Skeleton.m_joints;
		
		m_Skeleton.m_joints.clear();
		
		m_Skeleton.m_joints.push_back(tmpJoints[14]);	//Head
		m_Skeleton.m_joints.push_back(tmpJoints[20]);	//HairC1
		m_Skeleton.m_joints.push_back(tmpJoints[19]);	//HairC
		m_Skeleton.m_joints.push_back(tmpJoints[18]);	//HairB
		m_Skeleton.m_joints.push_back(tmpJoints[17]);	//HairA
		m_Skeleton.m_joints.push_back(tmpJoints[13]);	//Neck
		
		m_Skeleton.m_joints.push_back(tmpJoints[21]);	//Ponytail
		m_Skeleton.m_joints.push_back(tmpJoints[22]);	//Ponytail1
		m_Skeleton.m_joints.push_back(tmpJoints[23]);	//Ponytail2
		m_Skeleton.m_joints.push_back(tmpJoints[12]);	//Spine2
		
		m_Skeleton.m_joints.push_back(tmpJoints[15]);	//Bow_Left
		m_Skeleton.m_joints.push_back(tmpJoints[16]);	//Bow_Right
		
		m_Skeleton.m_joints.push_back(tmpJoints[49]);	//RightHand
		m_Skeleton.m_joints.push_back(tmpJoints[50]);	//RightHandThumb1
		m_Skeleton.m_joints.push_back(tmpJoints[53]);	//RightHandIndex1
		m_Skeleton.m_joints.push_back(tmpJoints[56]);	//RightHandMiddle1
		m_Skeleton.m_joints.push_back(tmpJoints[57]);	//RightHandMiddle2
		m_Skeleton.m_joints.push_back(tmpJoints[58]);	//RightHandMiddle3
		m_Skeleton.m_joints.push_back(tmpJoints[59]);	//RightHandRing1
		m_Skeleton.m_joints.push_back(tmpJoints[61]);	//RightHandRing3
		m_Skeleton.m_joints.push_back(tmpJoints[62]);	//RightHandPinky1
		m_Skeleton.m_joints.push_back(tmpJoints[64]);	//RightHandPinky3
		m_Skeleton.m_joints.push_back(tmpJoints[54]);	//RightHandIndex2
		m_Skeleton.m_joints.push_back(tmpJoints[55]);	//RightHandIndex3
		m_Skeleton.m_joints.push_back(tmpJoints[60]);	//RightHandRing2
		
		m_Skeleton.m_joints.push_back(tmpJoints[3]);	//LeftLeg
		m_Skeleton.m_joints.push_back(tmpJoints[4]);	//LeftFoot
		m_Skeleton.m_joints.push_back(tmpJoints[5]);	//LeftToeBase
		m_Skeleton.m_joints.push_back(tmpJoints[9]);	//RightToeBase
		m_Skeleton.m_joints.push_back(tmpJoints[1]);	//Pelvis
		m_Skeleton.m_joints.push_back(tmpJoints[2]);	//LeftUpLeg
		m_Skeleton.m_joints.push_back(tmpJoints[6]);	//RightUpLeg
		m_Skeleton.m_joints.push_back(tmpJoints[10]);	//Spine
		m_Skeleton.m_joints.push_back(tmpJoints[11]);	//Spine1
		m_Skeleton.m_joints.push_back(tmpJoints[7]);	//RightLeg
		m_Skeleton.m_joints.push_back(tmpJoints[8]);	//RightFoot
		m_Skeleton.m_joints.push_back(tmpJoints[70]);	//Belly
		m_Skeleton.m_joints.push_back(tmpJoints[24]);	//LeftShoulder
		m_Skeleton.m_joints.push_back(tmpJoints[46]);	//RightShoulder
		m_Skeleton.m_joints.push_back(tmpJoints[68]);	//Left_Breast
		m_Skeleton.m_joints.push_back(tmpJoints[45]);	//LeftUpArmCounterTwist
		m_Skeleton.m_joints.push_back(tmpJoints[71]);	//SkirtA
		m_Skeleton.m_joints.push_back(tmpJoints[72]);	//SkirtA1
		m_Skeleton.m_joints.push_back(tmpJoints[75]);	//SkirtB
		m_Skeleton.m_joints.push_back(tmpJoints[73]);	//SkirtC
		m_Skeleton.m_joints.push_back(tmpJoints[76]);	//SkirtB1
		m_Skeleton.m_joints.push_back(tmpJoints[44]);	//LeftUpArmTwist
		m_Skeleton.m_joints.push_back(tmpJoints[74]);	//SkirtC1
		m_Skeleton.m_joints.push_back(tmpJoints[67]);	//RightUpArmCounterTwist
		m_Skeleton.m_joints.push_back(tmpJoints[69]);	//Right_Breast
		m_Skeleton.m_joints.push_back(tmpJoints[66]);	//RightUpArmTwist
		
		m_Skeleton.m_joints.push_back(tmpJoints[27]);	//LeftHand
		m_Skeleton.m_joints.push_back(tmpJoints[28]);	//LeftHandThumb1
		m_Skeleton.m_joints.push_back(tmpJoints[43]);	//LeftWristTwist
		m_Skeleton.m_joints.push_back(tmpJoints[29]);	//LeftHandThumb2
		m_Skeleton.m_joints.push_back(tmpJoints[30]);	//LeftHandThumb3
		m_Skeleton.m_joints.push_back(tmpJoints[33]);	//LeftHandIndex3
		m_Skeleton.m_joints.push_back(tmpJoints[31]);	//LeftHandIndex1
		m_Skeleton.m_joints.push_back(tmpJoints[32]);	//LeftHandIndex2
		m_Skeleton.m_joints.push_back(tmpJoints[40]);	//LeftHandPinky1
		m_Skeleton.m_joints.push_back(tmpJoints[41]);	//LeftHandPinky2
		m_Skeleton.m_joints.push_back(tmpJoints[37]);	//LeftHandRing1
		m_Skeleton.m_joints.push_back(tmpJoints[38]);	//LeftHandRing2
		m_Skeleton.m_joints.push_back(tmpJoints[34]);	//LeftHandMiddle1
		m_Skeleton.m_joints.push_back(tmpJoints[35]);	//LeftHandMiddle2
		m_Skeleton.m_joints.push_back(tmpJoints[36]);	//LeftHandMiddle3
		m_Skeleton.m_joints.push_back(tmpJoints[42]);	//LeftHandPinky3
		m_Skeleton.m_joints.push_back(tmpJoints[39]);	//LeftHandRing3
		
		m_Skeleton.m_joints.push_back(tmpJoints[26]);	//LeftForeArm
		m_Skeleton.m_joints.push_back(tmpJoints[52]);	//RightHandThumb3
		m_Skeleton.m_joints.push_back(tmpJoints[51]);	//RightHandThumb2
		m_Skeleton.m_joints.push_back(tmpJoints[63]);	//RightHandPinky2
		m_Skeleton.m_joints.push_back(tmpJoints[65]);	//RightWristTwist
		m_Skeleton.m_joints.push_back(tmpJoints[48]);	//RightForeArm
	}												   
#pragma endregion									   
	
	
#pragma region[플레이어 캐릭터(남성) 본체계 재설정 160413]
	if (m_fileName == "../Assets/02_ModelData/02_CHARACTER_LIZZY/Juno.fbx"
		|| m_fileName == "../Assets/02_ModelData/02_CHARACTER_LIZZY/Juno_idle.fbx"
		|| m_fileName == "../Assets/02_ModelData/02_CHARACTER_LIZZY/Juno_running.fbx"
		|| m_fileName == "../Assets/02_ModelData/02_CHARACTER_LIZZY/Juno_left_strafe.fbx"
		|| m_fileName == "../Assets/02_ModelData/02_CHARACTER_LIZZY/Juno_right_strafe.fbx"
		|| m_fileName == "../Assets/02_ModelData/02_CHARACTER_LIZZY/Juno_left_forward_diagonal.fbx"
		|| m_fileName == "../Assets/02_ModelData/02_CHARACTER_LIZZY/Juno_right_forward_diagonal.fbx"
		|| m_fileName == "../Assets/02_ModelData/02_CHARACTER_LIZZY/Juno_smash.fbx"
		|| m_fileName == "../Assets/02_ModelData/02_CHARACTER_LIZZY/Juno_running_backward.fbx"
		|| m_fileName == "../Assets/02_ModelData/02_CHARACTER_LIZZY/Juno_standing_react_death_forward.FBX")
	{
		std::vector<Joint> tmpJoints;
		
		tmpJoints = m_Skeleton.m_joints;
		
		m_Skeleton.m_joints.clear();
		
		m_Skeleton.m_joints.push_back(tmpJoints[18]);	//head
		m_Skeleton.m_joints.push_back(tmpJoints[0]);	//hips
		m_Skeleton.m_joints.push_back(tmpJoints[69]);	//RHoodieString1
		m_Skeleton.m_joints.push_back(tmpJoints[70]);	//RHoodieString2
		m_Skeleton.m_joints.push_back(tmpJoints[16]);	//Spine2
		m_Skeleton.m_joints.push_back(tmpJoints[42]);	//RightShoulder
		m_Skeleton.m_joints.push_back(tmpJoints[68]);	//RHoodieString
		m_Skeleton.m_joints.push_back(tmpJoints[17]);	//Neck
		m_Skeleton.m_joints.push_back(tmpJoints[66]);	//LHoodieString1
		m_Skeleton.m_joints.push_back(tmpJoints[67]);	//LHoodieString2
		
		m_Skeleton.m_joints.push_back(tmpJoints[22]);	//LeftShoulder
		m_Skeleton.m_joints.push_back(tmpJoints[65]);	//LHoodieString
		m_Skeleton.m_joints.push_back(tmpJoints[14]);	//Spine
		m_Skeleton.m_joints.push_back(tmpJoints[62]);	//Backpack
		m_Skeleton.m_joints.push_back(tmpJoints[23]);	//LeftArm
		m_Skeleton.m_joints.push_back(tmpJoints[24]);	//LeftForeArm
		m_Skeleton.m_joints.push_back(tmpJoints[2]);	//LeftUpLeg
		m_Skeleton.m_joints.push_back(tmpJoints[71]);	//LBackpack_Strap
		m_Skeleton.m_joints.push_back(tmpJoints[43]);	//RightArm
		m_Skeleton.m_joints.push_back(tmpJoints[72]);	//RBackpack_Strap
		
		m_Skeleton.m_joints.push_back(tmpJoints[48]);	//RightHandThumb3
		m_Skeleton.m_joints.push_back(tmpJoints[51]);	//RightHandIndex3
		m_Skeleton.m_joints.push_back(tmpJoints[49]);	//RightHandIndex1
		m_Skeleton.m_joints.push_back(tmpJoints[50]);	//RightHandIndex2
		m_Skeleton.m_joints.push_back(tmpJoints[45]);	//RightHand
		m_Skeleton.m_joints.push_back(tmpJoints[47]);	//RightHandThumb2
		m_Skeleton.m_joints.push_back(tmpJoints[46]);	//RightHandThumb1
		m_Skeleton.m_joints.push_back(tmpJoints[58]);	//RightHandPinky1
		m_Skeleton.m_joints.push_back(tmpJoints[59]);	//RightHandPinky2
		m_Skeleton.m_joints.push_back(tmpJoints[60]);	//RightHandPinky3
		
		m_Skeleton.m_joints.push_back(tmpJoints[52]);	//RightHandMiddle1
		m_Skeleton.m_joints.push_back(tmpJoints[53]);	//RightHandMiddle2
		m_Skeleton.m_joints.push_back(tmpJoints[54]);	//RightHandMiddle3
		m_Skeleton.m_joints.push_back(tmpJoints[55]);	//RightHandRing1
		m_Skeleton.m_joints.push_back(tmpJoints[56]);	//RightHandRing2
		m_Skeleton.m_joints.push_back(tmpJoints[57]);	//RightHandRing3
		m_Skeleton.m_joints.push_back(tmpJoints[44]);	//RightForeArm
		m_Skeleton.m_joints.push_back(tmpJoints[25]);	//LeftHand
		m_Skeleton.m_joints.push_back(tmpJoints[26]);	//LeftHandThumb1
		m_Skeleton.m_joints.push_back(tmpJoints[28]);	//LeftHandThumb3
		
		m_Skeleton.m_joints.push_back(tmpJoints[31]);	//LeftHandIndex3
		m_Skeleton.m_joints.push_back(tmpJoints[29]);	//LeftHandIndex1
		m_Skeleton.m_joints.push_back(tmpJoints[30]);	//LeftHandIndex2
		m_Skeleton.m_joints.push_back(tmpJoints[27]);	//LeftHandThumb2
		m_Skeleton.m_joints.push_back(tmpJoints[38]);	//LeftHandPinky1
		m_Skeleton.m_joints.push_back(tmpJoints[32]);	//LeftHandMiddle1
		m_Skeleton.m_joints.push_back(tmpJoints[35]);	//LeftHandRing1
		m_Skeleton.m_joints.push_back(tmpJoints[39]);	//LeftHandPinky2
		m_Skeleton.m_joints.push_back(tmpJoints[40]);	//LeftHandPinky3
		m_Skeleton.m_joints.push_back(tmpJoints[33]);	//LeftHandMiddle2
		m_Skeleton.m_joints.push_back(tmpJoints[34]);	//LeftHandMiddle3
		
		m_Skeleton.m_joints.push_back(tmpJoints[37]);	//LeftHandRing3
		m_Skeleton.m_joints.push_back(tmpJoints[36]);	//LeftHandRing2
		m_Skeleton.m_joints.push_back(tmpJoints[19]);	//HairB
		m_Skeleton.m_joints.push_back(tmpJoints[20]);	//HairA
		m_Skeleton.m_joints.push_back(tmpJoints[8]);	//RightUpLeg
		m_Skeleton.m_joints.push_back(tmpJoints[3]);	//LeftLeg
		m_Skeleton.m_joints.push_back(tmpJoints[9]);	//RightLeg
		m_Skeleton.m_joints.push_back(tmpJoints[4]);	//LeftFoot
		m_Skeleton.m_joints.push_back(tmpJoints[7]);	//LeftPantLeg
		
		m_Skeleton.m_joints.push_back(tmpJoints[5]);	//LeftToBase
		m_Skeleton.m_joints.push_back(tmpJoints[6]);	//LFootTongue
		m_Skeleton.m_joints.push_back(tmpJoints[10]);	//RightFoot
		m_Skeleton.m_joints.push_back(tmpJoints[13]);	//RightPantLeg
		m_Skeleton.m_joints.push_back(tmpJoints[11]);	//RightToeBase
		m_Skeleton.m_joints.push_back(tmpJoints[12]);	//RFootTongue
		m_Skeleton.m_joints.push_back(tmpJoints[61]);	//RSleeve
		m_Skeleton.m_joints.push_back(tmpJoints[41]);	//LSleeve
		m_Skeleton.m_joints.push_back(tmpJoints[21]);	//Hat
		m_Skeleton.m_joints.push_back(tmpJoints[63]);	//Hood
		m_Skeleton.m_joints.push_back(tmpJoints[64]);	//Hood1
	}
#pragma endregion
	
	//20160401 송인희 교수님이 주관하는 중간 발표에서 좀비토끼를 보여드리기 위해
	//본 체계를 직접 정하여 만들었습니다.
	//이러한 방식은 모델마다 따로따로 지정해야하는 만큼 비효율적인 방법이지만,
	//본 체계가 망가진 모델을 다루기 위해서는 차선책입니다.
	
#pragma region[좀비토끼 본체계 재설정]
	if (m_fileName == "../Assets/02_ModelData/03_Monster/Zombunny.FBX"
		|| m_fileName == "../Assets/02_ModelData/03_Monster/Zombunny_running.fbx")
	{
		std::vector<Joint> tmpJoints;
		
		tmpJoints = m_Skeleton.m_joints;
		
		m_Skeleton.m_joints.clear();
		
		m_Skeleton.m_joints.push_back(tmpJoints[0]);
		m_Skeleton.m_joints.push_back(tmpJoints[1]);
		m_Skeleton.m_joints.push_back(tmpJoints[10]);
		m_Skeleton.m_joints.push_back(tmpJoints[2]);
		m_Skeleton.m_joints.push_back(tmpJoints[5]);
		m_Skeleton.m_joints.push_back(tmpJoints[6]);
		
		m_Skeleton.m_joints.push_back(tmpJoints[4]);
		m_Skeleton.m_joints.push_back(tmpJoints[11]);
		m_Skeleton.m_joints.push_back(tmpJoints[8]);
		m_Skeleton.m_joints.push_back(tmpJoints[9]);
		m_Skeleton.m_joints.push_back(tmpJoints[7]);
		m_Skeleton.m_joints.push_back(tmpJoints[3]);
	}
#pragma endregion
	
}

void FBXAnimationModelLoader::ProcessSkeletonHierarchyRecursively(FbxNode* inNode, int inDepth, int myIndex, int inParentIndex)
{
	if (inNode->GetNodeAttribute()
		&& inNode->GetNodeAttribute()->GetAttributeType()
		&& inNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton)
	{
		Joint currJoint;
		currJoint.mParentIndex = inParentIndex;
		currJoint.mName = inNode->GetName();
		m_Skeleton.m_joints.push_back(currJoint);
	}
	for (int i = 0; i < inNode->GetChildCount(); i++)
	{
		ProcessSkeletonHierarchyRecursively(inNode->GetChild(i), inDepth + 1, m_Skeleton.m_joints.size(), myIndex);
	}
}

void FBXAnimationModelLoader::TraversalFBXTree_forAnimation(FbxNode* Node)
{
	int numChild = Node->GetChildCount();
	FbxNode *childNode = nullptr;

	for (int i = 0; i < numChild; i++)
	{
		childNode = Node->GetChild(i);
		FbxMesh *childMesh = childNode->GetMesh();

		if (childMesh != nullptr)
		{
			if (0 == m_pMeshNodeInfo.size())
			{
				//20160404 애니메이션 정보에서 필요한 정보.
				//메쉬 정보를 얻지 않았을 경우에도 필요하다.
				//메쉬 정보가 없으면 인덱스 값만 추출한다.
				FBXSDK_MESH_NODE* tempMeshNodeInfo = new FBXSDK_MESH_NODE();
				tempMeshNodeInfo->m_numIndices = childMesh->GetPolygonVertexCount();
				tempMeshNodeInfo->m_indices = new int[tempMeshNodeInfo->m_numIndices];
				tempMeshNodeInfo->m_indices = childMesh->GetPolygonVertices();

				m_pMeshNodeInfo.push_back(*tempMeshNodeInfo);
				delete tempMeshNodeInfo;
			}

			//LoadControlPoints(childNode);
			//LoadSkeletalInfo(childMesh, geometryTransform);
			LoadSkeletalInfo(childNode);
		}
	}
}

void FBXAnimationModelLoader::LoadControlPoints(FbxNode* inNode)
{
	FbxMesh* currMesh = inNode->GetMesh();
	unsigned int ctrlPointCount = currMesh->GetControlPointsCount();
	for (unsigned int i = 0; i < ctrlPointCount; ++i)
	{
		XMFLOAT3 currPosition;
		currPosition.x = static_cast<float>(currMesh->GetControlPointAt(i).mData[0]);
		currPosition.y = static_cast<float>(currMesh->GetControlPointAt(i).mData[1]);
		currPosition.z = static_cast<float>(currMesh->GetControlPointAt(i).mData[2]);

		//m_blendingInfo[i].m_position = currPosition;
	}
}


void FBXAnimationModelLoader::LoadSkeletalInfo(FbxNode* inNode)
{
	//int numDeformers = fbxMesh->GetDeformerCount();

	FbxSkin* skin = (FbxSkin*)inNode->GetMesh()->GetDeformer(0, FbxDeformer::eSkin);
	if (skin != nullptr)
	{
		int boneCount = skin->GetClusterCount();

		std::vector<std::pair<unsigned int, std::string>> namelist;

		for (int boneIndex = 0; boneIndex < boneCount; ++boneIndex)
		{
			FbxCluster* cluster = skin->GetCluster(boneIndex);
			std::string currJointName = cluster->GetLink()->GetName();
			unsigned int currJointIndex = FindJointIndexUsingName(currJointName);

			namelist.push_back(make_pair(currJointIndex, currJointName));

			FbxAMatrix transformMatrix;
			FbxAMatrix transformLinkMatrix;			//부모의 월드변환행렬
			FbxAMatrix globalBindposeInverseMatrix;	//본 기준위치에 역행렬

			FbxNode* boneNode = cluster->GetLink();

			//bind pose(본의 기준위치)의 역행렬(globalBindposeInverseMatrix) 얻기
			cluster->GetTransformMatrix(transformMatrix);
			cluster->GetTransformLinkMatrix(transformLinkMatrix);

			//GetGeometryTransformation(inNode) : 노드의 지역행렬.
			globalBindposeInverseMatrix
				= transformLinkMatrix.Inverse() * transformMatrix * GetGeometryTransformation(inNode);

			globalBindposeInverseMatrix.SetT(
				FbxVector4(globalBindposeInverseMatrix.GetT().mData[0] * m_modelScale,
					globalBindposeInverseMatrix.GetT().mData[1] * m_modelScale,
					globalBindposeInverseMatrix.GetT().mData[2] * m_modelScale));

			FbxAMatrix a = GetGeometryTransformation(inNode);

			// Update the information in mSkeleton 
			m_Skeleton.m_joints[currJointIndex].mGlobalBindposeInverse
				= globalBindposeInverseMatrix;
			m_Skeleton.m_joints[currJointIndex].mNode
				= cluster->GetLink();

			//본 정점들의 인덱스 배열
			int *boneVertexIndices = cluster->GetControlPointIndices();
			//본 정점들의 Weight값 배열
			double *boneVertexWeights = cluster->GetControlPointWeights();

			// 본에 영향을 미치는 정점들을 갱신(스키닝)
			int numBoneVertexIndices = cluster->GetControlPointIndicesCount();

			for (int i = 0; i < numBoneVertexIndices; i++)
			{
				//20160317
				m_blendingInfo[boneVertexIndices[i]].mInfo
					.push_back(std::make_pair(currJointIndex, (float)cluster->GetControlPointWeights()[i]));
			}

			//매 프레임 당 애니메이션 변환행렬 추출
			for (FbxLongLong frameCount = m_animTimeStart.GetFrameCount(FBX_FPS); frameCount <= m_animTimeEnd.GetFrameCount(FBX_FPS); ++frameCount)
			{
				FbxTime currentTime;
				currentTime.SetFrame(frameCount, FBX_FPS);

				KeyFrame tempKeyFrame;
				tempKeyFrame.mFrameNum = (int)frameCount;

				FbxAMatrix currnetTransformOffset = inNode->EvaluateGlobalTransform(currentTime) *  GetGeometryTransformation(inNode);
				tempKeyFrame.mGlobalTransform
					= currnetTransformOffset.Inverse() * cluster->GetLink()->EvaluateGlobalTransform(currentTime);

				tempKeyFrame.mGlobalTransform.SetT(
					FbxVector4(tempKeyFrame.mGlobalTransform.GetT().mData[0] * m_modelScale,
						tempKeyFrame.mGlobalTransform.GetT().mData[1] * m_modelScale,
						tempKeyFrame.mGlobalTransform.GetT().mData[2] * m_modelScale));

				m_Skeleton.m_joints[boneIndex].mKeyFrames.push_back(tempKeyFrame);
			}
		}

		//if (m_fileName == "../Assets/02_ModelData/01_CHARACTER_MEI/Claire.fbx"
		//	|| m_fileName == "../Assets/02_ModelData/01_CHARACTER_MEI/Claire_running.fbx")
		//{
		//	std::cout << namelist.begin()->first << " / "<< namelist.begin()->second << std::endl;
		//}
	}
}


//노드의 지역행렬을 구한다. 
FbxAMatrix FBXAnimationModelLoader::GetGeometryTransformation(FbxNode* inNode)
{
	if (!inNode)
	{
		throw std::exception("Null for mesh geometry");
	}

	const FbxVector4 lT = inNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	const FbxVector4 lR = inNode->GetGeometricRotation(FbxNode::eSourcePivot);
	const FbxVector4 lS = inNode->GetGeometricScaling(FbxNode::eSourcePivot);

	return FbxAMatrix(lT, lR, lS);
}

unsigned int FBXAnimationModelLoader::FindJointIndexUsingName(std::string jointName)
{
	for (unsigned int i = 0; i < m_Skeleton.m_joints.size(); i++)
	{
		if (m_Skeleton.m_joints[i].mName == jointName)
		{
			return i;
		}
	}

	//영태가 임의로 추가
	return 0;
}

FbxAMatrix FBXAnimationModelLoader::GetSkeletonJointGlobalTransform(unsigned int jointIndex, int keyframeNum)
{
	if (GetSkeletonJoint(jointIndex).mKeyFrames.size() != 0)
	{
		return GetSkeletonJoint(jointIndex).mKeyFrames[keyframeNum].mGlobalTransform;
	}
	else
	{
		FbxAMatrix m;
		m.SetIdentity();
		return m;
	}
}

bool FBXAnimationModelLoader::HasAnitamionKeyframe()
{
	if (m_Skeleton.m_joints.cbegin()->mKeyFrames.size() != 0)
	{
		return true;
	}
	else
		return false;
}