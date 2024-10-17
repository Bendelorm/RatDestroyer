// Fill out your copyright notice in the Description page of Project Settings.


#include "RDTreeNode.h"

// Sets default values
ARDTreeNode::ARDTreeNode()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	RootComponent = MeshComponent;
	mData = 0;
	mMaxChildren = 2;

}

// Called when the game starts or when spawned
void ARDTreeNode::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARDTreeNode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Visual rep
	for(ARDTreeNode* Child : mChildren)
	{
		if (Child)
		{
			DrawDebugLine(GetWorld(), GetActorLocation(), Child->GetActorLocation(), FColor::Yellow, false, 0.5f, 0.f, 5.f);
		}
	}

}

void ARDTreeNode::AddChild(ARDTreeNode* ChildNode)
{
	if (mChildren.Num() < mMaxChildren && ChildNode)
	{
		mChildren.Add(ChildNode);
	}
}