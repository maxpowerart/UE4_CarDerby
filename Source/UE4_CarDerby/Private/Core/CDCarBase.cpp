// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/CDCarBase.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/InputComponent.h"
#include "WheeledVehicleMovementComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "UI/CDHealthBar.h"
#include "Components/BoxComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"

ACDCarBase::ACDCarBase() {

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(RootComponent);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetRelativeRotation(FRotator(-30.f, 0.f, 0.f));
	SpringArm->SetupAttachment(GetMesh());
	SpringArm->TargetArmLength = 500;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetRelativeRotation(FRotator(10.f, 0.f, 0.f));
	Camera->SetupAttachment(SpringArm);

	HPBarComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HP Bar"));
	HPBarComponent->SetupAttachment(RootComponent);
	HPBarComponent->SetWidgetSpace(EWidgetSpace::Screen);
	HPBarComponent->SetDrawAtDesiredSize(true);

	HitRadius = CreateDefaultSubobject<UBoxComponent>(TEXT("Radius"));
	HitRadius->SetupAttachment(RootComponent);
	HitRadius->SetWorldScale3D(FVector(8.0, 3.5, 2.3));
	HitRadius->SetRelativeLocation(FVector(0.0, 0.0, 50.0));

	GetMesh()->OnComponentHit.AddDynamic(this, &ACDCarBase::OnMeshHit);
	HitRadius->OnComponentEndOverlap.AddDynamic(this, &ACDCarBase::OnRadiusOverlapEnd);

	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception"));
	AIPerception->bEditableWhenInherited = true;
	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Sense"));
	Sight->SightRadius = 3000;
	Sight->LoseSightRadius = 3500;
	Sight->PeripheralVisionAngleDegrees = 180.0f;
	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectNeutrals = true;
	Sight->DetectionByAffiliation.bDetectFriendlies = true;

	AIPerception->ConfigureSense(*Sight);

	PrimaryActorTick.bCanEverTick = true;
}

#pragma region BasicUEFlow
void ACDCarBase::BeginPlay() {
	Super::BeginPlay();
	const auto HealthBarWidget = Cast<UCDHealthBar>(HPBarComponent->GetUserWidgetObject());
	if (HealthBarWidget) GetOwner() == GetWorld()->GetFirstPlayerController() ? HealthBarWidget->SetName(FText()) : HealthBarWidget->SetName(Name);
}
void ACDCarBase::Tick(float DeltaSecond) {
	Super::Tick(DeltaSecond);
	UpdateHPBarVisible();
}
#pragma endregion

#pragma region Events
void ACDCarBase::OnMeshHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
	if (AlreadyHit.Contains(OtherActor)) return;
	AlreadyHit.Add(OtherActor);
	int32 Damage = GetVelocity().Size() / 20.0;
	if (OtherActor->ActorHasTag(Tags[0])) Damage /= 2;
	UGameplayStatics::ApplyPointDamage(OtherActor, Damage, GetActorLocation(), Hit, GetInstigatorController(), this, TSubclassOf<UDamageType>());
}
void ACDCarBase::OnRadiusOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	AlreadyHit.Remove(OtherActor);
}
float ACDCarBase::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) {
	if (!CanBeDamaged() || !this->Tags.IsValidIndex(0) || DamageAmount <= 0) return 0.0;
	CurHealth -= DamageAmount;
	if (CurHealth <= 0) {
		if (GetController()) {
			GetController()->ChangeState(NAME_Spectating);
		}
		Destroy();
	}
	OnHealthChanged();
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}
void ACDCarBase::SetDamageImmune(float Duration) {
	ChangeDamageImmune();
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ACDCarBase::ChangeDamageImmune, Duration, false);
}
void ACDCarBase::ChangeDamageImmune() {
	SetCanBeDamaged(!CanBeDamaged());
}
#pragma endregion

#pragma region UI
void ACDCarBase::OnHealthChanged() {
	const auto HealthBarWidget = Cast<UCDHealthBar>(HPBarComponent->GetUserWidgetObject());
	if (!HealthBarWidget) return;
	HealthBarWidget->SetHealthPercent(CurHealth / MaxHealth);
}
void ACDCarBase::UpdateHPBarVisible() {
	if (!GetWorld() || !GetWorld()->GetFirstPlayerController() || !GetWorld()->GetFirstPlayerController()->GetPawn()) return;
	const auto PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	const auto Distance = FVector::Distance(PlayerLocation, GetActorLocation());
	HPBarComponent->SetVisibility(Distance < HealthVisibilityDistance, true);
}
#pragma endregion

#pragma region Input
void ACDCarBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	PlayerInputComponent->BindAxis("Forward", this, &ACDCarBase::MoveForward);
	PlayerInputComponent->BindAxis("Right", this, &ACDCarBase::MoveRight);
	PlayerInputComponent->BindAxis("TurnX", this, &ACDCarBase::TurnX);
	PlayerInputComponent->BindAxis("TurnY", this, &ACDCarBase::TurnY);
}
void ACDCarBase::MoveForward(float Amount) {
	GetVehicleMovementComponent()->SetThrottleInput(Amount);

}
void ACDCarBase::MoveRight(float Amount) {
	GetVehicleMovementComponent()->SetSteeringInput(Amount);
}
void ACDCarBase::TurnX(float Amount) {
	SpringArm->AddRelativeRotation(FRotator(0.f, Amount, 0.f));
}
void ACDCarBase::TurnY(float Amount) {
	SpringArm->AddRelativeRotation(FRotator(Amount, 0.f, 0.f));
}
#pragma endregion 
