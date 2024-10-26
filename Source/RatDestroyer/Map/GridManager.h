// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "GridManager.generated.h"


class ATile;

UCLASS()
class RATDESTROYER_API AGridManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridManager();

	UPROPERTY(VisibleAnywhere)
	USceneComponent* SceneComponent;

	struct FNode
	{
		FVector Position;
		float gCost;
		float hCost;
		float fCost;
		FNode* Parent;

		FNode(FVector InPosition) : Position (InPosition), gCost(0), hCost(0), fCost(0), Parent(nullptr) {}

		void CalculateFCost() { fCost = gCost + hCost; }
	};


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable")
	int32 GridSizeX = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable")
	int32 GridSizeY = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable")
	float TileSize = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	TSubclassOf<ATile> TileClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grid")
	TArray<ATile*> TileArray;

	UPROPERTY()
	TSet<FVector> BlockedTiles;


	UFUNCTION()
	bool IsTileBlocked(const FVector& TilePosition);

	UFUNCTION()
	bool IsWithinGrid(const FVector& Position) const;
		
	UFUNCTION()
	ATile* GetTileLocation(FVector Location);

	UFUNCTION()
	TArray<FVector>FindPath(const FVector& StartPos, const FVector& GoalPos);
	
	UFUNCTION()
	TArray<FNode*>GetNeighbors(FNode* Node);

	
	UFUNCTION()
	float CalculateHeuristic(const FVector& Start, const FVector& Goal);

	/*UFUNCTION()
	void SpawnEnemy();*/

		
};
