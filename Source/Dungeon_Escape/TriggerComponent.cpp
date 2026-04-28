// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"



//constructor
UTriggerComponent::UTriggerComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;
	
	
}



void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();
	if (MoverActor)
	{
		Mover = MoverActor->FindComponentByClass<UMover>();
		
	}
	if (IsPressurePlate)
	{
		OnComponentBeginOverlap.AddDynamic(this,&UTriggerComponent::OnOverlapBegin);
		OnComponentEndOverlap.AddDynamic(this,&UTriggerComponent::OnOverlapEnd);
	}
	
}

void UTriggerComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//On Overlap start
	
	if (OtherActor && OtherActor->ActorHasTag(Tag))
	{
		ActivatorCount++;// increase by 1
		if (!IsTriggered)
		{
			Trigger(true);
		}
		
			
		
	}
}

void UTriggerComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// On Overlap End
	
	if (OtherActor && OtherActor->ActorHasTag(Tag))
	{
		ActivatorCount--;// decrease By 1
		if( IsTriggered && ActivatorCount == 0)
		{
			Trigger(false);
		}
	}
}




void UTriggerComponent::Trigger(bool NewTriggerValue)
{
	IsTriggered = NewTriggerValue;

	if (Mover)
	{
		Mover->SetShouldMove(IsTriggered);
		
	}
	else
	{
		UE_LOG(LogTemp,Display,TEXT("%s Does not have mover to trigger"), *GetOwner()->GetActorNameOrLabel());
	}
	
	
	
}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}
