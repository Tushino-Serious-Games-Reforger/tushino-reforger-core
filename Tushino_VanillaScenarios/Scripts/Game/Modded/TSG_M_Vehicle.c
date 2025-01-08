modded class Vehicle
{
	void Vehicle(IEntitySource src, IEntity parent)
	{
		if (Replication.IsServer())
		{
			GetGame().GetCallqueue().Call(RegisterVehicleInTsgMissionDataManager);
		}
	}

	void RegisterVehicleInTsgMissionDataManager()
	{
		TSG_MissionDataManager.GetInstance().RegisterEntity(this);
	}
}
