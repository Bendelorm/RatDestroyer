// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveManager.h"
#include "RatDestroyer/Map/GridManager.h"




// Sets default values
AWaveManager::AWaveManager(): GridManager(nullptr), EnemySpawned(0)
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    bActiveWave = false;
}

// Called when the game starts or when spawned
void AWaveManager::BeginPlay()
{
    Super::BeginPlay();
    GridManager->NodeStart->WorldLocation;
}

    void AWaveManager::StartWave() const
    {
    if (!GridManager || !GridManager->NodeStart || !GridManager->NodeEnd)
    {
        UE_LOG(LogTemp, Error, TEXT("StartNode is nullptr"));
    }
}

    void AWaveManager::EndWave()
    {
        if(GetWorld())
        {
            GetWorld()->GetTimerManager().ClearTimer(WaveTimerHandle);
        }

        bActiveWave = false;

        EnemySpawned = 0;
    UE_LOG(LogTemp, Warning, TEXT("Wave ended and timer has been reset."));
    }


// Called every frame
void AWaveManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

