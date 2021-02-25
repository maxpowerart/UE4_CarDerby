// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickable/CDPickableBase.h"
#include "CDPickableTrap.generated.h"

/**
 * 
 */
UCLASS()
class UE4_CARDERBY_API ACDPickableTrap : public ACDPickableBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		float TrapDamage;

	void OnMeshBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
