# ifndef EVENT_H
# define EVENT_H


# include "FastDelegate.h"
# include <list>

using namespace fastdelegate;



// The Angel3D namespace
namespace A3D
{


/// Defines a generic delegate list type.
typedef std::list< DelegateMemento > DelegateList;

/// Defines a handle type used for identifying a delegate in an event.
typedef DelegateList::iterator HDELEGATE;


template <class Delegate>
class AEvent
{
protected:
	DelegateList m_delegates;

public:

	typedef Delegate DelegateType;

	/// Returns the list of delegates for iteration. Use the INVOKE_DELEGATE macro
	/// if you don't need to inspect return values.
	DelegateList& GetDelegateList() { return m_delegates; }

	/// Inserts a delegate at the end of the list.
	HDELEGATE InsertDelegate( typename Delegate::type dlgt )
	{ return m_delegates.insert( m_delegates.end(), dlgt.GetMemento() ); }

	/// Returns a delegate from a delegate handle, ready for invocation.
	DelegateType GetDelegate( HDELEGATE hDG )
	{ DelegateType dlgt; dlgt.SetMemento( *hDG ); return dlgt; }

	/// Removes the delegate identified by the specified delegate handle (no search required).
	void RemoveDelegate( HDELEGATE hDG ) { m_delegates.erase( hDG ); }

	/// Searches for and removes the specified delegate. A search is needed to find it in the delegate list.
	bool RemoveDelegate( typename Delegate::type dlgt )
	{
		for( HDELEGATE hdg = m_delegates.begin(); hdg != m_delegates.end(); hdg++ )
		{
			if( (*hdg).IsEqual( dlgt.GetMemento() ) )
			{
				m_delegates.erase( hdg );
				return true;
			}
		}
		return false; // the specified delegate was not found.
	}

	void Clear() { m_delegates.clear(); }

	HDELEGATE	operator += (typename Delegate::type dlgt ) { return InsertDelegate( dlgt ); }
	void		operator -= (HDELEGATE hDG ) { RemoveDelegate(hDG); }
	void		operator -= (typename Delegate::type dlgt ) { RemoveDelegate(dlgt); }

	HDELEGATE GetFirstDelegate() { return m_delegates.begin(); }
	HDELEGATE GetNextDelegate( HDELEGATE hDG ) { HDELEGATE hRet = hDG; return ++hRet; }
	HDELEGATE GetPrevDelegate( HDELEGATE hDG ) { HDELEGATE hRet = hDG; return --hRet; }
	HDELEGATE GetLastDelegate() { return m_delegates.back(); }
	HDELEGATE GetEndDelegate() { return m_delegates.end(); }
};


# define DECLARE_EVENT( retType, ... ) A3D::AEvent< FastDelegate<retType(__VA_ARGS__)> >

# ifndef TRIGGER_EVENT
# define TRIGGER_EVENT( theEvent, ... ) \
	for(A3D::HDELEGATE hDG = theEvent.GetDelegateList().begin(); hDG != theEvent.GetDelegateList().end(); )\
	{ A3D::HDELEGATE hCurDG = hDG++; theEvent.GetDelegate( hCurDG )(__VA_ARGS__); }

# endif


}; // namespace A3D


# endif // inclusion guard