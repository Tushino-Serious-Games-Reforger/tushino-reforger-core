class TSG_ObjectiveClass : PS_ObjectiveClass
{

}

/**
Just like regular objective but can be abandoned or assigned to faction
*/
class TSG_Objective: PS_Objective
{
	void Abandon()
	{
		SetFactionKey((FactionKey)"");
	}

	void SetFactionKey(FactionKey factionKey)
	{
		RPC_SetFactionKey(factionKey);
		Rpc(RPC_SetFactionKey, factionKey);
	}

	[RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
	protected void RPC_SetFactionKey(FactionKey factionKey)
	{
		m_sFactionKey = factionKey;
	}

	// JIP Replication
	override bool RplSave(ScriptBitWriter writer)
	{
		writer.WriteString(m_sFactionKey);
		super.RplSave(writer);		
		return true;
	}

	override bool RplLoad(ScriptBitReader reader)
	{
		reader.ReadString(m_sFactionKey);
		super.RplLoad(reader);	
		return true;
	}
	
	override RplId GetRplId()
	{
		return m_RplComponent.ChildId(this);
	}
}
