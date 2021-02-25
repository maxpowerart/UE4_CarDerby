// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehicle.h"
#include "CDCarBase.generated.h"

/**
 * 
 */
UCLASS()
class UE4_CARDERBY_API ACDCarBase : public AWheeledVehicle {
	GENERATED_BODY()

private:
	ACDCarBase();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSecond) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float Amount);
	void MoveRight(float Amount);

	void TurnX(float Amount);
	void TurnY(float Amount);

	void UpdateHPBarVisible();
	void ChangeDamageImmune();

protected:
	UPROPERTY(EditDefaultsOnly)
		class UArrowComponent* Arrow;
	UPROPERTY(EditDefaultsOnly)
		class USpringArmComponent* SpringArm;
	UPROPERTY(EditDefaultsOnly)
		class UCameraComponent* Camera;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UWidgetComponent* HPBarComponent;
	UPROPERTY(EditDefaultsOnly)
		class UBoxComponent* HitRadius;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Perception")
		class UAIPerceptionComponent* AIPerception;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Perception")
		class UAISenseConfig_Sight* Sight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (ExposeOnSpawn = "true"))
		FText Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float CurHealth = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float MaxHealth = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		float HealthVisibilityDistance;

	FTimerHandle TimerHandle;
	TArray<AActor*> AlreadyHit;
public:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void OnHealthChanged();

	UFUNCTION()
		void OnMeshHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
		void OnRadiusOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UFUNCTION(BlueprintCallable)
		void SetDamageImmune(float Duration);
};
