#ifndef LIBSPRINGLOBBY_HEADERGUARD_IBATTLE_H
#define LIBSPRINGLOBBY_HEADERGUARD_IBATTLE_H

#include <utils/global_interfaces.h>
#include "enum.h"

namespace LSL {
namespace Battle {

class IBattle;

struct BattleStartRect
{
	BattleStartRect() :
		toadd(false),
		todelete(false),
		toresize(false),
		exist(false),
		ally(-1),
		top(-1),
		left(-1),
		right(-1),
		bottom(-1)
	{
	}

	bool toadd;
	bool todelete;
	bool toresize;
	bool exist;

	int ally;
	int top;
	int left;
	int right;
	int bottom;

	bool IsOk() const
	{
		return exist && !todelete;
	}
};

struct BattleOptions
{
	BattleOptions();

	int battleid;
	bool islocked;
	Enum::BattleType battletype;
	bool ispassworded;
	int rankneeded;
	std::string proxyhost;
	bool userelayhost;
	bool lockexternalbalancechanges;

	std::string founder;

	Enum::NatType nattype;
	unsigned int port;
	std::string ip;
	unsigned int externaludpsourceport;
	unsigned int internaludpsourceport;

	unsigned int maxplayers;
	unsigned int spectators;
	std::string relayhost;
	std::string maphash;
	std::string modhash;

	std::string description;
	std::string mapname;
	std::string modname;
};

class IBattle : public HasIndex< int >
{
public:
	IBattle() {}

	int index() const { return 1; }
	static std::string className() { return "IBattle"; }

	bool Ingame() const { return false; }
	Enum::NatType GetNatType() const { return Enum::NAT_None; }
	bool IsFounderMe() const { return false; }
	bool IsProxy() const { return false; }

	class IBattle: public UserList, public wxEvtHandler, public UnitsyncReloadedSink< IBattle > , public SL::NonCopyable
	{
	public:

		IBattle();
		virtual ~IBattle();

		struct TeamInfoContainer
		{
			bool exist;
			int TeamLeader;
			int StartPosX;
			int StartPosY;
			int AllyTeam;
			wxColor RGBColor;
			wxString SideName;
			int Handicap;
			int SideNum;
		};

		struct AllyInfoContainer
		{
			bool exist;
			int NumAllies;
			int StartRectLeft;
			int StartRectTop;
			int StartRectRight;
			int StartRectBottom;
		};


		virtual void SetHostMap( const wxString& mapname, const wxString& hash );
		virtual void SetLocalMap( const UnitSyncMap& map );
		virtual const UnitSyncMap& LoadMap();
		virtual wxString GetHostMapName() const;
		virtual wxString GetHostMapHash() const;

		virtual void SetProxy( const wxString& proxyhost );
		virtual wxString GetProxy() const;
		virtual bool IsProxy() const;

		virtual bool IsSynced(); //cannot be const

		virtual bool IsFounderMe() const;
		virtual bool IsFounder( const User& user ) const;

		virtual int GetMyPlayerNum() const;

		virtual int GetPlayerNum( const User& user ) const;

		virtual void SetHostMod( const wxString& modname, const wxString& hash );
		virtual void SetLocalMod( const UnitSyncMod& mod );
		virtual const UnitSyncMod& LoadMod();
		virtual wxString GetHostModName() const;
		virtual wxString GetHostModHash() const;

		virtual bool MapExists() const;
		virtual bool ModExists() const;

		virtual BattleStartRect GetStartRect( unsigned int allyno ) const;
		User& OnUserAdded( User& user );
		void OnUserBattleStatusUpdated( User &user, UserBattleStatus status );
		void OnUserRemoved( User& user );

		bool IsEveryoneReady() const;

		void ForceSide( User& user, int side );
		void ForceAlly( User& user, int ally );
		void ForceTeam( User& user, int team );
		void ForceColour( User& user, const wxColour& col );
		void ForceSpectator( User& user, bool spectator );
		void SetHandicap( User& user, int handicap);
		void KickPlayer( User& user );


		virtual void AddStartRect( unsigned int allyno, unsigned int left, unsigned int top, unsigned int right, unsigned int bottom );
		virtual void RemoveStartRect( unsigned int allyno );
		virtual void ResizeStartRect( unsigned int allyno );
		virtual void StartRectRemoved( unsigned int allyno );
		virtual void StartRectResized( unsigned int allyno );
		virtual void StartRectAdded( unsigned int allyno );
		virtual void ClearStartRects();
		virtual unsigned int GetNumRects() const;
		virtual unsigned int GetLastRectIdx() const;
		virtual unsigned int GetNextFreeRectIdx() const;

		virtual int GetFreeTeam( bool excludeme = false ) const;

