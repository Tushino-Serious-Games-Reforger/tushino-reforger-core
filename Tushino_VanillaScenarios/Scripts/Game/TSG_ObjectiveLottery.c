class TSG_ObjectiveLotteryClass: TSG_BasePeriodicCheckClass
{

}


class TSG_ObjectiveTicketClass: GenericEntityClass
{
}


class TSG_ObjectiveLottery: TSG_BasePeriodicCheck
{
	[Attribute("Pairs of objectives to select randomly", UIWidgets.Object)]
	ref array<ref TSG_ObjectiveTicket> m_aTickets;

	[Attribute("Upper bound on selection time after freeze ended")]
	int m_iLotteryTimeoutSeconds;
	[Attribute("Lower bound on selection time after freeze ended")]
	int m_iMinLotteryTimeoutSeconds;
	[Attribute("Max delay between reveal of objective and counter objective")]
	int m_iCounterObjectiveRevealDelaySeconds;
	[Attribute("Min delay between reveal of objective and counter objective")]
	int m_iMinCounterObjectiveRevealDelaySeconds;
	ref RandomGenerator m_Random;

	override bool PerformCheck(PS_GameModeCoop gameModeCoop)
	{
		m_Random = RandomGenerator();
		int delay = RandIntInclusive(m_iMinLotteryTimeoutSeconds * 1000, m_iLotteryTimeoutSeconds * 1000);
		Print("Lottery will happen in " + delay + " ms");
		GetGame().GetCallqueue().CallLater(fn: PerformLottery, delay: delay, repeat: false);
		return true;
	}

	void PerformLottery()
	{
		int winner = Math.RandomInt(0, m_aTickets.Count());
		for (int i = 0; i < m_aTickets.Count(); i++) {
			if (i != winner)
			{
				TSG_ObjectiveTicket ticket = m_aTickets.Get(i);
				ticket.GetObjective(this).Abandon();
				ticket.GetCounterObjective(this).Abandon();
				Print("Abandoned " + ticket);
			}
			else
			{
				TSG_ObjectiveTicket ticket = m_aTickets.Get(i);
				ticket.RevealObjective(this);
				int delay = RandIntInclusive(m_iMinCounterObjectiveRevealDelaySeconds * 1000, 
				                           m_iCounterObjectiveRevealDelaySeconds * 1000);
				GetGame().GetCallqueue().CallLater(fn: RevealCounterObjective, delay: delay, repeat: false, param1: i);
				Print("Revealed " + ticket);
				Print("Counter objective reveal will happen in " + delay + " ms");
			}
		}
	}
	
	void RevealCounterObjective(int i)
	{
		TSG_ObjectiveTicket ticket = m_aTickets.Get(i);
		ticket.RevealCounterObjective(this);
		NotifyPlayers("Все приказы были отданы!");
		Print("Revealed " + ticket + " all orders given");
	}
	
	/**
	* Wrote it because Math.RandInt, RandomGenerator.RandInt return suspiciously small numbers 
	from big range like 300_000 - 600_000 in tools. Like 5 times in a row it was 300300 or similar number,
	never any number > 300500.
	*/
	int RandIntInclusive(int min, int max)
	{
		float v = m_Random.RandFloat01();
		int span = max - min;
		int generated = (int)(v * span) + min;
		if (generated > max)
		{
			return max;
		}
		return generated;
	}
}


[BaseContainerProps()]
class TSG_ObjectiveTicket: Managed
{
	[Attribute("")]
	string m_sObjectiveName;
	[Attribute("")]
	string m_sCounterObjectiveName;
	
	void RevealObjective(IEntity owner)
	{
		TSG_Objective objective = GetObjective(owner);
		FactionKey factionKey = objective.GetFactionKey();
		Faction faction = GetGame().GetFactionManager().GetFactionByKey(factionKey);
		objective.SetVisibleForFaction(faction, true);
	}
	
	void RevealCounterObjective(IEntity owner)
	{
		TSG_Objective objective = GetCounterObjective(owner);
		FactionKey factionKey = objective.GetFactionKey();
		Faction faction = GetGame().GetFactionManager().GetFactionByKey(factionKey);
		objective.SetVisibleForFaction(faction, true);
	}
	
	TSG_Objective GetObjective(IEntity owner)
	{
		return GetObjectiveByName(owner, m_sObjectiveName);
	}
	
	TSG_Objective GetCounterObjective(IEntity owner)
	{
		return GetObjectiveByName(owner, m_sCounterObjectiveName);
	}

	TSG_Objective GetObjectiveByName(IEntity owner, string name)
	{
		IEntity entity = owner.GetWorld().FindEntityByName(name);
		TSG_Objective objective = TSG_Objective.Cast(entity);
		return objective;
	}
}
