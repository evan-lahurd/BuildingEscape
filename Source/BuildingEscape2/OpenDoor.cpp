// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape2.h"
#include "OpenDoor.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	if (!TriggerVolume) {
		UE_LOG(LogTemp, Error, TEXT("%s is missing trigger volume"), *GetOwner()->GetName());
	}

	// Check the total weight on the trigger volume only when something enters/exits the volume
	TriggerVolume->OnActorBeginOverlap.AddDynamic(this, &UOpenDoor::CheckOverlappingActorsMass);
	TriggerVolume->OnActorEndOverlap.AddDynamic(this, &UOpenDoor::CheckOverlappingActorsMass);
}

void UOpenDoor::CheckOverlappingActorsMass(AActor* OtherActor) {
	if (GetTotalMassOfActorsOnPlate() > TriggerThreshhold) {
		OnOpen.Broadcast();
	} else {
		OnClose.Broadcast();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate() {

	float TotalMass = 0.0f;

	if (!TriggerVolume) {
		return TotalMass;
	}

	// Find all of the overlapping actors
	TArray<AActor*> OverlappingActors;
	TriggerVolume->GetOverlappingActors(OUT OverlappingActors);
	// Iterate through them, adding their masses
	for (const auto& Actor : OverlappingActors) {
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("Total mass is %s"), *FString::FromInt((float)TotalMass));
	}
	return TotalMass;
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


