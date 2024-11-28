// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"

//#include "AnimationEditorViewportClient.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
//#include "IContentBrowserSingleton.h"
#include "InputAction.h"
//#include "BaseGizmos/GizmoElementShared.h"
#include "Camera/CameraComponent.h"
//#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "RatDestroyer/Tower/RDTowerActor.h"
#include "RatDestroyer/Map/GridManager.h"
#include "RatDestroyer/Map/Tile.h"
#include "RatDestroyer/Tower/RDTowerManager.h"
#include "RatDestroyer/Enemy/WaveManager.h"
//#include "RatDestroyer/Enemy/RatEnemy.h"
//#include "Kismet/KismetMathLibrary.h"




// Sets default values
APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("DefaultSceneRoot");
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera");
	CameraComponent->SetupAttachment(RootComponent);

	
	Health = 150.0f;
	MaxHealth = 150.f;
	
	ShouldRotate = false;
	bCanBuild = false;
	bCanUndo = true;
	MoveSpeed = FVector2D(500, 500);
	ScreenEdgePadding = FVector2D(50, 50);
	ZoomSpeed = 1500;
	RotationSpeed = 200;
	Money = 50;
	MaxHealth = 150.0f;

	SetActorRotation(FRotator::MakeFromEuler(FVector3d(0, -30, 0)));

}

void APlayerPawn::MoveTriggered(const FInputActionValue& Value)
{
	CurrentInputMoveSpeed = Value.Get<FVector2D>();
}

void APlayerPawn::MoveCompleted()
{
	CurrentInputMoveSpeed = FVector2D::Zero();
}

void APlayerPawn::Zoom(const FInputActionValue& Value)
{
	auto Location = GetActorLocation();
	Location.Z += Value.Get<float>() * ZoomSpeed * FApp::GetDeltaTime();
	SetActorLocation(Location);
}

void APlayerPawn::RotationStarted()
{
	ShouldRotate = true;
}

void APlayerPawn::RotationCompleted()
{
	ShouldRotate = false;
}

void APlayerPawn::Look(const FInputActionValue& Value)
{
	const auto& LookVector = Value.Get<FVector2D>();

	if (ShouldRotate)
	{
		FRotator CurrentRotation = GetActorRotation();
		CurrentRotation.Pitch += RotationSpeed * FApp::GetDeltaTime() * LookVector.Y;
		CurrentRotation.Yaw += RotationSpeed * FApp::GetDeltaTime() * LookVector.X;
		SetActorRotation(CurrentRotation);
	}
}

//Either selects an object or builds tower
void APlayerPawn::Select(const FInputActionValue& Value)
{
	FHitResult HitResult;
	if (PlayerController->GetHitResultUnderCursor(ECC_Visibility, true, HitResult) && bCanBuild)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("bCanBuild is active")));
		if (BaseTower != nullptr && HitResult.GetActor()->ActorHasTag("Buildable"))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Base tower is not a nullptr and u clicked a tile")));
			ATile* ClickedTile = Cast<ATile>(HitResult.GetActor());
			for (ATile* TileInArray : GridManager->TileArray)
			{
				if (TileInArray == ClickedTile)
				{
					SelectedTile = TileInArray;
					BuildTower(SelectedTile);
				}
			}
		}
	}
	else //this else is currently unused
	{
		AActor* SelectedActor = HitResult.GetActor();
		if (SelectedActor)
		{
			//debug for testing
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("You selected: %s"), *SelectedActor->GetName()));
			//Code for what happens when you select something
		}
	}
}

