// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraCharacterBase.h"

#include "AbilitySystemComponent.h"

AAuraCharacterBase::AAuraCharacterBase() {
    PrimaryActorTick.bCanEverTick = false;

    // Weapon
    Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
    Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
    Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const {
    return AbilitySystemComponent;
}

UAttributeSet* AAuraCharacterBase::GetAttributeSet() const {
    return AttributeSet;
}

void AAuraCharacterBase::BeginPlay() {
    Super::BeginPlay();
}

void AAuraCharacterBase::InitAbilityActorInfo() {
}

void AAuraCharacterBase::InitializePrimaryAttributes() {
    check(IsValid(GetAbilitySystemComponent()));
    check(DefaultPrimaryAttribute);
    FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
    FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(
        DefaultPrimaryAttribute, 1.f, ContextHandle);
    GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}