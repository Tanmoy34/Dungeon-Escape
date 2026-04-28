// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Mover.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DUNGEON_ESCAPE_API UMover : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMover();

	FVector StartLocation;
	FVector TargetLocation;
	
	

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector MoveOffset;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float MoveTime = 4.0f;
	
	UPROPERTY(VisibleAnywhere)
	bool ReachedTargetLocation = false;
	
	bool GetShouldMove();
	void SetShouldMove(bool NewShouldMove);

private:
	
	UPROPERTY(VisibleAnywhere)
	bool ShouldMove = false;

	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
		
};
