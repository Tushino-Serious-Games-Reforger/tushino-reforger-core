class TSG_MissionDataManagerClass: ScriptComponentClass
{

};


/**
* Copies functionality PS_MissionDataManager to not depend on them, depends on PS_PlayableManager but this one looks tricky though
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
		if (m_mTrackedEntities.Contains(component.Id()))
		{
			Print("Double tracking " + entity + " prefab " + GetPrefabName(entity));
		}
		else
		{
			Print("Tracking " + entity + " prefab " + GetPrefabName(entity));
		}
		m_mTrackedEntities.Insert(component.Id(), entity);
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

	array<IEntity> AllEntities()
	{
		array<IEntity> result = {};

		PS_PlayableManager manager = PS_PlayableManager.GetInstance();
		foreach (PS_PlayableComponent playable : manager.GetPlayablesSorted())
		{
			IEntity owner = playable.GetOwner();
			RplComponent component = RplComponent.Cast(owner.FindComponent(RplComponent));
			if (!m_mTrackedEntities.Contains(component.Id()))
			{
				result.Insert(owner);
			}
		}

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
