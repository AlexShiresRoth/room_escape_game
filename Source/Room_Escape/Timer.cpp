// Copyright by Alex Rothenberg


#include "Timer.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Components/TextRenderComponent.h"



// Sets default values
ATimer::ATimer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	CountDownText = CreateAbstractDefaultSubobject<UTextRenderComponent>(TEXT("CountDownNumber"));
	CountDownText->SetHorizontalAlignment(EHTA_Center);
	CountDownText->SetWorldSize(150.0f);
	RootComponent = CountDownText;
	CountDownTime = 3;

}

//updates timer display every second until 0
void ATimer::UpdateTimerDisplay()
{
	CountDownText->SetText(FString::FromInt(FMath::Max(CountDownTime, 0)));
}

void ATimer::AdvanceTimer()
{
	--CountDownTime;
	UpdateTimerDisplay();
	if (CountDownTime < 1)
	{
		//We're done counting down, so stop running the timer.
		GetWorldTimerManager().ClearTimer(CountdownTimerHandle);
		CountdownHasFinished();
	}
}

void ATimer::CountdownHasFinished()
{
	//Change to a special readout
	CountDownText->SetText(TEXT("GO!"));
}

// Called when the game starts or when spawned
void ATimer::BeginPlay()
{
	Super::BeginPlay();
	
	UpdateTimerDisplay();
	
	GetWorldTimerManager().SetTimer(CountdownTimerHandle, this, &ATimer::AdvanceTimer, 1.0f, true);
	
}

// Called every frame
void ATimer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


