// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = StaticMeshComponent;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = StaticMeshComponent;

    DebugBoxColor = FColor::Green;

}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
	
    
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	UWorld* World = GetWorld();
	if (!World) return;

	FVector BoxLocation = GetActorLocation() + FVector(0.f, 0.f, 50.f);
	FVector BoxExtent = FVector(50.f, 50.f, 50.f);
	FQuat BoxRotation = FQuat::Identity;

	// Draw the debug box using the stored DebugBoxColor
	DrawDebugBox(World, BoxLocation, BoxExtent, BoxRotation, DebugBoxColor, false, -1.f, 0, 5.f);


}

void ATile::changeDebugColor(FColor NewColor)
{
	DebugBoxColor = NewColor;
}

