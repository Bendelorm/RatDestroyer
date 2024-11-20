// Fill out your copyright notice in the Description page of Project Settings.


#include "RDTowerActor.h"
#include "RatDestroyer/Map/Tile.h"
#include "Components/SphereComponent.h"
#include "RatDestroyer/Enemy/RatEnemy.h"
// Sets default values
ARDTowerActor::ARDTowerActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TowerMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = TowerMeshComponent;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("UBoxComponent"));
	BoxComponent->SetupAttachment(TowerMeshComponent);
	BaseCost = 10;
	BaseDamage = 2;
	BaseAttackTime = 1.0f;
	
	TargetRadius = CreateDefaultSubobject<UBoxComponent>(TEXT("TargetRadius"));
	TargetRadius->SetBoxExtent(FVector(100.f, 100.f, 50.f));
	TargetRadius->SetCollisionProfileName(TEXT("OverlapAll"));

	TargetRadius->OnComponentBeginOverlap.AddDynamic(this, &ARDTowerActor::OnTargetDetected);
	TargetRadius->OnComponentEndOverlap.AddDynamic(this, &ARDTowerActor::OnTargetLost);



	DetectedEnemies.Empty();

}



void ARDTowerActor::OnTargetDetected(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	
	if (OtherActor && OtherActor->ActorHasTag(FName("Enemy")))
	{
		ARatEnemy* Enemy = Cast<ARatEnemy>(OtherActor);
		if (Enemy)
		{
			
			DetectedEnemies.Add(Enemy);
			UE_LOG(LogTemp, Warning, TEXT("Detected Enemy: %s"), *Enemy->GetName());
		}
	}

}
void ARDTowerActor::OnTargetLost(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->ActorHasTag(FName("Enemy")))
	{
		ARatEnemy* Enemy = Cast<ARatEnemy>(OtherActor);
		if (Enemy)
		{
			DetectedEnemies.Remove(Enemy);
			UE_LOG(LogTemp, Warning, TEXT("Lost Enemy: %s"), *Enemy->GetName());
		}
	}

}


// Called when the game starts or when spawned
void ARDTowerActor::BeginPlay()
{
	Super::BeginPlay();
	

}

// Called every frame
void ARDTowerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}