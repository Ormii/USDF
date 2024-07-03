// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/USDFWidgetComponent.h"
#include "UI/USDFUserWidget.h"
void UUSDFWidgetComponent::InitWidget()
{
	Super::InitWidget();

	UUSDFUserWidget* UserWidget = Cast<UUSDFUserWidget>(GetWidget());
	if (UserWidget)
	{
		UserWidget->SetOwner(GetOwner());
	}
}
