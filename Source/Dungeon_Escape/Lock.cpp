// Fill out your copyright notice in the Description page of Project Settings.


#include "Lock.h"

// Sets default values
ALock::ALock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add("Lock");
	
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);
	KeyItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KeyItemMesh"));
	KeyItemMesh->SetupAttachment(RootComp);
	
	TriggerComp = CreateDefaultSubobject<UTriggerComponent>(TEXT("TriggerComp"));
	TriggerComp->SetupAttachment(RootComp);
}

// Called when the game starts or when spawned
void ALock::BeginPlay()
{
	Super::BeginPlay();

	SetIsKeyplaced(false);


	
}

// Called every frame
void ALock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ALock::GetIsKeyplaced()
{
	return IsKeyplaced;
}

void ALock::SetIsKeyplaced(bool NewisKeyplaced)
{
	IsKeyplaced = NewisKeyplaced;
	TriggerComp->Trigger(NewisKeyplaced);
	KeyItemMesh->SetVisibility(NewisKeyplaced);
}

