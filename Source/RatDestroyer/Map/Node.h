#pragma once

#include "CoreMinimal.h"
#include "Tile.h"


struct FNode
{
public:
    ATile* Tile;
    float gCost;
    float hCost;
    float fCost;
    FNode* Parent;

    // Constructor to initialize tile and costs
    FNode(ATile* InTile) : Tile(InTile), gCost(0), hCost(0), fCost(0), Parent(nullptr) {}

    // Function to calculate fCost
    void CalculateFCost() { fCost = gCost + hCost; }
};