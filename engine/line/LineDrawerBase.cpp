#include "LineDrawerBase.h"
#include "ModelDatas.h"
#include <algorithm>

void LineDrawerBase::Initialize(DirectXCommon* dxCommon, SrvManager* srvManager)
{
	dxCommon_ = dxCommon;
	srvManager_ = srvManager;

	pipeline_ = std::make_unique<Pipeline>();

	CreateRootSignature();

	CreatePipellineState();

	//CreateLineObject(Type::AABB);
	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();
	
	worldTransform_->useQuaternion_ = false;

	worldTransform_->transform.scale = { 1.0f,1.0f,1.0f };
	worldTransform_->transform.translate = { 0.0f,0.0f,0.0f };

	worldTransform_->UpdateMatrix();
}

void LineDrawerBase::Update()
{
	worldTransform_->UpdateMatrix();

	for (std::list<std::unique_ptr<LineObject>>::iterator iterator = lineObjects_.begin(); iterator != lineObjects_.end();)
	{
		//if (iterator->get()->type == Type::Skeleton)
		//{
		//	if (iterator->get()->skeleton)
		//	{
		//		int vertexIndex = 0;

		//		// skeleton は Skeleton 型の引数とする（もしくはグローバルなどから取得）
		//		const Skeleton& skeleton_ = *iterator->get()->skeleton; // 例: object に skeleton メンバがある場合

		//		// 全 Joint を走査
		//		for (const Joint& joint : skeleton_.joints)
		//		{
		//			// 親が存在する場合に親と子を線で結ぶ
		//			if (joint.parent.has_value())
		//			{
		//				// 親 Joint を取得
		//				int parentIndex = joint.parent.value();
		//				const Joint& parentJoint = skeleton_.joints[parentIndex];

		//				// 親のワールド座標 (skeletonSpaceMatrix の4列目)
		//				iterator->get()->vertexData[vertexIndex++].position = {
		//					parentJoint.transform.rotate.x,
		//					parentJoint.transform.rotate.y,
		//					parentJoint.transform.rotate.z,
		//					1.0f
		//				};

		//				// 子 (現在の joint) のワールド座標
		//				iterator->get()->vertexData[vertexIndex++].position = {
		//					joint.transform.rotate.x,
		//					joint.transform.rotate.y,
		//					joint.transform.rotate.z,
		//					1.0f
		//				};
		//			}
		//		}

		//		iterator->get()->vertexIndex = vertexIndex;
		//	}
		//}


		iterator->get()->transform->UpdateMatrix();

		iterator->get()->lineData->matWorld = iterator->get()->transform->matWorld_;

		iterator->get()->lineData->color = { 1.0f,1.0f,1.0f,1.0f };

		++iterator;
	}
}

void LineDrawerBase::SkeletonUpdate(Skeleton skeleton)
{
	for (std::list<std::unique_ptr<LineObject>>::iterator iterator = lineObjects_.begin(); iterator != lineObjects_.end();)
	{
		if (iterator->get()->type == Type::Skeleton)
		{
			int vertexIndex = 0;

			// skeleton は Skeleton 型の引数とする（もしくはグローバルなどから取得）
			Skeleton skeleton_ = skeleton; // 例: object に skeleton メンバがある場合

			// 全 Joint を走査
			for (Joint joint : skeleton_.joints)
			{
				// 親が存在する場合に親と子を線で結ぶ
				if (joint.parent.has_value())
				{
					// 親 Joint を取得
					int parentIndex = joint.parent.value();
					const Joint& parentJoint = skeleton_.joints[parentIndex];

					// 親のワールド座標 (skeletonSpaceMatrix の4列目)
					iterator->get()->vertexData[vertexIndex++].position = {
						parentJoint.skeletonSpaceMatrix.m[3][0],
						parentJoint.skeletonSpaceMatrix.m[3][1],
						parentJoint.skeletonSpaceMatrix.m[3][2],
						1.0f
					};

					// 子 (現在の joint) のワールド座標
					iterator->get()->vertexData[vertexIndex++].position = {
						joint.skeletonSpaceMatrix.m[3][0],
						joint.skeletonSpaceMatrix.m[3][1],
						joint.skeletonSpaceMatrix.m[3][2],
						1.0f
					};
				}
			}

			iterator->get()->vertexIndex = vertexIndex;
		}

		++iterator;
	}
}

