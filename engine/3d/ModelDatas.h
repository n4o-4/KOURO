#pragma once
#include <string>
#include <vector>
#include "Structs.h"
#include <map>

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


struct Joint
{
	QuaternionTransform transform; // transform���
	Matrix4x4 localMatrix; // localMatrix
	Matrix4x4 skeletonSpaceMatrix; // skeletonSpace�ł̕ϊ��s��
	std::string name; // ���O
	std::vector<int32_t> children; // �qJoint��Index�̃��X�g�B���Ȃ���΋�
	int32_t index; // ���g��Index
	std::optional<int32_t> parent; // �eJoint��Index�B���Ȃ����null
};

struct Skeleton
{
	int32_t root; //RootJoint��Index
	std::map<std::string, int32_t> jointMap; // Joint����Index�Ƃ̎���
	std::vector<Joint> joints; // �������Ă���Joint
};