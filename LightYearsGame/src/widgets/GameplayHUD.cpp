#include "widgets/GameplayHUD.h"
#include "player/Player.h"
#include "player/PlayerManager.h"
#include "player/PlayerSpaceship.h"
#include "framework/Actor.h"

namespace ly
{
	GameplayHUD::GameplayHUD()
		: mFramerateText{"Framerate: "},
		mPlayerHealthBar{},
		mHealthyHealthBarColor{128, 255, 128, 255},
		mCriticalHealthBarColor{255, 0, 0, 255},
		mCriticalThreshold{0.3}
	{
		mFramerateText.SetTextSize(25);
		
	}
	
	void GameplayHUD::Draw(sf::RenderWindow& windowRef)
	{
		mFramerateText.NativeDraw(windowRef);
		mPlayerHealthBar.NativeDraw(windowRef);
	}

	void GameplayHUD::Tick(float deltaTime)
	{
		int frameRate = int(1 / deltaTime);
		std::string frameRateStr = "Framerate: " + std::to_string(frameRate);
		mFramerateText.SetString(frameRateStr);
	}

	void GameplayHUD::Init(const sf::RenderWindow& windowRef)
	{
		auto windowSize = windowRef.getSize();
		mPlayerHealthBar.SetWidgetLocation(sf::Vector2f{0.f, windowSize.y-50.f});

		RefreshHealthBar();
		
		mPlayerHealthBar.UpdateValue(100.f, 100.f);
	}

	void GameplayHUD::PlayerHealthUpdated(float amt, float currentHealth, float maxHealth)
	{
		mPlayerHealthBar.UpdateValue(currentHealth, maxHealth);
		if (currentHealth / maxHealth <= mCriticalThreshold)
		{
			mPlayerHealthBar.SetForegroundColor(mCriticalHealthBarColor);
		}
		else
		{
			mPlayerHealthBar.SetForegroundColor(mHealthyHealthBarColor);
		}
	}

	void GameplayHUD::RefreshHealthBar()
	{
		Player* player = PlayerManager::Get().GetPlayer();
		if (player && !player->GetCurrentSpaceship().expired())
		{
			weak<PlayerSpaceship> playerSpaceship = player->GetCurrentSpaceship();
			playerSpaceship.lock()->onActorDestroyed.BindAction(GetWeakRef(), &GameplayHUD::PlayerSpaceshipDestroyed);
			HealthComponent& healthComp = player->GetCurrentSpaceship().lock()->GetHealthComp();
			healthComp.onHealthChanged.BindAction(GetWeakRef(), &GameplayHUD::PlayerHealthUpdated);
			mPlayerHealthBar.UpdateValue(healthComp.GetHealth(), healthComp.GetMaxHealth());
		}
	}

	void GameplayHUD::PlayerSpaceshipDestroyed(Actor* actor)
	{
		RefreshHealthBar();
	}
}
