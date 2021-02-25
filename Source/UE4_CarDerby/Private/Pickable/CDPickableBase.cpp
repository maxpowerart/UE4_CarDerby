// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickable/CDPickableBase.h"
#include "TimerManager.h"
#include "Particles/ParticleSystemComponent.h"
#include "Math/UnrealMathUtility.h"


ACDPickableBase::ACDPickableBase(){
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	ParticleOnSpawn = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Effect"));
	ParticleOnSpawn->SetupAttachment(Mesh);

	Mesh->OnComponentBeginOverlap.AddDynamic(this, &ACDPickableBase::OnMeshBeginOverlap);
	Mesh->SetNotifyRigidBodyCollision(true);
	Mesh->SetCollisionProfileName(TEXT("Pickups"));

	Mesh->SetVisibility(false);
	Mesh->SetGenerateOverlapEvents(false);

	PrimaryActorTick.bCanEverTick = true;
}

void ACDPickableBase::OnMeshBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){
	Mesh->SetVisibility(false);
	Mesh->SetGenerateOverlapEvents(false);
	SetActorLocation(FVector(FMath::RandRange(-2000.0f, 2000.0f), FMath::RandRange(-2000.0f, 2000.0f), 100));
	ParticleOnSpawn->SetVisibility(true);
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ACDPickableBase::SpawnMesh, SpawnTime, false);
}
void ACDPickableBase::SpawnMesh() {
	Mesh->SetVisibility(true);
	Mesh->SetGenerateOverlapEvents(true);
	ParticleOnSpawn->SetVisibility(false);
}
void ACDPickableBase::BeginPlay(){
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ACDPickableBase::SpawnMesh, SpawnTime, false);
}
void ACDPickableBase::Tick(float DeltaTime){
	Super::Tick(DeltaTime);

}

