// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RDTowerManager.generated.h"

class ARDTowerActor;
class ATile;
class AGridManager;

UCLASS()
class RATDESTROYER_API ARDTowerManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARDTowerManager();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	TObjectPtr<ATile> ParentTile;

	TObjectPtr<AGridManager> GridManager;

	//Variables;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TowerStack")
	TArray<AActor*> PlacedTowerStack;

	//Functions
	UFUNCTION()
	void Push(AActor* PlacedTower);

	UFUNCTION()
	AActor* Pop();


	UFUNCTION()
	int32 Size() const;


	UFUNCTION()
	bool IsEmpty() const;

	UFUNCTION()
	void DeleteTower(AActor* TowerToBeDeleted);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
