// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickable/CDPickableTrap.h"
#include "Core/CDCarBase.h"
#include "Kismet/GameplayStatics.h"

void ACDPickableTrap::OnMeshBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	ACDCarBase* Target = Cast<ACDCarBase>(OtherActor);
	if (!Target || !TrapDamage) return;
	UGameplayStatics::ApplyPointDamage(OtherActor, TrapDamage, GetActorLocation(), FHitResult(), GetInstigatorController(), this, TSubclassOf<UDamageType>());
	Super::OnMeshBeginOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}