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
		if (!Replication.IsServer())
            return;
		
		if (!GetGame().InPlayMode())
			return;

		GameModeCoop = PS_GameModeCoop.Cast(GetGame().GetGameMode());
		if(!GameModeCoop)
			return;
		
		GetGame().GetCallqueue().CallLater(DoIt, 4000, true);
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
			if (!Character)
				continue;
			Identity CharacterIdentity = CharacterIdentityComponent.Cast(Character.FindComponent(CharacterIdentityComponent)).GetIdentity();
			if (!CharacterIdentity)
				continue;
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
			GetGame().GetCallqueue().CallLater(DoIt, 300000, true);
		}
		
	}
}