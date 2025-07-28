class TSG_DeathConfirm : ScriptedUserAction
{
	[Attribute(defvalue: "true", desc: "If false game won't advance")]
	bool m_vGameAdvance;
	[Attribute(desc: "Names of objective which will complate after action")]
	ref array<string> m_aObjectivesToDone;
	[Attribute(desc: "Names of objective which will fail after action")]
	ref array<string> m_aObjectivesToFail;
	
	override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		if(!Replication.IsServer())
			return;
		
		if (!GetGame().InPlayMode())
			return;
		
		foreach(string m_aObjectiveToFail : m_aObjectivesToFail)
		{
			PS_Objective Objective = PS_Objective.Cast(GetGame().GetWorld().FindEntityByName(m_aObjectiveToFail));
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
			PS_Objective objective = PS_Objective.Cast(GetGame().GetWorld().FindEntityByName(m_aObjectiveToDone));
			if(!objective)
				continue;
			objective.SetCompleted(true);
		}
		
		foreach(string m_aObjectiveToFail : m_aObjectivesToFail)
		{
			PS_Objective Objective = PS_Objective.Cast(GetGame().GetWorld().FindEntityByName(m_aObjectiveToFail));
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
	}
	
	array<string> GetDoneObjectives()
	{
		return m_aObjectivesToDone;
	}
}

