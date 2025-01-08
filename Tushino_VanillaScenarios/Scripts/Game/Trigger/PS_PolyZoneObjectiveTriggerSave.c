class PS_PolyZoneObjectiveTriggerSaveClass : PS_PolyZoneObjectiveTriggerDestroyClass
{

}

/**
	Use it to fail objective when all entities in trigger are destroyed and set it completed otherwise
 */
class PS_PolyZoneObjectiveTriggerSave : PS_PolyZoneObjectiveTriggerDestroy
{
	override void UpdateObjectives()
	{
		if (m_bAfterGame)
			return;
		
		foreach (PS_Objective objective : m_aObjectives)
		{
			objective.SetCompleted(m_fDestroyedPercent < 1.0);
		}
	}
}
