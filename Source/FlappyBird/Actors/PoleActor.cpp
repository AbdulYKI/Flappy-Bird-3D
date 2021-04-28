// Fill out your copyright notice in the Description page of Project Settings.


#include "FlappyBird/Actors/PoleActor.h"

// Sets default values
APoleActor::APoleActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PoleMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Pole Mesh"));
}

// Called when the game starts or when spawned
void APoleActor::BeginPlay()
{
	Super::BeginPlay();
	
}

