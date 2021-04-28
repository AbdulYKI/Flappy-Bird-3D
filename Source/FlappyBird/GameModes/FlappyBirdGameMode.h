// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FlappyBirdGameMode.generated.h"
class APlatformActor;
class AFlappyBirdCharacter;
class UFlappyBirdSaveGameBase;
class USoundBase;
/**
 * 
 */
UCLASS()
class FLAPPYBIRD_API AFlappyBirdGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	void PlaceNextPlatform(FVector Location);
	void IncreaseScore();
	UFUNCTION(BlueprintPure)
		FText GetPlayerScore();

	int32 GetPipePairsSpawned();

	void FlagPoleShown();

	bool GetHasPoleBeenShown();

private:
	void PopulatePool();

	TArray<APlatformActor*> PlatformsPool;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pool", meta = (AllowPrivateAccess = "true"))
	uint8 PlatformsPoolSize=6;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pool", meta = (AllowPrivateAccess = "true"))
	FVector DistanceBetweenFirstPlatformAndCharacter = FVector(141.0f,297.0f,260.0f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pool", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<APlatformActor> PlatformType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CountDown", meta = (AllowPrivateAccess = "true"))
		float CountDownTime = 5.0f;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CountDown", meta = (AllowPrivateAccess = "true"))
		FString SaveGameSlotName = "FlappyBirdHighScore";

	int32 PlayerScore = 0;

	FTimerHandle CountDownTimerHandle;

	UFlappyBirdSaveGameBase* SaveGame;


	UFUNCTION()
	void ResumeCharacterMovementAndEnableInput();

	AFlappyBirdCharacter* PlayerCharacterRef;
	
	UFUNCTION(BlueprintPure)
		FText GetPlayerHighScore();

	int32 PipePairsShown = 0;
    
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
		USoundBase* HighScoreBrokenSound;

	bool bHasBrokenHighScore = false;

	bool bHasFlagPoolBeenShown = false;

	int32 HighScore = 0;

protected:
	virtual void BeginPlay() override;
};
