class TSG_CrewComponentClass : ScriptComponentClass
{
}

class TSG_CrewComponent : ScriptComponent
{
	[RplProp(), Attribute(desc: "Crew key to define access control vehicle")]
	protected string m_sCrewKey;
	
	string GetCrewKey()
	{
		return m_sCrewKey;
	}
	
	void SetCrewKey(string NewKey)
	{
		m_sCrewKey = NewKey;
		Replication.BumpMe();
	}
	
	void OnCrewKeyUpdated()
	{
		SetCrewKey(m_sCrewKey);
	}
}
