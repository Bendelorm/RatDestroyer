// Fill out your copyright notice in the Description page of Project Settings.


#include "RatEnemy.h"

// Sets default values
ARatEnemy::ARatEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MovementSpeed = 300.0f;
	Health = 10;
	bIsmoving = false;
	Damage = 1;
	
}

// Called when the game starts or when spawned
void ARatEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARatEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


