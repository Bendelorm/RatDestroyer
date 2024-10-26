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

	MaterialOne = CreateDefaultSubobject<UMaterialInterface>("MaterialOne");
	MaterialTwo = CreateDefaultSubobject<UMaterialInterface>("MaterialTwo");

	StaticMeshComponent->OnBeginCursorOver.AddDynamic(this, &ATile::ChangeMatOnMouseOver);
	StaticMeshComponent->OnEndCursorOver.AddDynamic(this, &ATile::EndMatOnMouseOver);

    DebugBoxColor = FColor::Green;

	bHasTower = false;
	Tags.Add("Buildable");
}

void ATile::ChangeMatOnMouseOver(UPrimitiveComponent* TouchedComponent)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Moused over lol")));
	StaticMeshComponent->SetMaterial(0, MaterialTwo);
}

void ATile::EndMatOnMouseOver(UPrimitiveComponent* TouchedComponent)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Stopped mousing over lol")));
	StaticMeshComponent->SetMaterial(0, MaterialOne);
}

void ATile::SetHasTower(bool bHasNewTower)
{
	bHasTower = bHasNewTower;
}

bool ATile::GetHasTower()
{
	return bHasTower;
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
	StaticMeshComponent->SetMaterial(0, MaterialOne);
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