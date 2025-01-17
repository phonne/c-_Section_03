// Copyright HaiweiZuo 2017

#include "Grabber.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/PrimitiveComponent.h" 
#include "Engine/InputActionDelegateBinding.h"
#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	PrimaryComponentTick.bCanEverTick = true;
}
// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();

}
// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PhysicsHandle) { return; }
	//if the physics handle is attached 
	if (PhysicsHandle->GrabbedComponent)
	{
		//move the object what we're holding
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());

	}
}


void UGrabber::Grab()
{	// Line Trace and see if we reach any actors with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent(); //Get the mesh in our case
	auto ActorHit = HitResult.GetActor();
		// if we hit something then attach a physical handle
	if (ActorHit) 
	{
		if (!PhysicsHandle) { return; }
		PhysicsHandle->GrabComponent(ComponentToGrab,//grabing truns outto be the mesh
			NAME_None, //no bones needed
			ComponentToGrab->GetOwner()->GetActorLocation(), //get the component location
			true //allow rotation
		);	
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Grab pressed "))
}

// called when grab is released 
void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab released"))
		if (!PhysicsHandle) { return; }
		PhysicsHandle->ReleaseComponent();
}

//look for attached physics handle
void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component "), *GetOwner()->GetName())
	}
}

//look for attached input component (only appears at run time)
void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
			// bind the input axis
			InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing input component "), *GetOwner()->GetName())
	}
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{	
	// Setup query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	// Line-trace (AKA ray-cast) out to reach distance
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	// see what we hit 
	AActor* ActorHit = HitResult.GetActor();

	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Line trace hit: %s"), *ActorHit->GetName())
	}
	return HitResult;
}

FVector UGrabber::GetReachLineEnd()
{

	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);
	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}

FVector UGrabber::GetReachLineStart()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);
	return PlayerViewPointLocation;
}