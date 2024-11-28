// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"

#include "Kismet/GameplayStatics.h"
#include "Ratdestroyer/Player/PlayerPawn.h"

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = TileMeshComponent;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(TileMeshComponent);

	MaterialOne = CreateDefaultSubobject<UMaterialInterface>("MaterialOne");
	MaterialTwo = CreateDefaultSubobject<UMaterialInterface>("MaterialTwo");

	TileMeshComponent->OnBeginCursorOver.AddDynamic(this, &ATile::ChangeMatOnMouseOver);
	TileMeshComponent->OnEndCursorOver.AddDynamic(this, &ATile::EndMatOnMouseOver);

    DebugBoxColor = FColor::Green;

	bHasTower = false;
	Tags.Add("Buildable");
}

void ATile::ChangeMatOnMouseOver(UPrimitiveComponent* TouchedComponent)
{
	if (PlayerPawn->bCanBuild && !GetHasTower())
	{
		TileMeshComponent->SetMaterial(0, MaterialTwo);
	}
}

void ATile::EndMatOnMouseOver(UPrimitiveComponent* TouchedComponent)
{
	TileMeshComponent->SetMaterial(0, MaterialOne);
}

void ATile::SetHasTower(bool bHasNewTower)
{
	bHasTower = bHasNewTower;
}

bool ATile::GetHasTower()
{
	return bHasTower;
}

UStaticMeshComponent* ATile::GetStaticMesh() const
{
	return TileMeshComponent;
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
	TileMeshComponent->SetMaterial(0, MaterialOne);
	PlayerPawn = Cast<APlayerPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerPawn::StaticClass()));

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
	//DrawDebugBox(World, BoxLocation, BoxExtent, BoxRotation, DebugBoxColor, false, -1.f, 0, 5.f);


}