void LineDrawerBase::Draw(ViewProjection viewProjection)
{
	

	// プリミティブトポロジーを設定
	dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);

	// ルートシグネチャを設定
	dxCommon_->GetCommandList()->SetGraphicsRootSignature(pipeline_->rootSignature.Get());

	// PSOを設定
	dxCommon_->GetCommandList()->SetPipelineState(pipeline_->pipelineState.Get());
	
	for (std::list<std::unique_ptr<LineObject>>::iterator iterator = lineObjects_.begin(); iterator != lineObjects_.end();)
	{
		if (iterator->get()->type == Type::Object3D)
		{
			// プリミティブトポロジーを設定
			dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
		}
		else
		{
			// プリミティブトポロジーを設定
			dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);
		}

		// VBVを設定
		dxCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &iterator->get()->vertexBufferView);

		dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0, iterator->get()->lineResource.Get()->GetGPUVirtualAddress());

		dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(1, viewProjection.GetViewProjectionResource()->GetGPUVirtualAddress());

		dxCommon_->GetCommandList()->DrawInstanced(iterator->get()->vertexIndex, 1, 0, 0);

		++iterator;
	}
}

void LineDrawerBase::CreateRootSignature()
{
	HRESULT hr;

	//RootSignature作成
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	
	
	D3D12_ROOT_PARAMETER rootParameters[2] = {};
	
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//CBVを使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;//VertexShaderで使う
	rootParameters[0].Descriptor.ShaderRegister = 0;//レジスタ番号0とバインド

	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//CBVを使う
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;//VertexShaderで使う
	rootParameters[1].Descriptor.ShaderRegister = 1;//レジスタ番号0とバインド

	descriptionRootSignature.pParameters = rootParameters;
	descriptionRootSignature.NumParameters = _countof(rootParameters);

	/*----------------------------------------------------------
	* シリアライズしてバイナリにする
	----------------------------------------------------------*/

	Microsoft::WRL::ComPtr<ID3DBlob> signatureBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;

	hr = D3D12SerializeRootSignature(&descriptionRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);

	if (FAILED(hr)) {
		Logger::Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
	}

	/*----------------------------------------------------------
	* バイナリを元RootSignatureを生成
	----------------------------------------------------------*/

	hr = dxCommon_->GetDevice()->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&pipeline_->rootSignature));
	assert(SUCCEEDED(hr));
}

void LineDrawerBase::CreatePipellineState()
{
	HRESULT hr;

	D3D12_INPUT_ELEMENT_DESC inputElementDescs[1] = {};
	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;


	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);

	/*----------------------------------------------------------
	* RasterizerStateの設定
	----------------------------------------------------------*/

	D3D12_RASTERIZER_DESC rasterizerDesc{};

	// 裏面(時計回り)を表示しない 
	rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;
	//rasterizerDesc.CullMode = D3D12_CULL_MODE_FRONT;

	// 三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_WIREFRAME;

	/*----------------------------------------------------------
	* Shaderのコンパイル
	----------------------------------------------------------*/

	Microsoft::WRL::ComPtr<IDxcBlob> vertexShaderBlob = dxCommon_->CompileShader(L"Resources/shaders/PrimitiveDrawerVS.hlsl", L"vs_6_0");

	Microsoft::WRL::ComPtr<IDxcBlob> pixelShaderBlob = dxCommon_->CompileShader(L"Resources/shaders/PrimitiveDrawerPS.hlsl", L"ps_6_0");

	/*----------------------------------------------------------
	* DepthStencilStateの設定
	----------------------------------------------------------*/

	//DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	//Depthの機能を有効化する
	depthStencilDesc.DepthEnable = true;

	// 書き込みします
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;

	// 比較関数はLessEqual。近ければ描画される
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;


	/*----------------------------------------------------------
	* BlendStateの設定
	----------------------------------------------------------*/

	D3D12_BLEND_DESC blendDesc{};

	// 全ての要素数を書き込む
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	blendDesc.RenderTarget[0].BlendEnable = FALSE;

	/*----------------------------------------------------------
	* GraphicsPipelineの設定
	----------------------------------------------------------*/
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	graphicsPipelineStateDesc.pRootSignature = pipeline_->rootSignature.Get();
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc;
	graphicsPipelineStateDesc.VS = { vertexShaderBlob->GetBufferPointer(),vertexShaderBlob->GetBufferSize() };
	graphicsPipelineStateDesc.PS = { pixelShaderBlob->GetBufferPointer(),pixelShaderBlob->GetBufferSize() };
	graphicsPipelineStateDesc.BlendState = blendDesc;
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc;

	// 書き込むRTVの情報
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	// 利用するトポロジー(形状)のタイプ。
	graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;

	// どのように画面に色をつけるか
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	// DepthStencilの設定
	graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// 実際に生成
	hr = dxCommon_->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc, IID_PPV_ARGS(&pipeline_->pipelineState));

	assert(SUCCEEDED(hr));
}