		virtual User& GetMe() = 0;
		virtual const User& GetMe() const = 0;

		virtual void SendHostInfo( HostInfo update );
		virtual void SendHostInfo( const wxString& Tag );
		virtual void Update ( const wxString& Tag );

		virtual unsigned int GetNumBots() const;
		virtual User& OnBotAdded( const wxString& nick, const UserBattleStatus& bs );

		virtual UserPosition GetFreePosition() ;
		virtual int GetFreeAlly( bool excludeme = false ) const;

		virtual void RestrictUnit( const wxString& unitname, int count = 0 );
		virtual void UnrestrictUnit( const wxString& unitname );
		virtual void UnrestrictAllUnits();
		virtual std::map<wxString,int> RestrictedUnits() const;

		virtual void OnUnitsyncReloaded( GlobalEvents::GlobalEventData /*data*/ );

		virtual OptionsWrapper& CustomBattleOptions() { return m_opt_wrap; }
		virtual const OptionsWrapper& CustomBattleOptions() const { return m_opt_wrap; }

		virtual bool LoadOptionsPreset( const wxString& name );
		virtual void SaveOptionsPreset( const wxString& name );
		virtual wxString GetCurrentPreset();
		virtual void DeletePreset( const wxString& name );
		virtual wxArrayString GetPresetList();

		virtual std::vector<wxColour> &GetFixColoursPalette( int numteams ) const;
		virtual int GetClosestFixColour(const wxColour &col, const std::vector<int> &excludes, int difference) const;
		virtual wxColour GetFixColour(int i) const;
		virtual wxColour GetFreeColour( User &for_whom ) const;
		wxColour GetFreeColour( User *for_whom = NULL ) const;
		wxColour GetNewColour() const;
		int ColourDifference(const wxColour &a, const wxColour &b)  const;

		User& GetFounder() const { return GetUser( m_opts.founder ); }

		bool IsFull() const { return GetMaxPlayers() == GetNumActivePlayers(); }

		virtual unsigned int GetNumPlayers() const;
		virtual unsigned int GetNumActivePlayers() const;

		virtual unsigned int GetNumReadyPlayers() const { return m_players_ready; }
		virtual unsigned int GetNumSyncedPlayers() const { return m_players_sync; }
		virtual unsigned int GetNumOkPlayers() const { return m_players_ok; }

		virtual int GetBattleId() const { return m_opts.battleid; }

		virtual bool GetGUIListActiv() const { return m_opts.guilistactiv; }
		virtual void SetGUIListActiv(bool Activ) { m_opts.guilistactiv = Activ; }

		virtual void SetInGame( bool ingame );
		virtual bool GetInGame() const { return m_ingame; }

		virtual void SetBattleType( BattleType type ) { m_opts.battletype = type; }
		virtual BattleType GetBattleType() { return m_opts.battletype; }

		virtual void SetIsLocked( const bool islocked ) { m_opts.islocked = islocked; }
		virtual bool IsLocked() const { return m_opts.islocked; }
		virtual void SetIsPassworded( const bool ispassworded ) { m_opts.ispassworded = ispassworded; }
		virtual bool IsPassworded() const { return m_opts.ispassworded; }

		virtual void SetNatType( const NatType nattype ) { m_opts.nattype = nattype; }
		virtual NatType GetNatType() const { return m_opts.nattype; }
		virtual void SetHostPort( unsigned int port) { m_opts.port = port; }

		virtual void SetMyExternalUdpSourcePort(unsigned int port){m_opts.externaludpsourceport=port;}
		virtual unsigned int GetMyExternalUdpSourcePort(){return m_opts.externaludpsourceport;}

		virtual void SetMyInternalUdpSourcePort(unsigned int port){m_opts.internaludpsourceport=port;}
		virtual unsigned int GetMyInternalUdpSourcePort(){return m_opts.internaludpsourceport;}

		virtual int GetHostPort() const { return m_opts.port; }
		virtual void SetFounder( const wxString& nick ) { m_opts.founder = nick; }
		virtual void SetHostIp( const wxString& ip ) { m_opts.ip = ip; }
		virtual wxString GetHostIp() const { return m_opts.ip; }

		virtual void SetMaxPlayers( const int& maxplayers ) { m_opts.maxplayers = maxplayers; }
		virtual unsigned int GetMaxPlayers() const { return m_opts.maxplayers; }
		virtual void SetSpectators( const int& spectators ) { m_opts.spectators = spectators; }
		virtual int GetSpectators() const { return m_opts.spectators; }

		virtual void SetRankNeeded( const int& rankneeded ) { m_opts.rankneeded = rankneeded; }
		virtual int GetRankNeeded() const { return m_opts.rankneeded; }

