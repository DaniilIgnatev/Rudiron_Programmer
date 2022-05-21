#ifndef TYPEDEFS_HPP
#define TYPEDEFS_HPP

#include <QDir>
#include <QString>
#include <QFile>
#include <QIODevice>

#define CONST               const

typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef float               FLOAT;
typedef FLOAT               *PFLOAT;

typedef int                 INT;
typedef unsigned int        UINT;
typedef unsigned int        *PUINT;

typedef char CHAR;
typedef short SHORT;
typedef long LONG;

//
// ANSI (Multi-byte Character) types
//
typedef CHAR *PCHAR, *LPCH, *PCH;
typedef CONST CHAR *LPCCH, *PCCH;

typedef CHAR *NPSTR, *LPSTR, *PSTR;
typedef PSTR *PZPSTR;
typedef CONST PSTR *PCZPSTR;
typedef CONST CHAR *LPCSTR, *PCSTR;
typedef PCSTR *PZPCSTR;
typedef CONST PCSTR *PCZPCSTR;

typedef CHAR *PZZSTR;
typedef CONST CHAR *PCZZSTR;

typedef  CHAR *PNZCH;
typedef  CONST CHAR *PCNZCH;

#endif // TYPEDEFS_HPP
