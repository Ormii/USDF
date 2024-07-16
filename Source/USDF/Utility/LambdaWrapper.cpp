// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/LambdaWrapper.h"

void ULambdaWrapper::OnMoveCompleteDispatch(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	OnMoveCompleteCallFn(RequestID,Result); 
}
