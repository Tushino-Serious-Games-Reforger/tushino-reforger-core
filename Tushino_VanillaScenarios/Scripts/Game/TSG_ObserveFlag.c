class TSG_ObserveFlagClass: GenericEntityClass
{

}


class TSG_ObserveFlag: GenericEntity
{
	[Attribute(desc: "Which flag to look at")]
	string m_sFlagName;
	[Attribute(desc: "How often to look on flag", defvalue: "10")]
	int m_iFlagPollingTimeoutSeconds;
	[Attribute(desc: "What faction to expect")]
	string m_sFactionKey;
	[Attribute(desc: "Names of objectives to complete")]
	ref array<string> m_aNamesOfObjectives;
	ref array<PS_Objective> m_aObjectives = {};
	IEntity m_flag;
	SCR_FlagComponent m_FlagComponent;
	FactionManager m_FactionManager;
	PS_GameModeCoop m_game;

	override void EOnActivate(IEntity owner)
	{
		if (Replication.IsServer())
		{
			m_aObjectives = TSG_ObjectiveHelper.FindObjectives(this, m_aNamesOfObjectives);
			m_flag = GetWorld().FindEntityByName(m_sFlagName);
			m_FlagComponent = SCR_FlagComponent.Cast(m_flag.FindComponent(SCR_FlagComponent));
			m_FactionManager = GetGame().GetFactionManager();
			m_game = PS_GameModeCoop.Cast(GetGame().GetGameMode());
			int delay = m_iFlagPollingTimeoutSeconds * 1000;
			GetGame().GetCallqueue().CallLater(fn: CheckFlag, delay: delay, repeat: true);
		}
	}

	void CheckFlag()
	{
		SCR_EGameModeState gameState = m_game.GetState();
		if ((gameState == SCR_EGameModeState.GAME))
		{
			if (m_FlagComponent.m_sFactionKey == m_sFactionKey)
			{
				TSG_ObjectiveHelper.CompleteObjectives(m_aObjectives);
			} else
			{
				TSG_ObjectiveHelper.FailObjectives(m_aObjectives);
			}
		} else if (gameState == SCR_EGameModeState.DEBRIEFING)
		{
			if (m_FlagComponent.m_sFactionKey == m_sFactionKey)
			{
				TSG_ObjectiveHelper.CompleteObjectives(m_aObjectives);
			} else
			{
				TSG_ObjectiveHelper.FailObjectives(m_aObjectives);
			}
			GetGame().GetCallqueue().Remove(CheckFlag);
		}
	}
}
