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

TArray<FVector> AGridManager::FindPath(const FVector& StartPos, const FVector& GoalPos)
{
	TArray<FNode*> OpenSet; //number of nodes to evaluate
	TArray<FNode*> ClosedSet; //nodes that have been evaluated
	TArray<FVector> Path; //path to return

	FNode* StartNode = new FNode(StartPos);
	FNode* GoalNode = new FNode(GoalPos);

	OpenSet.Add(StartNode);

	while (OpenSet.Num() > 0)
	{
		OpenSet.Sort([](const FNode& A, const FNode& B) {return A.fCost < B.fCost; });
		FNode* CurrentNode = OpenSet[0];

		if (CurrentNode->Position == GoalNode->Position)
		{
			//path found
			FNode* Node = CurrentNode;
			while (Node)
			{
				Path.Insert(Node->Position, 0);
				Node = Node->Parent;
			}
			break;
		}


		OpenSet.Remove(CurrentNode);
		ClosedSet.Add(CurrentNode);

		TArray<FNode*> Neighbors = GetNeighbors(CurrentNode);

		for (FNode* Neighbor : Neighbors)
		{
			if (IsTileBlocked(Neighbor->Position) || ClosedSet.Contains(Neighbor))
				continue;

			float NewMovementCost = CurrentNode->gCost + CalculateHeuristic(CurrentNode->Position, Neighbor->Position);
			if (NewMovementCost < Neighbor->gCost || !OpenSet.Contains(Neighbor))
			{
				Neighbor->gCost = NewMovementCost;
				Neighbor->hCost = CalculateHeuristic(Neighbor->Position, GoalNode->Position);
				Neighbor->CalculateFCost();
				Neighbor->Parent = CurrentNode;

				if (!OpenSet.Contains(Neighbor))
				{
					OpenSet.Add(Neighbor);
				}

			}

		}

	}

	//Cleaning up nodes
	for (FNode* Node : OpenSet) delete Node;
	for (FNode* Node : ClosedSet) delete Node;



	return Path;
}

TArray<FNode*>AGridManager::GetNeighbors(FNode* Node) 
{
	
	TArray<FNode*> Neighbors;

	TArray<FVector> NeighborPos =
	{

		FVector(Node->Position.X + TileSize, Node->Position.Y, 0),
		FVector(Node->Position.X - TileSize, Node->Position.Y, 0),
		FVector(Node->Position.X, Node->Position.Y + TileSize, 0),
		FVector(Node->Position.X, Node->Position.Y - TileSize, 0)

	};

	for (const FVector& Position : NeighborPos)
	{
		if (IsWithinGrid(Position))
		{
			Neighbors.Add(new FNode(Position));
		}
	}

	return Neighbors;
}

float AGridManager::CalculateHeuristic(const FVector& Start, const FVector& Goal)
{
	return FMath::Abs(Start.X - Goal.X) + FMath::Abs(Start.Y - Goal.Y);
}

void AGridManager::SpawnEnemy()
{
	FVector SpawnLocation = FVector(0.0f, 0.0f, 50.0f);;
	FVector GoalLocation =	FVector(1000.0f, 1000.0f, 50.0f); 


	ARatEnemy* NewEnemy = GetWorld()->SpawnActor<ARatEnemy>(SpawnLocation, FRotator::ZeroRotator);

	
	TArray<FVector> Path = FindPath(SpawnLocation, GoalLocation);

		if (NewEnemy)
			{
			NewEnemy->SetPath(Path);
			}

}

