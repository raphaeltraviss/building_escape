// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "OpenDoor.h"

#define OUT




// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();
	if (!PressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("Trigger volume is missing!"));
	}
}

void UOpenDoor::OpenDoor() {
	OnOpenRequest.Broadcast();
}

void UOpenDoor::CloseDoor() {
	Owner->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));

}

void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	float CurrentTime = GetWorld()->GetTimeSeconds();

	if (GetTotalMassOfActorsOnPlate() > 35.f) {
		OpenDoor();
		LastDoorOpenTime = CurrentTime;
	}

	if (CurrentTime - LastDoorOpenTime > DoorCloseDelay) {
		CloseDoor();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate() {
	TArray<AActor*> OverlappingActors;

	if (!PressurePlate) { return 0.f; }

	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	float TotalMass = 0.f;
	for (const auto* Actor : OverlappingActors) {
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	
	UE_LOG(LogTemp, Warning, TEXT("The total mass is %f"), TotalMass);
	
	// Get all overlapping actors.
	// Iterate through them, add their masses.
	return TotalMass;
}

