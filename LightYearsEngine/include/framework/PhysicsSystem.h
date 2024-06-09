#pragma once
#include <box2D/b2_world.h>
#include "framework/Core.h"

namespace ly
{
	class Actor;
	class PhysicsContactListener : public b2ContactListener
	{
		virtual void BeginContact(b2Contact* contact) override;
		virtual void EndContact(b2Contact* contact) override;
	};
	class PhysicsSystem
	{
	public:
		static PhysicsSystem& Get();
		void Step(float deltaTime);
		b2Body* AddListener(Actor* listener);
		void RemoveListener(b2Body* bodyToRemove);
	
		float GetPhysicsScale() const { return mPhysicsScale; }

		static void CleanUp();
	protected:
		PhysicsSystem();

	private:
		void ProcessPendingListeners();
		static unique<PhysicsSystem> physicsSystem;
		b2World mPhysicsWorld;
		float mPhysicsScale;
		int mVelocityIterations;
		int mPositionIterations;

		PhysicsContactListener mContactListener;

		Set<b2Body*> mPendingRemoveListeners;
	};
}