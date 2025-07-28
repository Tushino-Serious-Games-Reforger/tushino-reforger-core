class TSG_DeathConfirmAction : TSG_DeathConfirm
{
	[Attribute(defvalue: "USSR", desc: "Which faction can see action")]
	FactionKey m_vFactionKey;
	[Attribute(desc: "Should become unavailable after use?", defvalue: "true")]
	bool m_vActionEnd
	//------------------------------------------------------------------------------------------------
	override bool CanBeShownScript(IEntity user)
	{
		return CanBePerformedScript(user);
	}
	
	//------------------------------------------------------------------------------------------------
	override bool CanBePerformedScript(IEntity user)
	{
		ChimeraCharacter char = ChimeraCharacter.Cast(GetOwner());
		if (!char)
			return false;
		
		// Disallow action when alive
		CharacterControllerComponent contr = char.GetCharacterController();
		if (!contr)
			return false;
		
		if (contr.GetLifeState() != ECharacterLifeState.DEAD)
			return false;

		// Disallow action when in vehicle
		IEntity userVeh = CompartmentAccessComponent.GetVehicleIn(char);
		if (userVeh)
			return false;
		
		// Disallow action when wrong faction
		if (!m_vFactionKey)
			return false;
		
		FactionAffiliationComponent factionAffiliationComp = FactionAffiliationComponent.Cast(user.FindComponent(FactionAffiliationComponent));
		if (!factionAffiliationComp)
				return false;
		
		FactionManager factionManager = GetGame().GetFactionManager();
		if (!factionManager)
				return false;
		
		SCR_Faction faction = SCR_Faction.Cast(factionAffiliationComp.GetAffiliatedFaction());
		if (faction != factionManager.GetFactionByKey(m_vFactionKey)) //check faction via factions
			return false;
		
		if (m_vActionEnd == true)
		{
			foreach(string ObjectiveName : GetDoneObjectives())
			{
				PS_Objective Objective = PS_Objective.Cast(GetGame().GetWorld().FindEntityByName(ObjectiveName));
				if (Objective.GetCompleted() == true)
					return false;
			}
		}
		
		return true;
	}
}