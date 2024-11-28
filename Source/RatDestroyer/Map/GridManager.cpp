// Fill out your copyright notice in the Description page of Project Settings.



#include "GridManager.h"
#include "Tile.h"
#include "Kismet/GameplayStatics.h"
#include "RatDestroyer/Tower/RDTowerManager.h"
#include "Algo/Reverse.h"


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
	
	// A* 
	CreateGraph();
	//UpdateGraph(0);
	Solve_AStar();
	// A*
	TowerManager = Cast<ARDTowerManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ARDTowerManager::StaticClass()));



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
	TileArray[0]->bHasTower = true;
	TileArray[99]->bHasTower = true;
}

// Called every frame
void AGridManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AGridManager::CreateGraph()
{
	for (int32 x = 0; x < nMapWidth; x++)
		for (int32 y = 0; y < nMapHeight; y++)
		{
			int32 CurrentIndex = y * nMapHeight + x;
			FNode Node;
			Node.X = x;
			Node.Y = y;

			//bool bRandomBool = (FMath::FRand() <= 0.1); //probability of being an obstacle

			//if (bRandomBool)
			//	Node.bObstacle = true;

			//else
			//{
			//	Node.bObstacle = false;
			//}


			Node.bVisited = false;
			Node.parent = nullptr;
			Node.WorldLocation = FVector(x* TileSize, y * TileSize, 0); //set size of node
			Nodes.Add(Node);
		}

	//when the nodes are stored in the array, get neighbors
	for (int32 x = 0; x < nMapWidth; x++)
		for (int32 y = 0; y < nMapHeight; y++)
		{
			int32 CurrentIndex = y * nMapHeight + x;
			FNode* Node = &Nodes[CurrentIndex];
			GetNeighbors(*Node);
		}

	NodeStart = &Nodes[0]; //assigning path nodes
	NodeEnd = &Nodes[99];

	DrawDebugBox(GetWorld(), NodeStart->WorldLocation, FVector(100, 100, 50), FColor::Blue, false, 200.f, 30.f);
	DrawDebugBox(GetWorld(), NodeEnd->WorldLocation, FVector(100, 100, 50), FColor::Red, false, 200.f, 30.f);
}

//void AGridManager::UpdateGraph(float DeltaTime)
//{
//	for (int32 x = 0; x < nMapWidth; x++)
//		for (int32 y = 0; y < nMapHeight; y++)
//		{
//			int32 CurIndexNode = y * nMapWidth + x;
//			FNode* CurrentNode = &Nodes[CurIndexNode];
//			FVector CenterOfNode = CurrentNode->WorldLocation;
//
//			if (CurrentNode->bObstacle)
//				DrawDebugBox(GetWorld(), CenterOfNode, FVector(100, 100, 100), FColor::Black, false, 200.f, 10.f);
//
//			else
//			{
//				DrawDebugBox(GetWorld(), CenterOfNode, FVector(100, 100, 100), FColor::White, false, 200.f, 10.f);
//			}
//
//			if (CurrentNode->bVisited)
//				DrawDebugBox(GetWorld(), CurrentNode->WorldLocation, FVector(100, 100, 100), FColor::Turquoise, false, 200.f, 16.f);
//
//
//		}
//}

TArray<FNode> AGridManager::GetNeighbors(FNode& currentnode)
{

	TArray<FNode> Neighbors;
	for (int32 i = 0; i < 4; i++)
	{
		int32 x = currentnode.X + relativeCoords[i][0];
		int32 y = currentnode.Y + relativeCoords[i][1];

		if (x >= 0 && x < nMapWidth && y >= 0 && y < nMapHeight)
		{
			int32 neighborIndex = x * nMapHeight + y;
			FNode* Node = &Nodes[neighborIndex];
			currentnode.Neighbors.Add(Node);
		}

	}

	return TArray<FNode>();
}

void AGridManager::Solve_AStar()
{

	//Clear all checkpoints before start
	VisitedCheckpoints.Empty();
	PathCheckpoints.Empty();

	for (int32 i = 0; i < Nodes.Num(); i++)
	{
		FNode* Node = &Nodes[i];
		Node->bVisited = false;
		Node->fGlobalGoal = 99000000; //infinite
		Node->fLocalGoal = 99000000;  //infinite
		Node->parent = nullptr;

	}

	auto distance = [](FNode* a, FNode* b)
		{
			return sqrt((a->X - b->X) * (a->X - b->X) + (a->Y - b->Y) * (a->Y - b->Y));
		};

	auto heuristic = [distance](FNode* a, FNode* b)
		{
			return distance(a, b);

		};

	FNode* nodeCurrent = NodeStart;
	NodeStart->fLocalGoal = 0.f;
	NodeStart->fGlobalGoal = heuristic(NodeStart, NodeEnd);

	TArray<FNode*> ListNotTestedNodes;
	ListNotTestedNodes.Add(NodeStart); //start search by pushing first node to the array

	while (ListNotTestedNodes.Num() > 0 && nodeCurrent != NodeEnd)
	{
		//sorts untested nodes by the globalgoal, lowest is first
		ListNotTestedNodes.Sort([](const FNode& lhs, const FNode& rhs) {return lhs.fGlobalGoal < rhs.fGlobalGoal; });
		
		while (ListNotTestedNodes.Num() > 0 && ListNotTestedNodes[0]->bVisited)
			ListNotTestedNodes.RemoveAt(0);

		if (ListNotTestedNodes.Num() == 0)
			break;
		nodeCurrent = ListNotTestedNodes[0];
		nodeCurrent->bVisited = true; //we only visit a node once 

		//Store info about checkpoints 
		VisitedCheckpoints.Add(nodeCurrent->WorldLocation);

		for (auto nodeNeighbor : nodeCurrent->Neighbors)
		{

			if (!nodeNeighbor->bVisited && nodeNeighbor->bObstacle == 0)
			{
				ListNotTestedNodes.Add(nodeNeighbor);
			}

			float fPossiblyLowerGoal = nodeCurrent->fLocalGoal + distance(nodeCurrent, nodeNeighbor);


			if (fPossiblyLowerGoal < nodeNeighbor->fLocalGoal)
			{
				nodeNeighbor->parent = nodeCurrent;
				nodeNeighbor->fLocalGoal = fPossiblyLowerGoal;
				nodeNeighbor->fGlobalGoal = nodeNeighbor->fLocalGoal + heuristic(nodeNeighbor, NodeEnd);

			}

		}

	}

	FNode* p = NodeEnd;
	while (p->parent != nullptr)

	{
		PathCheckpoints.Add(p->WorldLocation);
		DrawDebugLine(GetWorld(), p->WorldLocation, p->parent->WorldLocation, FColor::Green, false, 2.f, 40.f, 10.f);
		p = p->parent;
	}
	if (NodeEnd->parent == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("No path found to destination!"));
		TowerManager->DeleteTower(TowerManager->Pop());
	}
	Algo::Reverse(PathCheckpoints);
}