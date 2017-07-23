// Copyright HaiweiZuo 2017

#include "OpenDoor.h"
#include "Gameframework/Actor.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h" 
#include "GameFramework/PlayerController.h"

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

	// find owner
	Owner = GetOwner();

	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
	 
}

void UOpenDoor::OpenDoor()
{
	// Set the door rotation
	Owner->SetActorRotation(FRotator(0.f, OpenAngle, 0.f));

}

void UOpenDoor::CloseDoor()
{

	//Create a rorator
	FRotator NewRotation = FRotator(0.f, -0.f, 0.f);

	// Set the door rotation
	Owner->SetActorRotation(NewRotation);
}



// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// poll the trigger volume
	// if the actorthatopen is in the volume
	if (GetTotalMassOfActorOnPlate()>30.f)
	{
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}
	// Check if it's time to close the door
	if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay)
	{
		CloseDoor();
	}

	GetTotalMassOfActorOnPlate();

}

float UOpenDoor::GetTotalMassOfActorOnPlate()
{
	float TotalMass = 0.f;

	//find all the overlaping actors
	TArray<AActor*> OverlapingActors;
	PressurePlate->GetOverlappingActors(OUT OverlapingActors);

	//Iterate through adding their masses
	for (auto* Actor : OverlapingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s on pressure plate"), *Actor->GetName())

	}
	return TotalMass;
}

