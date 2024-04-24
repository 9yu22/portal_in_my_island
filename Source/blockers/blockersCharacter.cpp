// Copyright Epic Games, Inc. All Rights Reserved.

#include "blockersCharacter.h"


#include "Kismet/GameplayStatics.h"
#include "Engine/LocalPlayer.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Controller.h"


#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


#include "blockersGameMode.h"
#include "Bullet.h"
#include "HealthBarWidget.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);


AblockersCharacter::AblockersCharacter()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_blockers(TEXT("/Game/Blockers/Meshs/new_player_Mesh.new_player_Mesh"));
	if (SK_blockers.Succeeded()) {
		GetMesh()->SetSkeletalMesh(SK_blockers.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimBPClass(TEXT("/Game/Blockers/Blueprints/BlockersAnimBP.BlockersAnimBP_C"));
	if (AnimBPClass.Succeeded()) {
		UE_LOG(LogTemp, Warning, TEXT("Animation OK"));
		GetMesh()->SetAnimInstanceClass(AnimBPClass.Class);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Animation NO"));
	}

	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -86.0f));
	GetMesh()->SetWorldScale3D(FVector(0.3f, 0.3f, 0.3f));
	FRotator NewRotation = FRotator(0.0f, -90.0f, 0.0f);
	GetMesh()->SetRelativeRotation(NewRotation);

		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	//총구 표시 컴포넌트를 생성하고 박스 컴포넌트의 자식컴포넌트로 설정한다.
	firePosition = CreateDefaultSubobject<UArrowComponent>(TEXT("Fire Position"));

	HealthWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthWidgetComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	health = MaxHealth;

}

void AblockersCharacter::BeginPlay()
{

	Super::BeginPlay();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AblockersCharacter::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		AblockersCharacter* Character = Cast<AblockersCharacter>(Actor);
		if (Character)
		{
			Players.Add(Character);
		}
	}
	//UE_LOG(LogTemp, Warning, TEXT("Number of characters in PlayerSet: %d"), PlayerSet.Num());

	UHealthBarWidget* HealthBar = Cast<UHealthBarWidget>(HealthWidgetComp->GetUserWidgetObject());
	HealthBar->SetOwnerCharacter(this);
}

void AblockersCharacter::Tick(float DeltaTime) {

	Super::Tick(DeltaTime);

	health = FMath::Clamp<float>(health - DeltaTime * 3.0, 0, MaxHealth); //시간에 따라 줄어들도록 설정.
	if (health < 0.f) {
		health = 0;
	}
}

void AblockersCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//UE_LOG(LogTemp, Warning, TEXT("Hello"));
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AblockersCharacter::Fire);

	//Sets up an input key action to call Restart Player.
	PlayerInputComponent->BindAction("Restart", IE_Pressed, this, &AblockersCharacter::CallRestartPlayer);
}

//마우스 왼쪽 버튼 입력 처리 함수
void AblockersCharacter::Fire()
{
	FVector BulletLocation = GetMesh()->GetSocketLocation("gun");
	ABullet* bullet = GetWorld()->SpawnActor<ABullet>(bulletFactory, BulletLocation, firePosition->GetComponentRotation());
}

void AblockersCharacter::Destroyed()
{
	Super::Destroyed();

	// Example to bind to OnPlayerDied event in GameMode.
	if (UWorld* World = GetWorld())
	{
		if (AblockersGameMode* GameMode = Cast<AblockersGameMode>(World->GetAuthGameMode()))
		{
			GameMode->GetOnPlayerDied().Broadcast(this);
		}
	}
}

void AblockersCharacter::CallRestartPlayer()
{
	//Get a reference to the Pawn Controller.
	AController* CortollerRef = GetController();

	//Destroy the Player.
	Destroy();

	//Get the World and GameMode in the world to invoke its restart player function.
	if (UWorld* World = GetWorld())
	{
		if (AblockersGameMode* GameMode = Cast<AblockersGameMode>(World->GetAuthGameMode()))
		{
			GameMode->RestartPlayer(CortollerRef);
		}
	}
}