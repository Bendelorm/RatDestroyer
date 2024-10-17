// Fill out your copyright notice in the Description page of Project Settings.



#include "GridManager.h"
#include "Tile.h"


// Sets default values
AGridManager::AGridManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));

	if (RootComponent == nullptr)
	{
		SceneComponent = RootComponent;
	}

	else
	{
		SceneComponent->SetupAttachment(RootComponent);
	}

}

// Called when the game starts or when spawned
void AGridManager::BeginPlay()
{
	Super::BeginPlay();
	
	for (int32 X = 0; X < GridSizeX; ++X)
	{
		for (int32 Y = 0; Y < GridSizeY; ++Y)
		{
			FVector TileLocation = FVector(X * TileSize, Y * TileSize, 0.0f);


			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			if (TileClass)
			{
				GetWorld()->SpawnActor<ATile>(TileClass, TileLocation, FRotator::ZeroRotator, SpawnParams);
			}

		}
	}



}

// Called every frame
void AGridManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

