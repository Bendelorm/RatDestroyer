// Fill out your copyright notice in the Description page of Project Settings.



#include "GridManager.h"
#include "Tile.h"
#include "RatDestroyer/Enemy/RatEnemy.h"



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
			FVector TileLocation = FVector(X * TileSize, Y * TileSize, 50.f);


			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			if (TileClass)
			{
				ATile* NewTile = GetWorld()->SpawnActor<ATile>(TileClass, TileLocation, FRotator::ZeroRotator, SpawnParams);
				
				if (NewTile)
					{
					TileArray.Add(NewTile);
					}

			}

		}
	}

	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AGridManager::SpawnEnemy, SpawnInterval, true);

}

// Called every frame
void AGridManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AGridManager::IsTileBlocked(const FVector& TilePosition) 
{
	return BlockedTiles.Contains(TilePosition); //modify to work with our code
}

bool AGridManager::IsWithinGrid(const FVector& Position) const
{
	int32 X = Position.X / TileSize;
	int32 Y = Position.Y / TileSize;

	return X >= 0 && X < GridSizeX && Y >= 0 && Y < GridSizeY;
}

ATile* AGridManager::GetTileLocation(FVector Location)
{

	for (ATile* Tile : TileArray)
	{
		if (Tile && Tile->GetActorLocation().Equals(Location, 50.f))
		{
			return Tile;
		}
	}


	return nullptr;
}

TArray<ATile*> AGridManager::FindPath(ATile* StartTile, ATile* GoalTile)
{
	TArray<FNode*> OpenSet;   // Tiles to evaluate
	TArray<FNode*> ClosedSet; // Tiles already evaluated
	TArray<ATile*> Path;      // The path to return

	FNode* StartNode = new FNode(StartTile);
	FNode* GoalNode = new FNode(GoalTile);

	OpenSet.Add(StartNode);

	while (OpenSet.Num() > 0)
	{
		OpenSet.Sort([](const FNode& A, const FNode& B) { return A.fCost < B.fCost; });
		FNode* CurrentNode = OpenSet[0];

		if (CurrentNode->Tile == GoalNode->Tile)
		{
			// Reconstruct path
			FNode* Node = CurrentNode;
			while (Node)
			{
				Path.Insert(Node->Tile, 0); // Insert each tile at the start of the path
				Node = Node->Parent;
			}
			break;
		}

		OpenSet.Remove(CurrentNode);
		ClosedSet.Add(CurrentNode);

		TArray<FNode*> Neighbors = GetNeighbors(CurrentNode);

		for (FNode* Neighbor : Neighbors)
		{
			if (IsTileBlocked(Neighbor->Tile->GetActorLocation()) || ClosedSet.Contains(Neighbor))
				continue;

			float NewMovementCost = CurrentNode->gCost + CalculateHeuristic(CurrentNode->Tile, Neighbor->Tile);
			if (NewMovementCost < Neighbor->gCost || !OpenSet.Contains(Neighbor))
			{
				Neighbor->gCost = NewMovementCost;
				Neighbor->hCost = CalculateHeuristic(Neighbor->Tile, GoalNode->Tile);
				Neighbor->CalculateFCost();
				Neighbor->Parent = CurrentNode;

				if (!OpenSet.Contains(Neighbor))
				{
					OpenSet.Add(Neighbor);
				}
			}
		}
	}

	// Cleanup
	for (FNode* Node : OpenSet) delete Node;
	for (FNode* Node : ClosedSet) delete Node;

	return Path;
}

TArray<FNode*> AGridManager::GetNeighbors(FNode* Node)
{
	TArray<FNode*> Neighbors;
	int32 Index = TileArray.IndexOfByKey(Node->Tile);

	if (Index != INDEX_NONE)
	{
		// Retrieve adjacent tiles by index
		if (Index - GridSizeX >= 0) Neighbors.Add(new FNode(TileArray[Index - GridSizeX])); // Up
		if (Index + GridSizeX < TileArray.Num()) Neighbors.Add(new FNode(TileArray[Index + GridSizeX])); // Down
		if (Index % GridSizeX > 0) Neighbors.Add(new FNode(TileArray[Index - 1])); // Left
		if ((Index + 1) % GridSizeX > 0) Neighbors.Add(new FNode(TileArray[Index + 1])); // Right
	}

	return Neighbors;
}

float AGridManager::CalculateHeuristic(ATile* StartTile, ATile* GoalTile)
{
	FVector StartLocation = StartTile->GetActorLocation();
	FVector GoalLocation = GoalTile->GetActorLocation();
	return FMath::Abs(StartLocation.X - GoalLocation.X) + FMath::Abs(StartLocation.Y - GoalLocation.Y);
}

void AGridManager::SpawnEnemy()
{
	

	if (TileArray.Num() > 1)
	{
		ATile* StartTile = TileArray[0];
		ATile* GoalTile = TileArray[TileArray.Num() - 1];

		FVector SpawnLocation = StartTile->GetActorLocation() + FVector(0, 0, 200.f);

		ARatEnemy* NewEnemy = GetWorld()->SpawnActor<ARatEnemy>(EnemyClass, SpawnLocation, FRotator::ZeroRotator);

		if (NewEnemy)
		{
			// Use `FindPath` with `ATile*` parameters
			TArray<ATile*> Path = FindPath(StartTile, GoalTile);

			// Pass tile locations to the enemy's path as FVectors
			TArray<FVector> PathLocations;
			for (ATile* Tile : Path)
			{
				PathLocations.Add(Tile->GetActorLocation());
			}

			if (PathLocations.Num() > 0)
			{
				NewEnemy->SetPath(PathLocations);
				UE_LOG(LogTemp, Warning, TEXT("Enemy spawned and path set with %d locations."), PathLocations.Num());
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to find a path from %s to %s"), *SpawnLocation.ToString(), *GoalTile->GetActorLocation().ToString());
				// Optionally, destroy the enemy here if the path is invalid
				NewEnemy->Destroy();
			}
		}
	}
}



