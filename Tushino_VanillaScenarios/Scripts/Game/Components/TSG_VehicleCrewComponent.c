class TSG_VehicleCrewComponentClass : TSG_CrewComponentClass
{

}

class TSG_VehicleCrewComponent : TSG_CrewComponent
{
	[RplProp(), Attribute(defvalue: "false", desc: "Should the turret be closed? Check only on vehicle entity")]
	bool m_bBlockTurret;
	[RplProp(), Attribute(desc: "Is that entity Vehicle's part?")]
	bool m_bIsVehiclePart;
	
	bool IsTurretBlocked()
	{
		return m_bBlockTurret;
	}
	
	bool IsVehiclePart()
	{
		return m_bIsVehiclePart;
	}
}