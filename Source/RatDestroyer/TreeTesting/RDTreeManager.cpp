// Fill out your copyright notice in the Description page of Project Settings.

#include "RDTreeManager.h"
#include "RDTreeNode.h"

// Sets default values
ARDTreeManager::ARDTreeManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mMaxDepth = 3.f;
	mObjectSpacing = 200.f;
}

// Called when the game starts or when spawned
void ARDTreeManager::BeginPlay()
{
	Super::BeginPlay();

	BuildTree();
}

// Called every frame
void ARDTreeManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARDTreeManager::BuildTree()
{
	FActorSpawnParameters SpawnParams;
	mRootNode = GetWorld()->SpawnActor<ARDTreeNode>(NodeObject, GetActorLocation(), FRotator::ZeroRotator, SpawnParams);


	mRootNode->mData = 1;
	CreateChildren(mRootNode, 1, mMaxDepth);

}

void ARDTreeManager::CreateChildren(ARDTreeNode* ParentNode, int32 CurrentDepth, int32 MaxDepth)
{
	if (CurrentDepth >= MaxDepth)
	{
		return;
	}

	int32 AmountOfChildren = ParentNode->mMaxChildren;

	// ---- Extra stuff for spawning and position in Unreal ----
	float Radius = FMath::Max(mObjectSpacing, mObjectSpacing * (MaxDepth - CurrentDepth));
	// ----------------------------------------------------

	for (int32 i = 0; i < AmountOfChildren; ++i)
	{
		// ---- Extra stuff for spawning and position in Unreal ----
		float Angle = (360.f / AmountOfChildren) * i;
		float Rad = FMath::DegreesToRadians(Angle);

		FVector Offset(FMath::Cos(Rad) * Radius, FMath::Sin(Rad) * Radius, -mObjectSpacing * CurrentDepth);
		FVector ChildLocation = ParentNode->GetActorLocation() + Offset;

		FActorSpawnParameters SpawnParams;
		ARDTreeNode* ChildNode = GetWorld()->SpawnActor<ARDTreeNode>(NodeObject, ChildLocation, FRotator::ZeroRotator, SpawnParams);
		// ----------------------------------------------------


		ChildNode->mData = ParentNode->mData * 10 + i + 1;
		ChildNode->mMaxChildren = ParentNode->mMaxChildren;

		ParentNode->AddChild(ChildNode);

		CreateChildren(ChildNode, CurrentDepth + 1, MaxDepth);
	}
}
