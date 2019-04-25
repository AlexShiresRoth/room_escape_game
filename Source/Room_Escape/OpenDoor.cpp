// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/Classes/Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	// ...
	Owner = GetOwner();

}

void UOpenDoor::OpenDoor()
{
	
	//rotates door on yaw

	Owner->SetActorRotation(FRotator(0.0f, OpenAngle, 0.0f));
}

void UOpenDoor::CloseDoor()
{
	
	//rotates door on yaw


	Owner->SetActorRotation(FRotator(0.0f, CloseAngle, 0.0f));
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (GetTotalMassOfActorsOnPlate() > 30.f)//TODO make into a param 
	{
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
		UE_LOG(LogTemp, Error, TEXT("Door Opened"));
	}

	//check if it is time to close the door
	if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay)
	{
		CloseDoor();
	}
	
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;

	//find all the overlapping actors
	TArray<AActor*> OverLappingActors;
	PressurePlate->GetOverlappingActors(OUT OverLappingActors);
	//iterate through them adding their masses
	//change to a pointer if error
	for (const auto* OverLappingActor : OverLappingActors)
	{
		TotalMass += OverLappingActor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		///UE_LOG(LogTemp, Warning, TEXT("%s on pressure plate"), *OverLappingActor->GetName());
	}

	return TotalMass;
}

