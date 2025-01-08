class TSG_MissionDataAwareClass: ScriptComponentClass
{

}


/**
* Add this component to prefab to register it into TSG_MissionDataManager manually
* (you don't need to do that for vehicles/characters, only maybe for some destructable structures or props)
*/
class TSG_MissionDataAware: ScriptComponent
{
	IEntity m_owner;
	override void OnPostInit(IEntity owner)
	{
		if (Replication.IsServer())
		{
			m_owner = owner;
			GetGame().GetCallqueue().CallLater(RegisterOwner);
		}
	}

	void RegisterOwner()
	{
		TSG_MissionDataManager.GetInstance().RegisterEntity(m_owner);
	}
}
