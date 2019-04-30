// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "Runtime/Core/Public/Delegates/Delegate.h"
#include "OpenDoor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDoorEvent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROOM_ESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

	UPROPERTY(BluePrintAssignable)
	FDoorEvent OnOpen;

	UPROPERTY(BluePrintAssignable)
	FDoorEvent OnClose;

private:

	UPROPERTY(EditAnywhere)
	float CloseAngle;

	UPROPERTY(EditAnyWhere)
	float TriggerMass = 30.f;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;	

	AActor* Owner = nullptr;//the owning door

	//returns total mass in kgs
	float GetTotalMassOfActorsOnPlate();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
