// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

   
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));

   
    

}



void AProjectile::BeginPlay()
{
    Super::BeginPlay();

}


// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



