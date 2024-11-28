// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "RDTowerActor.generated.h"

class AWaveManager;
class ARatEnemy;
class USphereComponent;
class ATile;

UCLASS()
class RATDESTROYER_API ARDTowerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Sets default values for this actor's properties
	ARDTowerActor();

	TObjectPtr<ATile> Tile;

	TObjectPtr<ARatEnemy> Enemy;

	TObjectPtr<AWaveManager> WaveManager;



	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UStaticMeshComponent* TowerMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	USphereComponent* AttackRangeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	USkeletalMeshComponent* AttackComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* FireAnimation;

	//Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable")
	int32 BaseDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable")
	int32 BaseCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable")
	float BaseAttackTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable")
	float BaseAttackRange;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy")
	TArray<AActor*> AttackPriorityQueue;
	FTimerHandle TimerHandle;





	//Functions

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void TowerAttackEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
