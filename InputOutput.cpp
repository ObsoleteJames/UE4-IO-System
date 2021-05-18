
#include "InputOutput.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
AInputOutput::AInputOutput()
{
	SetReplicates(true);

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root"));
}

void AInputOutput::FireOutput_Implementation(AActor* Activator, const FString& Output)
{
	for (auto k : _outputs)
	{
		if (k.OutputName == Output)
		{
			if (k.Delay != 0)
			{
				FTimerDelegate TimerDelegate;
				FTimerHandle TimerHandle;
				TimerDelegate.BindUFunction(k.TargetActor, FName("OnFireOutput"), k.InputToTrigger, Activator, k.Parameters);
				GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, k.Delay, false);
				continue;
			}

			k.TargetActor->OnFireOutput(k.InputToTrigger, Activator, k.Parameters);
		}
	}
}

void AInputOutput::OnFireOutput_Implementation(const FString& _Input, AActor* Activator, const FString& param)
{
	for (auto k : _inputs)
	{
		if (k.InputName == _Input)
		{
			k.InputDelegate.Broadcast(Activator, param);
		}
	}
}

int AInputOutput::AddInput(FString _InputName)
{
	return _inputs.Add(FInputs(_InputName));
}

void AInputOutput::PrintString(AActor* Activator, FString param)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Cyan, Activator->GetName() + ": " + param);
}

// Called when the game starts
void AInputOutput::BeginPlay()
{
	Super::BeginPlay();

	SetupIO();
}

void AInputOutput::SetupIO()
{
	if (bSetup)
		return;

	bSetup = true;

	int inputIndex = 0;

	inputIndex = AddInput("print");
	_inputs[inputIndex].InputDelegate.AddDynamic(this, &AInputOutput::PrintString);

	FireOutput(nullptr, "OnBeginPlay");
}
