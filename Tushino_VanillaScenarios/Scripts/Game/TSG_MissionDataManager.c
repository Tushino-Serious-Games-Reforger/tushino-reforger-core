class TSG_MissionDataManagerClass: ScriptComponentClass
{

};


/**
* Copies functionality of PS_PlayableManager and PS_MissionDataManager to not depend on them
*/
class TSG_MissionDataManager : ScriptComponent
{
	ref map<RplId, IEntity> m_mTrackedEntities = new map<RplId, IEntity>();

	static TSG_MissionDataManager GetInstance()
	{
		BaseGameMode gameMode = GetGame().GetGameMode();
		if (gameMode)
			return TSG_MissionDataManager.Cast(gameMode.FindComponent(TSG_MissionDataManager));
		else
			return null;
	}

	void RegisterEntity(IEntity entity)
	{
		RplComponent component = RplComponent.Cast(entity.FindComponent(RplComponent));
		m_mTrackedEntities.Insert(component.Id(), entity);
		Print("Tracking " + entity + " prefab " + GetPrefabName(entity));
	}

	array<IEntity> FindByPrefabs(array<ResourceName> prefabNames)
	{
		array<IEntity> result = {};
		foreach (IEntity entity: AllEntities())
		{
			if (entity)
			{
				string entityPrefabName = GetPrefabName(entity);
				if (prefabNames.Contains(entityPrefabName))
				{
					result.Insert(entity);
				}
			}
		}
		return result;
	}

	array<IEntity> FindByNames(array<string> entityNames)
	{
		array<IEntity> result = {};
		foreach (IEntity entity: AllEntities())
		{
			if (entity)
			{
				string entityName = entity.GetName();
				if (entityNames.Contains(entityName))
				{
					result.Insert(entity);
				}
			}
		}
		return result;
	}

	array<PS_PlayableComponent> FindPlayables()
	{
		array<PS_PlayableComponent> result = {};
		foreach (IEntity entity: AllEntities())
		{
			if (entity)
			{
				PS_PlayableComponent component = PS_PlayableComponent.Cast(entity.FindComponent(PS_PlayableComponent));
				if (component)
				{
					result.Insert(component);
				}
			}
		}
		return result;
	}

	array<IEntity> AllEntities()
	{
		array<IEntity> result = {};
		array<RplId> removedEntities = {};
		foreach (RplId id, IEntity entity: m_mTrackedEntities)
		{
			if (entity)
			{
				result.Insert(entity);
			}
			else
			{
				removedEntities.Insert(id);
			}
		}
		foreach (RplId removedId: removedEntities)
		{
			m_mTrackedEntities.Remove(removedId);
		}
		return result;
	}

	ResourceName GetPrefabName(IEntity entity)
	{
		ResourceName entityPrefabName = entity.GetPrefabData().GetPrefabName();
		if (!entityPrefabName)
		{
			BaseContainer ancestor = entity.GetPrefabData().GetPrefab().GetAncestor();
			if (ancestor)
			{
				entityPrefabName = ancestor.GetResourceName();
			}
		}
		return entityPrefabName;
	}
}
