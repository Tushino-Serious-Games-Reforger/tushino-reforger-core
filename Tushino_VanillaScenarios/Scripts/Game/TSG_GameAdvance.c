class TSG_GameAdvanceClass: TSG_BasePeriodicCheckClass
{

}

/**
* Advance game once specified number of objectives are complete
*/
class TSG_GameAdvance: TSG_BasePeriodicCheck
{
	[Attribute(desc: "Number of objectives to complete in order to advance game", defvalue: "1")]
	int m_iNumberOfObjectivesToComplete;
	[Attribute(desc: "If true, then watch for failed objectives instead of completed")]
	bool m_bReverse;
	[Attribute(desc: "Names of objectives to complete")]
	ref array<string> m_aNamesOfObjectives;
	ref array<PS_Objective> m_aObjectives;

	override void EOnActivate(IEntity owner)
	{
		super.EOnActivate(owner);
		if (Replication.IsServer())
		{
			m_aObjectives = TSG_ObjectiveHelper.FindObjectives(this, m_aNamesOfObjectives);
		}
	}

	override bool PerformCheck(PS_GameModeCoop gameModeCoop)
	{
		int nActual = 0;
		foreach (PS_Objective objective: m_aObjectives)
		{
			bool completed = objective.GetCompleted();
			if (m_bReverse)
			{
				completed = !completed;
			}
			if (completed)
			{
				nActual++;
			}
		}
		if (nActual >= m_iNumberOfObjectivesToComplete)
		{
			gameModeCoop.AdvanceGameState(SCR_EGameModeState.GAME);
			return true;
		}
		return false;
	}
}
