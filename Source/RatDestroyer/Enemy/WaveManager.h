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

    //Might Delete this
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Wave)
    int32 NumberOfEnemiesInWave;

    //Spawning the right Enemy Class
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Wave)
    TSubclassOf<ARatEnemy> TheRat;

    //Just for handling Active or inactive Waves
    UPROPERTY(BlueprintReadWrite, Category = "Wave", meta = (AllowPrivateAccess = "true"))
    bool bActiveWave = false;
    
    void EnqueueWave();
    
protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

private:

    void StartWave();
    
    UFUNCTION(BlueprintCallable, Category = "Spawning")
    void Spawn();
    
    UPROPERTY()
    AGridManager* GridManager;
    

    //Timer Handling
    UPROPERTY(EditDefaultsOnly, Category = "Wave")
    FTimerHandle SpawnTimerHandle;

    //This fixes timer for the next wave to spawn
    FTimerHandle WaveStartTimerHandle;

    //Queue of waves
    TQueue<FMyWave> WaveQueue;
    
    int32 WaveNumber;
    int32 EnemiesSpawned;

    // This is the Timer for spawning of Enemy
    float CurrentWaveSpawnInterval;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

};

