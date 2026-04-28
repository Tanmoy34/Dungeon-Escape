// Fill out your copyright notice in the Description page of Project Settings.


#include "Mover.h"

#include  "Math/UnrealMathUtility.h"

// Sets default values for this component's properties
UMover::UMover()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


bool UMover::GetShouldMove()
{
	return ShouldMove;
}

void UMover::SetShouldMove(bool NewShouldMove)
{
	ShouldMove = NewShouldMove;

	if (ShouldMove)
	{
		TargetLocation = StartLocation + MoveOffset;
	}
	else
	{
		TargetLocation = StartLocation;
	}

}

// Called when the game starts
void UMover::BeginPlay()
{
	Super::BeginPlay();

	AActor *myOwner =  GetOwner();
	if (myOwner)
	{
		StartLocation = myOwner->GetActorLocation();
		SetShouldMove(false);
	}
	
}


// Called every frame
void UMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	
	
	FVector currentLocation = GetOwner()->GetActorLocation();

	ReachedTargetLocation = currentLocation.Equals(TargetLocation);
	
	
	
	if (!ReachedTargetLocation)
	{
		float speed  = MoveOffset.Length() / MoveTime;
		FVector NewLocation=FMath::VInterpConstantTo(currentLocation, TargetLocation,DeltaTime,speed);
		GetOwner()->SetActorLocation(NewLocation);
	}
	
		
		 

	
	
}





