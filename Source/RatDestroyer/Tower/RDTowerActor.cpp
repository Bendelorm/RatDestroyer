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
		AttackPriorityQueue.Remove(LeavingEnemy);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("removed at %d"), i ));
	}
}

// Called when the game starts or when spawned
void ARDTowerActor::BeginPlay()
{
	Super::BeginPlay();
	WaveManager = Cast<AWaveManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AWaveManager::StaticClass()));

}

// Called every frame
void ARDTowerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}