std::unique_ptr<LineDrawerBase::LineObject> LineDrawerBase::CreateBaseLineData(Type type)
{
	// 新しいラインオブジェクトの生成と初期化
	std::unique_ptr<LineObject> newObject = std::make_unique<LineObject>();

	newObject->type = type;

	// vertexResourceの生成
	newObject->vertexResource = CreateVertexResource();

	// マップ
	newObject->vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&newObject->vertexData));

	// VertexBufferViewの生成
	CreateVertexBufferView(newObject.get()); // TODO: typeに応じて変更

	return std::move(newObject);
}

void LineDrawerBase::CreateLineObject(Type type, WorldTransform* transform)
{
	/*
	WriteLineData(newObject.get());

	CreateLineResource(newObject.get());

	if (transform == nullptr)
	{
		newObject->transform = worldTransform_.get();	
	}
	else
	{
		newObject->transform = transform;
	}

	lineObjects_.push_back(std::move(newObject));*/
}

void LineDrawerBase::CeateAABBLine(AABB aabb, WorldTransform* transform)
{
	auto newObject = CreateBaseLineData(Type::AABB);

	Vector3 min = aabb.min; // AABBの最小点
	Vector3 max = aabb.max; // AABBの最大点

	// AABBの8頂点
	Vector3 vertices[8] = {
		{ min.x, min.y, min.z }, // v0
		{ max.x, min.y, min.z }, // v1
		{ max.x, min.y, max.z }, // v2
		{ min.x, min.y, max.z }, // v3
		{ min.x, max.y, min.z }, // v4
		{ max.x, max.y, min.z }, // v5
		{ max.x, max.y, max.z }, // v6
		{ min.x, max.y, max.z }  // v7
	};

	// AABBのライン（12本）
	int lineIndices[24] = {
		0, 1,  1, 2,  2, 3,  3, 0,  // 底面
		4, 5,  5, 6,  6, 7,  7, 4,  // 上面
		0, 4,  1, 5,  2, 6,  3, 7   // 側面
	};


	// 頂点データに書き込み
	for (int i = 0; i < 24; ++i)
	{
		newObject.get()->vertexData[i].position = {vertices[lineIndices[i]].x,
										   vertices[lineIndices[i]].y,
										   vertices[lineIndices[i]].z, 1.0f };

		newObject.get()->vertexIndex = i;
	}
}

void LineDrawerBase::CreateCatmullRomLine(std::vector<Vector3> points, WorldTransform* transform)
{
	auto newObject = CreateBaseLineData(Type::CatmullRom);
	int vertexIndex = 0;
	const int segments = 1024;

	for (uint32_t i = 0; i < segments; ++i)
	{
		float t = (1.0f / segments) * i;

		Vector3 position = CatmullRomPosition(points, t);

		newObject.get()->vertexData[vertexIndex++].position = { position.x, position.y, position.z, 1.0f };
	}

	newObject.get()->vertexIndex = vertexIndex;

	CreateLineResource(newObject.get()); 

	if (transform == nullptr)
	{ 
		newObject.get()->transform = worldTransform_.get();
	}
	else
	{
		newObject.get()->transform = transform;
	}

	lineObjects_.push_back(std::move(newObject));
}

