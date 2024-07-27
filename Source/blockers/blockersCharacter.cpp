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

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MontageObj(TEXT("/Game/Blockers/Blueprints/Character/shootingmontage.shootingmontage"));
	if (MontageObj.Succeeded()) {
		UE_LOG(LogTemp, Warning, TEXT("Montage OK"));
		MontageToPlay = MontageObj.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> M_blockers(TEXT("/Game/NPC_Character_Four/Materials/M_NPC_Character_Four_Skin1.M_NPC_Character_Four_Skin1"));
	//character1:: /Game/NPC_Character_Four/Materials/M_NPC_Character_Four_Skin1.M_NPC_Character_Four_Skin1
	//character2:: /Game/NPC_Character_Four/Materials/M_NPC_Character_Four_Skin2.M_NPC_Character_Four_Skin2
	//character3:: /Game/NPC_Character_Four/Materials/M_NPC_Character_Four_Skin3.M_NPC_Character_Four_Skin3
	//character4:: /Game/NPC_Character_Four/Materials/M_NPC_Character_Four_Skin4.M_NPC_Character_Four_Skin4
	//if (M_blockers.Succeeded()) {
	//	GetMesh()->SetMaterial(0, M_blockers.Object);
	//}

	static ConstructorHelpers::FObjectFinder<UTexture> TextureAsset1(TEXT("/Game/NPC_Character_Four/Textures/T_NPC_Character_Four_BaseColor_Skin_1.T_NPC_Character_Four_BaseColor_Skin_1"));
	static ConstructorHelpers::FObjectFinder<UTexture> TextureAsset2(TEXT("/Game/NPC_Character_Four/Textures/T_NPC_Character_Four_BaseColor_Skin_2.T_NPC_Character_Four_BaseColor_Skin_2"));
	static ConstructorHelpers::FObjectFinder<UTexture> TextureAsset3(TEXT("/Game/NPC_Character_Four/Textures/T_NPC_Character_Four_BaseColor_Skin_3.T_NPC_Character_Four_BaseColor_Skin_3"));
	static ConstructorHelpers::FObjectFinder<UTexture> TextureAsset4(TEXT("/Game/NPC_Character_Four/Textures/T_NPC_Character_Four_BaseColor_Skin_4.T_NPC_Character_Four_BaseColor_Skin_4"));

	if (TextureAsset1.Succeeded()) TextureAssets[0] = TextureAsset1.Object;
	if (TextureAsset2.Succeeded()) TextureAssets[1] = TextureAsset2.Object;
	if (TextureAsset3.Succeeded()) TextureAssets[2] = TextureAsset3.Object;
	if (TextureAsset4.Succeeded()) TextureAssets[3] = TextureAsset4.Object;

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

	//health = FMath::Clamp<float>(health, 1, MaxHealth); //시간에 따라 줄어들도록 설정.
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

	// 점프 여부 판단
	FVector Start = GetActorLocation();
	FVector End = Start - FVector(0, 0, 110.0f); // 발 밑 110 유닛 아래까지 검사

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	// 라인 트레이스 수행
	bool bLandHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params);
	BIsInAir = !bLandHit;
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
		voxelTexture = TextureAssets[0];
		break;
	case 1:
		Path = TEXT("/Game/NPC_Character_Four/Materials/M_NPC_Character_Four_Skin2.M_NPC_Character_Four_Skin2");
		voxelTexture = TextureAssets[1];
		break;
	case 2:
		Path = TEXT("/Game/NPC_Character_Four/Materials/M_NPC_Character_Four_Skin3.M_NPC_Character_Four_Skin3");
		voxelTexture = TextureAssets[2];
		break;
	case 3:
		Path = TEXT("/Game/NPC_Character_Four/Materials/M_NPC_Character_Four_Skin4.M_NPC_Character_Four_Skin4");
		voxelTexture = TextureAssets[3];
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

void AblockersCharacter::SetMontage(int32 montageSectionNum)
{
	FName StartSectionName;
	float PlayRate = 1.0f;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	switch (montageSectionNum % 4)
	{
	case 0:
		StartSectionName = "drink";
		break;
	case 1:
		StartSectionName = "Slash";
		break;
	case 2:
		StartSectionName = "Swing";
		break;
	case 3:
		StartSectionName = "Shoot";
		break;
	default:
		break;
	}

	if (MontageToPlay && AnimInstance)
	{
		AnimInstance->Montage_Play(MontageToPlay, PlayRate);
		AnimInstance->Montage_JumpToSection(StartSectionName, MontageToPlay);
	}
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
	// 비교할 아이템 이름들을 배열에 저장
	TArray<FString> stackableItems = {
		"ResourceAmethyst",
		"ResourceDiamond",
		"ResourceRuby",
		"ResourceStone",
		"PotionRed",
		"ResourceBullet",
		"BlockAmethyst",
		"BlockDiamond",
		"BlockRuby",
		"BlockStone"
	};

	TArray<FString> toolItems = {
		"SwordStone",
		"SwordAmethyst",
		"SwordRuby",
		"SwordDiamond",
		"PickaxStone",
		"PickaxAmethyst",
		"PickaxRuby",
		"PickaxDiamond",
		"Pistol"
	};

	int32 ItemIndex = GetItemIndex(actor->Name);

	// stackableItem이면 Stack
	if (stackableItems.Contains(actor->Name))
	{
		if (ItemIndex != -1) {
			ItemInventory[ItemIndex]->amount += actor->amount;
		}
		else {
			ItemInventory.Add(actor);
		}
	}
	// toolItem이면 Add
	else if (toolItems.Contains(actor->Name))
	{
		actor->Durability = 100.f;
		ItemInventory.Add(actor);
		UE_LOG(LogTemp, Warning, TEXT("du: %f, am: %d"), actor->Durability, actor->amount);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid Item"));
	}
}

void AblockersCharacter::RemoveFromInventory(const int32 ItemIndex, const int32 number)
{
	int32 lastIndex = ItemInventory.Num() - 1;

	if (ItemIndex != -1) {
		if (ItemInventory[ItemIndex]->amount >= number)
		{
			ItemInventory[ItemIndex]->amount -= number;
		}
		if (ItemInventory[ItemIndex]->amount == 0)
		{
			ItemInventory.RemoveAt(ItemIndex);
			ItemInventory.Shrink();
			UE_LOG(LogTemp, Warning, TEXT("last item removed."));
			SelectedItemIndex = 0;
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Item not found in inventory."));
	}
}

bool AblockersCharacter::CheckItemNum(FString itemName, int32 number)
{
	int32 ItemIndex = GetItemIndex(itemName);
	if (ItemIndex != -1) {
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
	return -1;
}

APickUpItem* AblockersCharacter::GetItemActor() const
{
	if (SelectedItemIndex != -1)
		return ItemInventory[SelectedItemIndex];
	return nullptr;
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
