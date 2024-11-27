// Fill out your copyright notice in the Description page of Project Settings.


#include "RatDestroyer/UI/HealthBar.h"

#include "Kismet/GameplayStatics.h"

void UHealthBar::NativeConstruct()
{
	Super::NativeConstruct();

	Player = Cast<APlayerPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	Wave = Cast<AWaveManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AWaveManager::StaticClass()));
}

float UHealthBar::CalcHealthPercentage()
{
	if (Player)
	{
		return Player->Health / Player->MaxHealth;
	}
	return 0.f;
}

float UHealthBar::GetMoney()
{

	if (Player)
	{
		return Player->Money;

	}

	return 0.0f;
}

float UHealthBar::GetWave()
{

	if (Wave)
	{

		return Wave->WaveNumber - 1;

	}


	return 0.0f;
}

int32 UHealthBar::GetEnemy()
{

	return Wave->EnemiesAlive.Num();

}
