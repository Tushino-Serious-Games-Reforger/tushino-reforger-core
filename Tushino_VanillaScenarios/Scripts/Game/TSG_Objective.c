class TSG_ObjectiveClass : PS_ObjectiveClass
{

}

/**
Just like regular objective but can be abandoned or assigned to faction
*/
class TSG_Objective : PS_Objective
{
	void Abandon()
	{
		SetFactionKey("");
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
}
