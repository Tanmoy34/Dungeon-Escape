// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Mover.h"
#include "TriggerComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DUNGEON_ESCAPE_API UTriggerComponent : public UBoxComponent
{
	GENERATED_BODY()
	
public:	
	UTriggerComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* MoverActor;

	UMover *Mover;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsPressurePlate = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Tag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsTriggered =false;

	UPROPERTY(VisibleAnywhere)
	int32 ActivatorCount = 0;
	
	void Trigger(bool NewTriggerValue);

	
	

	
					

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	
private:





	
protected:
   virtual void BeginPlay() override;	


public:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


};

