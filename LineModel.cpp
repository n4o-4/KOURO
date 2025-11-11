#include "LineModel.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp\postprocess.h>

void LineModel::Initialize(LineDrawerBase* lienDrawer, const std::string& directoryPath, const std::string& filePath)
{
	lineDrawerBase_ = lienDrawer;

	dxCommon_ = lineDrawerBase_->GetdxCommon();

	LoadLineModelFile(directoryPath, filePath);

}

void LineModel::Draw()
{
	// VBVを設定
	dxCommon_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);

	dxCommon_->GetCommandList()->DrawInstanced(vertexCount_, 1, 0, 0);
}

void LineModel::LoadLineModelFile(const std::string& directoryPath, const std::string& filePath)
{
	LineModelData modelData;
	Assimp::Importer importer;
	std::string fullFilePath = directoryPath + "/" + filePath;

	/*const aiScene* scene = importer.ReadFile(filePath.c_str(),
		aiProcess_FlipWindingOrder | aiProcess_FlipUVs);*/
	const aiScene* scene = importer.ReadFile(fullFilePath.c_str(),
		aiProcess_FlipWindingOrder |
		aiProcess_FlipUVs |
		aiProcess_JoinIdenticalVertices);

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

			LineVertex vertex;
			vertex.position = { position.x, position.y, position.z, 1.0f };
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
	vertexResource_ = dxCommon_->CreateBufferResource(sizeof(LineVertex) * lineVertices.size());

	vertexData_ = nullptr;
	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
	std::memcpy(vertexData_, lineVertices.data(), sizeof(LineVertex) * lineVertices.size());

	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	vertexBufferView_.SizeInBytes = UINT(sizeof(LineVertex) * lineVertices.size());
    vertexBufferView_.StrideInBytes = sizeof(LineVertex);

	vertexCount_ = static_cast<uint32_t>(lineVertices.size());
}
