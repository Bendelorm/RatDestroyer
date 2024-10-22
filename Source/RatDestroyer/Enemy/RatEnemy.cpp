//// Fill out your copyright notice in the Description page of Project Settings.
//
//
//#include "RatEnemy.h"
//#include "GameFramework/CharacterMovementComponent.h"
////#include "RatDestroyer/TreeTesting/RDTreeNode.h"
//
//// Sets default values
//ARatEnemy::ARatEnemy()
//{
// 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
//	PrimaryActorTick.bCanEverTick = true;
//
//	MovementSpeed = 300.0f;
//	Health = 10;
//	bIsMoving = false;
//	Damage = 1;
//	
//	GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
//}
//
//// Called when the game starts or when spawned
//void ARatEnemy::BeginPlay()
//{
//	Super::BeginPlay();
//	
//}
//
//
//// Called every frame put in kode for pathfinding for enemy
//// void ARatEnemy::Tick(float DeltaTime)
//// {
//// 	Super::Tick(DeltaTime);
////
////
//// 	if (bIsMoving && ChildNode() > 0 && CurrentNode < ARDTreeNode())
//// 	{
//// 		FVector TargetLocation = ARDTreeNode()[CurrentNode]->GetActorLocation();
//// 		FVector Direction =(GetTargetLocation()- GetActorLocation().GetSafeNormal());
//// 		FVector NewLocation = GetActorLocation() + Direction * MovementSpeed * DeltaTime;
////
//// 		SetActorLocation(NewLocation);
////
//// 		if (FVector::Dist(GetActorLocation(), TargetLocation) < 10.0f)
//// 		{
//// 			CurrentNode++;
////
//// 			if (CurrentNode >= ARDTreeNode())
//// 			{
//// 				bIsMoving = false;
//// 			}
//// 		}
//// 	}
//// }
//// void ARatEnemy::Path
//// {
//// 	ARDTreeNode = newPath;
////}
//// void ARatEnemy::StartMoving()
//// {
//// 	bIsMoving = true;
//// }
//
//
