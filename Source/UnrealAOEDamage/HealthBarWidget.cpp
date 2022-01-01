// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBarWidget.h"

float UHealthBarWidget::GetHealthAsRatio()
{
	if(HealthInterface)
		return HealthInterface->GetHealthAsRatio();

	return .5f;
}
