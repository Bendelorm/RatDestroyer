// Fill out your copyright notice in the Description page of Project Settings.


#include "RDTowerActor.h"
#include "RatDestroyer/Map/Tile.h"
#include "Components/SphereComponent.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "RatDestroyer/Enemy/RatEnemy.h"
#include "RatDestroyer/Enemy/WaveManager.h"
// Sets default values
ARDTowerActor::ARDTowerActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TowerMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = TowerMeshComponent;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("UBoxComponent"));
	BoxComponent->SetupAttachment(TowerMeshComponent);

	AttackComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AttackComponent"));
	AttackComponent->SetupAttachment(TowerMeshComponent);

	BaseAttackRange = 200.0f;
	AttackRangeComponent = CreateDefaultSubobject<USphereComponent>(TEXT("AttackRangeComponent"));
	AttackRangeComponent->SetupAttachment(TowerMeshComponent);
	AttackRangeComponent->SetSphereRadius(BaseAttackRange);
	AttackRangeComponent->SetGenerateOverlapEvents(true);
	AttackRangeComponent->OnComponentBeginOverlap.AddDynamic(this, &ARDTowerActor::OnOverlapBegin);
	AttackRangeComponent->OnComponentEndOverlap.AddDynamic(this, &ARDTowerActor::OnOverlapEnd);

	BaseCost = 10;
	BaseDamage = 2;
	BaseAttackTime = 1.0f;
	Enemy = nullptr;
}

void ARDTowerActor::OnOverlapBegin(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
                                   class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Enemy"))
	{
		ARatEnemy* EnteredEnemy = Cast<ARatEnemy>(OtherActor);
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("%s entered"), *EnteredEnemy->GetName()));
		AttackPriorityQueue.Add(EnteredEnemy);
		if (Enemy == nullptr)
		{
			TowerAttackEnemy();
		}
	}
}

void ARDTowerActor::OnOverlapEnd(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag("Enemy"))
	{
		ARatEnemy* LeavingEnemy = Cast<ARatEnemy>(OtherActor);
		for (int32 i = 0; i < AttackPriorityQueue.Num(); i++)
		{
			if (LeavingEnemy->GetName() == AttackPriorityQueue[i]->GetName())
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("%s left"), *LeavingEnemy->GetName()));
				AttackPriorityQueue.RemoveAt(i);
				break;
			}
		}
	}
}

void ARDTowerActor::TowerAttackEnemy()
{
	if (AttackPriorityQueue.Num() > 0 && AttackPriorityQueue[0]->ActorHasTag("Enemy"))
	{
		if (AttackPriorityQueue[0] != nullptr)
		{
			ARatEnemy* TargetEnemy = Cast<ARatEnemy>(AttackPriorityQueue[0]);
			Enemy = TargetEnemy;
			FVector GunLoc = AttackComponent->GetComponentLocation();
			FVector TargetLoc = TargetEnemy->GetActorLocation();
			FVector Direction = (TargetLoc - GunLoc);
			Direction.Normalize();
			FRotator LookAtRotation = Direction.Rotation();
			LookAtRotation.Yaw += 90.0f;
			AttackComponent->SetWorldRotation(LookAtRotation);
			if (FireAnimation != nullptr)
			{
				UAnimInstance* AnimInstance = AttackComponent->GetAnimInstance();
				if (AnimInstance != nullptr)
				{
					AnimInstance->Montage_Play(FireAnimation, 1.f);
				}
			}
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("%s attacked"), *this->GetName()));
			TargetEnemy->AttackEnemy(BaseDamage);

			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("%s Took damage"), *TargetEnemy->GetName()));
			GetWorldTimerManager().SetTimer(TimerHandle, this, &ARDTowerActor::TowerAttackEnemy, BaseAttackTime, false);
		}
		else
		{
			//Search for enemy again after 0.1 second
			GetWorldTimerManager().SetTimer(TimerHandle, this, &ARDTowerActor::TowerAttackEnemy, 0.01f, false);
		}
	}
	else
	{
		GetWorldTimerManager().SetTimer(TimerHandle, this, &ARDTowerActor::TowerAttackEnemy, 0.01f, false);
	}
}

// Called when the game starts or when spawned
void ARDTowerActor::BeginPlay()
{
	Super::BeginPlay();
	WaveManager = Cast<AWaveManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AWaveManager::StaticClass()));
}

// Called every frame
void ARDTowerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (WaveManager->bCleanTowerArray)
	{
		AttackPriorityQueue.Empty();
		Enemy = nullptr;
	}
}