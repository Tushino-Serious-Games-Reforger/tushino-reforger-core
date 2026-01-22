class TSG_FactionLossesConstraintClass: TSG_BasePeriodicCheckClass
{

}


class TSG_FactionLossesConstraint: TSG_BasePeriodicCheck
{
	[Attribute(defvalue: "", desc: "Faction key")]
	string m_sFactionKey;
	[Attribute(desc: "Additional faction keys")]
	ref array<string> m_aAdditionalFactionKeys;
	[Attribute(defvalue: "0", desc: "Minimum number of alive characters")]
	int m_iMinNumberOfAliveCharacters;
	[Attribute(desc: "Names of objectives to complete")]
	ref array<string> m_aNamesOfObjectives;

	override bool PerformCheck(PS_GameModeCoop gameModeCoop)
	{
		int nAlive = CalculateNumberOfAliveCharacters();
		array<PS_Objective> objectives = TSG_ObjectiveHelper.FindObjectives(this, m_aNamesOfObjectives);
		Print(m_sFactionKey + ": " + nAlive);
		if (nAlive < m_iMinNumberOfAliveCharacters)
		{
			TSG_ObjectiveHelper.CompleteObjectives(objectives);
		}
		return nAlive < m_iMinNumberOfAliveCharacters;
	}

	int CalculateNumberOfAliveCharacters()
	{
		PS_PlayableManager manager = PS_PlayableManager.GetInstance();
		//TSG_MissionDataManager manager = TSG_MissionDataManager.GetInstance();
		int nAlive = 0;
		array<PS_PlayableContainer> playables = manager.GetPlayablesSorted();
		foreach(PS_PlayableContainer container : playables)
		{
			PS_PlayableComponent playable = container.GetPlayableComponent();
			FactionAffiliationComponent factionAffiliationComponent = playable.GetFactionAffiliationComponent();
			SCR_Faction faction = SCR_Faction.Cast(factionAffiliationComponent.GetDefaultAffiliatedFaction());
			string unitFaction = faction.GetFactionKey();
			if ((unitFaction == m_sFactionKey) || m_aAdditionalFactionKeys.Contains(unitFaction))
			{
				SCR_CharacterDamageManagerComponent characterDamageManagerComponent = playable.GetCharacterDamageManagerComponent();
				if (characterDamageManagerComponent.GetState() != EDamageState.DESTROYED)
				{
					nAlive++;
				}
			}
		}
		return nAlive;
	}
}
