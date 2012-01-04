#include "user.h"

namespace LSL {

void User::Said( const std::string& /*message*/ ) const
{
}

void User::Say( const std::string& message ) const
{
  GetServer().SayPrivate( m_nick, message );
}

void User::DoAction( const std::string& message ) const
{
  GetServer().DoActionPrivate( m_nick, message );
}

Battle* User::GetBattle() const
{
  return m_battle;
}

void User::SetBattle( Battle* battle )
{
  m_battle = battle;
  m_statusicon_idx = icons().GetUserListStateIcon( m_status, false, m_battle != 0 );
}

void User::SetStatus( const UserStatus& status )
{
  m_status = status;
  // If user is host of a game, then his in_game status tells if the game is on!
  if ( m_battle != 0 ) {
	try
	{
			User& user = m_battle->GetFounder();
			if ( user.GetNick() == m_nick ) {
				m_battle->Update();
			}
	}catch(...){}
  }

  m_statusicon_idx = icons().GetUserListStateIcon( m_status, false, m_battle != 0 );
  m_rankicon_idx =  icons().GetRankIcon( m_status.rank );
}

void User::SetCountry( const std::string& country )
{
	m_country = country;
	m_flagicon_idx = icons().GetFlagIcon( country );
}

void User::SendMyUserStatus() const
{
  GetServer().SendMyUserStatus();
}

bool User::ExecuteSayCommand( const std::string& cmd ) const
{
  if ( cmd.BeforeFirst(' ').Lower() == _T("/me") ) {
	GetServer().DoActionPrivate( m_nick, cmd.AfterFirst(' ') );
	return true;
  }  else return false;
}

UserStatus::RankContainer User::GetRank()
{
	return GetStatus().rank;
}

std::string User::GetRankName(UserStatus::RankContainer rank)
{
  //TODO: better interface to ranks?
	  switch( rank )
	  {
		  case UserStatus::RANK_1: return _("Newbie");
		  case UserStatus::RANK_2: return _("Beginner");
		  case UserStatus::RANK_3: return _("Average");
		  case UserStatus::RANK_4: return _("Above average");
		  case UserStatus::RANK_5: return _("Experienced");
		  case UserStatus::RANK_6: return _("Highly experienced");
		  case UserStatus::RANK_7: return _("Veteran");
		  case UserStatus::RANK_8: return _("Badly needs to get laid");
		  default:                 return _("Unknown");
	  }
}

float User::GetBalanceRank()
{
  return 1.0 + 0.1 * float( GetStatus().rank - UserStatus::RANK_1 ) / float( UserStatus::RANK_8 - UserStatus::RANK_1 );
}

std::string User::GetClan()
{
  std::string tmp = m_nick.AfterFirst('[');
  if ( tmp != m_nick )
  {
	std::string clan = tmp.BeforeFirst(']');
	if ( clan != tmp ) return clan;
  }
  return _T("");
}

User::~User()
{
}

} // namespace LSL {
