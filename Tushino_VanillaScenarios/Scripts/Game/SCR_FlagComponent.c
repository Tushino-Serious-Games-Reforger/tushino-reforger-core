modded class SCR_FlagComponent
{
	[RplProp(onRplName: "OnCurrentMaterialChange")]
	protected ResourceName m_sCurrentMaterial;
	
	[RplProp(), Attribute()]
	protected ref array<ref PS_FlagData> m_aFlagData;
	
	[RplProp(), Attribute()]
	FactionKey m_sFactionKey;
		
	override void EOnInit(IEntity owner)
	{
	SCR_FlagComponentClass prefabData = SCR_FlagComponentClass.Cast(GetComponentData(GetOwner()));
		if (m_sCurrentMaterial == "")
		m_sCurrentMaterial = prefabData.GetDefaultMaterial();
		ChangeMaterial(m_sCurrentMaterial);
	}
	
	override void ChangeMaterial(ResourceName flagResource, ResourceName resourceMLOD = string.Empty)
	{
		m_sCurrentMaterial = flagResource;
		super.ChangeMaterial(flagResource, resourceMLOD)
	}
	
	void OnCurrentMaterialChange()
	{
		ChangeMaterial(m_sCurrentMaterial);
	}
	
	array<ref PS_FlagData> GetFlagData(){
		return m_aFlagData;
	}
}

[BaseContainerProps()]
class PS_FlagData{	
	[Attribute()]
	FactionKey m_sFactionKey;
	
	[Attribute()]
	int m_iCaptureCounter;
	
	static bool Extract(PS_FlagData instance, ScriptCtx ctx, SSnapSerializerBase snapshot)
	{
		snapshot.SerializeInt(instance.m_iCaptureCounter);
		snapshot.SerializeString(instance.m_sFactionKey);
		return true;
	}

	static bool Inject(SSnapSerializerBase snapshot, ScriptCtx ctx, PS_FlagData instance)
	{
		snapshot.SerializeInt(instance.m_iCaptureCounter);
		snapshot.SerializeString(instance.m_sFactionKey);
		return true;
	}

	static void Encode(SSnapSerializerBase snapshot, ScriptCtx ctx, ScriptBitSerializer packet)
	{
		snapshot.EncodeInt(packet);    // m_Int
		snapshot.EncodeString(packet); // m_String
	}

	static bool Decode(ScriptBitSerializer packet, ScriptCtx ctx, SSnapSerializerBase snapshot)
	{
		snapshot.DecodeInt(packet);    // m_Int
		snapshot.DecodeString(packet); // m_String
		return true;
	}

	static bool SnapCompare(SSnapSerializerBase lhs, SSnapSerializerBase rhs , ScriptCtx ctx)
	{
		return lhs.CompareSnapshots(rhs, 4)
		    && lhs.CompareStringSnapshots(rhs); 
	}

	static bool PropCompare(PS_FlagData instance, SSnapSerializerBase snapshot, ScriptCtx ctx)
	{
		return snapshot.CompareInt(instance.m_iCaptureCounter)
		    && snapshot.CompareString(instance.m_sFactionKey);
	}
}	

class PS_FlagChangeAction : ScriptedUserAction
{
	SCR_FlagComponent m_FlagComponent;
	FactionManager m_FactionManager;

	override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		m_FlagComponent = SCR_FlagComponent.Cast(pOwnerEntity.FindComponent(SCR_FlagComponent));
		m_FactionManager = GetGame().GetFactionManager();
	}
	
	override bool CanBePerformedScript(IEntity user) {
		
		SCR_ChimeraCharacter character = SCR_ChimeraCharacter.Cast(user);
		FactionAffiliationComponent factionAffiliationComponent = FactionAffiliationComponent.Cast(character.FindComponent(FactionAffiliationComponent));
		Faction faction = factionAffiliationComponent.GetDefaultAffiliatedFaction();
		
		if (faction.GetFactionKey() == m_FlagComponent.m_sFactionKey) {
			SetCannotPerformReason("Это ваш флаг");
			return false;
		}
		
		
		array<ref PS_FlagData> flagData = m_FlagComponent.GetFlagData();
		
		foreach (PS_FlagData flagItemData : flagData){
			if (flagItemData.m_sFactionKey == faction.GetFactionKey()){
				if (flagItemData.m_iCaptureCounter != 0){
					return true;
				}
				SetCannotPerformReason("Перезахват запрещен");
				return false;
			}
		}
		
		return true;
	};
	
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{	
		if (!CanBePerformedScript(pUserEntity))
			return;
		
				
		FactionAffiliationComponent factionAffiliationComponent = FactionAffiliationComponent.Cast(pUserEntity.FindComponent(FactionAffiliationComponent));
		if (!factionAffiliationComponent)
			return;
		
		SCR_Faction faction = SCR_Faction.Cast(factionAffiliationComponent.GetDefaultAffiliatedFaction());
		
		m_FlagComponent.m_sFactionKey = faction.GetFactionKey();
		m_FlagComponent.ChangeMaterial(faction.GetFactionFlagMaterial());
				
		array<ref PS_FlagData> flagData = m_FlagComponent.GetFlagData();
		
		foreach (PS_FlagData flagItemData : flagData){
			if (flagItemData.m_sFactionKey == faction.GetFactionKey()){
				flagItemData.m_iCaptureCounter--;
			}
		}
		
		
	}
};