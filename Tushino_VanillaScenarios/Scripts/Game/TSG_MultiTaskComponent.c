[ComponentEditorProps(category: "GameScripted/Misc", description: "")]
class TSG_MultiTaskComponentClass : ScriptComponentClass
{}

class TSG_MultiTaskComponent : ScriptComponent
{
	[Attribute(defvalue: "3", desc: "Update delay in sec")]
	int UpdateDelay;
	[Attribute(desc: "Name of objectives to keep track of")]
	ref array<string> ObjectvesName;
	
	PS_GameModeCoop GameMode;
	
	override void OnPostInit(IEntity owner)
	{
		if(!GetGame().InPlayMode())
			return;
		
		if(!Replication.IsServer())
			return;
		
		
		GetGame().GetCallqueue().CallLater(ChekTasks, UpdateDelay*1000, true);
	}
	
	void ChekTasks()
	{
		GameMode = PS_GameModeCoop.Cast(GetGame().GetGameMode());
		if(!GameMode)
			return;
		
		if (GameMode.IsFreezeTimeEnd() == false)
			return;
		
		foreach (string ObjectiveName : ObjectvesName)
		{
			PS_Objective Objective = PS_Objective.Cast(GetGame().GetWorld().FindEntityByName(ObjectiveName));
			if (!Objective)
				continue;
			
			if (Objective.GetCompleted() != true)
				return;
		}
		
		GameMode.AdvanceGameState(SCR_EGameModeState.GAME);
		GetGame().GetCallqueue().Remove(ChekTasks);
	}
}