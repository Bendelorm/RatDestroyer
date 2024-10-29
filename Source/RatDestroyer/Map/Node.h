#pragma once

#include "CoreMinimal.h"



struct FNode
{
   

public: 
    FVector Position;
    float gCost;
    float hCost;
    float fCost;
    FNode* Parent;

    // Constructor to initialize position and costs
    FNode(FVector InPosition) : Position(InPosition), gCost(0), hCost(0), fCost(0), Parent(nullptr) {}

    // Function to calculate fCost
    void CalculateFCost() { fCost = gCost + hCost; }
};