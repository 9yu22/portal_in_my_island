// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpItem.h"
#include "Kismet/GameplayStatics.h"
#include "../blockersCharacter.h"
#include "../Network/SGameInstance.h"

// Sets default values
APickUpItem::APickUpItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//컴포넌트에 넣을 디폴트 값들을 설정함
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->SetupAttachment(RootComponent);
	RotationRate = FRotator(0.0f, 90.0f, 0.0f);
	Speed = 1.0f;

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	BoxCollider->SetGenerateOverlapEvents(true);
	BoxCollider->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &APickUpItem::OnOverlapBegin);
	BoxCollider->SetupAttachment(RootComponent);

	BoxCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxCollider->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	BoxCollider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
}

// Called when the game starts or when spawned
void APickUpItem::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APickUpItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorLocalRotation(RotationRate * DeltaTime * Speed);
}

void APickUpItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// OtherActor가 자신이 아닌지, NULL이 아닌지 체크한다
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr)) // 괄호 수정
	{
		OnInteract();
		
	}

}

void APickUpItem::OnInteract()
{
	FString pickup = FString::Printf(TEXT("Picked up: %s"), *Name);
	UGameplayStatics::PlaySoundAtLocation(this, PickUpSound, GetActorLocation());
	//GEngine->AddOnScreenDebugMessage(1, 5, FColor::White, pickup);
	//Destroy();

	AblockersCharacter* player = Cast<AblockersCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	if (player)
	{
		Show(false);
		player->AddToInventory(this);

		//USGameInstance* instance = USGameInstance::GetMyInstance(this);
		//AResourceItem* pickup_item = Cast<AResourceItem>(this);
		////instance->items.Remove(pickup_item);

		//CS_REMOVE_ITEM_PACKET remove_item;

		//remove_item.size = sizeof(CS_REMOVE_ITEM_PACKET);
		//remove_item.type = sizeof(CS_REMOVE_ITEM);
		//remove_item.id = pickup_item->id;

		//int BytesSent = 0;

		//instance->Socket->Send((uint8*)&remove_item, sizeof(remove_item), BytesSent);
	}


	GEngine->AddOnScreenDebugMessage(1, 3, FColor::Red, *pickup);
}

void APickUpItem::Show(bool visible)
{
	ECollisionEnabled::Type collision = visible ?
		ECollisionEnabled::QueryAndPhysics :
		ECollisionEnabled::NoCollision;

	SetActorTickEnabled(visible);
	ItemMesh->SetVisibility(visible);
	ItemMesh->SetCollisionEnabled(collision);
	BoxCollider->SetCollisionEnabled(collision);
}
