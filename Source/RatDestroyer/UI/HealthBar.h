// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RatDestroyer/Player/PlayerPawn.h"
#include "HealthBar.generated.h"

/**
 * 
 */
UCLASS()
class RATDESTROYER_API UHealthBar : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	
	virtual void NativeConstruct() override;

	
	UFUNCTION(BlueprintPure)
	float CalcHealthPercentage();

	UFUNCTION(BlueprintPure)
	float GetMoney();

	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<APlayerPawn> Player;




};
