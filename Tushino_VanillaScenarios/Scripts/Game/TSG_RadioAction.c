class TSG_RadioAction : TSG_DeathConfirm
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
			foreach(string DObjectiveName : GetDoneObjectives())
			{
				foreach(string FObjectiveName : GetFailObjectives())
				{
					PS_Objective DObjective = PS_Objective.Cast(GetGame().GetWorld().FindEntityByName(DObjectiveName));
					if(!DObjective)
						continue;
					PS_Objective FObjective = PS_Objective.Cast(GetGame().GetWorld().FindEntityByName(FObjectiveName));
					if(!FObjective)
						continue;
					if(DObjective.GetCompleted() == true && FObjective.GetCompleted() == false)
						return false;
				}
			}
		}
		
		return true;
	}
}