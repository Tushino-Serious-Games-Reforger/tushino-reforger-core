class TSG_EntityConstraintClass: TSG_BasePeriodicCheckClass
{

}

/**
	Use this class to check players/vehicles efficiently for liveness or destruction, possibly in or out of certain area.
*/
class TSG_EntityConstraint: TSG_BasePeriodicCheck
{
	[Attribute(desc: "Which prefabs to monitor (if set then names of entities are ignored)")]
	ref array<ResourceName> m_aPrefabs;
	[Attribute(desc: "Names of entities to monitor (used if no prefabs were set)")]
	ref array<string> m_aNamesOfEntities;
	[Attribute(desc: "If true then monitored entities expected to be alive, otherwise - destroyed")]
	bool m_bCheckAlive;
	[Attribute(desc: "How much entities in desired state to expect", defvalue: "1")]
	int m_iExpectedNumberOfEntities;
	[Attribute(desc: "What objectives to trigger")]
	ref array<string> m_aNamesOfObjectives;
	[Attribute(desc: "Disable polling when condition is satisfied for first time", defvalue: "false")]
	bool m_bDisableWhenTriggered;
	[Attribute(desc: "Name of polygon (trigger) to filter entities by")]
	string m_sPolygonName;
	[Attribute(desc: "If true, then entities inside polygon will be filtered out, otherwise those outside")]
	bool m_bInversed;
	[Attribute(defvalue: "0")]
	float m_fHeight;
	ref array<float> m_aPolygon = {};
	PolylineShapeEntity m_polygon;

	ref array<PS_Objective> m_aObjectives;

	override void EOnActivate(IEntity owner)
	{
		super.EOnActivate(owner);
		if (Replication.IsServer())
		{
			m_aObjectives = TSG_ObjectiveHelper.FindObjectives(this, m_aNamesOfObjectives);
			if (m_sPolygonName)
			{
				m_polygon = PolylineShapeEntity.Cast(owner.GetWorld().FindEntityByName(m_sPolygonName));
				if (m_polygon)
				{
					array<vector> points = {};
					m_polygon.GetPointsPositions(points);
					SCR_Math2D.Get2DPolygon(points, m_aPolygon);
				}
			}
		}
	}

	override bool PerformCheck(PS_GameModeCoop gameModeCoop)
	{
		int actualNumberOfEntities = 0;
		TSG_MissionDataManager manager = TSG_MissionDataManager.GetInstance();
		array<IEntity> entities = {};
		if (m_aPrefabs.Count() > 0)
		{
			entities = manager.FindByPrefabs(m_aPrefabs);
		} else if (m_aNamesOfEntities.Count() > 0) {
			entities = manager.FindByNames(m_aNamesOfEntities);
		}
		foreach (IEntity entity: entities)
		{
			SCR_DamageManagerComponent component = SCR_DamageManagerComponent.Cast(entity.FindComponent(SCR_DamageManagerComponent));
			if (component)
			{
				EDamageState state = component.GetState();
				if (CheckEntityState(state) && CheckEntityPosition(entity))
				{
					actualNumberOfEntities++;
				}
			}
		}
		TSG_ObjectiveHelper.SwitchObjectives(m_aObjectives, actualNumberOfEntities >= m_iExpectedNumberOfEntities);
		if (m_bDisableWhenTriggered)
		{
			return actualNumberOfEntities >= m_iExpectedNumberOfEntities;
		}
		return false;
	}

	bool CheckEntityState(EDamageState state)
	{
		if (m_bCheckAlive)
		{
			return state != EDamageState.DESTROYED;
		}
		else
		{
			return state == EDamageState.DESTROYED;
		}
	}

	bool CheckEntityPosition(IEntity entity)
	{
		if (!m_aPolygon.Count())
		{
			return true;
		}
		vector coords = m_polygon.CoordToLocal(entity.GetOrigin());

		bool isInside = Math2D.IsPointInPolygon(m_aPolygon, coords[0], coords[2]);
		if(m_fHeight > 0)
		{
			isInside = isInside && (coords[1] < m_fHeight);
		}
		if (m_bInversed)
		{
			isInside = !isInside;
		}
		return isInside;
	}
}
