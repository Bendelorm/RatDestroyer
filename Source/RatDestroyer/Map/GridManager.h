// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"
#include "Components/SceneComponent.h"
#include "GridManager.generated.h"

class ATile;
class ARatEnemy;
class ARDTowerManager;


USTRUCT(BlueprintType)
struct FNode
{
	GENERATED_BODY()
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
};

UCLASS()
class RATDESTROYER_API AGridManager : public AActor
{
	GENERATED_BODY()

	
public:	
	// Sets default values for this actor's properties
	AGridManager();

	UPROPERTY(VisibleAnywhere)
	USceneComponent* SceneComponent;

	TObjectPtr<ARDTowerManager> TowerManager;
	

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

	UPROPERTY()
	TArray <FNode> Nodes;
	int32 nMapWidth = 10;
	int32 nMapHeight = 10;
	FNode* NodeStart = nullptr;
	FNode* NodeEnd = nullptr;

	int32 relativeCoords[4][2] =
	{
		{0, -1}, {0, 1}, {-1, 0}, {1, 0}, //forward, backward, each side

	};

	TArray<FVector> VisitedCheckpoints;

	UFUNCTION()
	void CreateGraph();

	//UFUNCTION()
	////void UpdateGraph(float DeltaTime);

	UFUNCTION()
	TArray<FNode>GetNeighbors(FNode& currentNode);

	UFUNCTION()
	void Solve_AStar();
};