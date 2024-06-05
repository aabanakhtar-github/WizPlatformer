#include "Components.h" 
#include "Util.h" 
#include "PhysicsSystem.h"

void PhysicsSystem::update(ECS::Scene &scene, float delta) {
	ECS::SceneView<PhysicsBodyComponent> physics_bodies(scene);

	for (auto ID : physics_bodies.GetEntities())
	{
		auto& component = scene.getComponent<PhysicsBodyComponent>(ID);
		if (component.SimulatesPhysics)
		{
			component.Acceleration.y += m_Gravity;
		}	

		component.Velocity +=  { component.Acceleration.x * delta, component.Acceleration.y * delta};
		component.BoundingBox.x += static_cast<int>(component.Velocity.x);

		for (auto other : physics_bodies.GetEntities())
		{
			if (ID == other) continue;
			auto& other_component = scene.getComponent<PhysicsBodyComponent>(other);
			if (isColliding(other_component.BoundingBox, component.BoundingBox))
			{
				if (component.Velocity.x > 0)
				{
					component.BoundingBox.x = other_component.BoundingBox.x - component.BoundingBox.w;
				}
				else if (component.Velocity.x < 0)
				{
					component.BoundingBox.x = other_component.BoundingBox.x + other_component.BoundingBox.w;
				}

				component.Velocity.x = 0;
			}
		}	
		
		component.BoundingBox.y += static_cast<int>(component.Velocity.y);
		for (auto other : physics_bodies.GetEntities())
		{
			if (ID == other) continue;
			auto& other_component = scene.getComponent<PhysicsBodyComponent>(other);
			if (isColliding(other_component.BoundingBox, component.BoundingBox))
			{
				if (component.Velocity.y > 0)
				{
					component.BoundingBox.y = other_component.BoundingBox.y - component.BoundingBox.h;
				}
				else if (component.Velocity.y < 0)
				{
					component.BoundingBox.y = other_component.BoundingBox.y + other_component.BoundingBox.h;
				}

				component.Velocity.y = 0;
			}
		}		

		component.Acceleration = { 0, 0 };
	}
}

void PhysicsSystem::ResolveCollisions(PhysicsBodyComponent& component, ECS::SceneView<PhysicsBodyComponent>& scene_view, ECS::Scene& scene)
{
	for (auto ID : scene_view.GetEntities())
	{
		PhysicsBodyComponent& other = scene.getComponent<PhysicsBodyComponent>(ID);

		if (component != other)
		{
			// move horizontally first 
			component.BoundingBox.x += component.Velocity.x;
			if (isColliding(other.BoundingBox, component.BoundingBox))
			{
				if (component.Velocity.x > 0)
				{
					// hit left side of something 
					component.BoundingBox.x = other.BoundingBox.x - component.BoundingBox.w;
				}	
				else if (component.Velocity.x < 0)
				{
					// hit right side
					component.BoundingBox.x = other.BoundingBox.x + other.BoundingBox.w;
				}

				component.Velocity.x = 0;
				component.Acceleration.x = 0;
			}

			component.BoundingBox.y += component.Velocity.y;
			if (isColliding(other.BoundingBox, component.BoundingBox))
			{
				if (component.Velocity.y > 0)
				{
					// hit the top side
					component.BoundingBox.y = other.BoundingBox.y - component.BoundingBox.h;
				} 
				else if (component.Velocity.y < 0)
				{
					// hit the bottom side 
					component.BoundingBox.y = other.BoundingBox.y + other.BoundingBox.h;
				}

				component.Velocity.y = 0;
				component.Acceleration.y = 0;
			}

		}
	}
}
