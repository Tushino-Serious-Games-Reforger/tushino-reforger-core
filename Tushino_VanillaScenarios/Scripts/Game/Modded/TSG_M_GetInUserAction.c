modded class SCR_GetInUserAction
{
	string VehCrewKey;
	
	override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{	
		TSG_CrewComponent comp = TSG_CrewComponent.Cast(pOwnerEntity.FindComponent(TSG_CrewComponent));
		if (!comp)
			return;
		
		
		if (comp.IsVehiclePart() == true)
		{
			comp = TSG_CrewComponent.Cast(pOwnerEntity.GetParent().FindComponent(TSG_CrewComponent));
			if (!comp)
				return;
			if (comp.IsTurretBlocked() == false)
			{
				comp = TSG_CrewComponent.Cast(pOwnerEntity.FindComponent(TSG_CrewComponent));
			}
		}
		
		VehCrewKey = comp.GetCrewKey();
	}
	
	override bool CanBePerformedScript(IEntity user)
	{
		string UsCrewKey = TSG_CrewComponent.Cast(user.FindComponent(TSG_CrewComponent)).GetCrewKey();
		if (super.GetCompartmentSlot().GetType() != ECompartmentType.CARGO)
		{
			if (VehCrewKey != "" && VehCrewKey != UsCrewKey)
			{
				SetCannotPerformReason("#TYN-UserAction_OnlyCrewAccess");
				return false;
			}
		}
		return true;
	}
}