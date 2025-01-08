class TSG_BasePeriodicCheckClass: GenericEntityClass
{

}


/**
	Base class for periodic checks like faction losses, mission time etc
*/
class TSG_BasePeriodicCheck: GenericEntity
{
	[Attribute(defvalue: "30.0", desc: "How often to run check (in seconds)")]
	float m_fTriggerIntervalSeconds;

	override void EOnActivate(IEntity owner)
	{
		if (Replication.IsServer())
		{
			int delay = (int)(m_fTriggerIntervalSeconds * 1000);
			GetGame().GetCallqueue().CallLater(fn: PeriodicCheck, delay: delay, repeat: true);
		}
	}

	void PeriodicCheck()
	{
		PS_GameModeCoop gameModeCoop = PS_GameModeCoop.Cast(GetGame().GetGameMode());
		SCR_EGameModeState gameState = gameModeCoop.GetState();
		if ((gameState == SCR_EGameModeState.GAME) && gameModeCoop.IsFreezeTimeEnd())
		{
			if (PerformCheck(gameModeCoop)) {
				GetGame().GetCallqueue().Remove(PeriodicCheck);
				Print("Stopped check " + this);
			}
		} else if (gameState == SCR_EGameModeState.DEBRIEFING)
		{
			GetGame().GetCallqueue().Remove(PeriodicCheck);
		}
	}

	bool PerformCheck(PS_GameModeCoop gameModeCoop);
}
