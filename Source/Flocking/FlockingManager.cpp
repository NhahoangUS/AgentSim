// Fill out your copyright notice in the Description page of Project Settings.


#include "FlockingManager.h"

#define AGENT_COUNT 20

void UFlockingManager::Init( UWorld *world, UStaticMeshComponent *mesh ) {
    UE_LOG(LogTemp, Warning, TEXT("Manager initialized"));
    
    World = world;
    float incr = (PI * 2.f) / AGENT_COUNT;
    for( int i = 0; i < AGENT_COUNT; i++ ) {
        if( World != nullptr ) {
            FRotator rotation = FRotator();

            FVector location = FVector();
            location.X = FMath::Sin( incr * i ) * 650.f;
            location.Z = FMath::Cos( incr * i ) * 650.f;

            AAgent * agent = World->SpawnActor<AAgent>( location, rotation );
            agent->Init( mesh, i );
            Agents.Add( agent );
        }
    }

    initialized = true;
}

void UFlockingManager::Flock() {
    for (int i = 0; i < Agents.Num(); i++) {
		AAgent* agent = Agents[i];

		agent->Velocity +=  Rule1(i)       * m1 * agent->DeltaTimeGlobal 
                        +   Rule2(i)       * m2 * agent->DeltaTimeGlobal 
                        +   Rule3(i)       * m3 * agent->DeltaTimeGlobal 
                        +   TendToPlace(i) * m4 * agent->DeltaTimeGlobal;
		agent->GetActorLocation() = agent->GetActorLocation() + agent->Velocity;
		LimitVelocity(i);
	}
}

FVector UFlockingManager::Rule1(int id)
{
	FVector pc;
	for (int i = 0; i < Agents.Num(); i++) {
		if (Agents[i]->ID != id) {
			pc = pc + Agents[i]->GetActorLocation();
		}
	}

	pc = pc / (Agents.Num() - 1);
	return(pc - Agents[id]->GetActorLocation()) / 100;
}

FVector UFlockingManager::Rule2(int id)
{
	FVector c = FVector(0,0,0);

	for (int i = 0; i < Agents.Num(); i++) {
		if (Agents[i]->ID != id) {
			if (FVector::Dist(Agents[i]->GetActorLocation(), Agents[id]->GetActorLocation()) < 100.0)
				c = c - (Agents[i]->GetActorLocation() - Agents[id]->GetActorLocation());
		}
	}

	return c;
}

FVector UFlockingManager::Rule3(int id)
{
    FVector pv = FVector(0,0,0);
	for (int i = 0; i < Agents.Num(); i++) {
		if (Agents[i]->ID != id) {
			pv = pv + Agents[i]->Velocity;
		}
	}
	pv = pv / (Agents.Num() - 1);

	return(pv - Agents[id]->Velocity) / 8;
}

FVector UFlockingManager::TendToPlace(int id)
{
	FVector Place = FVector(20,20,20);
	return (Place - Agents[id]->GetActorLocation()) / 100;
}

void UFlockingManager::LimitVelocity(int id)
{
	int vlim = 20.0;

	if (Agents[id]->Velocity.Size() > vlim) {
		Agents[id]->Velocity = (Agents[id]->Velocity / Agents[id]->Velocity.Size()) * vlim;
	}
}