// Fill out your copyright notice in the Description page of Project Settings.


#include "RDTowerActor.h"
#include "RatDestroyer/Map/Tile.h"

// Sets default values
ARDTowerActor::ARDTowerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TowerMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = TowerMeshComponent;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("UBoxComponent"));
	BoxComponent->SetupAttachment(TowerMeshComponent);
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

