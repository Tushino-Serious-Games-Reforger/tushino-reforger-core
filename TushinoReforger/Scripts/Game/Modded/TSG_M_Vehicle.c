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
		TSG_MissionDataManager manager = TSG_MissionDataManager.GetInstance();
		if (manager != null) 
		{
			manager.RegisterEntity(this);
		} 
		else
		{
			Print("No TSG_MissionDataManager found");
		}
	}
}
