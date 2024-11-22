// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Projectile.h"
#include "RatDestroyer/Player/PlayerPawn.h"
#include "RDTowerActor.generated.h"


class AWaveManager;
class ARatEnemy;
class USphereComponent;
class ATile;
class UpgradeTree;

struct NodeUpgrade
{
	int32 NodeData;
	float damage;
	float fireRate;
	float accuracy;
	float range;
	float cost;
	TUniquePtr<NodeUpgrade> Left;
	TUniquePtr<NodeUpgrade> Right;

	NodeUpgrade(int32 InNodeData, float InDamage, float InFireRate, float InAccuracy, float InRange, float InCost)
		: NodeData(InNodeData), damage(InDamage), fireRate(InFireRate), accuracy(InAccuracy), range(InRange), cost(InCost),
		Left(nullptr), Right(nullptr) {}

};

UCLASS()
class UUpgradeTree : public UObject
{

	GENERATED_BODY()

public:
	TUniquePtr<NodeUpgrade> Root;

	UUpgradeTree()
	{
		//set the different values inside the parenthesis
		//								(Node Number, Damage, FireRate, Accuracy, Range, Cost)
		Root = MakeUnique<NodeUpgrade>(1, 0.f, 0.f, 0.f, 0.f, 0.f);
		Root->Left = MakeUnique<NodeUpgrade>(2, 1.f, 1.f, 0.f, 0.f, 25.f);
		Root->Right = MakeUnique<NodeUpgrade>(3, 0.f, 0.f, 100.f, 100.f, 25.f);
		Root->Left->Left = MakeUnique<NodeUpgrade>(4, 1.f, 0.f, 0.f, 0.f, 50.f);
		Root->Left->Right = MakeUnique<NodeUpgrade>(5, 0.f, 1.f, 0.f, 0.f, 50.f);
		Root->Right->Left = MakeUnique<NodeUpgrade>(6, 0.f, 0.f, 100.f, 0.f, 50.f);
		Root->Right->Right = MakeUnique<NodeUpgrade>(7, 0.f, 0.f, 0.f, 100.f, 50.f);

	};



};

UCLASS()
class RATDESTROYER_API ARDTowerActor : public AActor
{
	GENERATED_BODY()

	/////////////////  Upgrade system /////////////////////




	// Visualization of tree
   //		  1 (root)
   //		/   \
    //     2	 3
	//    / \	 / \
    //   4   5   6  7



	public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Sets default values for this actor's properties
	ARDTowerActor();

	TObjectPtr<ATile> Tile;

	TObjectPtr<ARatEnemy> Enemy;

	TObjectPtr<AWaveManager> WaveManager;

	TObjectPtr<APlayerPawn> PlayerPawn;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UStaticMeshComponent* TowerMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	USphereComponent* AttackRangeComponent;


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

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<AProjectile> Projectile;

	UPROPERTY(EditDefaultsOnly, Category = "FireSpeed")
	float LaunchSpeed = 4000;

	UPROPERTY()
	UUpgradeTree* UpgradeTreePTR;


	

	//Functions

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	




	////Pre Order Traversal - Node 1, 2 , 4 , 5 , 3 , 6 , 7
	//UFUNCTION()
	void TraverseTree(UUpgradeTree* root);

	////For applying the new values from the Upgrade tree to the tower 
	//UFUNCTION()
	void ApplyUpgrade(NodeUpgrade* upgradeNode);

	//////////////////////////////////////////////////


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
