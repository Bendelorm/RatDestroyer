#pragma once

#include "CoreMinimal.h"
#include "Tile.h"


struct FNode
{
public:
  
	bool bObstacle = false;
	bool bVisited = false;
	float fGlobalGoal;
	float fLocalGoal;
	int32 X;
	int32 Y;
	FVector WorldLocation;
	TArray<FNode*> Neighbors;
	int32 Index;
	FNode* parent;

public:

	//UPROPERTY()
	//TArray <FNode> Nodes;
	//int32 nMapWidth = 10;
	//int32 nMapHeight = 10;
	//FNode* NodeStart = nullptr;
	//FNode* NodeEnd = nullptr;

	//int32 relativeCoords[4][2] =
	//{
	//	{0, -1}, {0, 1}, {-1, 0}, {1, 0}, //forward, backward, each side

	//};


	

};