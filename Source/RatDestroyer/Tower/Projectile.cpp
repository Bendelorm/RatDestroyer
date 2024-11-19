// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bullet"));

    MeshComponent->SetupAttachment(RootComponent);


    CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
   
    RootComponent = CollisionSphere; 

    ProjectileMovement->InitialSpeed = 100.f;
    

}

void AProjectile::TargetEnemy()
{
     if (Enemy)
    {
        ARatEnemy* RatEnemy = Cast<ARatEnemy>(Enemy);
        FVector ProjectileLocation = GetActorLocation();
        FVector EnemyLocation = RatEnemy->GetActorLocation();
        FVector DirectionToEnemy = ProjectileLocation - EnemyLocation;

        DirectionToEnemy.Normalize();

        ProjectileMovement->Velocity = DirectionToEnemy * ProjectileMovement->InitialSpeed;

        FRotator NewRotation = FRotationMatrix::MakeFromX(DirectionToEnemy).Rotator();
        SetActorRotation(NewRotation);

        
        ProjectileMovement->HomingTargetComponent = RatEnemy->GetRootComponent();

        
        ProjectileMovement->HomingAccelerationMagnitude = 50.0f;  

        FTimerHandle DestroyTimerHandle;
        GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, this, &AProjectile::ProjectileDestroy, 5.0f, false);  


    }
}

void AProjectile::ProjectileDestroy()
{
    Destroy(); 
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
    Super::BeginPlay();

}






// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



