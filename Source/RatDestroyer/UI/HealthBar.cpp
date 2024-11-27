// Fill out your copyright notice in the Description page of Project Settings.


#include "RatDestroyer/UI/HealthBar.h"

#include "Kismet/GameplayStatics.h"

void UHealthBar::NativeConstruct()
{
	Super::NativeConstruct();

	Player = Cast<APlayerPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

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
