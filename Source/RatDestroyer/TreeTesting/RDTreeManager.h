// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RDTreeManager.generated.h"

class ARDTreeNode;

UCLASS()
class RATDESTROYER_API ARDTreeManager : public AActor
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Sets default values for this actor's properties
	ARDTreeManager();

	virtual void Tick(float DeltaTime) override;

	//Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TreeManager")
	ARDTreeNode* mRootNode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TreeManager")
	int32 mMaxDepth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TreeManager")
	float mObjectSpacing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TreeManager")
	TSubclassOf<AActor> NodeObject;

	//Functions

	UFUNCTION(BlueprintCallable, Category = "TreeManager")
	void BuildTree();

	UFUNCTION(BlueprintCallable, Category = "TreeManager")
	void CreateChildren(ARDTreeNode* ParentNode, int32 CurrentDepth, int32 MaxDepth);

};