void APlayerPawn::BuildMode(const FInputActionValue& Value)
{
	if (WaveManager && !WaveManager->bActiveWave)
	{
		return;
	}

	if (bCanBuild)
	{
		bCanBuild = false;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Now you can't build")));
	}
	else
	{
		bCanBuild = true;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("You can now build")));

	}
}
//Called when trying to build a tower on a tile
void APlayerPawn::BuildTower(ATile* TargetTile)
{
	SelectedTile = TargetTile;
	if (SelectedTile == nullptr || SelectedTile->GetHasTower())
	{
		return;
	}
	if (Money >= Tower->BaseCost)
	{
		AActor* SelectedTower = GetWorld()->SpawnActor(BaseTower);
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		SelectedTower->AttachToComponent(SelectedTile->GetStaticMesh(), AttachmentRules, FName(TEXT("TowerSocket")));
		TowerManager->Push(SelectedTower);
		SelectedTile->SetHasTower(true);
		int32 TileIndex = GridManager->TileArray.Find(SelectedTile);
		GridManager->Nodes[TileIndex].bObstacle = true;
		GridManager->Solve_AStar();
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Placed Tower")));
		Money = Money - Tower->BaseCost;
	}
}

 //Input to remove tower from stack and destroy the tower
void APlayerPawn::UndoTower(const FInputActionValue& Value)
{
	if (bCanUndo)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("WTF")));

		TowerManager->DeleteTower(TowerManager->Pop());
	}
}



// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	

	if(PlayerController = Cast<APlayerController>(Controller); IsValid(PlayerController))
	{
		PlayerController->SetShowMouseCursor(true);
		PlayerController->bEnableMouseOverEvents = true;
		PlayerController->bEnableClickEvents = true;
		GridManager = Cast<AGridManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGridManager::StaticClass()));
		TowerManager = Cast<ARDTowerManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ARDTowerManager::StaticClass()));
		Tower = Cast<ARDTowerActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ARDTowerActor::StaticClass()));
		WaveManager = Cast<AWaveManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AWaveManager::StaticClass()));
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(IMC, 0);
		}
	}

}



// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Find the correct movement speed of the camera
	auto CurrentMoveSpeed = CurrentInputMoveSpeed;
	if (FVector2D MousePosition; CurrentInputMoveSpeed.X == 0 && CurrentInputMoveSpeed.Y == 0 && PlayerController->GetMousePosition(MousePosition.X, MousePosition.Y))
	{
		int32 ViewportWidth, ViewportHeight;
		PlayerController->GetViewportSize(ViewportWidth, ViewportHeight);

		const auto X =
			-(MousePosition.X <= ScreenEdgePadding.X) | (MousePosition.X >= ViewportWidth - ScreenEdgePadding.X);

		const auto Y =
			(MousePosition.Y <= ScreenEdgePadding.Y) | -(MousePosition.Y >= ViewportHeight - ScreenEdgePadding.Y);

		CurrentMoveSpeed = FVector2D(X, Y);
	}

	//Move camera

	auto ForwardVector = FVector(GetActorForwardVector().X, GetActorForwardVector().Y, 0);
	ForwardVector.Normalize();

	const auto Forward = ForwardVector * CurrentMoveSpeed.Y * MoveSpeed.Y * DeltaTime;
	const auto Sideways = GetActorRightVector() * CurrentMoveSpeed.X * MoveSpeed.X * DeltaTime;

	const auto NextLocation = GetActorLocation() + Forward + Sideways;
	SetActorLocation(NextLocation);

}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (EnhancedInputComponent)
	{
		//Move camera
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerPawn::MoveTriggered);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &APlayerPawn::MoveCompleted);

		//Zoom camera
		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &APlayerPawn::Zoom);

		//Rotate camera
		EnhancedInputComponent->BindAction(RotateAction, ETriggerEvent::Started, this, &APlayerPawn::RotationStarted);
		EnhancedInputComponent->BindAction(RotateAction, ETriggerEvent::Canceled, this, &APlayerPawn::RotationCompleted);
		EnhancedInputComponent->BindAction(RotateAction, ETriggerEvent::Completed, this, &APlayerPawn::RotationCompleted);

		//Look
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerPawn::Look);

		//Select with mouse
		EnhancedInputComponent->BindAction(SelectAction, ETriggerEvent::Triggered, this, &APlayerPawn::Select);
		EnhancedInputComponent->BindAction(BuildModeAction, ETriggerEvent::Triggered, this, &APlayerPawn::BuildMode);

		//Undo tower
		EnhancedInputComponent->BindAction(UndoTowerAction, ETriggerEvent::Triggered, this, &APlayerPawn::UndoTower);
		
	}
}
