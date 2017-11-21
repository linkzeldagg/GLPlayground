#include "ChunkOctreeNode.h"

ChunkOctreeNode::ChunkOctreeNode(glm::vec3 _pos, glm::vec3 _centerPos, int _scale) :
	pos(_pos), 
	centerPos(_centerPos), 
	scale(_scale)
{
	memset(child, 0, sizeof(child));
	group = NULL;
}

ChunkOctreeNode::~ChunkOctreeNode()
{
}

void ChunkOctreeNode::CreateGroup()
{
	//TODO
	group = new blockGroup(true, (float)scale);
}

void ChunkOctreeNode::BuildGroupData()
{
	//TODO
	group->Init_sinXsinY(
		VariablePool::lambdax, VariablePool::lambdaz,
		VariablePool::px, VariablePool::pz,
		VariablePool::ax, VariablePool::az,
		pos.x, pos.z, pos.y);
}

void ChunkOctreeNode::InitGroupMesh()
{
	group->InitBuffers();
}

void ChunkOctreeNode::BuildGroupMesh()
{
	group->GenerateBuffer(false, VariablePool::cs_ChunkMeshGeneration_ScaleIndex);

	groupReady = true;
}

void ChunkOctreeNode::FreeGroupBuffer()
{
	if (group != NULL)
	{
		group->FreeBuffers();
	}
}

void ChunkOctreeNode::ClearGroup()
{
	//TODO
	delete group;
	group = NULL;
}

ChunkOctreeNode * ChunkOctreeNode::GetMostLeft()
{
	if (child[0] == NULL)
	{
		return this;
	}
	return child[0] -> GetMostLeft();
}

ChunkOctreeNode * ChunkOctreeNode::GetMostRight()
{
	if (child[7] == NULL)
	{
		return this;
	}
	return child[7] -> GetMostRight();
}