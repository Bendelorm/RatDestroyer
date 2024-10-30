// Fill out your copyright notice in the Description page of Project Settings.


#include "RDTowerManager.h"
#include "RatDestroyer/Map/Tile.h"

#include <memory>

#include "Kismet/GameplayStatics.h"
#include "RatDestroyer/Map/GridManager.h"
#include "RatDestroyer/Tower/RDTowerActor.h"

// Sets default values
ARDTowerManager::ARDTowerManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ARDTowerManager::Push(AActor* PlacedTower)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Added to stack")));

	PlacedTowerStack.Add(PlacedTower);
}

AActor* ARDTowerManager::Pop()
{
	if (IsEmpty())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Stack is empty")));

		return nullptr;
	}

	AActor* TopTowerActor = PlacedTowerStack.Last();
	PlacedTowerStack.RemoveAt(Size() - 1);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Actor has been removed from stack")));
	ATile* DeletedFromTile = Cast<ATile>(TopTowerActor->GetAttachParentActor());
	for (ATile* TileInArray : GridManager->TileArray)
	{
		if (TileInArray == DeletedFromTile)
		{
			ParentTile = TileInArray;
			ParentTile->SetHasTower(false);
		}
	}
	TopTowerActor->Destroy();
	return TopTowerActor;
}

int32 ARDTowerManager::Size() const
{
	return PlacedTowerStack.Num();
}

bool ARDTowerManager::IsEmpty() const
{
	return PlacedTowerStack.Num() == 0;
}

// Called when the game starts or when spawned
void ARDTowerManager::BeginPlay()
{
	Super::BeginPlay();
	GridManager = Cast<AGridManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGridManager::StaticClass()));
}

// Called every frame
void ARDTowerManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

