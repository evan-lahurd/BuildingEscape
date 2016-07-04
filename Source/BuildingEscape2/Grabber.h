// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE2_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

private:
	// how far ahead of the player we can reach in cm
	float Reach;

	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;

	// Ray-cast and grab what's in reach
	void Grab();

	void Release();

	// Find attached physics handle component
	void FindPhysicsHandleComponent();

	// Set up attached input component
	void SetUpInputComponent();

	// Get the start of the player reach location
	FVector GetReachLineStart();

	// Get the end of the player reach location
	FVector GetReachLineEnd();

	// Return hit for first physics body in reach
	const FHitResult GetFirstPhysicsBodyInReach();
};
