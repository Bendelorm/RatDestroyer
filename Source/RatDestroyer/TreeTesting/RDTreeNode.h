// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RDTreeNode.generated.h"

UCLASS()
class RATDESTROYER_API ARDTreeNode : public AActor
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Sets default values for this actor's properties
	ARDTreeNode();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Variables

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TreeNode")
	int32 mData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TreeNode")
	int32 mMaxChildren;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TreeNode")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TreeNode")
	TArray<ARDTreeNode*> mChildren;


	//Functions

	UFUNCTION(BlueprintCallable, Category = "TreeNode")
	void AddChild(ARDTreeNode* ChildNode);
};
