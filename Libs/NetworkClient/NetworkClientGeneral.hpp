#ifndef NETWORKCLIENTGENERAL_HPP
#define NETWORKCLIENTGENERAL_HPP

#include <QtGlobal>

//----------------------------------------------------------------------------//

#ifdef NETWORK_CLIENT_LIBRARY
#define NETWORK_CLIENT_EXPORTED Q_DECL_EXPORT
#else
#define NETWORK_CLIENT_EXPORTED Q_DECL_IMPORT
#endif

//----------------------------------------------------------------------------//

#endif // NETWORKCLIENTGENERAL_HPP
