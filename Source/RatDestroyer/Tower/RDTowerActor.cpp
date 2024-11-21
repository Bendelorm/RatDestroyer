// Fill out your copyright notice in the Description page of Project Settings.


#include "RDTowerActor.h"
#include "RatDestroyer/Map/Tile.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "RatDestroyer/Enemy/RatEnemy.h"
#include "RatDestroyer/Enemy/WaveManager.h"

// Sets default values
ARDTowerActor::ARDTowerActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TowerMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = TowerMeshComponent;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("UBoxComponent"));
	BoxComponent->SetupAttachment(TowerMeshComponent);

	BaseAttackRange = 200.0f;
	AttackRangeComponent = CreateDefaultSubobject<USphereComponent>(TEXT("AttackRangeComponent"));
	AttackRangeComponent->SetupAttachment(TowerMeshComponent);
	AttackRangeComponent->SetSphereRadius(BaseAttackRange);
	AttackRangeComponent->SetGenerateOverlapEvents(true);
	AttackRangeComponent->OnComponentBeginOverlap.AddDynamic(this, &ARDTowerActor::OnOverlapBegin);
	AttackRangeComponent->OnComponentEndOverlap.AddDynamic(this, &ARDTowerActor::OnOverlapEnd);

	BaseCost = 10;
	BaseDamage = 2;
	BaseAttackTime = 1.0f;
	Enemy = nullptr;
}

void ARDTowerActor::OnOverlapBegin(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
                                   class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("SOMETHING ENTERED THE SPHERE")));

	if (OtherActor->ActorHasTag("Enemy"))
	{
		ARatEnemy* EnteredEnemy = Cast<ARatEnemy>(OtherActor);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("%s entered"), *EnteredEnemy->GetName()));
		AttackPriorityQueue.Add(EnteredEnemy);

	}
}

void ARDTowerActor::OnOverlapEnd(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag("Enemy"))
	{
		ARatEnemy* LeavingEnemy = Cast<ARatEnemy>(OtherActor);
		for (int32 i = 0; i < AttackPriorityQueue.Num(); i++)
		{
			if (LeavingEnemy->GetName() == AttackPriorityQueue[i]->GetName())
			{
				AttackPriorityQueue.RemoveAt(i);
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("removed at %d"), i));
				break;
			}
		}
	}
}

void ARDTowerActor::TraverseTree(NodeUpgrade* root)
{
	if (root == nullptr) return;

	// Process the current node first (pre-order)
	if (PlayerPawn->Money >= root->cost)
	{
		// Apply the upgrade
		ApplyUpgrade(root);

		// Deduct the cost
		PlayerPawn->Money -= root->cost;

		UE_LOG(LogTemp, Warning, TEXT("Purchased upgrade: Node %d"), root->NodeData);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot afford upgrade: Node %d"), root->NodeData);
		// Stop processing further nodes if a node is unaffordable
		return;  // No need to continue if one node is unaffordable
	}

	// Now process the left child (if exists)
	if (root->Left.IsValid())
	{
		TraverseTree(root->Left.Get());  // Recursively process left child
		UE_LOG(LogTemp, Warning, TEXT("left"));
	}

	// Then process the right child (if exists)
	if (root->Right.IsValid())
	{
		TraverseTree(root->Right.Get());  // Recursively process right child
		UE_LOG(LogTemp, Warning, TEXT("right"));
	}

}


void ARDTowerActor::ApplyUpgrade(NodeUpgrade* upgradeNode)
{
	BaseDamage += upgradeNode->damage;
	BaseAttackTime += upgradeNode->fireRate;
	//BaseAccuracy + upgradeNode->accuracy;
	BaseAttackRange += upgradeNode->range;
}


// Called when the game starts or when spawned
void ARDTowerActor::BeginPlay()
{
	Super::BeginPlay();

	WaveManager = Cast<AWaveManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AWaveManager::StaticClass()));
	PlayerPawn = Cast<APlayerPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerPawn::StaticClass()));


	UpgradeTreePTR = new UpgradeTree();

	TraverseTree(NodeUpgrade* root);



}

// Called every frame
void ARDTowerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}