// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "TriggerComponent.h"
#include "Lock.generated.h"

UCLASS()
class DUNGEON_ESCAPE_API ALock : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALock();
	UPROPERTY(VisibleAnywhere)
	UTriggerComponent* TriggerComp;
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* KeyItemMesh;
	
	UPROPERTY(VisibleAnywhere)
	USceneComponent* RootComp;
	
	UPROPERTY(EditAnywhere)
	FString KeyItemName;

private:
	UPROPERTY(VisibleAnywhere)
	bool IsKeyplaced = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool GetIsKeyplaced();
	void SetIsKeyplaced(bool NewisKeyplaced);
	
};
