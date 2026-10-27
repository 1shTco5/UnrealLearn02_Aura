// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "AuraAttributeSet.generated.h"

USTRUCT()
struct FEffectProperty {
    GENERATED_BODY()

    FEffectProperty() = default;

    FGameplayEffectContextHandle EffectContextHandle;

    UPROPERTY()
    UAbilitySystemComponent* TargetASC = nullptr;
    UPROPERTY()
    AActor* TargetAvatarActor = nullptr;
    UPROPERTY()
    AController* TargetController = nullptr;
    UPROPERTY()
    ACharacter* TargetCharacter = nullptr;

    UPROPERTY()
    UAbilitySystemComponent* SourceASC = nullptr;
    UPROPERTY()
    AActor* SourceAvatarActor = nullptr;
    UPROPERTY()
    AController* SourceController = nullptr;
    UPROPERTY()
    ACharacter* SourceCharacter = nullptr;
};

/**
 * 
 */
UCLASS()
class AURA_API UAuraAttributeSet : public UAttributeSet {
    GENERATED_BODY()

public:
    UAuraAttributeSet();

    // 注册需要在服务器与客户端之间复制的 Attribute。
    // 每个 Attribute 都需要在实现文件中通过 DOREPLIFETIME_CONDITION_NOTIFY 显式注册。
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
    virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

    /*
     * Primary Attributes
     */

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Strength, Category = "Primary Attributes")
    FGameplayAttributeData Strength;
    ATTRIBUTE_ACCESSORS_BASIC(UAuraAttributeSet, Strength)

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Intelligence, Category = "Primary Attributes")
    FGameplayAttributeData Intelligence;
    ATTRIBUTE_ACCESSORS_BASIC(UAuraAttributeSet, Intelligence)

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Resilience, Category = "Primary Attributes")
    FGameplayAttributeData Resilience;
    ATTRIBUTE_ACCESSORS_BASIC(UAuraAttributeSet, Resilience)

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Vigor, Category = "Primary Attributes")
    FGameplayAttributeData Vigor;
    ATTRIBUTE_ACCESSORS_BASIC(UAuraAttributeSet, Vigor)

    /*
     * Vital Attributes
     */

    // FGameplayAttributeData 不是简单的 float，它由 GAS 管理基础值和当前值。
    // Gameplay Effect 应通过 GAS 修改这些属性，而不是直接赋值。
    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital Attributes")
    FGameplayAttributeData Health;
    ATTRIBUTE_ACCESSORS_BASIC(UAuraAttributeSet, Health)

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Vital Attributes")
    FGameplayAttributeData MaxHealth;
    ATTRIBUTE_ACCESSORS_BASIC(UAuraAttributeSet, MaxHealth)

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Vital Attributes")
    FGameplayAttributeData Mana;
    ATTRIBUTE_ACCESSORS_BASIC(UAuraAttributeSet, Mana)

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Vital Attributes")
    FGameplayAttributeData MaxMana;
    ATTRIBUTE_ACCESSORS_BASIC(UAuraAttributeSet, MaxMana)

    // RepNotify 在客户端收到服务器同步的新值后执行。
    // 函数内部需要调用 GAMEPLAYATTRIBUTE_REPNOTIFY，
    // 才能将网络变化正确通知给 GAS 及其属性变化委托。
    // OldHealth 是复制前的旧值，可用于计算属性变化。
    UFUNCTION()
    void OnRep_Strength(const FGameplayAttributeData& OldStrength) const;

    UFUNCTION()
    void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const;

    UFUNCTION()
    void OnRep_Resilience(const FGameplayAttributeData& OldResilience) const;

    UFUNCTION()
    void OnRep_Vigor(const FGameplayAttributeData& OldVigor) const;

    UFUNCTION()
    void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

    UFUNCTION()
    void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

    UFUNCTION()
    void OnRep_Mana(const FGameplayAttributeData& OldMana) const;

    UFUNCTION()
    void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;

private:
    void SetEffectProperty(const FGameplayEffectModCallbackData& Data, FEffectProperty& Props) const;
};