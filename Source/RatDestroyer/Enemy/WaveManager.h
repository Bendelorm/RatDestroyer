// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RatEnemy.h"
#include "RatDestroyer/Map/GridManager.h"
#include "WaveManager.generated.h"

UCLASS()
class RATDESTROYER_API AWaveManager : public AActor
{
    GENERATED_BODY()
	
public:	
    // Sets default values for this actor's properties
    AWaveManager();
    FVector StartLocation;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Wave)
    int32 NumberOfEnemiesInWave = 5;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Wave)
    TSubclassOf<AActor> EnemyClass;

    //UFUNCTION(BlueprintCallable, Category = "Grid")
    //void SpawnEnemy(int32 EnemyCount, TSubclassOf<ARatEnemy> EnemyType);
    
    UPROPERTY(BlueprintReadWrite, Category = wave)
    bool bActiveWave;

    bool GetActiveWave() const
    {
        return bActiveWave;
    }
    
    void StartWave() const;

    UFUNCTION(BlueprintCallable, Category = "Wave")
    void EndWave();

    //Planlegger å adde ny enemy hver wave
   // UFUNCTION(BlueprintCallable, Category = "Wave")
    //void AddNewEnemy(ARatEnemy* newEnemy);
    
protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

 

private:

    UPROPERTY()
    AGridManager* GridManager;
    
    // This is the Timer for spawning of Enemy
    FTimerHandle WaveTimerHandle;
    int32 EnemySpawned;

    UPROPERTY(EditDefaultsOnly, Category = "Wave")
    float SpawnInterval = 1.0f;
    
    void SpawnEnemyInWave();
    
public:
    
    // Called every frame
    virtual void Tick(float DeltaTime) override;

};