//void LineDrawerBase::CreateObject3DLine(std::string modelPath, WorldTransform* transform)
//{
//	// 新しいラインオブジェクトの生成と初期化
//	std::unique_ptr<LineObject> newObject = std::make_unique<LineObject>();
//
//	newObject->type = Type::Object3D;
//
//	newObject->transform = transform;
//
//	// モデルの読み込み
//
//	std::string directoryPath = "Resources";
//	
//	LineModelData modelData;
//	Assimp::Importer importer;
//
//	std::string filePath = directoryPath + "/" + modelPath;
//
//	const aiScene* scene = importer.ReadFile(filePath.c_str(),
//		aiProcess_FlipWindingOrder | aiProcess_FlipUVs);
//
//	if (!scene) {
//		std::cerr << "Failed to load model: " << filePath << std::endl;
//		std::cerr << "Assimp error: " << importer.GetErrorString() << std::endl;
//		throw std::runtime_error("Model loading failed.");
//	}
//
//	assert(scene->HasMeshes()); // メッシュがないのは対応しない
//
//	for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
//		aiMesh* mesh = scene->mMeshes[meshIndex];
//		assert(mesh->HasNormals());        // 法線がないMeshは非対応
//		assert(mesh->HasTextureCoords(0)); // TexcoordがないMeshは非対応
//
//		// 頂点の追加
//		for (uint32_t vertexIndex = 0; vertexIndex < mesh->mNumVertices; ++vertexIndex) {
//			aiVector3D& position = mesh->mVertices[vertexIndex];
//			aiVector3D& normal = mesh->mNormals[vertexIndex];
//			aiVector3D& texcoord = mesh->mTextureCoords[0][vertexIndex];
//
//			VertexData vertex;
//			vertex.position = { position.x, position.y, position.z, 1.0f };
//			vertex.normal = { normal.x,   normal.y,   normal.z };
//			vertex.texcoord = { texcoord.x, texcoord.y };
//
//			// 左手系/右手系変換 (必要に応じて)
//			// vertex.position.x *= -1.0f;
//			// vertex.normal.x   *= -1.0f;
//
//			modelData.vertices.push_back(vertex);
//		}
//
//		// インデックスの追加
//		for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {
//			aiFace& face = mesh->mFaces[faceIndex];
//			assert(face.mNumIndices == 3); // 三角形のみ対応
//
//			for (uint32_t element = 0; element < face.mNumIndices; ++element) {
//				modelData.indices.push_back(face.mIndices[element]);
//			}
//		}
//	}
//
//	// マテリアル
//	for (uint32_t materialIndex = 0; materialIndex < scene->mNumMaterials; ++materialIndex) {
//		aiMaterial* material = scene->mMaterials[materialIndex];
//		if (material->GetTextureCount(aiTextureType_DIFFUSE) != 0) {
//			aiString textureFilePath;
//			material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePath);
//			modelData.material.textureFilePath = directoryPath + "/" + textureFilePath.C_Str();
//		}
//	}
//
//	std::set<std::pair<int, int>> uniqueEdges;
//
//	std::vector<uint32_t> indices = modelData.indices;
//
//	for (size_t i = 0; i < indices.size(); i += 3) {
//		int i0 = indices[i];
//		int i1 = indices[i + 1];
//		int i2 = indices[i + 2];
//
//		auto AddEdge = [&](int a, int b) {
//			if (a > b) std::swap(a, b);
//			uniqueEdges.insert({ a,b });
//			};
//
//		AddEdge(i0, i1);
//		AddEdge(i1, i2);
//		AddEdge(i2, i0);
//	}
//	//最終的なラインリストを構築
//
//	std::vector<LineVertex> lineVertices;
//
//	for (auto& [a, b] : uniqueEdges) {
//		LineVertex va, vb;
//		va.position = { modelData.vertices[a].position.x,
//						modelData.vertices[a].position.y,
//						modelData.vertices[a].position.z,
//		                1.0f                             };
//		vb.position = { modelData.vertices[b].position.x,
//						modelData.vertices[b].position.y,
//						modelData.vertices[b].position.z,
//		                1.0f                             };
//
//		lineVertices.push_back(va);
//		lineVertices.push_back(vb);
//	}
//
//	newObject->vertexResource =
//		dxCommon_->CreateBufferResource(sizeof(LineVertex) * lineVertices.size());
//
//	// データをアップロード
//	newObject->vertexData = nullptr;
//	newObject->vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&newObject->vertexData));
//	std::memcpy(newObject->vertexData, lineVertices.data(), sizeof(LineVertex) * lineVertices.size());
//
//	// VertexBufferViewの生成
//	newObject->vertexBufferView.BufferLocation = newObject->vertexResource->GetGPUVirtualAddress();
//
//	newObject->vertexBufferView.SizeInBytes = UINT(sizeof(LineVertex) * lineVertices.size());
//
//	newObject->vertexBufferView.StrideInBytes = sizeof(LineVertex);
//
//	newObject->lineResource = dxCommon_->CreateBufferResource(sizeof(LineForGPU));
//
//	newObject->lineResource->Map(0, nullptr, reinterpret_cast<void**>(&newObject->lineData));
//
//	newObject->vertexIndex = static_cast<int>(lineVertices.size());
//
//	lineObjects_.push_back(std::move(newObject));
//}

