// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Agent.h"
#include "FlockingManager.generated.h"

UCLASS()
class FLOCKING_API UFlockingManager : public UObject
{

public:
	GENERATED_BODY()

	void Init( UWorld *world, UStaticMeshComponent *mesh );
	void Flock();

	FVector Rule1(int id);
	FVector Rule2(int id);
	FVector Rule3(int id);
	FVector TendToPlace(int id);
	void LimitVelocity(int id);


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float m1 = .4f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float m2 = .2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float m3 = .03f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float m4 = .06f;

private:
	UWorld *World;	
	bool initialized;
	TArray<class AAgent *> Agents;
	
};