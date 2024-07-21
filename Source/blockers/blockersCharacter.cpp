// Copyright Epic Games, Inc. All Rights Reserved.

#include "blockersCharacter.h"
#include "../Network/SGameInstance.h"

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


#include "BKPlayerController.h"
#include "../blockers/Public/InventoryComponent.h"
#include "../blockers/Public/Item.h"
#include "blockersGameMode.h"
#include "Bullet.h"
#include "HealthBarWidget.h"


DEFINE_LOG_CATEGORY(LogTemplateCharacter);


AblockersCharacter::AblockersCharacter()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_blockers(TEXT("/Game/NPC_Character_Four/Meshes/SM_NPC_Character_Four.SM_NPC_Character_Four"));
	if (SK_blockers.Succeeded()) {
		GetMesh()->SetSkeletalMesh(SK_blockers.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> M_blockers(TEXT("/Game/NPC_Character_Four/Materials/M_NPC_Character_Four_Skin1.M_NPC_Character_Four_Skin1"));
	//character1:: /Game/NPC_Character_Four/Materials/M_NPC_Character_Four_Skin1.M_NPC_Character_Four_Skin1
	//character2:: /Game/NPC_Character_Four/Materials/M_NPC_Character_Four_Skin2.M_NPC_Character_Four_Skin2
	//character3:: /Game/NPC_Character_Four/Materials/M_NPC_Character_Four_Skin3.M_NPC_Character_Four_Skin3
	//character4:: /Game/NPC_Character_Four/Materials/M_NPC_Character_Four_Skin4.M_NPC_Character_Four_Skin4
	//if (M_blockers.Succeeded()) {
	//	GetMesh()->SetMaterial(0, M_blockers.Object);
	//}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimBPClass(TEXT("/Game/NPC_Character_Four/Animations/Demo/ThirdPerson_AnimBP.ThirdPerson_AnimBP_C"));
	if (AnimBPClass.Succeeded()) {
		UE_LOG(LogTemp, Warning, TEXT("Animation OK"));
		GetMesh()->SetAnimInstanceClass(AnimBPClass.Class);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Animation NO"));
	}

	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -86.0f));
	GetMesh()->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	FRotator NewRotation = FRotator(0.0f, -90.0f, 0.0f);
	GetMesh()->SetRelativeRotation(NewRotation);

	//boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	//SetRootComponent(boxComp);
	//boxComp->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));
		
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

	//HealthWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	//HealthWidgetComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	health = MaxHealth;

	Inventory = CreateDefaultSubobject<UInventoryComponent>("Inventory");
	Inventory->Capacity = 20; //you can input 20 items

}

void AblockersCharacter::BeginPlay()
{

	Super::BeginPlay();

	/*TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AblockersCharacter::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		AblockersCharacter* Character = Cast<AblockersCharacter>(Actor);
		if (Character)
		{
			Players.Add(Character);
		}
	}*/

	PacketLocation = GetActorLocation();
	PacketRotation = GetActorRotation();
	//UE_LOG(LogTemp, Warning, TEXT("Number of characters in PlayerSet: %d"), PlayerSet.Num());

	//UHealthBarWidget* HealthBar = Cast<UHealthBarWidget>(HealthWidgetComp->GetUserWidgetObject());
	//HealthBar->SetOwnerCharacter(this);

}

void AblockersCharacter::Tick(float DeltaTime) {

	Super::Tick(DeltaTime);

	SendMovePacketTime += DeltaTime;
	if (IsSelf == true && SendMovePacketTime >= 0.1f) {
		SendMovePacket();
	}

	health = FMath::Clamp<float>(health - DeltaTime * 0.5, 0, MaxHealth); //시간에 따라 줄어들도록 설정.
	if (health < 1.f) {
		health = MaxHealth;
	}

	USGameInstance* instance = USGameInstance::GetMyInstance(this);

	if (instance->Socket != nullptr) {
		SendMovePacketTime += DeltaTime;
		if (IsSelf == true && SendMovePacketTime >= 0.1f) {
			SendMovePacket();
		}

		if (IsSelf == false) {
			InterpolateCharacter(PacketLocation, PacketRotation, DeltaTime);
		}
	}	

	// 현재 위치와 이전 위치를 사용하여 속도를 계산
	FVector CurrentLocation = GetActorLocation();
	FVector Velocity = (CurrentLocation - PrevLocation) / DeltaTime;
	CurrSpeed = Velocity.Size();

	// 이전 위치를 업데이트
	PrevLocation = CurrentLocation;
}

void AblockersCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//UE_LOG(LogTemp, Warning, TEXT("Hello"));
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AblockersCharacter::Fire);

}

void AblockersCharacter::InterpolateCharacter(FVector NewLocation, FRotator NewRotation, float DeltaTime)
{
	FVector CurrentLocation = GetActorLocation();
	FRotator CurrentRotation = GetActorRotation();

	// 이동 속도 가져오기
	float InterpolationSpeed = GetCharacterMovement()->MaxWalkSpeed;

	// 다음 위치 계산
	FVector InterpolatedLocation = FMath::VInterpConstantTo(CurrentLocation, PacketLocation, DeltaTime, InterpolationSpeed);
	FRotator InterpolatedRotation = FMath::RInterpConstantTo(CurrentRotation, PacketRotation, DeltaTime, InterpolationSpeed);

	SetActorLocation(InterpolatedLocation);
	SetActorRotation(InterpolatedRotation);
}

