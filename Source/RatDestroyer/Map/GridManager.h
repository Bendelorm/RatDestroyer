// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "Node.h"
#include "GridManager.generated.h"



class ATile;
class ARatEnemy;



UCLASS()
class RATDESTROYER_API AGridManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridManager();

	UPROPERTY(VisibleAnywhere)
	USceneComponent* SceneComponent;

	

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	TSubclassOf<ARatEnemy> EnemyClass;

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





	// the code below should be moved //

	UFUNCTION()
	TArray<ATile*>FindPath(ATile* StartTile, ATile* GoalTile);
	
	
	UFUNCTION()
	float CalculateHeuristic(ATile* StartTile, ATile* GoalTile);

	UFUNCTION()
	void SpawnEnemy();

	TArray<FNode*>GetNeighbors(FNode* Node);
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	float SpawnInterval = 5.0f; 

	FTimerHandle SpawnTimerHandle;

	// the code above should be moved //

};
