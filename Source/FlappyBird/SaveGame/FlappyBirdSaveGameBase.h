// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "FlappyBirdSaveGameBase.generated.h"

/**
 * 
 */
UCLASS()
class FLAPPYBIRD_API UFlappyBirdSaveGameBase : public USaveGame
{
	GENERATED_BODY()
		UFlappyBirdSaveGameBase();
private:
	UPROPERTY(VisibleAnywhere)
		int32 HighScore;
public:
	int32 GetHighScore();

	void SetHighScore(int32 NewHighScore);

};
