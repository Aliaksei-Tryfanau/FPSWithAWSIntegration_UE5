// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portal/Dashboard/DashboardOverlay.h"
#include "Components/Button.h"
#include "UI/Portal/Dashboard/GamePage.h"
#include "UI/Portal/Dashboard/CareerPage.h"
#include "UI/Portal/Dashboard/LeaderboardPage.h"
#include "Components/WidgetSwitcher.h"
#include "UI/GameStats/GameStatsManager.h"

void UDashboardOverlay::NativeConstruct()
{
	Super::NativeConstruct();
	
	Button_Game->OnClicked.AddDynamic(this, &UDashboardOverlay::ShowGamePage);
	Button_Career->OnClicked.AddDynamic(this, &UDashboardOverlay::ShowCareerPage);
	Button_Leaderboard->OnClicked.AddDynamic(this, &UDashboardOverlay::ShowLeaderboardPage);
	GameStatsManager = NewObject<UGameStatsManager>(this, GameStatsManagerClass);
	GameStatsManager->OnRetrieveMatchStatsResponseReceived.AddDynamic(CareerPage, &UCareerPage::OnRetrieveMatchStats);
	GameStatsManager->RetrieveMatchStatsStatusMessage.AddDynamic(CareerPage, &UCareerPage::SetStatusMessage);
	GameStatsManager->OnRetrieveLeaderboard.AddDynamic(LeaderboardPage, &ULeaderboardPage::PopulateLeaderboard);
	GameStatsManager->RetrieveLeaderboardStatusMessage.AddDynamic(LeaderboardPage, &ULeaderboardPage::SetStatusMessage);
	ShowGamePage();
}

void UDashboardOverlay::ShowGamePage()
{
	DisableButton(Button_Game);
	WidgetSwitcher->SetActiveWidget(GamePage);
}

void UDashboardOverlay::ShowCareerPage()
{
	DisableButton(Button_Career);
	WidgetSwitcher->SetActiveWidget(CareerPage);
	GameStatsManager->RetrieveMatchStats();
}

void UDashboardOverlay::ShowLeaderboardPage()
{
	DisableButton(Button_Leaderboard);
	WidgetSwitcher->SetActiveWidget(LeaderboardPage);
	GameStatsManager->RetrieveLeaderboard();
}

void UDashboardOverlay::DisableButton(UButton* Button) const
{
	Button_Game->SetIsEnabled(true);
	Button_Career->SetIsEnabled(true);
	Button_Leaderboard->SetIsEnabled(true);
	Button->SetIsEnabled(false);
}
