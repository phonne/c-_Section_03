// Copyright HaiweiZuo 2017

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Engine/InputActionDelegateBinding.h"
#include "Grabber.generated.h"
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	float Reach = 100.f; // how far ahead of the player can we reach in cm
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;

	//Ray-cast and grab what's in reach
	void Grab();
	void Release();

	//find (assumed) attached physics handle
	void FindPhysicsHandleComponent();

	//setup (assumed) attached input component
	void SetupInputComponent();

	//result hit for first body in reach
	const FHitResult GetFirstPhysicsBodyInReach();

	// Return end of reach line
	FVector GetReachLineEnd();

	//Return start of reach line
	FVector GetReachLineStart();
};
