#ifndef	__HEADER_PET_SYSTEM__
#define	__HEADER_PET_SYSTEM__


class CHARACTER;

// TODO: ЖкАё·Ој­АЗ ґЙ·ВДЎ? ¶уґш°Ў ДЈ№Рµµ, №и°нЗД ±вЕёµоµо... јцДЎ
struct SPetAbility
{
};

/**
*/
class CPetActor //: public CHARACTER
{
public:
	enum EPetOptions
	{
		EPetOption_Followable		= 1 << 0,
		EPetOption_Mountable		= 1 << 1,
		EPetOption_Summonable		= 1 << 2,
		EPetOption_Combatable		= 1 << 3,		
	};


protected:
	friend class CPetSystem;

	CPetActor(LPCHARACTER owner, DWORD vnum, DWORD options = EPetOption_Followable | EPetOption_Summonable);
//	CPetActor(LPCHARACTER owner, DWORD vnum, const SPetAbility& petAbility, DWORD options = EPetOption_Followable | EPetOption_Summonable);

	virtual ~CPetActor();

	virtual bool	Update(DWORD deltaTime);

protected:
	virtual bool	_UpdateFollowAI();				///< БЦАОА» µы¶уґЩґПґВ AI Гіё®
	virtual bool	_UpdatAloneActionAI(float fMinDist, float fMaxDist);			///< БЦАО ±ЩГіїЎј­ ИҐАЪ ілґВ AI Гіё®

	/// @TODO
	//virtual bool	_UpdateCombatAI();

private:
	bool Follow(float fMinDistance = 50.f);

public:
	LPCHARACTER		GetCharacter()	const					{ return m_pkChar; }
	LPCHARACTER		GetOwner()	const						{ return m_pkOwner; }
	DWORD			GetVID() const							{ return m_dwVID; }
	DWORD			GetVnum() const							{ return m_dwVnum; }

	bool			HasOption(EPetOptions option) const		{ return m_dwOptions & option; }

	void			SetName(const char* petName);

	bool			Mount();
	void			Unmount();

	DWORD			Summon(const char* petName, LPITEM pSummonItem, bool bSpawnFar = false);
	void			Unsummon();

	bool			IsSummoned() const			{ return 0 != m_pkChar; }
	void			SetSummonItem (LPITEM pItem);
	DWORD			GetSummonItemVID () { return m_dwSummonItemVID; }
	// №цЗБ БЦґВ ЗФјцїН °ЕµОґВ ЗФјц.
	// АМ°Ф Б» ±«¶цЗС°Ф, ј­№ц°Ў ¤ґ¶уј­, 
	// POINT_MOV_SPEED, POINT_ATT_SPEED, POINT_CAST_SPEEDґВ PointChange()¶х ЗФјцёё Ѕбј­ єЇ°жЗШ єБѕЯ јТїлАМ ѕшґВ°Ф,
	// PointChange() АМИДїЎ ѕоµрј±°Ў ComputePoints()ё¦ ЗПёй ЅПґЩ ГК±вИ­µЗ°н, 
	// ґх їф±д°З, ComputePoints()ё¦ єОёЈБц ѕКАёёй Е¬¶уАЗ POINTґВ АьЗф єЇЗПБц ѕКґВґЩґВ °ЕґЩ.
	// ±Ч·Ўј­ №цЗБё¦ БЦґВ °НАє ComputePoints() і»єОїЎј­ petsystem->RefreshBuff()ё¦ єОёЈµµ·П ЗПїґ°н,
	// №цЗБё¦ »©ґВ °НАє ClearBuff()ё¦ єОёЈ°н, ComputePointsё¦ ЗПґВ °НАё·О ЗСґЩ.
	void			GiveBuff();
	void			ClearBuff();

private:
	DWORD			m_dwVnum;
	DWORD			m_dwVID;
	DWORD			m_dwOptions;
	DWORD			m_dwLastActionTime;
	DWORD			m_dwSummonItemVID;
	DWORD			m_dwSummonItemVnum;

	short			m_originalMoveSpeed;

	std::string		m_name;

	LPCHARACTER		m_pkChar;					// Instance of pet(CHARACTER)
	LPCHARACTER		m_pkOwner;

//	SPetAbility		m_petAbility;				// ґЙ·ВДЎ
};

/**
*/
class CPetSystem
{
public:
	typedef	boost::unordered_map<DWORD,	CPetActor*>		TPetActorMap;		/// <VNUM, PetActor> map. (ЗС ДіёЇЕН°Ў °°Ає vnumАЗ ЖкА» ї©·Ї°і °ЎБъ АПАМ АЦА»±о..??)

public:
	CPetSystem(LPCHARACTER owner);
	virtual ~CPetSystem();

	CPetActor*	GetByVID(DWORD vid) const;
	CPetActor*	GetByVnum(DWORD vnum) const;

	bool		Update(DWORD deltaTime);
	void		Destroy();

	size_t		CountSummoned() const;			///< ЗцАз јТИЇµИ(ЅЗГјИ­ µИ ДіёЇЕН°Ў АЦґВ) ЖкАЗ °іјц

public:
	void		SetUpdatePeriod(DWORD ms);

	CPetActor*	Summon(DWORD mobVnum, LPITEM pSummonItem, const char* petName, bool bSpawnFar, DWORD options = CPetActor::EPetOption_Followable | CPetActor::EPetOption_Summonable);

	void		Unsummon(DWORD mobVnum, bool bDeleteFromList = false);
	void		Unsummon(CPetActor* petActor, bool bDeleteFromList = false);

	// TODO: БшВҐ Жк ЅГЅєЕЫАМ µйѕо°Ґ ¶§ ±ёЗц. (ДіёЇЕН°Ў єёАЇЗС ЖкАЗ Б¤єёё¦ ГЯ°ЎЗТ ¶§ ¶уґш°Ў...)
	CPetActor*	AddPet(DWORD mobVnum, const char* petName, const SPetAbility& ability, DWORD options = CPetActor::EPetOption_Followable | CPetActor::EPetOption_Summonable | CPetActor::EPetOption_Combatable);

	void		DeletePet(DWORD mobVnum);
	void		DeletePet(CPetActor* petActor);
	void		RefreshBuff();

private:
	TPetActorMap	m_petActorMap;
	LPCHARACTER		m_pkOwner;					///< Жк ЅГЅєЕЫАЗ Owner
	DWORD			m_dwUpdatePeriod;			///< ѕчµҐАМЖ® БЦ±в (msґЬА§)
	DWORD			m_dwLastUpdateTime;
	LPEVENT			m_pkPetSystemUpdateEvent;
};

/**
// Summon Pet
CPetSystem* petSystem = mainChar->GetPetSystem();
CPetActor* petActor = petSystem->Summon(~~~);

DWORD petVID = petActor->GetVID();
if (0 == petActor)
{
	ERROR_LOG(...)
};


// Unsummon Pet
petSystem->Unsummon(petVID);

// Mount Pet
petActor->Mount()..


CPetActor::Update(...)
{
	// AI : Follow, actions, etc...
}

*/



#endif	//__HEADER_PET_SYSTEM__