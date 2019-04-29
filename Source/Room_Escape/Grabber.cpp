// Copyright by Alex Rothenberg

#include "Grabber.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/PlayerController.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	//find objects that have physics enabled
	FindPhysicsHandleComponent();
	SetupInputComponent();
	// ...
	UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty"));
	
}

void UGrabber::FindPhysicsHandleComponent()
{
	//look for attatched physics handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandle == nullptr)
	{
		//physics handle not found
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component"), *GetOwner()->GetName());
	}
}

void UGrabber::SetupInputComponent()
{
	InputHandler = GetOwner()->FindComponentByClass<UInputComponent>();


	//look for input handler(only appears at runtime)
	if (InputHandler)
	{
		InputHandler->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);

		InputHandler->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s is missing input handler component"), *GetOwner()->GetName());
	}
}



void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab pressed"));

	//line trace and  try and reach any actors with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();
	///if we hit something then attatch a physics handle
	//attatch physics handle
	if (ActorHit){
		if (!PhysicsHandle) { return; }
		PhysicsHandle->GrabComponent(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true // allow rotation
		);
	}
}

void UGrabber::Release()
{
	if (!PhysicsHandle) { return; }
	PhysicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	

	if (!PhysicsHandle) { return; }
	//if the physics handle is attatched 
	if (PhysicsHandle->GrabbedComponent)
	{
		//move the object that we're holding
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	}

}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	///setup query paramaters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	///Line-trace(Ray-cast) out to reach distance
	FHitResult HitResult;

	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	///see what we hit 
	AActor* ActorHit = HitResult.GetActor();

	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("LineTrace hit %s"), *(ActorHit->GetName()));
	}

	return HitResult;
}


FVector UGrabber::GetReachLineStart()
{
	// Get Player view point this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	return PlayerViewPointLocation;
}

FVector UGrabber::GetReachLineEnd()
{
	// Get Player view point this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}