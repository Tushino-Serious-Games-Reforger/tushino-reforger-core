class TSG_PolyZoneObjectivePresenceDualClass : PS_PolyZoneObjectivePresenceClass {
	
}

/**
	Variant of PS_PolyZoneObjectivePresence which fails list of counter objectives 
	in addition to completion of listed objectives when triggered (useful for making zero-sum games)
*/
class TSG_PolyZoneObjectivePresenceDual : PS_PolyZoneObjectivePresence {	
	[Attribute(desc: "List of objectives to fail when trigger is triggered")]
	ref array<string> m_aCounterObjectiveNames;
	ref array<PS_Objective> m_aCounterObjectives = {};

	override void UpdateObjectives() {
		foreach (PS_Objective objective : m_aCounterObjectives) {
			if (!m_bOneTime) {
				bool completed = m_iCount >= m_iNeedCount;
				if (completed) {
					bool objectiveValue = !completed;
					if (m_bReverseObjective) {
						objectiveValue = !objectiveValue;
					}
					if (objective.GetCompleted() == !m_bReverseObjective) {
						objective.SetCompleted(objectiveValue);
					}
				}
			}
		}
		super.UpdateObjectives();
	}

	override void LinkObjectives() {
		super.LinkObjectives();
		foreach (string objectiveName : m_aCounterObjectiveNames) {
			PS_Objective objective = PS_Objective.Cast(GetWorld().FindEntityByName(objectiveName));
			if (!objective)
				Print("Missing trigger objective: " + objectiveName, LogLevel.ERROR);
			else if (!m_aCounterObjectives.Contains(objective)) {
				objective.SetCompleted(!m_bReverseObjective);
				m_aCounterObjectives.Insert(objective);
			}
		}
	}
}