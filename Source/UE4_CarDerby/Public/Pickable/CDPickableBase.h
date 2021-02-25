// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CDPickableBase.generated.h"

UCLASS()
class UE4_CARDERBY_API ACDPickableBase : public AActor
{
	GENERATED_BODY()

private:

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
		class UParticleSystemComponent* ParticleOnSpawn;
	UPROPERTY(EditDefaultsOnly)
		class UStaticMeshComponent* Mesh;
	UPROPERTY(EditDefaultsOnly)
		float SpawnTime =0;
	FTimerHandle TimerHandle;

public:

	ACDPickableBase();

	UFUNCTION()
		virtual void OnMeshBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void SpawnMesh();
};
