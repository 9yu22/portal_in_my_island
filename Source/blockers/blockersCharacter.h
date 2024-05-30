// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "../blockers/Public/PickUpItem.h"
#include "Logging/LogMacros.h"
#include "Delegates/DelegateCombinations.h"
#include "GameFramework/Character.h"
#include "blockersCharacter.generated.h"


class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateInventoryDelegate, const TArray<APickUpItem*>&, InventoryItems);

UCLASS(config=Game)
class AblockersCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	/**Inventory Component */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true")) //category
	class UInventoryComponent* Inventory;

	/** HealthBar Component */
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	//class UWidgetComponent* HealthWidgetComp;

public:
	AblockersCharacter();

	virtual void Tick(float DeltaTime) override;
	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();		

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UPROPERTY(EditAnywhere)
	class UBoxComponent* boxComp;

	//총구위치
	UPROPERTY(EditAnywhere)
	class UArrowComponent* firePosition;

	//총알 블루프린트
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABullet> bulletFactory;

	int8 id = -1;
	TArray<AblockersCharacter*> Players;
	bool loginOk = false;

	//bullet
private:
	void Fire();

	//Respawn
protected:
	virtual void Destroyed();
	void CallRestartPlayer();

	//HP & HealthBar
public:

	float GetHealth() const { return health; }
	void SetHealth(float val) { health = val; }

	float GetMaxHealth() const { return MaxHealth; }
	void SetMaxHealth(float val) { health = val; }


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
	float health;

	UFUNCTION(BlueprintCallable, Category = "Items")
	void UseItem(class UItem* Item);

	float MaxHealth = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "bulletNum")
	int bulletNum = 100;

	/*pickup inventory*/
public:
	TArray<APickUpItem*> _inventory;

	void AddToInventory(APickUpItem* actor);

	UFUNCTION(BlueprintCallable)
	void UpdateInventory();

	UPROPERTY(BlueprintAssignable, Category = "Pickup")
	FUpdateInventoryDelegate OnUpdateInventory;

};

