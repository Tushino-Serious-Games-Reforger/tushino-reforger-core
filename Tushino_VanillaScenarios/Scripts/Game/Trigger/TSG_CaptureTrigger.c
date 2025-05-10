class TSG_CaptureTriggerClass: TSG_BasePeriodicCheckClass
{

}

/**
 Trigger on capture which mimics logic of TSG A3 capture trigger.
 Place it under PolyShape and don't forget to add Hierarchy and RplComponent on it.
*/
class TSG_CaptureTrigger: TSG_BasePeriodicCheck
{
	[Attribute(desc: "Factions which defend objective")]
	ref array<string> m_aDefenderFactions;
	[Attribute(desc: "Factions which attack objective")]
	ref array<string> m_aAttackerFactions;
	[Attribute(desc: "Attacker objective names")]
	ref array<string> m_aAttackerObjectiveNames;
	[Attribute(desc: "Defender objective names")]
	ref array<string> m_aDefenderObjectiveNames;
	[Attribute(desc: "Minimum number of living defenders", defvalue: "1")]
	int m_iNumberOfDefenders;
	[Attribute(desc: "Minimum number of living attackers", defvalue: "1")]
	int m_iNumberOfAttackers;
	[Attribute(desc: "If true, trigger can be recaptured", defvalue: "false")]
	bool m_bCanRecapture;
	[Attribute(desc: "Use it to limit height of trigger, if 0 then height is ignored entirely", defvalue: "0")]
	float m_fHeight;
	ref array<float> m_aPolygon = {};
	PolylineShapeEntity m_polygon;
	ref array<PS_Objective> m_aAttackerObjectives = {};
	ref array<PS_Objective> m_aDefenderObjectives = {};
	bool m_bCaptured = false;

	override void EOnActivate(IEntity owner)
	{
		super.EOnActivate(owner);
		if (Replication.IsServer())
		{
			m_aAttackerObjectives = TSG_ObjectiveHelper.FindObjectives(this, m_aAttackerObjectiveNames, false);
			m_aDefenderObjectives = TSG_ObjectiveHelper.FindObjectives(this, m_aDefenderObjectiveNames, false);
			m_polygon = PolylineShapeEntity.Cast(owner.GetParent());
			array<vector> points = {};
			m_polygon.GetPointsPositions(points);
			SCR_Math2D.Get2DPolygon(points, m_aPolygon);
		}
	}

	override bool PerformCheck(PS_GameModeCoop gameModeCoop)
	{
		map<string, int> headcount = SurveyTrigger(m_aAttackerFactions, m_aDefenderFactions);
		foreach (string faction, int count: headcount)
		{
			Print("[" + this.GetName() + "] " + faction + ": " + count);
		}
		m_bCaptured = IsCaptured(headcount);
		TSG_ObjectiveHelper.SwitchObjectives(m_aAttackerObjectives, m_bCaptured);
		TSG_ObjectiveHelper.SwitchObjectives(m_aDefenderObjectives, !m_bCaptured);
		if (!m_bCanRecapture)
		{
			return m_bCaptured;
		}
		return false;
	}

	bool IsCaptured(map<string, int> headcount)
	{
		if (!m_bCaptured || m_bCanRecapture)
		{
			if (!m_bCaptured)
			{
				return CheckCapture(headcount, m_aAttackerFactions, m_aDefenderFactions, m_iNumberOfAttackers, m_iNumberOfDefenders);
			}
			else
			{
				return !CheckCapture(headcount, m_aDefenderFactions, m_aAttackerFactions, m_iNumberOfAttackers, m_iNumberOfDefenders);
			}
		}
		else
		{
			return m_bCaptured;
		}
	}

	map<string, int> SurveyTrigger(array<string> attackerFactions, array<string> defenderFactions)
	{
		map<string, int> triggerState = new map<string, int>();
		foreach (string faction: attackerFactions)
		{
			triggerState[faction] = 0;
		}
		foreach(string faction: defenderFactions)
		{
			triggerState[faction] = 0;
		}
		PS_PlayableManager manager = PS_PlayableManager.GetInstance();
		foreach (PS_PlayableContainer container : manager.GetPlayablesSorted())
		{
			PS_PlayableComponent playable = container.GetPlayableComponent();
			if (IsAlive(playable.GetOwner()) && IsInside(playable.GetOwner()))
			{
				FactionAffiliationComponent factionAffiliationComponent = playable.GetFactionAffiliationComponent();
				string faction = factionAffiliationComponent.GetDefaultFactionKey();
				if (triggerState.Contains(faction))
				{
					triggerState[faction] = triggerState[faction] + 1;
				}
			}
		}
		return triggerState;
	}

	private bool CheckCapture(map<string, int> headcount, array<string> attackerFactions, array<string> defenderFactions, int minAttackers, int minDefenders)
	{
		int numDefenders = 0;
		int numAttackers = 0;
		foreach (string faction: attackerFactions)
		{
			numAttackers += headcount[faction];
		}
		foreach (string faction: defenderFactions)
		{
			numDefenders += headcount[faction];
		}
		if (numDefenders < minDefenders)
		{
			if (numAttackers >= minAttackers)
			{
				return true;
			}
		}
		return false;
	}

	private bool IsInside(IEntity entity)
	{
		vector coords = m_polygon.CoordToLocal(entity.GetOrigin());

		bool isInside = Math2D.IsPointInPolygon(m_aPolygon, coords[0], coords[2]);
		if(m_fHeight > 0)
		{
			isInside = isInside && (coords[1] < m_fHeight);
		}
		return isInside;
	}

	private bool IsAlive(IEntity entity)
	{
		SCR_DamageManagerComponent component = SCR_DamageManagerComponent.Cast(entity.FindComponent(SCR_DamageManagerComponent));
		if (component)
		{
			EDamageState state = component.GetState();
			return state != EDamageState.DESTROYED;
		}
		return false;
	}
}
