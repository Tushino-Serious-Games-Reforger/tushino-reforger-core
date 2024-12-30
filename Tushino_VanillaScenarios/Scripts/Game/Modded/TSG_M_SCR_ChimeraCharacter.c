modded class SCR_ChimeraCharacter
{
	override void EOnActivate(IEntity owner)
	{
		super.EOnActivate(owner);
		if (Replication.IsServer())
		{
			GetGame().GetCallqueue().Call(RegisterCharacterInTsgMissionDataManager);
		}
	}

	void RegisterCharacterInTsgMissionDataManager()
	{
		TSG_MissionDataManager.GetInstance().RegisterEntity(this);
	}

	void SCR_ChimeraCharacter(IEntitySource src, IEntity parent)
	{
	}

	void ~SCR_ChimeraCharacter()
	{
	}
}
