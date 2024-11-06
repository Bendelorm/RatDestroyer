//// Fill out your copyright notice in the Description page of Project Settings.
//
//
//#include "WaveManager.h"
//#include "EngineUtils.h"
//
//
//
//// Sets default values
//AWaveManager::AWaveManager(): GridManager(nullptr), EnemySpawned(0)
//{
//    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
//    PrimaryActorTick.bCanEverTick = true;
//
//    bActiveWave = false;
//}
//
//// Called when the game starts or when spawned
//void AWaveManager::BeginPlay()
//{
//    Super::BeginPlay();
//
//    for (TActorIterator<AGridManager> It(GetWorld()); It;)
//    {
//        GridManager = *It;
//        break;
//    }
//    if (!GridManager)
//    {
//        UE_LOG(LogTemp, Error, TEXT("WaveManager and gridmanager dont find eachother."));
//    }
//}
//
//    void AWaveManager::StartWave()
//    {
//        if (EnemyClass)
//        {
//            bActiveWave = true;
//            EnemySpawned = 0;
//
//            GetWorld()->GetTimerManager().SetTimer(WaveTimerHandle, this, &AWaveManager::SpawnEnemyInWave, SpawnInterval, true);
//        }
//    }
//
//    void AWaveManager::SpawnEnemyInWave()
//    {
//    if (EnemySpawned >= NumberOfEnemiesInWave)
//    {
//        EndWave();
//        return;
//    }
//        SpawnEnemy(1, TSubclassOf<ARatEnemy>(EnemyClass));
//        EnemySpawned++;
//    }
//
//    void AWaveManager::EndWave()
//    {
//        if(GetWorld())
//        {
//            GetWorld()->GetTimerManager().ClearTimer(WaveTimerHandle);
//        }
//
//        bActiveWave = false;
//
//        EnemySpawned = 0;
//    UE_LOG(LogTemp, Warning, TEXT("Wave ended and timer has been reset."));
//    }
//
//void AWaveManager::SpawnEnemy(int32 EnemyCount, TSubclassOf<ARatEnemy> EnemyType)
//{
//    if (GridManager == nullptr || EnemyType == nullptr)
//    {
//        UE_LOG(LogTemp, Error, TEXT("GridManager or EnemyType is null, cannot spawn enemy."));
//        return;
//    }
//
//    // Assuming GridManager has TileArray available
//    if (GridManager->TileArray.Num() < 2)
//    {
//        UE_LOG(LogTemp, Error, TEXT("Not enough tiles to spawn an enemy."));
//        return;
//    }
//
//    ATile* StartTile = GridManager->TileArray[0];
//    ATile* GoalTile = GridManager->TileArray[GridManager->TileArray.Num() - 1];
//
//    FVector SpawnLocation = StartTile->GetActorLocation() + FVector(0, 0, 200.f);
//   
//    // Spawn the enemy
//    ARatEnemy* NewEnemy = GetWorld()->SpawnActor<ARatEnemy>(EnemyType, SpawnLocation, FRotator::ZeroRotator);
//
//    if (NewEnemy)
//    {
//        // Use FindPath with ATile* parameters
//            GridManager->Solve_AStar(); // Assuming FindPath is still in GridManager
//
//        // Pass tile locations to the enemy's path as FVectors
//        //TArray<FVector> PathLocations;
//        //for (ATile* Tile : Path)
//        //{
//        //    PathLocations.Add(Tile->GetActorLocation());
//        //}
//
//        //if (PathLocations.Num() > 0)
//        //{
//        //    NewEnemy->SetPath(PathLocations);
//        //    UE_LOG(LogTemp, Warning, TEXT("Enemy spawned and path set with %d locations."), PathLocations.Num());
//        //}
//        //else
//        //{
//        //    UE_LOG(LogTemp, Error, TEXT("Failed to find a path for the enemy from %s to %s"), *SpawnLocation.ToString(), *GoalTile->GetActorLocation().ToString());
//        //    // Optionally, destroy the enemy if the path is invalid
//        //    NewEnemy->Destroy();
//        //}
//    }
//}
//
//// Called every frame
//void AWaveManager::Tick(float DeltaTime)
//{
//    Super::Tick(DeltaTime);
//}
//
