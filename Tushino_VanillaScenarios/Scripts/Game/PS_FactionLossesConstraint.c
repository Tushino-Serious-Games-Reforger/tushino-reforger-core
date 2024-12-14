class PS_FactionLossesConstraintClass: GenericEntityClass {

}


class PS_FactionLossesConstraint: GenericEntity {
	[Attribute(defvalue: "", desc: "Faction key")]
	string m_sFactionKey;
	[Attribute(defvalue: "30.0", desc: "How often to check for losses")]
	float m_fTriggerIntervalSeconds;
	[Attribute(defvalue: "0", desc: "Minimum number of alive characters")]
	int m_iMinNumberOfAliveCharacters;
	[Attribute(desc: "Names of objectives to complete")]
	ref array<string> m_aNamesOfObjectives;
	PS_PlayableManager m_PlayableManager;

	override void EOnActivate(IEntity owner) {
		m_PlayableManager = PS_PlayableManager.GetInstance();
		if (Replication.IsServer()) {
			int delay = (int)(m_fTriggerIntervalSeconds * 1000);
			GetGame().GetCallqueue().CallLater(fn: CheckLosses, delay: delay, repeat: true);
		}	
	}
	
	void CheckLosses() {
		PS_GameModeCoop gameModeCoop = PS_GameModeCoop.Cast(GetGame().GetGameMode());
		SCR_EGameModeState gameState = gameModeCoop.GetState();
		if ((gameState == SCR_EGameModeState.GAME) && gameModeCoop.IsFreezeTimeEnd()) {
			int nAlive = CalculateNumberOfAliveCharacters();
			array<PS_Objective> objectives = FindObjectives();
			Print(m_sFactionKey + ": " + nAlive);
			if (nAlive < m_iMinNumberOfAliveCharacters) {
				CompleteObjectives(objectives);
				GetGame().GetCallqueue().Remove(CheckLosses);
			}
		} else if (gameState == SCR_EGameModeState.DEBRIEFING) {
			GetGame().GetCallqueue().Remove(CheckLosses);
		}
	}
	
	int CalculateNumberOfAliveCharacters() {
		int nAlive = 0;
		array<PS_PlayableComponent> playables = m_PlayableManager.GetPlayablesSorted();
		foreach(PS_PlayableComponent playable : playables) {
			FactionAffiliationComponent factionAffiliationComponent = playable.GetFactionAffiliationComponent();
			SCR_Faction faction = SCR_Faction.Cast(factionAffiliationComponent.GetDefaultAffiliatedFaction());
			if (faction.GetFactionKey() == m_sFactionKey) {
				SCR_CharacterDamageManagerComponent characterDamageManagerComponent = playable.GetCharacterDamageManagerComponent();
				if (characterDamageManagerComponent.GetState() != EDamageState.DESTROYED) {
					nAlive++;
				}
			}
		}
		return nAlive;
	}
	
	void CompleteObjectives(array<PS_Objective> objectives) {
		foreach (PS_Objective objective: objectives) {
			objective.SetCompleted(true);
		}
	}
	
	array<PS_Objective> FindObjectives() {
		array<PS_Objective> objectives = new array<PS_Objective>();
		foreach (string objectiveName : m_aNamesOfObjectives) {
			IEntity entity = GetWorld().FindEntityByName(objectiveName);
			PS_Objective objective = PS_Objective.Cast(entity);
			objectives.Insert(objective);
		}
		IEntity maybeObjective = GetChildren();
		while (maybeObjective) {
			PS_Objective objective = PS_Objective.Cast(maybeObjective);
			if (objective) {
				objectives.Insert(objective);
			}
			maybeObjective = maybeObjective.GetSibling();
		}
		return objectives;
	}
}