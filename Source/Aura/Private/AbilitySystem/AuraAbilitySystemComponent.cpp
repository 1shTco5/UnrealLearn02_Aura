// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemComponent.h"

void UAuraAbilitySystemComponent::OnAbilityActorInfoSet() {
    OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::OnEffectApplied);
}

void UAuraAbilitySystemComponent::OnEffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
                                                  const FGameplayEffectSpec& EffectSpec,
                                                  FActiveGameplayEffectHandle ActiveEffectHandle) {
    FGameplayTagContainer TagContainer;
    EffectSpec.GetAllAssetTags(TagContainer);

    for (const FGameplayTag& Tag : TagContainer) {
        //TODO: Broadcast the Tag to the WidgetController
    }
}