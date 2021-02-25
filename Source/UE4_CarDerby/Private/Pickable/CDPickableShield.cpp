// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickable/CDPickableShield.h"
#include "Core/CDCarBase.h"

void ACDPickableShield::OnMeshBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	ACDCarBase* Target = Cast<ACDCarBase>(OtherActor);
	if (!Target || !ShieldDuration) return;
	Target->SetDamageImmune(ShieldDuration);
	Super::OnMeshBeginOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}