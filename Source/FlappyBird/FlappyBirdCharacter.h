// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FlappyBirdCharacter.generated.h"
class UCameraComponent;
class USpringArmComponent;
class APlayerController;
class USkeletalMeshComponent;
UCLASS()
class FLAPPYBIRD_API AFlappyBirdCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFlappyBirdCharacter();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintPure)
	bool GetIsDead();

	void StopMovement();

	void ResumeMovement();

	void EnableControllerInput();

	void DisableControllerInput();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void NotifyHit
	(
		class UPrimitiveComponent* MyComp,
		AActor* Other,
		class UPrimitiveComponent* OtherComp,
		bool bSelfMoved,
		FVector HitLocation,
		FVector HitNormal,
		FVector NormalImpulse,
		const FHitResult& Hit
	) override;

private: 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Delay", meta = (AllowPrivateAccess = "true"))
		float DeathDelayInSeconds=0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Delay", meta = (AllowPrivateAccess = "true"))
		float PlayingFallSoundDelayInSeconds = 0.25f;
	 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Jump", meta = (AllowPrivateAccess = "true"))
		float JumpHeight=500.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Velocity", meta = (AllowPrivateAccess = "true"))
		float ScaleValueMovingForward = 1.8f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Velocity", meta = (AllowPrivateAccess = "true"))
		float HowMuchToAddToScaleMovingForward = 0.001f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Velocity", meta = (AllowPrivateAccess = "true"))
		float HowLongInSecondsBeforeIncreasingScaleMovingForward = 30.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Velocity", meta = (AllowPrivateAccess = "true"))
		float InterpolationSpeedForRotation = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Jump", meta = (AllowPrivateAccess = "true"))
		bool bOverrideZAxisOnJump =false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
		USoundBase* JumpSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
		USoundBase* HitSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
		USoundBase* FallSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
		FVector CharacterVelocityOnTick= FVector(0.0f,-50.0f,0.0f);

	bool bIsDead = false;

	void MoveCharacterForward(float DeltaTime);

	void RotateCharacterDependingCurrentVelocity(float DeltaTime);

	void BirdJump();

	void PlayFallingSound();

	void DetachCameraOnDeath();

	APlayerController* GetPlayerController();
	
	FTimerHandle PlayFallingSoundTimerHandle;

	USkeletalMeshComponent* CharacterMeshRef;


};