void LineDrawerBase::CreateObject3DLine(std::string modelPath, WorldTransform* transform)
{
	// 新しいラインオブジェクトの生成と初期化
	std::unique_ptr<LineObject> newObject = std::make_unique<LineObject>();
	newObject->type = Type::Object3D;
	newObject->transform = transform;

	// モデルの読み込み
	std::string directoryPath = "Resources";
	LineModelData modelData;
	Assimp::Importer importer;
	std::string filePath = directoryPath + "/" + modelPath;

	/*const aiScene* scene = importer.ReadFile(filePath.c_str(),
		aiProcess_FlipWindingOrder | aiProcess_FlipUVs);*/
	const aiScene* scene = importer.ReadFile(filePath.c_str(),
		aiProcess_FlipWindingOrder |
		aiProcess_FlipUVs |
		aiProcess_JoinIdenticalVertices); // ← 追加

	if (!scene) {
		std::cerr << "Failed to load model: " << filePath << std::endl;
		std::cerr << "Assimp error: " << importer.GetErrorString() << std::endl;
		throw std::runtime_error("Model loading failed.");
	}

	assert(scene->HasMeshes());

	for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
		aiMesh* mesh = scene->mMeshes[meshIndex];
		assert(mesh->HasNormals());

		// 頂点の追加
		for (uint32_t vertexIndex = 0; vertexIndex < mesh->mNumVertices; ++vertexIndex) {
			aiVector3D& position = mesh->mVertices[vertexIndex];
			aiVector3D& normal = mesh->mNormals[vertexIndex];

			VertexData vertex;
			vertex.position = { position.x, position.y, position.z, 1.0f };
			vertex.normal = { normal.x, normal.y, normal.z };
			modelData.vertices.push_back(vertex);
		}

		// インデックス（三角形）を追加
		for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {
			aiFace& face = mesh->mFaces[faceIndex];
			assert(face.mNumIndices == 3);
			for (uint32_t element = 0; element < face.mNumIndices; ++element) {
				modelData.indices.push_back(face.mIndices[element]);
			}
		}
	}

	// ---- 外形エッジを抽出（法線一致なら削除） ----
	struct EdgeKey {
		uint32_t v0, v1;
		bool operator==(const EdgeKey& other) const {
			return v0 == other.v0 && v1 == other.v1;
		}
	};
	struct EdgeKeyHash {
		std::size_t operator()(const EdgeKey& e) const {
			return std::hash<uint32_t>()(e.v0) ^ (std::hash<uint32_t>()(e.v1) << 1);
		}
	};
	struct EdgeData {
		uint32_t v0, v1;
		Vector3 normal;
		bool hasPair = false;
	};

	std::unordered_map<EdgeKey, EdgeData, EdgeKeyHash> edgeMap;

	for (size_t i = 0; i < modelData.indices.size(); i += 3) {
		uint32_t i0 = modelData.indices[i];
		uint32_t i1 = modelData.indices[i + 1];
		uint32_t i2 = modelData.indices[i + 2];

		// 三角形の法線を計算
		Vector3 p0 = { modelData.vertices[i0].position.x, modelData.vertices[i0].position.y, modelData.vertices[i0].position.z };
		Vector3 p1 = { modelData.vertices[i1].position.x, modelData.vertices[i1].position.y, modelData.vertices[i1].position.z };
		Vector3 p2 = { modelData.vertices[i2].position.x, modelData.vertices[i2].position.y, modelData.vertices[i2].position.z };

		Vector3 n = Normalize(Cross(p1 - p0, p2 - p0));

		uint32_t tri[3] = { i0, i1, i2 };
		for (int e = 0; e < 3; e++) {
			uint32_t v0 = tri[e];
			uint32_t v1 = tri[(e + 1) % 3];

			// v0 < v1 になるように正規化
			if (v0 > v1) std::swap(v0, v1);

			EdgeKey key{ v0, v1 };

			auto it = edgeMap.find(key);
			if (it == edgeMap.end()) {
				// 新しいエッジ
				edgeMap[key] = EdgeData{ v0, v1, n, false };
			}
			else {
				// 既存エッジと比較
				EdgeData& ed = it->second;
				float dot = Dot(Normalize(ed.normal), n);
				if (dot > 0.999f) {
					// 同一平面 → 削除（分割線）
					edgeMap.erase(it);
				}
				else {
					// 異なる面 → 外形として残す
					ed.hasPair = true;
				}
			}
		}
	}

	// 外形エッジだけを収集
	std::vector<LineVertex> lineVertices;
	for (auto& kv : edgeMap) {
		LineVertex va, vb;
		va.position = modelData.vertices[kv.second.v0].position;
		vb.position = modelData.vertices[kv.second.v1].position;
		lineVertices.push_back(va);
		lineVertices.push_back(vb);
	}

	// ---- VertexBuffer 作成（インデックスは使わない）----
	newObject->vertexResource =
		dxCommon_->CreateBufferResource(sizeof(LineVertex) * lineVertices.size());

	newObject->vertexData = nullptr;
	newObject->vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&newObject->vertexData));
	std::memcpy(newObject->vertexData, lineVertices.data(), sizeof(LineVertex) * lineVertices.size());

	newObject->vertexBufferView.BufferLocation = newObject->vertexResource->GetGPUVirtualAddress();
	newObject->vertexBufferView.SizeInBytes = UINT(sizeof(LineVertex) * lineVertices.size());
	newObject->vertexBufferView.StrideInBytes = sizeof(LineVertex);

	newObject->lineResource = dxCommon_->CreateBufferResource(sizeof(LineForGPU));
	newObject->lineResource->Map(0, nullptr, reinterpret_cast<void**>(&newObject->lineData));

	newObject->vertexIndex = static_cast<int>(lineVertices.size());

	lineObjects_.push_back(std::move(newObject));
}

