// Copyright by Alex Rothenberg

#pragma once

#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/InputComponent.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Grabber.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROOM_ESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private: 
	//how far ahead of the player can we reach
	float Reach = 250.0f;

	UPhysicsHandleComponent* PhysicsHandle = nullptr;

	UInputComponent* InputHandler = nullptr;

	//ray-cast and grab whats in reach
	void Grab();

	//release what is being grabbed
	void Release();

	//find attatched physics
	void FindPhysicsHandleComponent();

	//setup assumed attatched input component
	void SetupInputComponent();

	//return hit for first physics body in reach
	const FHitResult GetFirstPhysicsBodyInReach();

	FVector GetReachLineStart();

	//returns end of the reach line
	FVector GetReachLineEnd();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
