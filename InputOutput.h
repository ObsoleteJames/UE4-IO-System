
#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "GenericPlatform/GenericPlatformProcess.h"

#include "InputOutput.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInputDelegateType, AActor*, Activator, FString, param);

class AInputOutput;

USTRUCT()
struct FOutputs
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(EditInstanceOnly)
	FString OutputName;

	UPROPERTY(EditInstanceOnly)
	AInputOutput* TargetActor;

	UPROPERTY(EditInstanceOnly)
	FString InputToTrigger;

	UPROPERTY(EditInstanceOnly)
	FString Parameters;

	UPROPERTY(EditInstanceOnly)
	float Delay;

	FOutputs()
	{
		OutputName = FString(TEXT(""));
		TargetActor = nullptr;
		InputToTrigger = FString(TEXT(""));
		Parameters = FString(TEXT(""));
	}

	FOutputs(FString _outputName, AInputOutput* _targetActor, FString _input, FString _param, float _delay)
	{
		OutputName = _outputName;
		TargetActor = _targetActor;
		InputToTrigger = _input;
		Parameters = _param;
		Delay = _delay;
	}
};

USTRUCT()
struct FInputs
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(VisibleAnywhere)
	FString InputName;

	UPROPERTY()
	FInputDelegateType InputDelegate;

	FInputs()
	{
		InputName = FString(TEXT(""));
	}

	FInputs(FString _inputName)
	{
		InputName = _inputName;
	}
};

UCLASS()
class REPLACE_ME_API AInputOutput : public AActor
{
	GENERATED_BODY()

	bool bSetup = false;

public:	
	// Sets default values for this component's properties
	AInputOutput();

	UPROPERTY(EditInstanceOnly)
	TArray<FOutputs> _outputs;

	UPROPERTY(VisibleAnywhere)
	TArray<FInputs> _inputs;

public:
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void FireOutput(AActor* Activator, const FString& _Output);

	// Called by FireOutput
	UFUNCTION(NetMulticast, Reliable)
	void OnFireOutput(const FString& _Output, AActor* Activator, const FString& param);

	UFUNCTION(BlueprintCallable)
	int AddInput(FString _InputName);

	UFUNCTION()
	void PrintString(AActor* Activator, FString param);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	UFUNCTION()
	void SetupIO();
};
