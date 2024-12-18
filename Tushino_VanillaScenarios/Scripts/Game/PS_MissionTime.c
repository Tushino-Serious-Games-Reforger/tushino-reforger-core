class PS_MissionTimeClass: GenericEntityClass {

}


class PS_MissionTime: GenericEntity {
    [Attribute(defvalue: "60", desc: "Mission time in minutes")]
    int m_iMissionTimeMinutes;
    [Attribute(defvalue: "10", desc: "Approximate resolution time, " +
              "mission will be advanced in general within " +
              "m_iMissionTimeMinutes + m_iMissionTimeResolutionDelaySeconds, " +
              "altorough there may be more delay if server is lagging")]
    int m_iMissionTimeResolutionDelaySeconds;
    float m_fMissionTimeMillis;
    WorldTimestamp m_lastFreezeTimestamp;
    PS_GameModeCoop m_game;	

    override void EOnActivate(IEntity owner) {
        if (Replication.IsServer()) {
            m_fMissionTimeMillis = 60.0 * 1000 * m_iMissionTimeMinutes;
            m_game = PS_GameModeCoop.Cast(GetGame().GetGameMode());
            m_lastFreezeTimestamp = GetWorld().GetTimestamp();

            int delay = m_iMissionTimeResolutionDelaySeconds * 1000;
            GetGame().GetCallqueue().CallLater(fn: CheckTime, delay: delay, repeat: true);
        }
    }
    
    void CheckTime() {
        SCR_EGameModeState gameState = m_game.GetState();
        if ((gameState == SCR_EGameModeState.GAME)) {
            if (m_game.IsFreezeTimeEnd()) {
                WorldTimestamp currentTimestamp = GetWorld().GetTimestamp();
                float elapsedTimeMillis = currentTimestamp.DiffMilliseconds(m_lastFreezeTimestamp);
                if (elapsedTimeMillis > m_fMissionTimeMillis) {
                    m_game.AdvanceGameState(gameState);
                    GetGame().GetCallqueue().Remove(CheckTime);
                }
            } else {
                m_lastFreezeTimestamp = GetWorld().GetTimestamp();
            }
        } else if (gameState == SCR_EGameModeState.DEBRIEFING) {
            GetGame().GetCallqueue().Remove(CheckTime);
        }
    }
}