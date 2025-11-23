class TSG_CrewComponentClass : ScriptComponentClass
{
}

class TSG_CrewComponent : ScriptComponent
{
	[Attribute(desc: "Crew key to define access control vehicle")]
	string m_sCrewKey;
	[Attribute(defvalue: "true", desc: "Should the turret be closed? Check only on vehicle entity")]
	bool m_bBlockTurret;
	[Attribute(desc: "Is that entity Vehicle's part?")]
	bool m_bIsVehiclePart;
	
	string GetCrewKey()
	{
		return m_sCrewKey;
	}
	
	void SetCrewKey(string NewKey)
	{
		m_sCrewKey = NewKey;
	}
	
	bool IsVehiclePart()
	{
		return m_bIsVehiclePart;
	}
	
	bool IsTurretBlocked()
	{
		return m_bBlockTurret;
	}
}