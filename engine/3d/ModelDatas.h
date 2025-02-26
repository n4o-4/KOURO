#pragma once
#include <string>
#include <vector>
#include "Structs.h"

struct Node
{
	QuaternionTransform transform;
	Matrix4x4 localMatrix;
	std::string name;
	std::vector<Node> children;

};

struct MaterialData
{
	std::string textureFilePath;
	uint32_t textureIndex;
};

struct Material {
	Vector4 color;
	int enableLighting;
	float padding[3];
	Matrix4x4 uvTransform;
	float shininess;
	Vector3 specularColor;
};

struct VertexData
{
	Vector4 position;
	Vector2 texcoord;
	Vector3 normal;
};

struct ModelData {
	std::vector<VertexData> vertices;
	MaterialData material;
	Node rootNode;
};