void LineDrawerBase::CreateSkeletonObject(Skeleton skeleton, WorldTransform* transform)
{
	// 新しいラインオブジェクトの生成と初期化
	std::unique_ptr<LineObject> newObject = std::make_unique<LineObject>();

	newObject->type = Type::Skeleton;

	///----------Vertex----------////

	// vertexResourceの生成
	newObject->vertexResource = CreateVertexResource();

	// マップ
	newObject->vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&newObject->vertexData));

	// VertexBufferViewの生成
	CreateVertexBufferView(newObject.get());

	WriteSkeletonLineData(newObject.get(),skeleton);

	CreateLineResource(newObject.get());

	if (transform == nullptr)
	{
		newObject->transform = worldTransform_.get();
	}
	else
	{
		newObject->transform = transform;
	}
	
	lineObjects_.push_back(std::move(newObject));
}



Microsoft::WRL::ComPtr<ID3D12Resource> LineDrawerBase::CreateVertexResource()
{
	Microsoft::WRL::ComPtr<ID3D12Resource> resource = dxCommon_->CreateBufferResource(sizeof(LineVertex) * kMaxLines);

	return resource;
}


void LineDrawerBase::CreateVertexBufferView(LineObject* object)
{
	// VertexBufferViewの生成
	object->vertexBufferView.BufferLocation = object->vertexResource->GetGPUVirtualAddress();

    object->vertexBufferView.SizeInBytes = UINT(sizeof(LineVertex) * kMaxLines);

	object->vertexBufferView.StrideInBytes = sizeof(LineVertex);
}