		// virtual void SetMapHash( const wxString& maphash ) { m_opts.maphash = maphash; }
		// virtual void SetMapname( const wxString& map ) { m_opts.mapname = map; }
		virtual void SetDescription( const wxString& desc ) { m_opts.description = desc; }
		virtual wxString GetDescription() const { return m_opts.description; }
		// virtual void SetModname( const wxString& mod ) { m_opts.modname = mod; }


		typedef std::map<wxString, User> UserVec;
		typedef UserVec::const_iterator UserVecCIter;
		typedef UserVec::iterator UserVecIter;

		void SetBattleOptions( const BattleOptions& options ) { m_opts = options; }

		virtual void OnSelfLeftBattle();

		/// replay&savegame parsing stuff
		typedef std::map<int, TeamInfoContainer> TeamVec;
		typedef TeamVec::const_iterator TeamVecCIter;
		typedef TeamVec::iterator TeamVecIter;

		typedef std::map<int, AllyInfoContainer> AllyVec;
		typedef AllyVec::const_iterator AllyVecCIter;
		typedef AllyVec::iterator AllyVecIter;

		TeamVec GetParsedTeamsVec() { return m_parsed_teams; }
		AllyVec GetParsedAlliesVec() { return m_parsed_allies; }

		void SetParsedTeamsVec( const TeamVec& t ) { m_parsed_teams = t; }
		void SetParsedAlliesVec( const AllyVec& a ) { m_parsed_allies = a; }

		const BattleOptions& GetBattleOptions() const { return m_opts; }

		bool Equals( const IBattle& other ) const { return m_opts.battleid == other.GetBattleOptions().battleid; }

		virtual void DisableHostStatusInProxyMode( bool value ) { m_generating_script = value; }

		virtual void UserPositionChanged( const User& usr );

		virtual void SetScript( const wxString& script ) { m_script = script; }
		virtual void AppendScriptLine( const wxString& line ) { m_script << line; }
		virtual void ClearScript() { m_script.Clear(); }
		virtual wxString GetScript() const { return m_script; }

		virtual void SetPlayBackFilePath( const wxString& path ) { m_playback_file_path = path; }
		virtual wxString GetPlayBackFilePath() const { return m_playback_file_path; }

		virtual void AddUserFromDemo( User& user );

		virtual void GetBattleFromScript( bool loadmapmod );

		virtual bool ShouldAutoStart() const;

		virtual void StartSpring() = 0;

		virtual std::map<int, int> GetAllySizes() { return m_ally_sizes; }
		virtual std::map<int, int> GetTeamSizes() { return m_teams_sizes; }

		std::map<wxString, wxString> m_script_tags; // extra script tags to reload in the case of map/mod reload

		virtual long GetBattleRunningTime() const; // returns 0 if not started

	protected:

		void LoadScriptMMOpts( const wxString& sectionname, const SL::PDataList& node );
		void LoadScriptMMOpts( const SL::PDataList& node );

		void PlayerLeftTeam( int team );
		void PlayerLeftAlly( int ally );
		void PlayerJoinedTeam( int team );
		void PlayerJoinedAlly( int ally );

		bool m_map_loaded;
		bool m_mod_loaded;
		bool m_map_exists;
		bool m_mod_exists;
		UnitSyncMap m_local_map;
		UnitSyncMod m_local_mod;
		UnitSyncMap m_host_map;
		UnitSyncMod m_host_mod;
		wxString m_previous_local_mod_name;

		std::map<wxString, int> m_restricted_units;

		OptionsWrapper m_opt_wrap;

		BattleOptions m_opts;

		bool m_ingame;

		bool m_auto_unspec; // unspec as soon as there's a free slot

		bool m_generating_script;

		std::map<unsigned int,BattleStartRect> m_rects;

		std::map<wxString, time_t> m_ready_up_map; // player name -> time counting from join/unspect

		unsigned int m_players_ready;
		unsigned int m_players_sync;
		unsigned int m_players_ok; // players which are ready and in sync
		std::map<int, int> m_teams_sizes; // controlteam -> number of people in
		std::map<int, int> m_ally_sizes; // allyteam -> number of people in

		wxString m_preset;

		bool m_is_self_in;
		UserVec m_internal_bot_list;

		/// replay&savegame stuff
		wxString m_script;
		wxString m_playback_file_path;
		TeamVec m_parsed_teams;
		AllyVec m_parsed_allies;
		UserVec m_internal_user_list; /// to store users from savegame/replay

		wxTimer* m_timer;

		long m_start_time;

	};

} // namespace Battle
} // namespace LSL

#endif // LIBSPRINGLOBBY_HEADERGUARD_IBATTLE_H
