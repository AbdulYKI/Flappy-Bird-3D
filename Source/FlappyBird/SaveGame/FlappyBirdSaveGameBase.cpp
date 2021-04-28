// Fill out your copyright notice in the Description page of Project Settings.


#include "FlappyBird/SaveGame/FlappyBirdSaveGameBase.h"

UFlappyBirdSaveGameBase::UFlappyBirdSaveGameBase()
{
	HighScore = 0;
}

int32 UFlappyBirdSaveGameBase::GetHighScore()
{
	return HighScore;
}

void UFlappyBirdSaveGameBase::SetHighScore(int32 NewHighScore)
{
	HighScore=NewHighScore;
}
