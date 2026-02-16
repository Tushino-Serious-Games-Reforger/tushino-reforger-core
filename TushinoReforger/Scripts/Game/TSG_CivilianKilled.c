class TSG_CivilianKilledClass : ScriptComponentClass
{
}

class TSG_CivilianKilled : ScriptComponent
{
	[Attribute()]
	ref array<FactionKey> FactionsToInspect;
	[Attribute()]
	int MaxNumsOfKills;
	[Attribute()]
	ref array<string> NamesOfObjectives;
	
	int KillCounter;
	PS_GameModeCoop GameMode;
	
	override void OnPostInit(IEntity owner)
	{
		super.OnPostInit(owner);
		SetEventMask(owner, EntityEvent.INIT);
	}
	
	override void EOnInit(IEntity owner)
	{
		super.EOnInit(owner);
		GameMode = PS_GameModeCoop.Cast(GetGame().GetGameMode());
		if(!GameMode)
			return;
		
		if(!GetGame().InPlayMode())
			return;
		
		GameMode.GetOnControllableDestroyed().Remove(CheckKill);
		GameMode.GetOnControllableDestroyed().Insert(CheckKill);
	}
	
	void CheckKill(notnull SCR_InstigatorContextData instigatorContextData)
	{
		
		IEntity victim = instigatorContextData.GetVictimEntity();
		IEntity killer = instigatorContextData.GetKillerEntity();
		if(!killer)
			return;
		
		Faction victimFaction = FactionAffiliationComponent.Cast(victim.FindComponent(FactionAffiliationComponent)).GetAffiliatedFaction();
		if(!victimFaction)
			return;
		
		Faction killerFaction = FactionAffiliationComponent.Cast(killer.FindComponent(FactionAffiliationComponent)).GetAffiliatedFaction();
		if(!killerFaction)
			return;
		
		foreach (FactionKey FactionToInspect : FactionsToInspect)
		{
			if(SCR_Faction.Cast(victimFaction).IsMilitary())
				return;
			if(killerFaction.GetFactionKey() != FactionToInspect)
				continue;
			
			IncreaseCounter();
		}
	}
	
	void IncreaseCounter()
	{
		KillCounter += 1;
		
		if(KillCounter >= MaxNumsOfKills && MaxNumsOfKills != 0)
		{
			foreach(string NameObjective : NamesOfObjectives)
			{
				PS_Objective Objective = PS_Objective.Cast(GetGame().GetWorld().FindEntityByName(NameObjective));
				if (!Objective)
					continue;
				
				foreach (FactionKey FactionToInspect : FactionsToInspect)
				{
					if(FactionToInspect == Objective.GetFactionKey())
						Objective.SetCompleted(false);
					else
						Objective.SetCompleted(true)
				}
			}
			
			GameMode.AdvanceGameState(SCR_EGameModeState.GAME);
		}
	}
}