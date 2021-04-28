// Fill out your copyright notice in the Description page of Project Settings.


#include "PipeActor.h"

// Sets default values
APipeActor::APipeActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PipeMesh= CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pipe Mesh"));
}

// Called when the game starts or when spawned
void APipeActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APipeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

