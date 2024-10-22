// Fill out your copyright notice in the Description page of Project Settings.


#include "baseTurret.h"

// Sets default values
AbaseTurret::AbaseTurret()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = StaticMeshComponent;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("UBoxComponent"));
	RootComponent = StaticMeshComponent;

}

// Called when the game starts or when spawned
void AbaseTurret::BeginPlay()
{
	Super::BeginPlay();
	
	


}

// Called every frame
void AbaseTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