void AblockersCharacter::SendMovePacket()
{
	USGameInstance* instance = USGameInstance::GetMyInstance(this);
	if (instance) {
		CS_MOVE_PACKET new_pos;            
		FVector CurrentLocation = GetActorLocation();
		FRotator CurrentRotation = GetActorRotation();

		new_pos.type = CS_MOVE;
		new_pos.size = sizeof(CS_MOVE_PACKET);
		new_pos.x = CurrentLocation.X;
		new_pos.y = CurrentLocation.Y;
		new_pos.z = CurrentLocation.Z;
		new_pos.pitch = CurrentRotation.Pitch;
		new_pos.yaw = CurrentRotation.Yaw;
		new_pos.roll = CurrentRotation.Roll;

		int BytesSent = 0;
		instance->Socket->Send((uint8*)&new_pos, sizeof(new_pos), BytesSent);
	}
	else
		UE_LOG(LogTemp, Log, TEXT("Fail GetInstance"));
}

void AblockersCharacter::SetTextureForCharacter(int Character_id)
{
	UMaterial* Material = nullptr;
	FString Path;

	switch (Character_id % 4)
	{
	case 0:
		Path = TEXT("/Game/NPC_Character_Four/Materials/M_NPC_Character_Four_Skin1.M_NPC_Character_Four_Skin1");
		break;
	case 1:
		Path = TEXT("/Game/NPC_Character_Four/Materials/M_NPC_Character_Four_Skin2.M_NPC_Character_Four_Skin2");
		break;
	case 2:
		Path = TEXT("/Game/NPC_Character_Four/Materials/M_NPC_Character_Four_Skin3.M_NPC_Character_Four_Skin3");
		break;
	case 3:
		Path = TEXT("/Game/NPC_Character_Four/Materials/M_NPC_Character_Four_Skin4.M_NPC_Character_Four_Skin4");
		break;
	default:
		break;
	}

	if (!Path.IsEmpty())
	{
		Material = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), nullptr, *Path));
		if (Material)
		{
			GetMesh()->SetMaterial(0, Material);
		}
		else
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Failed to load material"));
			}
		}
	}
}


//마우스 왼쪽 버튼 입력 처리 함수
void AblockersCharacter::Fire()
{
	FVector BulletLocation = GetMesh()->GetSocketLocation("gun");
	BulletLocation.X += 100.f;
	ABullet* bullet = GetWorld()->SpawnActor<ABullet>(bulletFactory, BulletLocation, firePosition->GetComponentRotation());
	bulletNum -= 1;
}

void AblockersCharacter::UseItem(UItem* Item)
{
	if (Item) {
		Item->Use(this);
		Item->OnUse(this); //BP EVENT
	}
}

void AblockersCharacter::AddToInventory(APickUpItem* actor)
{
	//UE_LOG(LogTemp, Warning, TEXT("init size: %d"), _inventory.Num());
	int32 ItemIndex = GetItemIndex(actor->Name);
	if (ItemIndex != -2) {
		ItemInventory[ItemIndex]->amount += actor->amount;
	}
	else {
		ItemInventory.Add(actor);
		/*int32 EmptyIndex = FindEmptySlot();
		if (EmptyIndex != -1) {
			ItemInventory.Add(actor);
		}*/
	}
	//UE_LOG(LogTemp, Warning, TEXT("Inventory size: %d"), _inventory.Num());
}

void AblockersCharacter::RemoveFromInventory(const FString itemName, const int32 number)
{
	int32 ItemIndex = GetItemIndex(itemName);
	int32 lastIndex = ItemInventory.Num() - 1;

	if (ItemIndex != -2) {
		if (ItemInventory[ItemIndex]->amount > number) {
			ItemInventory[ItemIndex]->amount -= number;
		}
		else {
			ItemInventory.RemoveAt(ItemIndex);
			ItemInventory.Shrink();
			UE_LOG(LogTemp, Warning, TEXT("last item removed."));
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Item not found in inventory."));
	}
}

bool AblockersCharacter::CheckItemNum(FString itemName, int32 number)
{
	int32 ItemIndex = GetItemIndex(itemName);
	if (ItemIndex != -2) {
		if (ItemInventory[ItemIndex]->amount >= number) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}



int32 AblockersCharacter::GetItemIndex(FString itemName)
{
	for (int32 i = 0; i < ItemInventory.Num(); ++i) {
		if (ItemInventory[i]->Name == *itemName) {
			return i;
		}
	}
	return -2;
}

int32 AblockersCharacter::FindEmptySlot()
{
	for (int32 i = 0; i < ItemInventory.Num(); ++i)
	{
		if (ItemInventory[i]->Name == "") {
			return i;
		}
	}
	return -1;
}

void AblockersCharacter::UpdateInventory()
{
	/*FString sInventory = "";

	for (APickUpItem* actor : _inventory)
	{
		sInventory.Append(actor->Name);
		sInventory.Append(" | ");
	}

	GEngine->AddOnScreenDebugMessage(1, 3, FColor::White, *sInventory);*/

	OnUpdateInventory.Broadcast(ItemInventory);
}
