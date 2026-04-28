// Copyright Epic Games, Inc. All Rights Reserved.

#include "Dungeon_EscapeCharacter.h"

#include "CollectableItem.h"
#include "Animation/AnimInstance.h"
#include "CollectableItem.h"
#include "Lock.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Dungeon_Escape.h"
#include "Evaluation/Blending/MovieSceneBlendType.h"

ADungeon_EscapeCharacter::ADungeon_EscapeCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	
	// Create the first person mesh that will be viewed only by this character's owner
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("First Person Mesh"));

	FirstPersonMesh->SetupAttachment(GetMesh());
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
	FirstPersonMesh->SetCollisionProfileName(FName("NoCollision"));

	// Create the Camera Component	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	FirstPersonCameraComponent->SetupAttachment(FirstPersonMesh, FName("head"));
	FirstPersonCameraComponent->SetRelativeLocationAndRotation(FVector(-2.8f, 5.89f, 0.0f), FRotator(0.0f, 90.0f, -90.0f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	FirstPersonCameraComponent->bEnableFirstPersonFieldOfView = true;
	FirstPersonCameraComponent->bEnableFirstPersonScale = true;
	FirstPersonCameraComponent->FirstPersonFieldOfView = 70.0f;
	FirstPersonCameraComponent->FirstPersonScale = 0.6f;

	// configure the character comps
	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;

	GetCapsuleComponent()->SetCapsuleSize(34.0f, 96.0f);

	// Configure character movement
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->AirControl = 0.5f;
}

void ADungeon_EscapeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ADungeon_EscapeCharacter::DoJumpStart);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ADungeon_EscapeCharacter::DoJumpEnd);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADungeon_EscapeCharacter::MoveInput);

		// Looking/Aiming
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADungeon_EscapeCharacter::LookInput);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &ADungeon_EscapeCharacter::LookInput);

		EnhancedInputComponent->BindAction(Interaction, ETriggerEvent::Started, this, &ADungeon_EscapeCharacter::Interact);
	}
	else
	{
		UE_LOG(LogDungeon_Escape, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void ADungeon_EscapeCharacter::MoveInput(const FInputActionValue& Value)
{
	// get the Vector2D move axis
	FVector2D MovementVector = Value.Get<FVector2D>();

	// pass the axis values to the move input
	DoMove(MovementVector.X, MovementVector.Y);

}

void ADungeon_EscapeCharacter::LookInput(const FInputActionValue& Value)
{
	// get the Vector2D look axis
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// pass the axis values to the aim input
	DoAim(LookAxisVector.X, LookAxisVector.Y);

}

void ADungeon_EscapeCharacter::DoAim(float Yaw, float Pitch)
{
	if (GetController())
	{
		// pass the rotation inputs
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void ADungeon_EscapeCharacter::DoMove(float Right, float Forward)
{
	if (GetController())
	{
		// pass the move inputs
		AddMovementInput(GetActorRightVector(), Right);
		AddMovementInput(GetActorForwardVector(), Forward);
	}
}

void ADungeon_EscapeCharacter::DoJumpStart()
{
	// pass Jump to the character
	Jump();
}

void ADungeon_EscapeCharacter::DoJumpEnd()
{
	// pass StopJumping to the character
	StopJumping();
}

void ADungeon_EscapeCharacter::Interact()
{
	//UE_LOG(LogTemp, Display, TEXT("Game Time %f"),GetWorld()->TimeSeconds);
	FVector Start  =  FirstPersonCameraComponent->GetComponentLocation();  // Ray trace Start location
	
	FVector End = Start + (FirstPersonCameraComponent->GetForwardVector() * MaxInteractionDistance); // Ray Trace end Location
	
	FCollisionShape InteractionSphere  = FCollisionShape::MakeSphere(InteractionSphereRadius);

	DrawDebugSphere(GetWorld(),End,InteractionSphereRadius,20,FColor::Blue,false,5.0f);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());

	FHitResult HitResult;
	bool HasHit =  GetWorld()->SweepSingleByChannel(
		HitResult,
		Start,
		End,
		FQuat::Identity,
		ECC_GameTraceChannel2,
		InteractionSphere,
		Params);
	

	if (HasHit)
	{
		AActor* HitActor =  HitResult.GetActor();
		UE_LOG(LogTemp,Display,TEXT("HitActor: %s"),*HitActor->GetActorNameOrLabel());
		// For Collectable Item
		if(HitActor->ActorHasTag("CollectableItem"))
		{
			ACollectableItem* CollectableItem = Cast<ACollectableItem>(HitActor);
			if (CollectableItem)
			{
				ItemList.Add(CollectableItem->ItemName);
				CollectableItem->Destroy();
			}
		}
		//For Lock
		else if (HitActor->ActorHasTag("Lock"))
		{
			ALock* LockActor = Cast<ALock>(HitActor);
			if (LockActor)
			{
				// 1 is The lock empty
				if (LockActor->GetIsKeyplaced()==false)
				{
					//2  do we have the KeyItemname Is our Item list?
					int32 ItemRemoved =  ItemList.RemoveSingle(LockActor->KeyItemName);
					if (ItemRemoved ==1)
					{
						//Keyremoved
						LockActor->SetIsKeyplaced(true);
					}
					else
					{
						UE_LOG(LogTemp,Display,TEXT("Item Removed"));
					}
				}
				else
				{
					//take back item
					ItemList.Add(LockActor->KeyItemName);
					LockActor->SetIsKeyplaced(false);
				}
				
				
				
			}
		}
		
	}
	else
	{
		UE_LOG(LogTemp,Display,TEXT("No Hit"));
	}
}
