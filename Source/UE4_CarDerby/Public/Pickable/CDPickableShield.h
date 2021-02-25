// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickable/CDPickableBase.h"
#include "CDPickableShield.generated.h"

/**
 * 
 */
UCLASS()
class UE4_CARDERBY_API ACDPickableShield : public ACDPickableBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		float ShieldDuration;
	
	void OnMeshBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
