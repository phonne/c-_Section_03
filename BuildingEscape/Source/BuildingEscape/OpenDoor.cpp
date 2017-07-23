// Copyright HaiweiZuo 2017

#include "OpenDoor.h"
#include "Gameframework/Actor.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h" 
#define OUT
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

	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing pressure plate "), *GetOwner()->GetName())
	}
	 
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// poll the trigger volume
	if (GetTotalMassOfActorOnPlate()>30.f)
	{
		OnOpen.Broadcast();
	}else{
		OnClose.Broadcast();
	}

	GetTotalMassOfActorOnPlate();

}

float UOpenDoor::GetTotalMassOfActorOnPlate()
{
	float TotalMass = 0.f;
	//create an array to store *actor
	TArray<AActor*> OverlapingActors;

	//find all the overlaping actors
	//point protector
	if (!PressurePlate) { 
		UE_LOG(LogTemp, Warning, TEXT("cannot use ATriggerVolume* pressureplate"))
		return TotalMass; }
	PressurePlate->GetOverlappingActors(OUT OverlapingActors);

	//Iterate through adding their masses
	for (auto* Actor : OverlapingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s on pressure plate"), *Actor->GetName())

	}
	return TotalMass;
}

