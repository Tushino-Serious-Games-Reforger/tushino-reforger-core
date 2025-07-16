class TSG_DeathConfirm : ScriptedUserAction
{
	[Attribute(defvalue: "true", desc: "If false game won't advance")]
	bool m_vGameAdvance;
	[Attribute(desc: "Names of objective which will complate after action")]
	ref array<string> m_aObjectivesToDone;
	[Attribute(desc: "Names of objective which will fail after action")]
	ref array<string> m_aObjectivesToFail;
	
	PS_Objective Objective
	bool WasUsed = false;
	
	override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		if(!Replication.IsServer())
			return;
		
		if (!GetGame().InPlayMode())
			return;
		
		foreach(string m_aObjectiveToFail : m_aObjectivesToFail)
		{
			Objective = PS_Objective.Cast(GetGame().GetWorld().FindEntityByName(m_aObjectiveToFail));
			if(!Objective)
				continue;
			Objective.SetCompleted(true);
		}
	}
	
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		if(!Replication.IsServer())
			return;
		
		foreach(string m_aObjectiveToDone : m_aObjectivesToDone)
		{
			PS_Objective objective1 = PS_Objective.Cast(GetGame().GetWorld().FindEntityByName(m_aObjectiveToDone));
			if(!objective1)
				continue;
			objective1.SetCompleted(true);
		}
		
		foreach(string m_aObjectiveToFail : m_aObjectivesToFail)
		{
			Objective = PS_Objective.Cast(GetGame().GetWorld().FindEntityByName(m_aObjectiveToFail));
			if(!Objective)
				continue;
			Objective.SetCompleted(false);
		}
		
		if (m_vGameAdvance == true)
		{
			PS_GameModeCoop gameModeCoop = PS_GameModeCoop.Cast(GetGame().GetGameMode());
			if(!gameModeCoop)
			{
				return;			
			}
			gameModeCoop.AdvanceGameState(SCR_EGameModeState.GAME);
		}
		WasUsed = true;
	}
}

