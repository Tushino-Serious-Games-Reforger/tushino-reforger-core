class TSG_ObjectiveHelper
{
	static void CompleteObjectives(array<PS_Objective> objectives)
	{
		foreach (PS_Objective objective: objectives)
		{
			objective.SetCompleted(true);
		}
	}

	static void FailObjectives(array<PS_Objective> objectives)
	{
		foreach (PS_Objective objective: objectives)
		{
			objective.SetCompleted(false);
		}
	}

	static void SwitchObjectives(array<PS_Objective> objectives, bool switchValue)
	{
		foreach (PS_Objective objective: objectives)
		{
			objective.SetCompleted(switchValue);
		}
	}

	static array<PS_Objective> FindObjectives(IEntity owner, array<string> objectiveNames)
	{
		return FindObjectives(owner, objectiveNames, true);
	}

	static array<PS_Objective> FindObjectives(IEntity owner, array<string> objectiveNames, bool findChildren)
	{
		array<PS_Objective> objectives = new array<PS_Objective>();
		foreach (string objectiveName : objectiveNames)
		{
			IEntity entity = owner.GetWorld().FindEntityByName(objectiveName);
			PS_Objective objective = PS_Objective.Cast(entity);
			objectives.Insert(objective);
		}
		if (findChildren)
		{
			IEntity maybeObjective = owner.GetChildren();
			while (maybeObjective)
			{
				PS_Objective objective = PS_Objective.Cast(maybeObjective);
				if (objective)
				{
					objectives.Insert(objective);
				}
				maybeObjective = maybeObjective.GetSibling();
			}
		}
		return objectives;
	}
}
