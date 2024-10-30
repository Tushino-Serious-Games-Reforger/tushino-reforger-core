modded class SCR_FlagComponent
	{
	[RplProp(onRplName: "OnCurrentMaterialChange")]
	
	protected ResourceName m_sCurrentMaterial;
		
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
	
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
	FactionAffiliationComponent factionAffiliationComponent = FactionAffiliationComponent.Cast(pUserEntity.FindComponent(FactionAffiliationComponent));
	if (!factionAffiliationComponent)
	return;
	
	SCR_Faction faction = SCR_Faction.Cast(factionAffiliationComponent.GetDefaultAffiliatedFaction());
	
	m_FlagComponent.ChangeMaterial(faction.GetFactionFlagMaterial());
	}
};