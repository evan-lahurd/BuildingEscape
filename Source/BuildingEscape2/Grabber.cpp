// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape2.h"
#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
	Reach = 200.0f;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandleComponent();
	SetUpInputComponent();
}

/// Look for attached physics handle
void UGrabber::FindPhysicsHandleComponent() {
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle) {
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component"), *GetOwner()->GetName());
	}
}

/// Look for attached input component (only appears at run time)
void UGrabber::SetUpInputComponent() {
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) {
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	} else {
		UE_LOG(LogTemp, Error, TEXT("%s missing input component"), *GetOwner()->GetName());
	}
}

FVector UGrabber::GetReachLineStart() {
	// Get the player viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	return PlayerViewPointLocation;
}

FVector UGrabber::GetReachLineEnd() {
	// Get the player viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	/// Ray cast out to reach distance
	FHitResult LineTraceHitResult;
	GetWorld()->LineTraceSingleByObjectType(
		OUT LineTraceHitResult,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		FCollisionQueryParams(FName(TEXT("")), false, GetOwner())
	);
	// See what we hit
	AActor* ActorHit = LineTraceHitResult.GetActor();
	if (ActorHit) {
		UE_LOG(LogTemp, Warning, TEXT("%s was hit by ray trace"), *ActorHit->GetName());
	}
	return LineTraceHitResult;
}

void UGrabber::Grab() {

	if (!PhysicsHandle) {
		return;
	}

	// Ray trace and try and reach any actors with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent(); // gets the mesh component in our case
	auto ActorHit = HitResult.GetActor();

	// If we hit something then attach a physics handle
	// TODO attach physics handle
	if (ActorHit) {
		PhysicsHandle->GrabComponent(
			ComponentToGrab,
			NAME_None,
			ActorHit->GetActorLocation(),
			true // allow rotation
		);
	}
}

void UGrabber::Release() {
	if (!PhysicsHandle) {
		return;
	}
	PhysicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PhysicsHandle) {
		return;
	}

	FVector LineTraceEnd = GetReachLineEnd();

	// if the physics handle is attached, move the object that we're holding
	if (PhysicsHandle->GrabbedComponent) {
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
}


