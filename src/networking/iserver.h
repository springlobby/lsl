class StringMap;
class StringVector;

class ServerEvents;

class iServer : public ChannelList, public UserList, public BattleList
{
  public:
	friend class ServerEvents;

	// Server interface

	virtual bool ExecuteSayCommand( const std::string& cmd ) = 0;

	virtual bool Register( const std::string& addr, const int port, const std::string& nick, const std::string& password,std::string& reason ) = 0;
	virtual void AcceptAgreement() = 0;

	virtual void Connect( const std::string& servername, const std::string& addr, const int port ) = 0;
	virtual void Disconnect() = 0;
	virtual bool IsConnected() = 0;

	virtual void Login() = 0;
	virtual void Logout() = 0;
	virtual bool IsOnline()  const = 0;

	virtual void Update( int mselapsed ) = 0;

	virtual void JoinChannel( const std::string& channel, const std::string& key ) = 0;
	virtual void PartChannel( const std::string& channel ) = 0;

	virtual void DoActionChannel( const std::string& channel, const std::string& msg ) = 0;
	virtual void SayChannel( const std::string& channel, const std::string& msg ) = 0;

	virtual void SayPrivate( const std::string& nick, const std::string& msg ) = 0;
	virtual void DoActionPrivate( const std::string& nick, const std::string& msg ) = 0;

	virtual void SayBattle( int battleid, const std::string& msg ) = 0;
	virtual void DoActionBattle( int battleid, const std::string& msg ) = 0;

	virtual void Ring( const std::string& nick ) = 0;

	virtual void ModeratorSetChannelTopic( const std::string& channel, const std::string& topic ) = 0;
	virtual void ModeratorSetChannelKey( const std::string& channel, const std::string& key ) = 0;
	virtual void ModeratorMute( const std::string& channel, const std::string& nick, int duration, bool byip ) = 0;
	virtual void ModeratorUnmute( const std::string& channel, const std::string& nick ) = 0;
	virtual void ModeratorKick( const std::string& channel, const std::string& reason ) = 0;
	virtual void ModeratorBan( const std::string& nick, bool byip ) = 0;
	virtual void ModeratorUnban( const std::string& nick ) = 0;
	virtual void ModeratorGetIP( const std::string& nick ) = 0;
	virtual void ModeratorGetLastLogin( const std::string& nick ) = 0;
	virtual void ModeratorGetLastIP( const std::string& nick ) = 0;
	virtual void ModeratorFindByIP( const std::string& ipadress ) = 0;

	virtual void AdminGetAccountAccess( const std::string& nick ) = 0;
	virtual void AdminChangeAccountAccess( const std::string& nick, const std::string& accesscode ) = 0;
	virtual void AdminSetBotMode( const std::string& nick, bool isbot ) = 0;

	virtual void HostBattle( BattleOptions bo, const std::string& password = "" ) = 0;
	virtual void JoinBattle( const int& battleid, const std::string& password = "" ) = 0;
	virtual void LeaveBattle( const int& battleid ) = 0;
	virtual void StartHostedBattle() = 0;

	virtual void ForceSide( int battleid, User& user, int side ) = 0;
	virtual void ForceTeam( int battleid, User& user, int team ) = 0;
	virtual void ForceAlly( int battleid, User& user, int ally ) = 0;
	virtual void ForceColour( int battleid, User& user, int r, int g, int b ) = 0;
	virtual void ForceSpectator( int battleid, User& user, bool spectator ) = 0;
	virtual void BattleKickPlayer( int battleid, User& user ) = 0;
	virtual void SetHandicap( int battleid, User& user, int handicap) = 0;

	virtual void AddBot( int battleid, const std::string& nick, UserBattleStatus& status ) = 0;
	virtual void RemoveBot( int battleid, User& user ) = 0;
	virtual void UpdateBot( int battleid, User& user, UserBattleStatus& status ) = 0;

	virtual void SendHostInfo( HostInfo update ) = 0;
	virtual void SendHostInfo( const std::string& Tag ) = 0;
	virtual void SendRaw( const std::string& raw ) = 0;
	virtual void SendUserPosition( const User& usr ) = 0;

	virtual void RequestInGameTime( const std::string& nick ) = 0;

	virtual Battle* GetCurrentBattle() = 0;

	virtual void RequestChannels() = 0;

	virtual void SendMyBattleStatus( UserBattleStatus& bs ) = 0;
	virtual void SendMyUserStatus() = 0;

	virtual void SetKeepaliveInterval( int seconds ) { m_keepalive = seconds; }
	virtual int GetKeepaliveInterval() { return m_keepalive; }

	virtual void SetUsername( const std::string& username ) { m_user = username; }
	virtual void SetPassword( const std::string& password ) { m_pass = password; }
	virtual bool IsPasswordHash( const std::string& pass ) const = 0;
	virtual std::string GetPasswordHash( const std::string& pass ) const = 0;

	std::string GetRequiredSpring() const { return m_min_required_spring_ver; }

	void SetRequiredSpring( const std::string& version ) { m_min_required_spring_ver = version; }

	virtual void OnConnected( Socket* sock ) = 0;
	virtual void OnDisconnected( Socket* sock ) = 0;
	virtual void OnDataReceived( Socket* sock ) = 0;

	virtual const User& GetMe() const = 0;

	virtual void SendScriptToProxy( const std::string& script ) = 0;

	virtual void SendScriptToClients( const std::string& script ) = 0;

	std::map<std::string,std::string> m_channel_pw;  /// channel name -> password, filled on channel join

	std::string GetServerName() const { return m_server_name; }

	virtual void RequestSpringUpdate();

	virtual void SetRelayIngamePassword( const User& user ) = 0;

	virtual StringVector GetRelayHostList();

  protected:
	Socket* m_sock;
	int m_keepalive; //! in seconds
	int m_ping_timeout; //! in seconds
	std::string m_user;
	std::string m_pass;
	std::string m_server_name;
	bool m_pass_hash;
	std::string m_min_required_spring_ver;

	int m_relay_host_bot_id;
	int m_relay_host_manager_id;

	StringVector m_relay_host_manager_list;

	User* _AddUser( const std::string& user, const int id );
	void _RemoveUser( const std::string& nickname );
	void _RemoveUser( const int id );

	Channel* _AddChannel( const std::string& chan );
	void _RemoveChannel( const std::string& name );

	Battle* _AddBattle( const int& id );
	void _RemoveBattle( const int& id );

	virtual void SendCmd( const std::string& command, const std::string& param ) = 0;
	virtual void RelayCmd( const std::string& command, const std::string& param ) = 0;
};
