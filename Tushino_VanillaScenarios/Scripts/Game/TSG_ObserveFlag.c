class TSG_ObserveFlagClass: GenericEntityClass {}


class TSG_ObserveFlag: GenericEntity {
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

    override void EOnActivate(IEntity owner) {
		if (Replication.IsServer()) {
            m_aObjectives = FindObjectives();
            m_flag = GetWorld().FindEntityByName(m_sFlagName);
            m_FlagComponent = SCR_FlagComponent.Cast(m_flag.FindComponent(SCR_FlagComponent));
            m_FactionManager = GetGame().GetFactionManager();
            m_game = PS_GameModeCoop.Cast(GetGame().GetGameMode());
			int delay = m_iFlagPollingTimeoutSeconds * 1000;
			GetGame().GetCallqueue().CallLater(fn: CheckFlag, delay: delay, repeat: true);
		}
    }

    void CheckFlag() {
        SCR_EGameModeState gameState = m_game.GetState();
		if ((gameState == SCR_EGameModeState.GAME)) {
			if (m_FlagComponent.m_sFactionKey == m_sFactionKey) {
                CompleteObjectives(m_aObjectives);
            } else {
				FailObjectives(m_aObjectives);
			}
		} else if (gameState == SCR_EGameModeState.DEBRIEFING) {
			if (m_FlagComponent.m_sFactionKey == m_sFactionKey) {
                CompleteObjectives(m_aObjectives);
            } else {
				FailObjectives(m_aObjectives);
			}
			GetGame().GetCallqueue().Remove(CheckFlag);
		}
    }

    void CompleteObjectives(array<PS_Objective> objectives) {
		foreach (PS_Objective objective: objectives) {
			objective.SetCompleted(true);
		}
	}

	void FailObjectives(array<PS_Objective> objectives) {
		foreach (PS_Objective objective: objectives) {
			objective.SetCompleted(false);
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