void LineDrawerBase::WriteLineData(LineObject* object)
{
	int vertexIndex = 0;

	if (object->type == Type::AABB)
	{
		
	}
	else if (object->type == Type::Sphere)
	{
		float step = 2.0f * std::numbers::pi / 16;

		// XZ 平面の円
		for (int i = 0; i < 16; ++i)
		{
			float theta1 = i * step;
			float theta2 = (i + 1) * step;

			object->vertexData[vertexIndex++].position = { cosf(theta1), 0.0f, sinf(theta1), 1.0f };
			object->vertexData[vertexIndex++].position = { cosf(theta2), 0.0f, sinf(theta2), 1.0f };
		}

		// XY 平面の円
		for (int i = 0; i < 16; ++i)
		{
			float phi1 = i * step;
			float phi2 = (i + 1) * step;

			object->vertexData[vertexIndex++].position = { cosf(phi1), sinf(phi1), 0.0f, 1.0f };
			object->vertexData[vertexIndex++].position = { cosf(phi2), sinf(phi2), 0.0f, 1.0f };
		}

		// YZ 平面の円
		for (int i = 0; i < 16; ++i)
		{
			float psi1 = i * step;
			float psi2 = (i + 1) * step;

			object->vertexData[vertexIndex++].position = { 0.0f, cosf(psi1), sinf(psi1), 1.0f };
			object->vertexData[vertexIndex++].position = { 0.0f, cosf(psi2), sinf(psi2), 1.0f };
		}

		// 全ループ後に最終の頂点インデックスを更新
		object->vertexIndex = vertexIndex;
	}
	else if (object->type == Type::Grid)
	{
		int vertexIndex = 0;
		for (int i = 0; i < 21; ++i)
		{
			object->vertexData[vertexIndex++].position = { -10.0f, 0.0f, -10.0f + i, 1.0f };
			object->vertexData[vertexIndex++].position = { 10.0f, 0.0f, -10.0f + i, 1.0f };
		}
		for (int i = 0; i < 21; ++i)
		{
			object->vertexData[vertexIndex++].position = { -10.0f + i, 0.0f, -10.0f, 1.0f };
			object->vertexData[vertexIndex++].position = { -10.0f + i, 0.0f, 10.0f, 1.0f };
		}

		// 書き込んだ頂点数を object->vertexIndex に反映
		object->vertexIndex = vertexIndex;
	}
}

void LineDrawerBase::WriteSkeletonLineData(LineObject* object, Skeleton skeleton)
{
	int vertexIndex = 0;

	// skeleton は Skeleton 型の引数とする（もしくはグローバルなどから取得）
	const Skeleton& skeleton_ = skeleton; // 例: object に skeleton メンバがある場合

	// 全 Joint を走査
	for (const Joint& joint : skeleton_.joints)
	{
		// 親が存在する場合に親と子を線で結ぶ
		if (joint.parent.has_value())
		{
			// 親 Joint を取得
			int parentIndex = joint.parent.value();
			const Joint& parentJoint = skeleton_.joints[parentIndex];

			// 親のワールド座標 (skeletonSpaceMatrix の4列目)
			object->vertexData[vertexIndex++].position = {
				parentJoint.skeletonSpaceMatrix.m[3][0],
				parentJoint.skeletonSpaceMatrix.m[3][1],
				parentJoint.skeletonSpaceMatrix.m[3][2],
				1.0f
			};

			// 子 (現在の joint) のワールド座標
			object->vertexData[vertexIndex++].position = {
				joint.skeletonSpaceMatrix.m[3][0],
				joint.skeletonSpaceMatrix.m[3][1],
				joint.skeletonSpaceMatrix.m[3][2],
				1.0f
			};
		}
	}

	// 書き込んだ頂点数を反映
	object->vertexIndex = vertexIndex;

	object->skeleton = skeleton;
}

void LineDrawerBase::CreateLineResource(LineObject* object)
{
	object->lineResource = dxCommon_->CreateBufferResource(sizeof(LineForGPU));

	object->lineResource->Map(0, nullptr, reinterpret_cast<void**>(&object->lineData));
}

