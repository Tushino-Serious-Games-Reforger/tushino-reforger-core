/*modded class SCR_GetInUserAction
{
	string VehCrewKey;
	
	override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{	
		TSG_VehicleCrewComponent VehUIInfo = TSG_VehicleCrewComponent.Cast(pOwnerEntity.FindComponent(TSG_VehicleCrewComponent));
		if (!VehUIInfo)
			return;
		
		if (VehUIInfo.IsVehiclePart() == true)
		{
			VehUIInfo = TSG_VehicleCrewComponent.Cast(pOwnerEntity.GetParent().FindComponent(TSG_VehicleCrewComponent));
			if (!VehUIInfo)
				return;
			if (VehUIInfo.IsTurretBlocked() == false)
			{
				VehUIInfo = TSG_VehicleCrewComponent.Cast(pOwnerEntity.FindComponent(TSG_VehicleCrewComponent));
			}
		}
		
		VehCrewKey = VehUIInfo.GetCrewKey();
	}
	
	override bool CanBePerformedScript(IEntity user)
	{
		string UsCrewKey = TSG_CrewComponent.Cast(user.FindComponent(TSG_CrewComponent)).GetCrewKey();
		if (this.GetCompartmentSlot().GetType() != ECompartmentType.CARGO)
		{
			if (VehCrewKey != "" && VehCrewKey != UsCrewKey)
			{
				SetCannotPerformReason("#TYN-UserAction_OnlyCrewAccess");
				return false;
			}
		}
		return true;
	}
}*/

modded class SCR_GetInUserAction
{
	override bool CanBePerformedScript(IEntity user)
	{
		SCR_EditableEntityComponent UserComp = SCR_EditableEntityComponent.Cast(user.FindComponent(SCR_EditableEntityComponent));
		if(!UserComp)
			return false;
		
		SCR_EditableEntityComponent VehComp = SCR_EditableEntityComponent.Cast(this.GetCompartmentSlot().GetVehicle().FindComponent(SCR_EditableEntityComponent));
		if(!VehComp)
			return false;
		
		SCR_EditableEntityUIInfo VehUIInfo = SCR_EditableEntityUIInfo.Cast(VehComp.GetInfo());;
		SCR_EditableEntityUIInfo UserUIInfo = SCR_EditableEntityUIInfo.Cast(UserComp.GetInfo());;
		
		if(this.GetCompartmentSlot().GetType() != ECompartmentType.CARGO)
		{
			if(VehUIInfo.HasEntityLabel(53) == true)
			{
				if(UserUIInfo.HasEntityLabel(271) == false)
				{
					SetCannotPerformReason("#TYN-UserAction_OnlyCrewAccess");
					return false;
				}
			}
		
			if(VehUIInfo.HasEntityLabel(51) == true)
			{
				if(UserUIInfo.HasEntityLabel(270) == false)
				{
					SetCannotPerformReason("#TYN-UserAction_OnlyCrewAccess");
					return false;
				}
			}
		}
		
		return true;
	}
}