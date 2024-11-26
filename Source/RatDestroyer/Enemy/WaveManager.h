// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RatEnemy.h"
#include "RatDestroyer/Map/GridManager.h"
#include "WaveManager.generated.h"

USTRUCT()
struct FMyWave
{
    GENERATED_BODY()

    //Number of Enemies to spawn in wave
    UPROPERTY()
    int32 EnemyCount;        

    //Spawn interval for each Enemy
    UPROPERTY()
    float SpawnInterval;    
};

UCLASS()
class RATDESTROYER_API AWaveManager : public AActor
{
    GENERATED_BODY()
	
public:	
    // Sets default values for this actor's properties
    AWaveManager();

    //Sets the NumberOfEnemies in the wave
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Wave)
    int32 NumberOfEnemiesInWave;

    //Spawning the right Enemy Class
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Wave)
    TSubclassOf<ARatEnemy> TheRat;

    //Just for handling Active or inactive Waves
    UPROPERTY(BlueprintReadWrite, Category = "Wave", meta = (AllowPrivateAccess = "true"))
    bool bActiveWave = false;
    
    void EnqueueWave();

    // Called every frame
    virtual void Tick(float DeltaTime) override;
    
protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

private:

    UPROPERTY()
    AGridManager* GridManager;
    
    UPROPERTY()
    TArray<ARatEnemy*> EnemiesAlive;
    
    UFUNCTION(BlueprintCallable, Category = "Wave")
    void StartWave();
    
    UFUNCTION(BlueprintCallable, Category = "Spawning")
    void Spawn();
    
    //Queue of waves
    TQueue<FMyWave> WaveQueue;
    
    //Timer Handling
    UPROPERTY(EditDefaultsOnly, Category = "Wave")
    FTimerHandle SpawnTimerHandle;

    //This fixes timer for the next wave to spawn
    FTimerHandle WaveStartTimerHandle;


    
    int32 WaveNumber;
    int32 EnemiesSpawned;

    // This is the Timer for spawning of Enemy
    float CurrentWaveSpawnInterval;
    
};

