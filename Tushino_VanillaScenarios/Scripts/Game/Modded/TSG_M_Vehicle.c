modded class Vehicle
{
	override void EOnActivate(IEntity owner)
	{
		super.EOnActivate(owner);
		if (Replication.IsServer())
		{
			GetGame().GetCallqueue().Call(RegisterVehicleInTsgMissionDataManager);
		}
	}

	void RegisterVehicleInTsgMissionDataManager()
	{
		TSG_MissionDataManager.GetInstance().RegisterEntity(this);
	}

	void Vehicle(IEntitySource src, IEntity parent)
	{
	}

	void ~Vehicle()
	{
	}
}
