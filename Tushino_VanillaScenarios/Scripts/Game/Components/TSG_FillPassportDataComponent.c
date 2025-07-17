[ComponentEditorProps(category: "GameScripted/Misc", description: "")]
class TSG_FillPassportDataComponentClass : ScriptComponentClass
{}

class TSG_FillPassportDataComponent : ScriptComponent
{
	PS_GameModeCoop GameModeCoop;
	PS_PlayableManager PlaybleManager;
	ref array<int> PlayersID = new array<int>();
	
	override void OnPostInit(IEntity owner)
	{
		if (!GetGame().InPlayMode() || !Replication.IsServer())
		{
			Print("Server replication error or game is not running", LogLevel.ERROR);
            return;
		}
		GameModeCoop = PS_GameModeCoop.Cast(GetGame().GetGameMode());
		if(!GameModeCoop)
			return;
		
		GetGame().GetCallqueue().CallLater(DoIt, 1000, true);
	}
	
	void FindAllPlayers()
	{
		PlaybleManager = PS_PlayableManager.GetInstance();
		if (!PlaybleManager)
			return;
		ref array<PS_PlayableContainer> Players = PlaybleManager.GetPlayablesSorted();
		ref array<int> IDes = new array<int>();
		foreach(PS_PlayableContainer Player : Players)
		{
			if (PlaybleManager.GetPlayerByPlayable(Player.GetRplId()) == -1)
				continue;
			if (IDes.Find(PlaybleManager.GetPlayerByPlayable(Player.GetRplId())) != -1)
				continue;
			IDes.Insert(PlaybleManager.GetPlayerByPlayable(Player.GetRplId()));
		}
		PlayersID = IDes;
	}
	void ChangeAIAllias()
	{
		PlaybleManager = PS_PlayableManager.GetInstance();
		if (!PlaybleManager)
			return;
		foreach(int ID : PlayersID)
		{
			IEntity Character = PlaybleManager.GetPlayableById(PlaybleManager.GetPlayableByPlayer(ID)).GetPlayableComponent().GetOwner();
			Identity CharacterIdentity = CharacterIdentityComponent.Cast(Character.FindComponent(CharacterIdentityComponent)).GetIdentity();
			CharacterIdentity.SetAlias(PlaybleManager.GetPlayerName(ID));
		}
	}
	void DoIt()
	{
		GameModeCoop = PS_GameModeCoop.Cast(GetGame().GetGameMode());
		if(!GameModeCoop)
			return;
		
		SCR_EGameModeState GameState = GameModeCoop.GetState();
		if(GameState != SCR_EGameModeState.GAME)
			return;
		
		FindAllPlayers();
		
		if(GameModeCoop.IsFreezeTimeEnd())
		{
			ChangeAIAllias();
			GetGame().GetCallqueue().Remove(DoIt);
		}
		
	}
}