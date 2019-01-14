// filesystem experimental header
#pragma once
#ifndef _FILESYSTEM_
#define _FILESYSTEM_
#ifndef RC_INVOKED
#include <algorithm>	// for replace
#include <chrono>	// for chrono::time_point
#include <codecvt>	// for codecvt_utf8_*
#include <list>		// for recursive_directory_iterator stack
#include <locale>	// for wstring_convert
#include <memory>	// for shared_ptr
#include <vector>	// for canonical's .. handling

 #pragma pack(push,_CRT_PACKING)
 #pragma warning(push,3)
 #pragma push_macro("new")
 #undef new



 #ifndef _FS_DLL
  #define _FS_DLL	extern "C" _CRTIMP2_PURE
 #endif /* _FS_DLL */

 #define _MAX_FILESYS_NAME	260	/* longest Windows or Posix filename + 1 */

_STD_BEGIN
namespace experimental {
	namespace filesystem {
		inline namespace v1 {
typedef wchar_t _Pchar;	// UTF16
#define _FS_ISSEP(x)	((x) == L'/' || (x) == L'\\')
#define _FS_PREF	L'\\'

#define _FS_COLON	L':'
#define _FS_PERIOD	L'.'
#define _FS_SLASH	L'/'
#define _FS_BSLASH	L'\\'

#define _FSPFX	_STD experimental::filesystem::v1::

struct char8_t;	/* flag for UTF8 */

		// ENUM file_type

enum class file_type {	// names for file types
	not_found = -1, none, regular, directory, symlink,
	block, character, fifo, socket, unknown
	};

typedef file_type _File_type;

		// ENUM copy_options

enum class copy_options {	// names for copy options
	none = 0,
	skip_existing = 1, overwrite_existing = 2, update_existing = 4,
	recursive = 8,
	copy_symlinks = 16, skip_symlinks = 32,
	directories_only = 64, create_symlinks = 128, create_hard_links = 256,
	_Unspecified_recursion_prevention_tag = 512
	};

typedef copy_options _Copy_options;

_BITMASK_OPS(copy_options)

		// ENUM dirctory_options

enum class directory_options {	// names for directory options
	none = 0, follow_directory_symlink
	};

typedef directory_options _Directory_options;

		// ENUM perms

enum class perms {	// names for permissions
	none = 0,
	owner_read = 0400,	// S_IRUSR
	owner_write = 0200,	// S_IWUSR
	owner_exec = 0100,	// S_IXUSR
	owner_all = 0700,	// S_IRWXU
	group_read = 040,	// S_IRGRP
	group_write = 020,	// S_IWGRP
	group_exec = 010,	// S_IXGRP
	group_all = 070,	// S_IRWXG
	others_read = 04,	// S_IROTH
	others_write = 02,	// S_IWOTH
	others_exec = 01,	// S_IXOTH
	others_all = 07,	// S_IRWXO
	all = 0777,
	set_uid = 04000,	// S_ISUID
	set_gid = 02000,	// S_ISGID
	sticky_bit = 01000,	// S_ISVTX
	mask = 07777,
	unknown = 0xFFFF,
	add_perms = 0x10000,
	remove_perms = 0x20000,
	resolve_symlinks = 0x40000
	};

typedef perms _Perms;

_BITMASK_OPS(perms)

		// CLASS file_status
class file_status
	{	// stores file status
public:
	explicit file_status(
		_File_type _Ftype = _File_type::none,
			_Perms _Prms = perms::unknown) _NOEXCEPT
		: _Myftype(_Ftype), _Myperms(_Prms)
		{	// construct with optional arguments
		}

	file_status(const file_status&) _NOEXCEPT = default;
	~file_status() _NOEXCEPT = default;
	file_status& operator=(const file_status&) _NOEXCEPT = default;

	file_status(file_status&&) = default;
	file_status& operator=(file_status&&) = default;

	_File_type type() const _NOEXCEPT
		{	// get file type
		return (_Myftype);
		}

	_Perms permissions() const _NOEXCEPT
		{	// get file permissions
		return (_Myperms);
		}

	void type(_File_type _Ftype) _NOEXCEPT
		{	// set file type
		_Myftype = _Ftype;
		}

	void permissions(_Perms _Prms) _NOEXCEPT
		{	// set permissions
		_Myperms = _Prms;
		}

private:
	_File_type _Myftype;
	_Perms _Myperms;
	};

		// STRUCT space_info
struct space_info
	{	// space information for a file
	uintmax_t capacity;
	uintmax_t free;
	uintmax_t available;
	};

		// SYSTEM SUPPORT FUNCTIONS
	// wide filenames
_FS_DLL void *__CLRCALL_PURE_OR_CDECL _Open_dir(wchar_t (&)[_MAX_FILESYS_NAME],
	const wchar_t *, int&, _File_type&);
_FS_DLL wchar_t *__CLRCALL_PURE_OR_CDECL _Read_dir(wchar_t (&)[_MAX_FILESYS_NAME],
	void *, _File_type&);
_FS_DLL void __CLRCALL_PURE_OR_CDECL _Close_dir(void *);
_FS_DLL bool __CLRCALL_PURE_OR_CDECL _Current_get(wchar_t (&)[_MAX_FILESYS_NAME]);
_FS_DLL bool __CLRCALL_PURE_OR_CDECL _Current_set(const wchar_t *);
_FS_DLL wchar_t *__CLRCALL_PURE_OR_CDECL _Symlink_get(wchar_t (&)[_MAX_FILESYS_NAME],
	const wchar_t *);
_FS_DLL wchar_t *__CLRCALL_PURE_OR_CDECL _Temp_get(wchar_t (&)[_MAX_FILESYS_NAME]);

_FS_DLL int __CLRCALL_PURE_OR_CDECL _Make_dir(const wchar_t *, const wchar_t *);
_FS_DLL bool __CLRCALL_PURE_OR_CDECL _Remove_dir(const wchar_t *);

_FS_DLL _File_type __CLRCALL_PURE_OR_CDECL _Stat(const wchar_t *, _Perms *);
_FS_DLL _File_type __CLRCALL_PURE_OR_CDECL _Lstat(const wchar_t *, _Perms *);
_FS_DLL uintmax_t __CLRCALL_PURE_OR_CDECL _File_size(const wchar_t *);
_FS_DLL uintmax_t __CLRCALL_PURE_OR_CDECL _Hard_links(const wchar_t *);
_FS_DLL int64_t __CLRCALL_PURE_OR_CDECL _Last_write_time(const wchar_t *);
_FS_DLL int __CLRCALL_PURE_OR_CDECL _Set_last_write_time(const wchar_t *, int64_t);
_FS_DLL space_info __CLRCALL_PURE_OR_CDECL _Statvfs(const wchar_t *);
_FS_DLL int __CLRCALL_PURE_OR_CDECL _Equivalent(const wchar_t *, const wchar_t *);

_FS_DLL int __CLRCALL_PURE_OR_CDECL _Link(const wchar_t *, const wchar_t *);
_FS_DLL int __CLRCALL_PURE_OR_CDECL _Symlink(const wchar_t *, const wchar_t *);
_FS_DLL int __CLRCALL_PURE_OR_CDECL _Rename(const wchar_t *, const wchar_t *);
_FS_DLL int __CLRCALL_PURE_OR_CDECL _Resize(const wchar_t *, uintmax_t);
_FS_DLL int __CLRCALL_PURE_OR_CDECL _Unlink(const wchar_t *);
_FS_DLL int __CLRCALL_PURE_OR_CDECL _Copy_file(const wchar_t *, const wchar_t *);
_FS_DLL int __CLRCALL_PURE_OR_CDECL _Chmod(const wchar_t *, _Perms);

		// TEMPLATE STRUCT _Path_cvt
template<class _Inchar,
	class _Outchar,
	class _Outtraits = char_traits<_Outchar>,
	class _Outalloc = allocator<_Outchar> >
	struct _Path_cvt
	{	// converts among {char, wchar_t, UTF8, char16_t, char32_t} paths
	typedef basic_string<_Outchar, _Outtraits, _Outalloc> _Outstr;
	static _Outstr&& _Cvt(_Outstr& _Str,
		const _Inchar *_First, size_t _Size,
			const locale& = locale())
		{	// convert _Inchar sequence to _Outchar one-to-one
		for (; 0 < _Size; --_Size, ++_First)
			_Str.append(1, (_Outchar)*_First);
		return (_STD move(_Str));
		}
	};

template<class _Outchar,
	class _Outtraits,
	class _Outalloc>
	struct _Path_cvt<_Outchar, _Outchar, _Outtraits, _Outalloc>
	{	// converts among {char, wchar_t, UTF8, char16_t, char32_t} paths
	typedef basic_string<_Outchar, _Outtraits, _Outalloc> _Outstr;
	static _Outstr&& _Cvt(_Outstr& _Str,
		const _Outchar *_First, size_t _Size,
			const locale& = locale())
		{	// copy _Outchar sequence to _Outchar one-to-one
		return (_STD move(_Str.append(_First, _First + _Size)));
		}
	};

template<class _Outtraits,
	class _Outalloc>
	struct _Path_cvt<char, char16_t, _Outtraits, _Outalloc>
	{	// converts among {char, wchar_t, UTF8, char16_t, char32_t} paths
	typedef char _Inchar;
	typedef char16_t _Outchar;
	typedef basic_string<_Outchar, _Outtraits, _Outalloc> _Outstr;
	static _Outstr&& _Cvt(_Outstr& _Str,
		const _Inchar *_First, size_t _Size,
			const locale& = locale())
		{	// convert char sequence to char16_t string
		wstring_convert<codecvt_utf8_utf16<_Outchar>, _Outchar> _Wcvt;
		return (_STD move(
			_Str.append(_Wcvt.from_bytes(_First, _First + _Size))));
		}
	};

template<class _Outtraits,
	class _Outalloc>
	struct _Path_cvt<char16_t, char, _Outtraits, _Outalloc>
	{	// converts among {char, wchar_t, UTF8, char16_t, char32_t} paths
	typedef char16_t _Inchar;
	typedef char _Outchar;
	typedef basic_string<_Outchar, _Outtraits, _Outalloc> _Outstr;
	static _Outstr&& _Cvt(_Outstr& _Str,
		const _Inchar *_First, size_t _Size,
			const locale& = locale())
		{	// convert char16_t sequence to char string
		wstring_convert<codecvt_utf8_utf16<_Inchar>, _Inchar> _Wcvt;
		return (_STD move(
			_Str.append(_Wcvt.to_bytes(_First, _First + _Size))));
		}
	};

template<class _Outtraits,
	class _Outalloc>
	struct _Path_cvt<char, char32_t, _Outtraits, _Outalloc>
	{	// converts among {char, wchar_t, UTF8, char16_t, char32_t} paths
	typedef char _Inchar;
	typedef char32_t _Outchar;
	typedef basic_string<_Outchar, _Outtraits, _Outalloc> _Outstr;
	static _Outstr&& _Cvt(_Outstr& _Str,
		const _Inchar *_First, size_t _Size,
			const locale& = locale())
		{	// convert char sequence to char32_t string
		wstring_convert<codecvt_utf8<_Outchar>, _Outchar> _Wcvt;
		return (_STD move(
			_Str.append(_Wcvt.from_bytes(_First, _First + _Size))));
		}
	};

template<class _Outtraits,
	class _Outalloc>
	struct _Path_cvt<char32_t, char, _Outtraits, _Outalloc>
	{	// converts among {char, wchar_t, UTF8, char16_t, char32_t} paths
	typedef char32_t _Inchar;
	typedef char _Outchar;
	typedef basic_string<_Outchar, _Outtraits, _Outalloc> _Outstr;
	static _Outstr&& _Cvt(_Outstr& _Str,
		const _Inchar *_First, size_t _Size,
			const locale& = locale())
		{	// convert char32_t sequence to char string
		wstring_convert<codecvt_utf8<_Inchar>, _Inchar> _Wcvt;
		return (_STD move(
			_Str.append(_Wcvt.to_bytes(_First, _First + _Size))));
		}
	};

_FS_DLL int __CLRCALL_PURE_OR_CDECL _To_byte(const wchar_t *, char *);
_FS_DLL int __CLRCALL_PURE_OR_CDECL _To_wide(const char *, wchar_t *);

template<class _Outtraits,
	class _Outalloc>
	struct _Path_cvt<char, wchar_t, _Outtraits, _Outalloc>
	{	// converts among {char, wchar_t, UTF8, char16_t, char32_t} paths
	typedef char _Inchar;
	typedef wchar_t _Outchar;
	typedef basic_string<_Outchar, _Outtraits, _Outalloc> _Outstr;
	static _Outstr&& _Cvt(_Outstr& _Str,
		const _Inchar *_First, size_t)
		{	// convert char sequence to wchar_t string -- Windows
		_Outchar _Fname_wide[_MAX_FILESYS_NAME];
		if (_To_wide(_First, _Fname_wide) == 0)
			_Xinvalid_argument("invalid char filename argument");
		return (_STD move(_Str.append(&_Fname_wide[0])));
		}

	static _Outstr&& _Cvt(_Outstr& _Str,
		const _Inchar *_First, size_t _Size,
			const locale& _Loc)
		{	// convert char sequence to wchar_t string, locale -- Windows
		typedef codecvt<_Outchar, _Inchar, _Mbstatet> _Mycvt;
		wstring_convert<_Mycvt> _Wcvt(&_USE(_Loc, _Mycvt));
		return (_STD move(
			_Str.append(_Wcvt.from_bytes(_First, _First + _Size))));
		}
	};

template<class _Outtraits,
	class _Outalloc>
	struct _Path_cvt<wchar_t, char, _Outtraits, _Outalloc>
	{	// converts among {char, wchar_t, UTF8, char16_t, char32_t} paths
	typedef wchar_t _Inchar;
	typedef char _Outchar;
	typedef basic_string<_Outchar, _Outtraits, _Outalloc> _Outstr;
	static _Outstr&& _Cvt(_Outstr& _Str,
		const _Inchar *_First, size_t)
		{	// convert wchar_t sequence to char string -- Windows
		_Outchar _Fname_byte[_MAX_FILESYS_NAME];
		if (_To_byte(_First, _Fname_byte) == 0)
			_Xinvalid_argument("invalid wchar_t filename argument");
		return (_STD move(_Str.append(&_Fname_byte[0])));
		}

	static _Outstr&& _Cvt(_Outstr& _Str,
		const _Inchar *_First, size_t _Size,
			const locale& _Loc)
		{	// convert wchar_t sequence to char string, locale -- Windows
		typedef codecvt<_Inchar, _Outchar, _Mbstatet> _Mycvt;
		wstring_convert<_Mycvt> _Wcvt(&_USE(_Loc, _Mycvt));
		return (_STD move(
			_Str.append(_Wcvt.to_bytes(_First, _First + _Size))));
		}
	};

template<>
	struct _Path_cvt<char, char8_t, char_traits<char>, allocator<char> >
	{	// converts among {char, wchar_t, UTF8, char16_t, char32_t} paths
	typedef char _Inchar;
	typedef char _Outchar;
	typedef char_traits<_Outchar> _Outtraits;
	typedef allocator<_Outchar> _Outalloc;
	typedef basic_string<_Outchar, _Outtraits, _Outalloc> _Outstr;
	static _Outstr&& _Cvt(_Outstr& _Str,
		const _Inchar *_First, size_t,
			const locale& = locale())
		{	// convert char sequence to UTF8 string -- Windows
		wchar_t _Fname_wide[_MAX_FILESYS_NAME];
		if (_To_wide(_First, _Fname_wide) == 0)
			_Xinvalid_argument("invalid char filename argument");

		wstring_convert<codecvt_utf8<wchar_t>, wchar_t> _Wcvt;
		wchar_t *_Last1 = &_Fname_wide[0];
		for (; *_Last1 != wchar_t(0); ++_Last1)
			;
		return (_STD move(
			_Str.append(_Wcvt.to_bytes(&_Fname_wide[0], _Last1))));
		}
	};

template<class _Outtraits,
	class _Outalloc>
	struct _Path_cvt<char8_t, char, _Outtraits, _Outalloc>
	{	// converts among {char, wchar_t, UTF8, char16_t, char32_t} paths
	typedef char _Inchar;
	typedef char _Outchar;
	typedef basic_string<_Outchar, _Outtraits, _Outalloc> _Outstr;
	static _Outstr&& _Cvt(_Outstr& _Str,
		const _Inchar *_First, size_t _Size,
			const locale& = locale())
		{	// convert UTF8 sequence to char string -- Windows
		wstring_convert<codecvt_utf8_utf16<wchar_t>, wchar_t> _Wcvt;
		wstring _Str0(_Wcvt.from_bytes(_First, _First + _Size));

		_Outchar _Fname_byte[_MAX_FILESYS_NAME];
		if (_To_byte(_Str0.c_str(), _Fname_byte) == 0)
			_Xinvalid_argument("invalid UTF8 filename argument");
		return (_STD move(_Str.append(&_Fname_byte[0])));
		}
	};

template<>
	struct _Path_cvt<wchar_t, char8_t, char_traits<char>, allocator<char> >
	{	// converts among {char, wchar_t, UTF8, char16_t, char32_t} paths
	typedef wchar_t _Inchar;
	typedef char _Outchar;
	typedef char_traits<_Outchar> _Outtraits;
	typedef allocator<_Outchar> _Outalloc;
	typedef basic_string<_Outchar, _Outtraits, _Outalloc> _Outstr;
	static _Outstr&& _Cvt(_Outstr& _Str,
		const _Inchar *_First, size_t _Size,
			const locale& = locale())
		{	// convert wchar_t sequence to UTF8 string -- Windows
		wstring_convert<codecvt_utf8<_Inchar>, _Inchar> _Wcvt;
		return (_STD move(
			_Str.append(_Wcvt.to_bytes(_First, _First + _Size))));
		}
	};

template<class _Outtraits,
	class _Outalloc>
	struct _Path_cvt<char8_t, wchar_t, _Outtraits, _Outalloc>
	{	// converts among {char, wchar_t, UTF8, char16_t, char32_t} paths
	typedef char _Inchar;
	typedef wchar_t _Outchar;
	typedef basic_string<_Outchar, _Outtraits, _Outalloc> _Outstr;
	static _Outstr&& _Cvt(_Outstr& _Str,
		const _Inchar *_First, size_t _Size,
			const locale& = locale())
		{	// convert UTF8 sequence to wchar_t string -- Windows
		wstring_convert<codecvt_utf8<_Outchar>, _Outchar> _Wcvt;
		return (_STD move(
			_Str.append(_Wcvt.from_bytes(_First, _First + _Size))));
		}
	};

//	struct _Path_cvt<wchar_t, char16_t, _Outtraits, _Outalloc> Windows (copy)
//	struct _Path_cvt<char16_t, wchar_t, _Outtraits, _Outalloc> Windows (copy)

template<class _Outtraits,
	class _Outalloc>
	struct _Path_cvt<wchar_t, char32_t, _Outtraits, _Outalloc>
	{	// converts among {char, wchar_t, UTF8, char16_t, char32_t} paths
	typedef wchar_t _Inchar;
	typedef char32_t _Outchar;
	typedef basic_string<_Outchar, _Outtraits, _Outalloc> _Outstr;
	static _Outstr&& _Cvt(_Outstr& _Str,
		const _Inchar *_First, size_t _Size,
			const locale& = locale())
		{	// convert wchar_t sequence to char32_t string -- Windows
		wstring_convert<codecvt_utf8_utf16<_Inchar>, _Inchar> _Wcvt0;
		_STD string _Str0(_Wcvt0.to_bytes(_First, _First + _Size));

		wstring_convert<codecvt_utf8<_Outchar>, _Outchar> _Wcvt;
		return (_STD move(
			_Str.append(_Wcvt.from_bytes(_Str0))));
		}
	};

template<class _Outtraits,
	class _Outalloc>
	struct _Path_cvt<char32_t, wchar_t, _Outtraits, _Outalloc>
	{	// converts among {char, wchar_t, UTF8, char16_t, char32_t} paths
	typedef char32_t _Inchar;
	typedef wchar_t _Outchar;
	typedef basic_string<_Outchar, _Outtraits, _Outalloc> _Outstr;
	static _Outstr&& _Cvt(_Outstr& _Str,
		const _Inchar *_First, size_t _Size,
			const locale& = locale())
		{	// convert char32_t sequence to wchar_t string -- Windows
		wstring_convert<codecvt_utf8<_Inchar>, _Inchar> _Wcvt0;
		_STD string _Str0(_Wcvt0.to_bytes(_First, _First + _Size));

		wstring_convert<codecvt_utf8_utf16<_Outchar>, _Outchar> _Wcvt;
		return (_STD move(
			_Str.append(_Wcvt.from_bytes(_Str0))));
		}
	};

		// FORWARD REFERENCES
class path;
path operator/(const path&, const path&);

path _Absolute(const path&, const path&, error_code&);
//path absolute(const path&, const path& = current_path());

void _Canonicalize_string_only(path&, const path&);
//path canonical(const path&,
//	const path& _Base = current_path());
path canonical(const path&,
	error_code&);
path canonical(const path&, const path&,
	error_code&);

void copy(const path&, const path&);
void copy(const path&, const path&,
	error_code&) _NOEXCEPT;
void copy(const path&, const path&, _Copy_options);
void copy(const path&, const path&, _Copy_options,
	error_code&) _NOEXCEPT;

bool copy_file(const path&, const path&, _Copy_options = copy_options::none);
bool copy_file(const path&, const path&,
	error_code&) _NOEXCEPT;
bool copy_file(const path&, const path&, _Copy_options,
	error_code&) _NOEXCEPT;

void copy_symlink(const path&, const path&);
void copy_symlink(const path&, const path&,
	error_code&) _NOEXCEPT;

bool create_directories(const path&);
bool create_directories(const path&,
	error_code&) _NOEXCEPT;

bool create_directory(const path&);
bool create_directory(const path&,
	error_code&) _NOEXCEPT;
bool create_directory(const path&, const path&);
bool create_directory(const path&, const path&,
	error_code&) _NOEXCEPT;

void create_directory_symlink(const path&, const path&);
void create_directory_symlink(const path&, const path&,
	error_code&) _NOEXCEPT;

void create_hard_link(const path&, const path&);
void create_hard_link(const path&, const path&,
	error_code&) _NOEXCEPT;

void create_symlink(const path&, const path&);
void create_symlink(const path&, const path&,
	error_code&) _NOEXCEPT;

path current_path();
path current_path(
	error_code&);
void current_path(const path&);
void current_path(const path&,
	error_code&) _NOEXCEPT;

bool equivalent(const path&, const path&);
bool equivalent(const path&, const path&,
	error_code&) _NOEXCEPT;

bool exists(file_status) _NOEXCEPT;
bool exists(const path&);
bool exists(const path&,
	error_code&) _NOEXCEPT;

uintmax_t file_size(const path&);
uintmax_t file_size(const path&,
	error_code&) _NOEXCEPT;

uintmax_t hard_link_count(const path&);
uintmax_t hard_link_count(const path&,
	error_code&) _NOEXCEPT;

bool is_block_file(file_status) _NOEXCEPT;
bool is_block_file(const path&);
bool is_block_file(const path&,
	error_code&) _NOEXCEPT;

bool is_character_file(file_status) _NOEXCEPT;
bool is_character_file(const path&);
bool is_character_file(const path&,
	error_code&) _NOEXCEPT;

bool is_directory(file_status) _NOEXCEPT;
bool is_directory(const path&);
bool is_directory(const path&,
	error_code&) _NOEXCEPT;

bool is_empty(const path&);
bool is_empty(const path&,
	error_code&) _NOEXCEPT;

bool is_fifo(file_status) _NOEXCEPT;
bool is_fifo(const path&);
bool is_fifo(const path&,
	error_code&) _NOEXCEPT;

bool is_other(file_status) _NOEXCEPT;
bool is_other(const path&);
bool is_other(const path&,
	error_code&) _NOEXCEPT;

bool is_regular_file(file_status) _NOEXCEPT;
bool is_regular_file(const path&);
bool is_regular_file(const path&,
	error_code&) _NOEXCEPT;

bool is_socket(file_status) _NOEXCEPT;
bool is_socket(const path&);
bool is_socket(const path&,
	error_code&) _NOEXCEPT;

bool is_symlink(file_status) _NOEXCEPT;
bool is_symlink(const path&);
bool is_symlink(const path&,
	error_code&) _NOEXCEPT;

typedef chrono::system_clock::time_point file_time_type;

file_time_type last_write_time(const path&,
	error_code&) _NOEXCEPT;
file_time_type last_write_time(const path&);
void last_write_time(const path&, file_time_type,
	error_code&) _NOEXCEPT;
void last_write_time(const path&, file_time_type);

void permissions(const path&, _Perms);
void permissions(const path&, _Perms,
	error_code&) _NOEXCEPT;

path read_symlink(const path&);
path read_symlink(const path&,
	error_code&);

bool remove(const path&);
bool remove(const path&,
	error_code&) _NOEXCEPT;

uintmax_t remove_all(const path&);
uintmax_t remove_all(const path&,
	error_code&) _NOEXCEPT;

void rename(const path&, const path&);
void rename(const path&, const path&,
	error_code&) _NOEXCEPT;

void resize_file(const path&, uintmax_t);
void resize_file(const path&, uintmax_t,
	error_code&) _NOEXCEPT;

space_info space(const path&);
space_info space(const path&,
	error_code&) _NOEXCEPT;

file_status status(const path&);
file_status status(const path&,
	error_code&) _NOEXCEPT;

bool status_known(file_status) _NOEXCEPT;

file_status symlink_status(const path&);
file_status symlink_status(const path&,
	error_code&) _NOEXCEPT;

path system_complete(const path&);
path system_complete(const path&,
	error_code&);

path temp_directory_path();
path temp_directory_path(
	error_code&);

		// TEMPLATE CLASS _Path_iterator
template<class _Path_type>
	class _Path_iterator
	{	// bidirectional iterator for path
public:
	typedef _Path_iterator<_Path_type> _Myt;
	typedef _Path_type path_type;
	typedef typename _Path_type::string_type string_type;

	typedef bidirectional_iterator_tag iterator_category;
	typedef path_type value_type;
	typedef ptrdiff_t difference_type;
	typedef const value_type *pointer;
	typedef const value_type& reference;

	_Path_iterator()
		: _Myptr(0), _Myoff(0)
		{	// construct singular iterator
		}

	_Path_iterator(const path_type& _Path, size_t _Off)
		: _Myptr(&_Path), _Myoff(_Off)
		{	// construct iterator
		_Getval();
		}

	_Path_iterator(const _Path_iterator& _Right)
		: _Myptr(_Right._Myptr),
			_Myelem(_Right._Myelem),
			_Myoff(_Right._Myoff)
		{	// copy construct
		}

	_Path_iterator& operator=(const _Path_iterator& _Right)
		{	// copy assign
		_Myptr = _Right._Myptr;
		_Myelem = _Right._Myelem;
		_Myoff = _Right._Myoff;
		return (*this);
		}

	_Path_iterator(_Path_iterator&& _Right) _NOEXCEPT
		: _Myptr(_Right._Myptr),
			_Myelem(_STD move(_Right._Myelem)),
			_Myoff(_Right._Myoff)
		{	// move construct
		}

	_Path_iterator& operator=(_Path_iterator&& _Right) _NOEXCEPT
		{	// move assign
		_Myptr = _Right._Myptr;
		_Myelem = _STD move(_Right._Myelem);
		_Myoff = _Right._Myoff;
		return (*this);
		}

	reference operator*() const
		{	// return designated value
		return (_Myelem);
		}

	pointer operator->() const
		{	// return pointer to class object
		return (pointer_traits<pointer>::pointer_to(**this));
		}

	_Myt& operator++()
		{	// preincrement
 #pragma warning(push)
 #pragma warning(disable: 6011)	/* quiet analyze noise */

 #if _ITERATOR_DEBUG_LEVEL == 2
		if (_Myptr == 0 || _Myptr->_Mystr.size() <= _Myoff)
			_DEBUG_ERROR(
				"path::iterator is not incrementable");

 #elif _ITERATOR_DEBUG_LEVEL == 1
		_SCL_SECURE_VALIDATE(_Myptr != 0 && _Myoff < _Myptr->_Mystr.size());
 #endif /* _ITERATOR_DEBUG_LEVEL */

		size_t _Pend = _Myptr->_Prefix_end();
		size_t _Size = _Myptr->_Mystr.size();

		if (_Myoff < _Pend)
			_Myoff = _Pend;	// point past "x:"
		else if (_Myoff == _Pend && _Pend < _Size
			&& _FS_ISSEP(_Myptr->_Mystr[_Pend]))
			{	// point past root "/" and following slashes
			for (++_Myoff; _Myoff < _Size; ++_Myoff)
				if (!_FS_ISSEP(_Myptr->_Mystr[_Myoff]))
					break;
			}
		else
			{	// point past slashes followed by stuff
			for (; _Myoff < _Size; ++_Myoff)
				if (!_FS_ISSEP(_Myptr->_Mystr[_Myoff]))
					break;
			for (; _Myoff < _Size; ++_Myoff)
				if (_FS_ISSEP(_Myptr->_Mystr[_Myoff]))
					break;
			}
		_Getval();
		return (*this);

 #pragma warning(pop)
		}

	_Myt operator++(int)
		{	// postincrement
		_Myt _Tmp = *this;
		++*this;
		return (_Tmp);
		}

	_Myt& operator--()
		{	// predecrement
 #if _ITERATOR_DEBUG_LEVEL == 2
		if (_Myptr == 0 || _Myoff == 0)
			_DEBUG_ERROR(
				"path::iterator is not decrementable");

 #elif _ITERATOR_DEBUG_LEVEL == 1
		_SCL_SECURE_VALIDATE(_Myptr != 0 && _Myoff != 0);
 #endif /* _ITERATOR_DEBUG_LEVEL */

		size_t _Myoff_sav = _Myoff;
		size_t _Off_back = 0;
		_Myoff = 0;
		do
			{	// scan down to previous
			_Off_back = _Myoff;
			++*this;
			} while (_Myoff < _Myoff_sav);
		_Myoff = _Off_back;
		_Getval();
		return (*this);
		}

	_Myt operator--(int)
		{	// postdecrement
		_Myt _Tmp = *this;
		--*this;
		return (_Tmp);
		}

	bool operator==(const _Myt& _Right) const
		{	// test for iterator equality
		return (_Myptr == _Right._Myptr && _Myoff == _Right._Myoff);
		}

	bool operator!=(const _Myt& _Right) const
		{	// test for iterator inequality
		return (!(*this == _Right));
		}

private:
	void _Getval()
		{	// determine _Myelem
		string_type _Str;
		size_t _Pend = _Myptr->_Prefix_end();
		size_t _Size = _Myptr->_Mystr.size();

		if (_Size <= _Myoff)
			;	// off end, no field
		else if (_Myoff < _Pend)
			_Str = _Myptr->_Mystr.substr(0, _Pend);	// get "x:"
		else if (_Myoff == _Pend && _Pend < _Size
			&& _FS_ISSEP(_Myptr->_Mystr[_Pend]))
			_Str = _FS_PREF;	// get "/"
		else
			{	// determine next field as slashes followed by stuff
			size_t _Nslash = 0;
			size_t _Nstuff = 0;

			for (; _Myoff + _Nslash < _Size; ++_Nslash)
				if (!_FS_ISSEP(_Myptr->_Mystr[_Myoff + _Nslash]))
					break;
			for (; _Myoff + _Nslash + _Nstuff < _Size; ++_Nstuff)
				if (_FS_ISSEP(_Myptr->_Mystr[_Myoff + _Nslash + _Nstuff]))
					break;
			if (0 < _Nstuff)
				_Str = _Myptr->_Mystr.substr(_Myoff + _Nslash,
					_Nstuff);	// get "stuff"
			else if (0 < _Nslash)
				_Str = _FS_PERIOD;	// get "."
			}
		_Myelem = _Str;
		}

	const path_type *_Myptr;	// pointer to full path
	path_type _Myelem;	// current element subpath
	size_t _Myoff;	// current offset in full path
	};

		// CLASS path AND FRIENDS
class path
	{	// stores a pathname
public:
	typedef _Pchar value_type;
	typedef basic_string<value_type> string_type;
	static constexpr value_type preferred_separator = _FS_PREF;

	path()
		{	// default construct
		}

	path(const path& _Right)
		: _Mystr(_Right._Mystr)
		{	// copy construct
		}

	// ARBITRARY SOURCE CONSTRUCT

	template<class _InIt,
		class = typename enable_if<_Is_iterator<_InIt>::value,
			void>::type>
		path(_InIt _First, _InIt _Last)

		{	// construct from [_First, _Last), given iterators
		typedef _Iter_value_t<_InIt> _Valty;
		basic_string<_Valty> _Str(_First, _Last);
		string_type _Str_out;
		*this /= _Path_cvt<_Valty, value_type>::
			_Cvt(_Str_out, _Str.c_str(), _Str.size());
		}

	template<class _InIt,
		class = typename enable_if<_Is_iterator<_InIt>::value,
			void>::type>
		path(_InIt _First)

		{	// construct from NTCTS, given iterator
		typedef _Iter_value_t<_InIt> _Valty;
		basic_string<_Valty> _Str;
		for (; *_First != _Valty(0); ++_First)
			_Str += *_First;
		string_type _Str_out;
		*this /= _Path_cvt<_Valty, value_type>::
			_Cvt(_Str_out, _Str.c_str(), _Str.size());
		}

	template<class _Elem,
		class _Traits,
		class _Alloc>
		path(const basic_string<_Elem, _Traits, _Alloc>& _Str)
		{	// construct from arbitrary source string
		string_type _Str_out;
		*this /= _Path_cvt<_Elem, value_type>::
			_Cvt(_Str_out, _Str.c_str(), _Str.size());
		}

	// ARBITRARY SOURCE CONSTRUCT, WITH LOCALE

	template<class _InIt,
		class = typename enable_if<_Is_iterator<_InIt>::value,
			void>::type>
		path(_InIt _First, _InIt _Last,
			const locale& _Loc)

		{	// construct from [_First, _Last), given iterators
		typedef _Iter_value_t<_InIt> _Valty;
		basic_string<_Valty> _Str(_First, _Last);
		string_type _Str_out;
		*this /= _Path_cvt<_Valty, value_type>::
			_Cvt(_Str_out, _Str.c_str(), _Str.size(), _Loc);
		}

	template<class _InIt,
		class = typename enable_if<_Is_iterator<_InIt>::value,
			void>::type>
		path(_InIt _First,
			const locale& _Loc)

		{	// construct from NTCTS, given iterator
		typedef _Iter_value_t<_InIt> _Valty;
		basic_string<_Valty> _Str;
		for (; *_First != _Valty(0); ++_First)
			_Str += *_First;
		string_type _Str_out;
		*this /= _Path_cvt<_Valty, value_type>::
			_Cvt(_Str_out, _Str.c_str(), _Str.size(), _Loc);
		}

	template<class _Elem,
		class _Traits,
		class _Alloc>
		path(const basic_string<_Elem, _Traits, _Alloc>& _Str,
			const locale& _Loc)
		{	// construct from arbitrary source string
		string_type _Str_out;
		*this /= _Path_cvt<_Elem, value_type>::
			_Cvt(_Str_out, _Str.c_str(), _Str.size(), _Loc);
		}

	path(path&& _Right) _NOEXCEPT
		: _Mystr(_STD move(_Right._Mystr))
		{	// move construct
		}

	path& operator=(path&& _Right) _NOEXCEPT
		{	// move assign
		_Mystr = _STD move(_Right._Mystr);
		return (*this);
		}

	~path() _NOEXCEPT
		{	// destroy the object
		}

	// ARBITRARY SOURCE ASSIGN
	path& operator=(const path& _Right)
		{	// assign copy
		_Mystr = _Right._Mystr;
		return (*this);
		}

	template<class _InIt>
		path& operator=(_InIt _First)
		{	// assign from NTCTS, given iterator
		return (*this = path(_First));
		}

	template<class _Elem,
		class _Traits,
		class _Alloc>
		path& operator=(
			const basic_string<_Elem, _Traits, _Alloc>& _Str)
		{	// assign from arbitrary source string
		return (*this = path(_Str));
		}

	template<class _InIt>
		path& assign(_InIt _First, _InIt _Last)
		{	// assign from [_First, _Last), given iterators
		return (*this = path(_First, _Last));
		}

	template<class _InIt>
		path& assign(_InIt _First)
		{	// assign from NTCTS
		return (*this = path(_First));
		}

	template<class _Elem,
		class _Traits,
		class _Alloc>
		path& assign(
			const basic_string<_Elem, _Traits, _Alloc>& _Str)
		{	// assign from arbitrary source string
		return (*this = path(_Str));
		}

	// ARBITRARY SOURCE APPEND
	path& operator/=(const path& _Path)
		{	// append copy
		return (append(_Path._Mystr));
		}

	template<class _InIt>
		path& operator/=(_InIt _First)
		{	// append NTCTS, given iterator
		return (append(path(_First)._Mystr));
		}

	template<class _Elem,
		class _Traits,
		class _Alloc>
		path& operator/=(
			const basic_string<_Elem, _Traits, _Alloc>& _Str)
		{	// append arbitrary source string
		return (append(_Str));
		}

	template<class _InIt>
		path& append(_InIt _First, _InIt _Last)
		{	// append NTCTS
		return (append(path(_First, _Last)._Mystr));
		}

	template<class _InIt>
		path& append(_InIt _First)
		{	// append NTCTS
		return (append(path(_First)._Mystr));
		}

	template<class _Elem,
		class _Traits,
		class _Alloc>
		path& append(
			const basic_string<_Elem, _Traits, _Alloc>& _Str0)
		{	// append arbitrary source string
		string_type _Str(_Str0.begin(), _Str0.end());	// avoid overlap

		for (size_t _Idx = 0; _Idx < _Str.size(); ++_Idx)
			if (_Str[_Idx] == _FS_SLASH)
				_Str[_Idx] = _FS_PREF;	// convert '/' to '\'

		if (0 < _Mystr.size() && 0 < _Str.size()
			&& _Mystr.back() != _FS_COLON
			&& !_FS_ISSEP(_Mystr.back())
			&& !_FS_ISSEP(_Str[0]))
			_Mystr.append(1, _FS_PREF);	// add needed separator

		_Mystr.append(_Str);
		return (*this);
		}

	// ARBITRARY SOURCE CONCATENATE
	path& operator+=(const path& _Path)
		{	// concatenate copy
		return (concat(_Path._Mystr));
		}

	template<class _Ty>
		path& operator+=(_Ty _Val)
		{	// concatenate NTCTS, given iterator/element
		return (concat(_Val));
		}

	template<class _Elem,
		class _Traits,
		class _Alloc>
		path& operator+=(
			const basic_string<_Elem, _Traits, _Alloc>& _Str)
		{	// concatenate arbitrary source string
		return (concat(_Str));
		}

	template<class _Elem>
		path& operator+=(const _Elem *_Ptr)
		{	// concatenate arbitrary NTCTS
		return (concat(basic_string<_Elem>(_Ptr)));
		}

	template<class _InIt>
		path& concat(_InIt _First, _InIt _Last)
		{	// concatenate NTCTS
		return (concat(path(_First, _Last)._Mystr));
		}

	template<class _InIt,
		class = typename enable_if<_Is_iterator<_InIt>::value,
			void>::type>
		path& concat(_InIt _First)

		{	// concatenate NTCTS
		return (concat(path(_First)._Mystr));
		}

	template<class _Elem>
		path& concat(const _Elem *_Ptr)
		{	// concatenate arbitrary NTCTS
		return (concat(basic_string<_Elem>(_Ptr)));
		}

	template<class _Elem>
		path& concat(_Elem _Val)
		{	// concatenate element
		return (concat(path(basic_string<_Elem>((size_t)1, _Val))._Mystr));
		}

	template<class _Elem,
		class _Traits,
		class _Alloc>
		path& concat(
			const basic_string<_Elem, _Traits, _Alloc>& _Str)
		{	// concatenate arbitrary source string
		_Mystr.append(_Str.begin(), _Str.end());
		return (*this);
		}

	// PATH MODIFIERS
	void clear() _NOEXCEPT
		{	// clear the stored string
		_Mystr.clear();
		}

	path& make_preferred()
		{	// convert any '/' to '\'
		for (size_t _Idx = 0; _Idx < _Mystr.size(); ++_Idx)

			if (_Mystr[_Idx] == _FS_SLASH)
				_Mystr[_Idx] = _FS_BSLASH;

		return (*this);
		}

	path& remove_filename()
		{	// remove filename (leaf)
		if (!empty() && begin() != --end())
			{	// leaf to remove, back up over it
			size_t _Rend = _Root_end();
			size_t _Idx = _Mystr.size();

			for (; _Rend < _Idx; --_Idx)
				if (_FS_ISSEP(_Mystr[_Idx - 1]))
					break;	// back up over stuff at end
			for (; _Rend < _Idx; --_Idx)
				if (!_FS_ISSEP(_Mystr[_Idx - 1]))
					break;	// back up over trailing non-root slashes
			_Mystr.erase(_Idx);
			}
		return (*this);
		}

	path& replace_filename(const path& _Path)
		{	// replace filename
		remove_filename();
		*this /= _Path;
		return (*this);
		}

	path& replace_extension(const path& _Newext = path())
		{	// replace extension with _Newext
		if (_Newext.empty() || _Newext.c_str()[0] == _FS_PERIOD)
			*this = parent_path()
				/ path((stem().native() + _Newext.native()));
		else
			*this = parent_path()
				/ path((stem().native() + _FS_PERIOD + _Newext.native()));
		return (*this);
		}

	void swap(path& _Right) _NOEXCEPT
		{	// swap the stored strings
		_Mystr.swap(_Right._Mystr);
		}

	// NATIVE FORMAT OBSERVERS
	const string_type& native() const _NOEXCEPT
		{	// get the stored string
		return (_Mystr);
		}

	const value_type *c_str() const _NOEXCEPT
		{	// get the stored string
		return (_Mystr.c_str());
		}

	operator string_type() const
		{	// convert to file string
		return (_Mystr);
		}

	template<class _Elem,
		class _Traits = char_traits<_Elem>,
		class _Alloc = allocator<_Elem> >
		basic_string<_Elem, _Traits, _Alloc>
			string(const _Alloc& _Al = _Alloc()) const
		{	// return path as basic_string<_Elem, _Traits, _Alloc>
		basic_string<_Elem, _Traits, _Alloc> _Str(_Al);
		return (_Path_cvt<value_type, _Elem>::
			_Cvt(_Str, _Mystr.c_str(), _Mystr.size()));
		}

	_STD string string() const
		{	// return path as basic_string<char> native
		_STD string _Str;
		return (_Path_cvt<value_type, char>::
			_Cvt(_Str, _Mystr.c_str(), _Mystr.size()));
		}

	_STD wstring wstring() const
		{	// return path as basic_string<wchar_t> native
		_STD wstring _Str;
		return (_Path_cvt<value_type, wchar_t>::
			_Cvt(_Str, _Mystr.c_str(), _Mystr.size()));
		}

	_STD string u8string() const
		{	// return path as basic_string<char> UTF8
		_STD string _Str;
		return (_Path_cvt<value_type, char8_t,
			char_traits<char>, allocator<char> >::
			_Cvt(_Str, _Mystr.c_str(), _Mystr.size()));
		}

	_STD u16string u16string() const
		{	// return path as basic_string<char16_t> UTF16
		_STD u16string _Str;
		return (_Path_cvt<value_type, char16_t>::
			_Cvt(_Str, _Mystr.c_str(), _Mystr.size()));
		}

	_STD u32string u32string() const = delete;	// TRANSITION
#if 0
		{	// return path as basic_string<char32_t> UTF32
		_STD u32string _Str;
		return (_Path_cvt<value_type, char32_t>::
			_Cvt(_Str, _Mystr.c_str(), _Mystr.size()));
		}
#endif /* 0 */

	// GENERIC FORMAT OBSERVERS

	string_type _Make_generic() const
		{	// copy and convert any '\' to '/'
		string_type _Str = _Mystr;
		_STD replace(_Str.begin(), _Str.end(), _FS_BSLASH, _FS_SLASH);
		return (_Str);
		}

	template<class _Elem,
		class _Traits = char_traits<_Elem>,
		class _Alloc = allocator<_Elem> >
		basic_string<_Elem, _Traits, _Alloc>
			generic_string(const _Alloc& _Al = _Alloc()) const
		{	// return path as basic_string<_Elem, _Traits, _Alloc>
		basic_string<_Elem, _Traits, _Alloc> _Str(_Al);
		const string_type _Genstr = _Make_generic();
		return (_Path_cvt<value_type, _Elem>::
			_Cvt(_Str, _Genstr.c_str(), _Genstr.size()));
		}

	_STD string generic_string() const
		{	// return path as basic_string<char> native
		_STD string _Str;
		const string_type _Genstr = _Make_generic();
		return (_Path_cvt<value_type, char>::
			_Cvt(_Str, _Genstr.c_str(), _Genstr.size()));
		}

	_STD wstring generic_wstring() const
		{	// return path as basic_string<wchar_t> native
		_STD wstring _Str;
		const string_type _Genstr = _Make_generic();
		return (_Path_cvt<value_type, wchar_t>::
			_Cvt(_Str, _Genstr.c_str(), _Genstr.size()));
		}

	_STD string generic_u8string() const
		{	// return path as basic_string<char> UTF8
		_STD string _Str;
		const string_type _Genstr = _Make_generic();
		return (_Path_cvt<value_type, char8_t,
			char_traits<char>, allocator<char> >::
			_Cvt(_Str, _Genstr.c_str(), _Genstr.size()));
		}

	_STD u16string generic_u16string() const
		{	// return path as basic_string<char16_t> UTF16
		_STD u16string _Str;
		const string_type _Genstr = _Make_generic();
		return (_Path_cvt<value_type, char16_t>::
			_Cvt(_Str, _Genstr.c_str(), _Genstr.size()));
		}

	_STD u32string generic_u32string() const = delete;	// TRANSITION
#if 0
		{	// return path as basic_string<char32_t> UTF32
		_STD u32string _Str;
		const string_type _Genstr = _Make_generic();
		return (_Path_cvt<value_type, char32_t>::
			_Cvt(_Str, _Genstr.c_str(), _Genstr.size()));
		}
#endif /* 0 */

	// PATH COMPARISON
	int compare(const path& _Path) const _NOEXCEPT
		{	// compare native() to _Path.native()
		return (_Mystr.compare(_Path._Mystr));
		}

	int compare(const string_type& _Str) const
		{	// compare native() to string
		return (_Mystr.compare(_Str));
		}

	int compare(const value_type *_Ptr) const
		{	// compare native() to NTCS
		return (_Mystr.compare(_Ptr));
		}

	// PATH DECOMPOSITION
	path root_name() const
		{	// get root name
		return (path(_Mystr.substr(0, _Prefix_end())));
		}

	path root_directory() const
		{	// get root directory
		size_t _Idx = _Prefix_end();
		if (_Idx < _Mystr.size()
			&& _FS_ISSEP(_Mystr[_Idx]))
			return (path(string_type(1, _FS_PREF)));
		else
			return (path());
		}

	path root_path() const
		{	// get root path
		return (path(_Mystr.c_str(),
			_Mystr.c_str() + _Root_end()));
		}

	path relative_path() const
		{	// get relative path
		size_t _Idx = _Root_end();
		while (_Idx < _Mystr.size()
			&& _FS_ISSEP(_Mystr[_Idx]))
			++_Idx;	// skip extra '/' after root

		return (path(_Mystr.substr(_Idx)));
		}

	path parent_path() const
		{	// get parent (branch) path
		path _Ans;
		if (!empty())
			{	// append all but last element
			iterator _End = --end();
			for (iterator _Next = begin(); _Next != _End; ++_Next)
				_Ans /= *_Next;
			}
		return (_Ans);
		}

	path filename() const
		{	// get filename (leaf)
		return (empty() ? path() : path(*--end()));
		}

	path stem() const
		{	// pick off stem (basename) in filename (leaf) before dot
		string_type _Str0 = filename().native();
		string_type _Str1 = extension().native();
		_Str0.resize(_Str0.size() - _Str1.size());
		return (path(_Str0));
		}

	path extension() const
		{	// pick off .extension in filename (leaf), including dot
		string_type _Str = filename().native();
		size_t _Idx = _Str.rfind(_FS_PERIOD);
		return (_Idx == string_type::npos	// no .
			|| _Str.size() == 1	// only .
			|| (_Str.size() == 2 && _Str[0] == _FS_PERIOD
				&& _Str[1] == _FS_PERIOD)	// only ..
			? path() : path(_Str.substr(_Idx)));
		}

	bool empty() const _NOEXCEPT
		{	// test if stored string is empty
		return (_Mystr.empty());
		}

	bool has_root_name() const
		{	// test if root name is nonempty
		return (!root_name().empty());
		}

	bool has_root_directory() const
		{	// test if root directory is nonempty
		return (!root_directory().empty());
		}

	bool has_root_path() const
		{	// test if root path is nonempty
		return (!root_path().empty());
		}

	bool has_relative_path() const
		{	// test if relative path is nonempty
		return (!relative_path().empty());
		}

	bool has_parent_path() const
		{	// test if parent (branch) path is nonempty
		return (!parent_path().empty());
		}

	bool has_filename() const
		{	// test if filename (leaf) is nonempty
		return (!filename().empty());
		}

	bool has_stem() const
		{	// test if stem (basename) is nonempty
		return (!stem().empty());
		}

	bool has_extension() const
		{	// test if extension is nonempty
		return (!extension().empty());
		}

	bool is_absolute() const
		{	// test if path is absolute
		return (has_root_name() && has_root_directory());
		}

	bool is_relative() const
		{	// test if path is relative
		return (!is_absolute());
		}

	// PATH ITERATOR SUPPORT
	typedef _Path_iterator<path> iterator;
	typedef iterator const_iterator;

	iterator begin() const
		{	// get beginning of path
		return (iterator(*this, (size_t)0));
		}

	iterator end() const
		{	// get end of path
		return (iterator(*this, _Mystr.size()));
		}

	size_t _Prefix_end() const
		{	// get end of prefix
		if (2 < _Mystr.size()
			&& _FS_ISSEP(_Mystr[0])
			&& _FS_ISSEP(_Mystr[1])
			&& !_FS_ISSEP(_Mystr[2]))
			{	// network name, pick off \\name
			size_t _Idx = 3;
			for (; _Idx < _Mystr.size() && !_FS_ISSEP(_Mystr[_Idx]); ++_Idx)
				;
			return (_Idx);
			}
		else
			{	// no network name, pick off drive:
			size_t _Idx = _Mystr.find(_FS_COLON, 0);
			if (_Idx == _Mystr.npos)
				_Idx = 0;
			else
				++_Idx;
			return (_Idx);
			}
		}

	size_t _Root_end() const
		{	// get end of root
		size_t _Idx = _Prefix_end();
		if (_Idx < _Mystr.size()
			&& _FS_ISSEP(_Mystr[_Idx]))
			++_Idx;
		return (_Idx);
		}

	string_type _Mystr;
	};

inline void swap(path& _Left,
		path& _Right) _NOEXCEPT
	{	// swap two paths
	_Left.swap(_Right);
	}

inline size_t hash_value(const path& _Path) _NOEXCEPT
	{	// get hash value for path
	return (hash<path::string_type>()(_Path.native()));
	}

inline bool operator==(const path& _Left, const path& _Right) _NOEXCEPT
	{	// test for path equality
	return (_Left.native() == _Right.native());
	}

inline bool operator!=(const path& _Left, const path& _Right) _NOEXCEPT
	{	// test for path inequality
	return (!(_Left == _Right));
	}

inline bool operator<(const path& _Left, const path& _Right) _NOEXCEPT
	{	// test if _Left < _Right
	return (_Left.native() < _Right.native());
	}

inline bool operator<=(const path& _Left, const path& _Right) _NOEXCEPT
	{	// test if _Left <= _Right
	return (!(_Right < _Left));
	}

inline bool operator>(const path& _Left, const path& _Right) _NOEXCEPT
	{	// test if _Left > _Right
	return (_Right < _Left);
	}

inline bool operator>=(const path& _Left, const path& _Right) _NOEXCEPT
	{	// test if _Left >= _Right
	return (!(_Left < _Right));
	}

inline path operator/(const path& _Left, const path& _Right)
	{	// concatenate paths
	path _Ans = _Left;
	return (_Ans /= _Right);
	}

template<class _Elem,
	class _Traits> inline
	basic_ostream<_Elem, _Traits>&
		operator<<(basic_ostream<_Elem, _Traits>& _Ostr, const path& _Path)
	{	// insert a path
	return (_Ostr << _Path.string<_Elem, _Traits, allocator<_Elem> >());
	}

template<class _Elem,
	class _Traits> inline
	basic_istream<_Elem, _Traits>&
		operator>>(basic_istream<_Elem, _Traits>& _Istr, path& _Path)
	{	// extract a path
	basic_string<_Elem, _Traits> _Str;
	_Istr >> _Str;
	_Path = _Str;
	return (_Istr);
	}

	// TEMPLATE FUNCTION u8path

template<class _InIt,
	class = typename enable_if<_Is_iterator<_InIt>::value,
		void>::type> inline
	path u8path(_InIt _First, _InIt _Last)

	{	// make path from [_First, _Last) UTF8, given iterators
	string _Str(_First, _Last);
	path::string_type _Str_out;
	return (path(_Path_cvt<char8_t, _Pchar>::
		_Cvt(_Str_out, _Str.c_str(), _Str.size())));
	}

template<class _InIt,
	class = typename enable_if<_Is_iterator<_InIt>::value,
		void>::type> inline
	path u8path(_InIt _First)

	{	// make path from NTBS UTF8, given iterator
	string _Str;
	for (; *_First != '\0'; ++_First)
		_Str += *_First;
	path::string_type _Str_out;
	return (path(_Path_cvt<char8_t, _Pchar>::
		_Cvt(_Str_out, _Str.c_str(), _Str.size())));
	}

template<class _Traits,
	class _Alloc> inline
	path u8path(const basic_string<char, _Traits, _Alloc>& _Str)
	{	// make path from arbitrary char string UTF8
	path::string_type _Str_out;
	return (path(_Path_cvt<char8_t, _Pchar>::
		_Cvt(_Str_out, _Str.c_str(), _Str.size())));
	}

		// CLASS filesystem_error AND FRIENDS
class filesystem_error
	: public system_error
	{	// base of all filesystem-error exceptions
public:
	explicit filesystem_error(const string& _Message,
		error_code _Errcode = make_error_code(errc::operation_not_permitted))
		: system_error(_Errcode, _Message)
		{	// construct from message string and error code
		}

	filesystem_error(const string& _Message,
		const path& _Path1,
		error_code _Errcode)
		: system_error(_Errcode, _Message),
			_Mypval1(_Path1)
		{	// construct from message string and error code
		}

	filesystem_error(const string& _Message,
		const path& _Path1,
		const path& _Path2,
		error_code _Errcode)
		: system_error(_Errcode, _Message),
			_Mypval1(_Path1), _Mypval2(_Path2)
		{	// construct from message string and error code
		}

	virtual __CLR_OR_THIS_CALL ~filesystem_error() _NOEXCEPT
		{	// destroy the object
		}

	filesystem_error(filesystem_error&& _Right) _NOEXCEPT
		: system_error(_Right.code(), _Right.what()),
			_Mypval1(_STD move(_Right._Mypval1)),
			_Mypval2(_STD move(_Right._Mypval2))
		{	// move construct
		}

	filesystem_error& operator=(filesystem_error&& _Right) _NOEXCEPT
		{	// move assign
		*((system_error *)this) = *(system_error *)&_Right;
		_Mypval1 = _STD move(_Right._Mypval1);
		_Mypval2 = _STD move(_Right._Mypval2);
		return (*this);
		}

	const path& path1() const _NOEXCEPT
		{	// return stored first path
		return (_Mypval1);
		}

	const path& path2() const _NOEXCEPT
		{	// return stored second path
		return (_Mypval2);
		}

	filesystem_error(const filesystem_error&) = default;
	filesystem_error& operator=(const filesystem_error&) = default;

private:
	path _Mypval1;
	path _Mypval2;

 #if _HAS_EXCEPTIONS

 #else /* _HAS_EXCEPTIONS */
protected:
	virtual __CLR_OR_THIS_CALL void _Doraise() const
		{	// perform class-specific exception handling
		_RAISE(*this);
		}
 #endif /* _HAS_EXCEPTIONS */
	};

		// CLASS directory_entry AND FRIENDS
class directory_entry
	{	// represents a directory entry
public:
	typedef directory_entry _Myt;
	typedef _FSPFX path::string_type string_type;

	directory_entry()
		: _Mystat(),
			_Mysymstat()
		{	// default construct
		}

	directory_entry(const directory_entry&) = default;
	~directory_entry() _NOEXCEPT = default;
	directory_entry& operator=(const directory_entry&) = default;

	directory_entry(directory_entry&&) = default;
	directory_entry& operator=(directory_entry&&) = default;

	explicit directory_entry(const _FSPFX path& _Path,
		file_status _Statarg = file_status(),
		file_status _Symstatarg = file_status())
		: _Mypval(_Path),
			_Mystat(_Statarg),
			_Mysymstat(_Symstatarg)
		{	// construct from path and status
		}

	void assign(const _FSPFX path& _Path,
		file_status _Statarg = file_status(),
		file_status _Symstatarg = file_status())
		{	// assign path and status
		_Mypval = _Path;
		_Mystat = _Statarg;
		_Mysymstat = _Symstatarg;
		}

	void replace_filename(const _FSPFX path& _Path,
		file_status _Statarg = file_status(),
		file_status _Symstatarg = file_status())
		{	// replace filename and assign status
		_Mypval = _Mypval.parent_path() / _Path;
		_Mystat = _Statarg;
		_Mysymstat = _Symstatarg;
		}

	operator const _FSPFX path&() const _NOEXCEPT
		{	// get path
		return (_Mypval);
		}

	const _FSPFX path& path() const _NOEXCEPT
		{	// get path
		return (_Mypval);
		}

	file_status status() const
		{	// get file status
		error_code _Code;
		return (status(_Code));
		}

	file_status status(error_code& _Code) const _NOEXCEPT
		{	// get file status
		_Code = error_code();
		if (status_known(_Mystat))
			;
		else if (status_known(_Mysymstat)
			&& !is_symlink(_Mysymstat))
			_Mystat = _Mysymstat;
		else
			_Mystat = _FSPFX status(_Mypval, _Code);
		return (_Mystat);
		}

	file_status symlink_status() const
		{	// get file symlink status
		error_code _Code;
		return (symlink_status(_Code));
		}

	file_status symlink_status(error_code& _Code) const _NOEXCEPT
		{	// get file symlink status
		_Code = error_code();
		if (!status_known(_Mysymstat))
			_Mysymstat = _FSPFX symlink_status(_Mypval, _Code);
		return (_Mysymstat);
		}

	bool operator==(const _Myt& _Right) const _NOEXCEPT
		{	// test if *this == _Right
		return (_Mypval == _Right._Mypval);
		}

	bool operator!=(const _Myt& _Right) const _NOEXCEPT
		{	// test if *this == _Right
		return (!(*this == _Right));
		}

	bool operator<(const _Myt& _Right) const _NOEXCEPT
		{	// test if *this < _Right
		return (_Mypval < _Right._Mypval);
		}

	bool operator>(const _Myt& _Right) const _NOEXCEPT
		{	// test if *this > _Right
		return (_Right < *this);
		}
	bool operator<=(const _Myt& _Right) const _NOEXCEPT
		{	// test if *this <= _Right
		return (!(_Right < *this));
		}

	bool operator>=(const _Myt& _Right) const _NOEXCEPT
		{	// test if *this <= _Right
		return (!(*this < _Right));
		}

private:
	_FSPFX path _Mypval;
	mutable file_status _Mystat;
	mutable file_status _Mysymstat;
	};

template<class _Prefix_directory>
	class _Directory_iterator
	{	// walks a directory
public:
	typedef _Directory_iterator<_Prefix_directory> _Myt;
	typedef path::string_type string_type;

	typedef input_iterator_tag iterator_category;
	typedef directory_entry value_type;
	typedef ptrdiff_t difference_type;
	typedef const value_type *pointer;
	typedef const value_type& reference;

	_Directory_iterator() _NOEXCEPT
		: _Mypdir(new void *(0), &_Delptr)
		{	// default construct
		}

	explicit _Directory_iterator(const path& _Path)
		: _Mypdir(new void *(0), &_Delptr),
			_Mydirpath(_Path)
		{	// construct from _Path
		typename string_type::value_type _Dest[_MAX_FILESYS_NAME];
		int _Errno = 0;
		_File_type _Ftype;

		*_Mypdir = _Open_dir(_Dest, _Path.c_str(), _Errno, _Ftype);
		if (*_Mypdir != 0)
			_Form_name(_Dest, _Ftype, _Prefix_directory());
		}

	_Directory_iterator(const path& _Path, error_code& _Code) _NOEXCEPT
		: _Mypdir(new void *(0), &_Delptr),
			_Mydirpath(_Path)
		{	// construct from _Path, errors to _Code
		typename string_type::value_type _Dest[_MAX_FILESYS_NAME];
		int _Errno = 0;
		_File_type _Ftype;

		*_Mypdir = _Open_dir(_Dest, _Path.c_str(), _Errno, _Ftype);
		if (*_Mypdir != 0)
			_Form_name(_Dest, _Ftype, _Prefix_directory());
		_Code = error_code(_Errno, _STD system_category());
		}

	_Directory_iterator(const _Myt&) = default;
	~_Directory_iterator() _NOEXCEPT = default;
	_Myt& operator=(const _Myt&) = default;

	_Directory_iterator(_Myt&&) = default;
	_Myt& operator=(_Myt&&) = default;

	const value_type& operator*() const
		{	// return designated value
 #if _ITERATOR_DEBUG_LEVEL == 2
		if (!_Mypdir || *_Mypdir == 0)
			_DEBUG_ERROR(
				"directory_iterator is not dereferencable");

 #elif _ITERATOR_DEBUG_LEVEL == 1
		_SCL_SECURE_VALIDATE(_Mypdir != 0 && *_Mypdir != 0);
 #endif /* _ITERATOR_DEBUG_LEVEL */

		return (_Myentry);
		}

	pointer operator->() const
		{	// return pointer to class object
		return (pointer_traits<pointer>::pointer_to(**this));
		}

	_Myt& operator++()
		{	// preincrement
 #if _ITERATOR_DEBUG_LEVEL == 2
		if (!_Mypdir || *_Mypdir == 0)
			_DEBUG_ERROR(
				"directory_iterator is not incrementable");

 #elif _ITERATOR_DEBUG_LEVEL == 1
		_SCL_SECURE_VALIDATE(_Mypdir != 0 && *_Mypdir != 0);
 #endif /* _ITERATOR_DEBUG_LEVEL */

		_Get();
		return (*this);
		}

	_Myt& increment(error_code& _Code) _NOEXCEPT
		{	// preincrement
		_Code = error_code();
		if (!_Mypdir || *_Mypdir == 0)
			_Code = make_error_code(errc::operation_not_permitted);
		else
			_Get();
		return (*this);
		}

	_Myt operator++(int)
		{	// postincrement
		_Myt _Tmp = *this;
		++*this;
		return (_Tmp);
		}

	bool _Equal(const _Myt& _Right) const
		{	// test for equality
		return (_Mypdir.get() != 0 && _Right._Mypdir.get() != 0
			? *_Mypdir == *_Right._Mypdir
			: _Mypdir.get() == 0 && _Right._Mypdir.get() == 0);
		}

private:
	static void _Delptr(void **_Ptr)
		{	// close directory and delete pointer
		if (_Ptr != 0 && *_Ptr != 0)
			_Close_dir(*_Ptr);
		delete _Ptr;
		}

	void _Get()
		{	// peek at next input element
		if (*_Mypdir != 0)
			{	// read a directory entry
			typename string_type::value_type _Dest[_MAX_FILESYS_NAME];
			_File_type _Ftype;
			if (_Read_dir(_Dest, *_Mypdir, _Ftype)[0] == 0)
				{	// end of directory, close it
				_Close_dir(*_Mypdir);
				*_Mypdir = 0;	// end of directory
				}
			else
				_Form_name(_Dest, _Ftype, _Prefix_directory());
			}
		}

	void _Form_name(typename string_type::value_type *_Filename,
			_File_type _Ftype, true_type)
		{	// set entry to _Mydirpath/_Filename
		_Myentry.assign(_Mydirpath / path(_Filename),
			file_status(_Ftype));
		}

	void _Form_name(typename string_type::value_type *_Filename,
			_File_type _Ftype, false_type)
		{	// set entry to _Filename
		_Myentry.assign(path(_Filename), file_status(_Ftype));
		}

	shared_ptr<void *> _Mypdir;
	path _Mydirpath;
	value_type _Myentry;
	};

template<class _Prefix_directory> inline
	bool operator==(
		const _Directory_iterator<_Prefix_directory>& _Left,
		const _Directory_iterator<_Prefix_directory>& _Right)
	{	// test for _Directory_iterator equality
	return (_Left._Equal(_Right));
	}

template<class _Prefix_directory> inline
	bool operator!=(
		const _Directory_iterator<_Prefix_directory>& _Left,
		const _Directory_iterator<_Prefix_directory>& _Right)
	{	// test for _Directory_iterator inequality
	return (!(_Left == _Right));
	}

typedef _Directory_iterator<true_type> directory_iterator;

inline const directory_iterator& begin(
	const directory_iterator& _Iter) _NOEXCEPT
	{	// return begin directory_iterator for range-based for
	return (_Iter);
	}

inline directory_iterator end(
	const directory_iterator&) _NOEXCEPT
	{	// return end directory_iterator for range-based for
	return (directory_iterator());
	}

		// CLASS recursive_directory_iterator
class recursive_directory_iterator
	{	// walks a directory
public:
	typedef recursive_directory_iterator _Myt;
	typedef _Directory_iterator<false_type> _Myiter;
	typedef pair<_Myiter, path> _Mypair;
	typedef path::string_type string_type;
	typedef string_type::value_type char_type;

	typedef input_iterator_tag iterator_category;
	typedef directory_entry value_type;
	typedef ptrdiff_t difference_type;
	typedef const value_type *pointer;
	typedef const value_type& reference;

	recursive_directory_iterator() _NOEXCEPT
		: _Mylist(1, _Mypair()),
			_Myentry(),
			_No_push(false),
			_Options(_Directory_options::none)
		{	// default construct
		}

	explicit recursive_directory_iterator(const path& _Path,
		_Directory_options _Opts = _Directory_options::none)
		: _Mylist(1, _Mypair(_Myiter(_Path), _Path)),
			_Myentry(),
			_No_push(false),
			_Options(_Opts)
		{	// construct from _Path
		_Get();
		}

	recursive_directory_iterator(const path& _Path,
		_Directory_options _Opts, error_code& _Code) _NOEXCEPT
		: _Mylist(1, _Mypair(_Myiter(_Path), _Path)),
			_Myentry(),
			_No_push(false),
			_Options(_Opts)
		{	// construct from _Path
		_Code = error_code();
		_Get();
		}

	recursive_directory_iterator(const path& _Path,
		error_code& _Code) _NOEXCEPT
		: _Mylist(1, _Mypair(_Myiter(_Path), _Path)),
			_Myentry(),
			_No_push(false),
			_Options(_Directory_options::none)
		{	// construct from _Path
		_Code = error_code();
		_Get();
		}

	recursive_directory_iterator(const _Myt&) = default;
	~recursive_directory_iterator() _NOEXCEPT = default;
	_Myt& operator=(const _Myt&) = default;

	recursive_directory_iterator(_Myt&&) = default;
	_Myt& operator=(_Myt&&) = default;

	_Directory_options options() const
		{	// get directory options
		return (_Options);
		}

	int depth() const
		{	// get recursion depth
		return (static_cast<int>(_Mylist.size()) - 1);
		}

	bool recursion_pending() const
		{	// get whether recursion are pending
		return (!_No_push);
		}

	void pop()
		{	// pop a level
		if (1 < _Mylist.size())
			_Mylist.pop_front();	// something to pop, do it
		}

	void disable_recursion_pending()
		{	// disable directory recursion
		_No_push = true;
		}

	const value_type& operator*() const
		{	// return designated value
 #if _ITERATOR_DEBUG_LEVEL == 2
		if (_Mylist.front().first == _Myiter())
			_DEBUG_ERROR(
				"recursive_directory_iterator is not dereferencable");

 #elif _ITERATOR_DEBUG_LEVEL == 1
		_SCL_SECURE_VALIDATE(_Mylist.front().first != _Myiter());
 #endif /* _ITERATOR_DEBUG_LEVEL */

		return (_Myentry);
		}

	pointer operator->() const
		{	// return pointer to class object
		return (pointer_traits<pointer>::pointer_to(**this));
		}

	_Myt& operator++()
		{	// preincrement
 #if _ITERATOR_DEBUG_LEVEL == 2
		if (_Mylist.front().first == _Myiter())
			_DEBUG_ERROR(
				"recursive_directory_iterator is not incrementable");

 #elif _ITERATOR_DEBUG_LEVEL == 1
		_SCL_SECURE_VALIDATE(_Mylist.front().first != _Myiter());
 #endif /* _ITERATOR_DEBUG_LEVEL */

		if (_No_push
			|| !is_directory(_Mylist.front().first->status())
			|| (_Options != _Directory_options::follow_directory_symlink
				&& is_symlink(_Mylist.front().first->status())))
			{
			_No_push = false;
			++_Mylist.front().first;
			}
		else
			{	// push down a subdirectory
			path _Newpath = _Mylist.front().second
				/ _Mylist.front().first->path();
			++_Mylist.front().first;
			_Mylist.push_front(_Mypair(_Myiter(_Newpath), _Newpath));
			}
		while (1 < _Mylist.size() && _Mylist.front().first == _Myiter())
			pop();
		_Get();
		return (*this);
		}

	_Myt& increment(error_code& _Code) _NOEXCEPT
		{	// preincrement
		_Code = error_code();
		if (_Mylist.front().first == _Myiter())
			_Code = make_error_code(errc::operation_not_permitted);
		else
			++*this;
		return (*this);
		}

	_Myt operator++(int)
		{	// postincrement
		_Myt _Tmp = *this;
		++*this;
		return (_Tmp);
		}

	bool _Equal(const _Myt& _Right) const
		{	// test for equality
		return (_Mylist.front().first == _Right._Mylist.front().first);
		}

	void _Get()
		{	// prime _Myentry for future gets
		if (_Mylist.front().first != _Myiter())
			{	// item present, get it
			value_type _Dentry = *_Mylist.front().first;
			_Myentry.assign(_Mylist.front().second / _Dentry.path(),
				_Dentry.status(), _Dentry.symlink_status());
			}
		}

private:
	list<_Mypair> _Mylist;
	value_type _Myentry;
	bool _No_push;
	_Directory_options _Options;
	};

inline bool operator==(
	const recursive_directory_iterator& _Left,
	const recursive_directory_iterator& _Right)
	{	// test for recursive_directory_iterator equality
	return (_Left._Equal(_Right));
	}

inline bool operator!=(
	const recursive_directory_iterator& _Left,
	const recursive_directory_iterator& _Right)
	{	// test for recursive_directory_iterator inequality
	return (!(_Left == _Right));
	}

inline const recursive_directory_iterator& begin(
	const recursive_directory_iterator& _Iter) _NOEXCEPT
	{	// return begin recursive_directory_iterator for range-based for
	return (_Iter);
	}

inline recursive_directory_iterator end(
	const recursive_directory_iterator&) _NOEXCEPT
	{	// return end recursive_directory_iterator for range-based for
	return (recursive_directory_iterator());
	}

		// OPERATIONAL FUNCTIONS
inline path _Absolute(const path& _Path,
	const path& _Base, error_code& _Code)
	{	// make absolute path from _Path and directory _Base; errors in _Code
	_Code = error_code();
	bool _Path_has_root_name = _Path.has_root_name();
	bool _Path_has_root_directory = _Path.has_root_directory();
	if (_Path_has_root_name && _Path_has_root_directory)
		return (_Path);	// fine as is

	path _Current = current_path(_Code);
	if (_Code != error_code())
		return (path());
	path _Abs_base = _Absolute(_Base, _Current, _Code);
	if (_Code != error_code())
		return (path());

	if (_Path_has_root_name)
		{	// insert _Base
		return (_Path.root_name()
			/ _Abs_base.root_directory()
			/ _Abs_base.relative_path()
			/ _Path.relative_path());
		}
	else
		if (_Path_has_root_directory)
			return (_Abs_base.root_name()
				/ _Path);	// prepend root name of base
		else
			return (_Abs_base
				/ _Path);	// prepend absolute name of base
	}

inline path absolute(const path& _Path,
	const path& _Base = current_path())
	{	// make absolute path from _Path and directory _Base
	error_code _Code;
	path _Result = _Absolute(_Path, _Base, _Code);
	if (_Code != error_code())
		_THROW_NCEE(filesystem_error,
			"absolute(p1, p2): current_path() couldn't get current working directory");
	return (_Result);
	}

inline void _Canonicalize_string_only(path& _Canon_path, const path& _Abs_path)
	{	// transform absolute path _Abs_path, removing .s, and ..s, using string
		// manipulations (rather than platform support)
	vector<path> _Components;
	path::const_iterator _Abs_path_next = _Abs_path.begin();
	const path::const_iterator _Abs_path_end = _Abs_path.end();
	// This is safe because we have already made the path absolute
	_Canon_path /= *_Abs_path_next; // root_name
	++_Abs_path_next;
	_Canon_path /= *_Abs_path_next; // root_directory
	++_Abs_path_next;
	for (; _Abs_path_next != _Abs_path_end; ++_Abs_path_next)
		{
		const auto _Cstr = _Abs_path_next->c_str();
		if (_Cstr[0] == _FS_PERIOD)
			{
			if (_Cstr[1] == static_cast<path::value_type>(0))
				continue; // consume "."
			else if (_Cstr[1] == _FS_PERIOD
				&& _Cstr[2] == static_cast<path::value_type>(0))
				{
				// If no parent is found to remove with .., ignore ..
				// (that is, C:\..\..\..\Meow canonicalizes to C:\Meow)
				if (!_Components.empty())
					_Components.pop_back();

				continue; // consume ".."
				}
			}

		_Components.push_back(*_Abs_path_next);
		}

	for (const auto& _Component : _Components)
		_Canon_path /= _Component;
	}

inline path canonical(const path& _Path,
	const path& _Base = current_path())
	{	// make absolute path from _Path, _Base with no ., .., or symlink
	error_code _Code;
	path _Ans = canonical(_Path, _Base, _Code);

	if (_Code != error_code())
		_THROW_NCEE(filesystem_error,
			"canonical(p1, p2): invalid arguments");
	return (_Ans);
	}

inline path canonical(const path& _Path,
	error_code& _Code)
	{	// make absolute path from _Path with no ., .., or symlink
	path _Current = current_path(_Code);
	if (_Code != error_code())
		return (path());
	return (canonical(_Path, _Current, _Code));
	}

inline path canonical(const path& _Path, const path& _Base,
	error_code& _Code)
	{	// make absolute path from _Path, _Base with no ".", "..", symlink
	_Code = error_code();
	path _Canon_path; // NRVO this variable
	path _Abs_path = _Absolute(_Path, _Base, _Code);
	if (_Code != error_code())
		return (_Canon_path);

	if (_Abs_path.native().size() >= _MAX_FILESYS_NAME)
		{
		_Code = make_error_code(errc::filename_too_long);
		return (_Canon_path);
		}

	_Canonicalize_string_only(_Canon_path, _Abs_path);
	return (_Canon_path);
	}

inline bool _Copy_options_indicate_recursive_copy(_Copy_options _Opts)
	{	// Checks a _Copy_options for whether copy should call itself recursively
	if (_Opts == _Copy_options::none)
		// This supports "copying a directory" as copying the directory and
		// files therein but not subdirectories.
		return (true);
	if ((_Opts & _Copy_options::recursive) != _Copy_options::none)
		return (true);
	return (false);
	}

inline void copy(const path& _Oldpval, const path& _Newpval)
	{	// copy _Oldpval to _Newpval, general
	error_code _Code;

	copy(_Oldpval, _Newpval, _Copy_options::none, _Code);
	if (_Code != error_code())
		_THROW_NCEE(filesystem_error,
			"copy(p1, p2): invalid arguments");
	}

inline void copy(const path& _Oldpval, const path& _Newpval,
	error_code& _Code) _NOEXCEPT
	{	// copy _Oldpval to _Newpval, general
	copy(_Oldpval, _Newpval, _Copy_options::none, _Code);
	}

inline void copy(const path& _Oldpval, const path& _Newpval,
	_Copy_options _Opts)
	{	// copy _Oldpval to _Newpval, general
	error_code _Code;

	copy(_Oldpval, _Newpval, _Opts, _Code);
	if (_Code != error_code())
		_THROW_NCEE(filesystem_error,
			"copy(p1, p2, options): invalid arguments");
	}

inline void copy(const path& _Oldpval, const path& _Newpval,
	_Copy_options _Opts,
	error_code& _Code) _NOEXCEPT
	{	// copy _Oldpval to _Newpval, general
	file_status _Oldstat;
	file_status _Newstat;
	error_code _Code2;

	_Code = error_code();
	if ((_Opts & _Copy_options::create_symlinks) != _Copy_options::none
		|| (_Opts & _Copy_options::skip_symlinks) != _Copy_options::none)
		{	// get symlink status
		_Oldstat = symlink_status(_Oldpval);
		_Newstat = symlink_status(_Newpval);
		}
	else
		{	// get file status
		_Oldstat = status(_Oldpval);
		_Newstat = status(_Newpval);
		}
	if (!exists(_Oldstat)
		|| equivalent(_Oldpval, _Newpval, _Code2)
		|| is_other(_Oldstat)
		|| is_other(_Newstat)
		|| (is_directory(_Oldstat) && is_regular_file(_Newstat)))
		_Code = make_error_code(errc::operation_not_permitted);
	else if (is_symlink(_Oldstat))
		if ((_Opts & _Copy_options::skip_symlinks) != _Copy_options::none)
			;
		else if (!exists(_Newstat)
			&& (_Opts & _Copy_options::copy_symlinks) != _Copy_options::none)
			copy_symlink(_Oldpval, _Newpval, _Code);
		else
			_Code = make_error_code(errc::operation_not_permitted);
	else if (is_regular_file(_Oldstat))
		if ((_Opts & _Copy_options::directories_only) != _Copy_options::none)
			;
		else if ((_Opts & _Copy_options::create_symlinks)
			!= _Copy_options::none)
			create_symlink(_Oldpval, _Newpval, _Code);
		else if ((_Opts & _Copy_options::create_hard_links)
			!= _Copy_options::none)
			create_hard_link(_Oldpval, _Newpval, _Code);
		else if (is_directory(_Newstat))
			copy_file(_Oldpval, _Newpval / _Oldpval.filename(), _Opts, _Code);
		else
			copy_file(_Oldpval, _Newpval, _Opts, _Code);
	else if (is_directory(_Oldstat)
		&& _Copy_options_indicate_recursive_copy(_Opts))
		{	// copy directory recursively
		if (!exists(_Newpval) && !create_directory(_Newpval, _Code) && !_Code)
			_Code = make_error_code(errc::operation_not_permitted);
		for (directory_iterator _Next(_Oldpval), _End;
			_Code == error_code() && _Next != _End; ++_Next)
			copy(_Next->path(),
				_Newpval / _Next->path().filename(),
				_Opts | _Copy_options::_Unspecified_recursion_prevention_tag,
				_Code);
		}
	}

inline bool copy_file(const path& _Oldpval, const path& _Newpval,
		_Copy_options _Opt /* = copy_options::none */)
	{	// copy _Oldpval to _Newpval
	error_code _Code;
	bool _Ans = copy_file(_Oldpval, _Newpval, _Opt, _Code);
	if (_Code != error_code())
		_THROW_NCEE(filesystem_error,
			"copy_file(p1, p2, options): invalid arguments");
	return (_Ans);
	}

inline bool copy_file(const path& _Oldpval, const path& _Newpval,
	error_code& _Code) _NOEXCEPT
	{	// copy _Oldpval to _Newpval
	return (copy_file(_Oldpval, _Newpval, copy_options::none, _Code));
	}

inline bool copy_file(const path& _Oldpval, const path& _Newpval,
	_Copy_options _Opt,
	error_code& _Code) _NOEXCEPT
	{	// copy _Oldpval to _Newpval
	_Opt &= (copy_options::skip_existing
		| copy_options::overwrite_existing
		| copy_options::update_existing);
	bool _Exists = exists(_Newpval);
	bool _Ans = false;
	file_time_type _Oldtime;
	file_time_type _Newtime;

	_Code = error_code();
	if (_Exists && (_Opt == copy_options::none
		|| equivalent(_Oldpval, _Newpval, _Code)))
		_Code = make_error_code(errc::file_exists);
	else if (_Code)
		;	// equivalent() failed, report error
	else if (!_Exists
		|| (_Opt & copy_options::overwrite_existing) != copy_options::none
		|| ((_Opt & copy_options::update_existing) != copy_options::none
			&& ((void)(_Oldtime = last_write_time(_Oldpval, _Code)), !_Code)
			&& ((void)(_Newtime = last_write_time(_Newpval, _Code)), !_Code)
			&& _Newtime < _Oldtime))
		{	// ok to overwrite, if necessary
		if (_Copy_file(_Oldpval.c_str(), _Newpval.c_str()) == 0)
			_Ans = true;
		else
			_Code = make_error_code(errc::operation_not_permitted);
		}
	return (_Ans);
	}

inline void copy_symlink(const path& _Oldpval, const path& _Newpval)
	{	// copy symlink, file or directory
	error_code _Code;
	copy_symlink(_Oldpval, _Newpval, _Code);
	if (_Code != error_code())
		_THROW_NCEE(filesystem_error,
			"copy_symlink(p1, p2): invalid arguments");
	}

inline void copy_symlink(const path& _Oldpval, const path& _Newpval,
	error_code& _Code) _NOEXCEPT
	{	// copy symlink, file or directory
	if (is_directory(_Oldpval))
		create_directory_symlink(_Oldpval, _Newpval, _Code);
	else
		create_symlink(_Oldpval, _Newpval, _Code);
	}

inline bool create_directories(const path& _Path)
	{	// create directories chain
	error_code _Code;
	bool _Ans = create_directories(_Path, _Code);
	if (_Code != error_code())
		_THROW_NCEE(filesystem_error,
			"create_directories(p): invalid argument");
	return (_Ans);
	}

inline bool create_directories(const path& _Path,
	error_code& _Code) _NOEXCEPT
	{	// create directory chain
	_Code = error_code();
	if (_Path.empty())
		return (false);
	else if (!exists(_Path))
		{	// recursively create parent directories, then current
		create_directories(_Path.parent_path(), _Code);
		if (_Code)
			return (false);	// report error

		create_directory(_Path, _Code);
		if (_Code)
			return (false);	// report error

		return (true);
		}
	else if (is_directory(_Path))
		return (false);	// directory already there
	else
		{	// report bad argument
		_Code = make_error_code(errc::operation_not_permitted);
		return (false);
		}
	}

inline bool create_directory(const path& _Path)
	{	// create a directory
	error_code _Code;
	bool _Ans = create_directory(_Path, _Code);

	if (_Code != error_code())
		_THROW_NCEE(filesystem_error,
			"create_directory(p): invalid argument");
	return (_Ans);
	}

inline bool create_directory(const path& _Path,
	error_code& _Code) _NOEXCEPT
	{	// create a directory
	int _Ans = _Make_dir(_Path.c_str(), 0);

	_Code = error_code();
	if (_Ans < 0)
		_Code = make_error_code(errc::operation_not_permitted);
	return (0 < _Ans);
	}

inline bool create_directory(const path& _Path, const path& _Attrs)
	{	// create a directory, copying attributes
	error_code _Code;
	bool _Ans = create_directory(_Path, _Attrs, _Code);

	if (_Code != error_code())
		_THROW_NCEE(filesystem_error,
			"create_directory(p1, p2): invalid arguments");
	return (_Ans);
	}

inline bool create_directory(const path& _Path, const path& _Attrs,
	error_code& _Code) _NOEXCEPT
	{	// create a directory, copying attributes
	int _Ans = _Make_dir(_Path.c_str(), _Attrs.c_str());

	_Code = error_code();
	if (_Ans < 0)
		_Code = make_error_code(errc::operation_not_permitted);
	return (0 < _Ans);
	}

inline void create_directory_symlink(const path& _Oldpval,
	const path& _Newpval)
	{	// symlink directory _Newpval to _Oldpval
	error_code _Code;
	create_directory_symlink(_Oldpval, _Newpval, _Code);
	if (_Code != error_code())
		_THROW_NCEE(filesystem_error,
			"create_directory_symlink(p1, p2): invalid arguments");
	}

inline void create_directory_symlink(const path& _Oldpval,
	const path& _Newpval,
	error_code& _Code) _NOEXCEPT
	{	// symlink directory_Newpval to _Oldpval (NB: SAME AS FILE)
	int _Ans = _Symlink(_Oldpval.c_str(), _Newpval.c_str());

	_Code = error_code();
	if (_Ans != 0)
		_Code = error_code(_Ans, _STD system_category());
	}

inline void create_hard_link(const path& _Oldpval, const path& _Newpval)
	{	// hard link _Newpval to _Oldpval
	error_code _Code;
	create_hard_link(_Oldpval, _Newpval, _Code);
	if (_Code != error_code())
		_THROW_NCEE(filesystem_error,
			"create_hard_link(p1, p2): invalid arguments");
	}

inline void create_hard_link(const path& _Oldpval, const path& _Newpval,
	error_code& _Code) _NOEXCEPT
	{	// hard link _Newpval to _Oldpval
	int _Ans = _Link(_Oldpval.c_str(), _Newpval.c_str());

	_Code = error_code();
	if (_Ans != 0)
		_Code = error_code(_Ans, _STD system_category());
	}

inline void create_symlink(const path& _Oldpval, const path& _Newpval)
	{	// symlink _Newpval to _Oldpval
	error_code _Code;
	create_symlink(_Oldpval, _Newpval, _Code);
	if (_Code != error_code())
		_THROW_NCEE(filesystem_error,
			"create_symlink(p1, p2): invalid arguments");
	}

inline void create_symlink(const path& _Oldpval, const path& _Newpval,
	error_code& _Code) _NOEXCEPT
	{	// symlink _Newpval to _Oldpval
	int _Ans = _Symlink(_Oldpval.c_str(), _Newpval.c_str());

	_Code = error_code();
	if (_Ans != 0)
		_Code = error_code(_Ans, _STD system_category());
	}

inline path current_path()
	{	// get current working directory
	error_code _Code;
	path _Ans = current_path(_Code);
	if (_Code != error_code())
		_THROW_NCEE(filesystem_error,
			"current_path(): can't get current working directory");
	return (_Ans);
	}

inline path current_path(
	error_code& _Code)
	{	// get current working directory
	_Code = error_code();
	_Pchar _Dest[_MAX_FILESYS_NAME];
	if (!_Current_get(_Dest))
		{	// report error
		_Code = make_error_code(errc::operation_not_permitted);
		return (path());
		}
	return (path(_Dest));
	}

inline void current_path(const path& _Path)
	{	// set current working directory
	if (!_Current_set(_Path.c_str()))
		_THROW_NCEE(filesystem_error,
			"current_path(p): invalid argument");
	}

inline void current_path(const path& _Path,
	error_code& _Code) _NOEXCEPT
	{	// set current working directory
	_Code = error_code();
	if (!_Current_set(_Path.c_str()))
		_Code = make_error_code(errc::no_such_file_or_directory);
	}

inline bool equivalent(const path& _Path1, const path& _Path2)
	{	// test if paths designate same file
	error_code _Code;
	int _Ans = equivalent(_Path1, _Path2, _Code);

	if (_Code != error_code())
		_THROW_NCEE(filesystem_error,
			"equivalent(p1, p2): invalid arguments");
	return (0 < _Ans);
	}

inline bool equivalent(const path& _Path1, const path& _Path2,
	error_code& _Code) _NOEXCEPT
	{	// test if paths designate same file
	int _Ans = _Equivalent(_Path1.c_str(), _Path2.c_str());

	_Code = error_code();
	if (_Ans < 0)
		_Code = make_error_code(errc::operation_not_permitted);
	return (0 < _Ans);
	}

inline bool exists(file_status _Stat) _NOEXCEPT
	{	// test if path exists
	return (status_known(_Stat)
		&& _Stat.type() != _File_type::not_found);
	}

inline bool exists(const path& _Path)
	{	// test if path exists
	return (exists(status(_Path)));
	}

inline bool exists(const path& _Path,
	error_code& _Code) _NOEXCEPT
	{	// test if path exists
	_Code = error_code();
	return (exists(status(_Path)));
	}

inline uintmax_t file_size(const path& _Path)
	{	// get file size
	error_code _Code;
	uintmax_t _Ans = file_size(_Path, _Code);
	if (_Code != error_code())
		_THROW_NCEE(filesystem_error,
			"file_size(p): invalid argument");
	return (_Ans);
	}

inline uintmax_t file_size(const path& _Path,
	error_code& _Code) _NOEXCEPT
	{	// get file size
	uintmax_t _Ans = (uintmax_t)-1;
	file_status _Stat = status(_Path);

	_Code = error_code();
	if (exists(_Stat) && is_regular_file(_Stat))
		_Ans = _File_size(_Path.c_str());
	if (_Ans == (uintmax_t)-1)
		_Code = make_error_code(errc::operation_not_permitted);
	return (_Ans);
	}

inline uintmax_t hard_link_count(const path& _Path)
	{	// get hard link count
	error_code _Code;
	uintmax_t _Ans = hard_link_count(_Path, _Code);
	if (_Code != error_code())
		_THROW_NCEE(filesystem_error,
			"hard_link_count(p): invalid argument");
	return (_Ans);
	}

inline uintmax_t hard_link_count(const path& _Path,
	error_code& _Code) _NOEXCEPT
	{	// get hard link count
	_Code = error_code();
	uintmax_t _Ans = _Hard_links(_Path.c_str());
	if (_Ans == (uintmax_t)-1)
		_Code = make_error_code(errc::operation_not_permitted);
	return (_Ans);
	}

inline bool is_block_file(file_status _Stat) _NOEXCEPT
	{	// test for block file
	return (_Stat.type() == _File_type::block);
	}

inline bool is_block_file(const path& _Path)
	{	// test for block file
	return (is_block_file(status(_Path)));
	}

inline bool is_block_file(const path& _Path,
	error_code& _Code) _NOEXCEPT
	{	// test for block file
	return (is_block_file(status(_Path, _Code)));
	}

inline bool is_character_file(file_status _Stat) _NOEXCEPT
	{	// test for character file
	return (_Stat.type() == _File_type::character);
	}

inline bool is_character_file(const path& _Path)
	{	// test for character file
	return (is_character_file(status(_Path)));
	}

inline bool is_character_file(const path& _Path,
	error_code& _Code) _NOEXCEPT
	{	// test for character file
	return (is_character_file(status(_Path, _Code)));
	}

inline bool is_directory(file_status _Stat) _NOEXCEPT
	{	// test for directory
	return (_Stat.type() == _File_type::directory);
	}

inline bool is_directory(const path& _Path)
	{	// test for directory
	return (is_directory(status(_Path)));
	}

inline bool is_directory(const path& _Path,
	error_code& _Code) _NOEXCEPT
	{	// test for directory
	return (is_directory(status(_Path, _Code)));
	}

inline bool is_empty(const path& _Path)
	{	// test if path is empty
	error_code _Code;
	bool _Ans = _FSPFX is_empty(_Path, _Code);
	if (_Code != error_code())
		_THROW_NCEE(filesystem_error,
			"is_empty(p): invalid argument");
	return (_Ans);
	}

inline bool is_empty(const path& _Path,
	error_code& _Code) _NOEXCEPT
	{	// test if path is empty
	file_status _Stat = status(_Path);

	_Code = error_code();
	if (is_directory(_Stat))
		return (directory_iterator(_Path)
			== directory_iterator());
	else
		return (file_size(_Path, _Code) == 0);
	}

inline bool is_fifo(file_status _Stat) _NOEXCEPT
	{	// test for fifo file
	return (_Stat.type() == _File_type::fifo);
	}

inline bool is_fifo(const path& _Path)
	{	// test for fifo file
	return (is_fifo(status(_Path)));
	}

inline bool is_fifo(const path& _Path,
	error_code& _Code) _NOEXCEPT
	{	// test for fifo file
	return (is_fifo(status(_Path, _Code)));
	}

inline bool is_other(file_status _Stat) _NOEXCEPT
	{	// test if path is other file type
	return (exists(_Stat)
		&& !is_regular_file(_Stat)
		&& !is_directory(_Stat)
		&& !is_symlink(_Stat));
	}

inline bool is_other(const path& _Path)
	{	// test if path is other file type
	return (is_other(status(_Path)));
	}

inline bool is_other(const path& _Path,
	error_code& _Code) _NOEXCEPT
	{	// test for other file types
	return (is_other(status(_Path, _Code)));
	}

inline bool is_regular_file(file_status _Stat) _NOEXCEPT
	{	// test for regular file
	return (_Stat.type() == _File_type::regular);
	}

inline bool is_regular_file(const path& _Path)
	{	// test for regular file
	return (is_regular_file(status(_Path)));
	}

inline bool is_regular_file(const path& _Path,
	error_code& _Code) _NOEXCEPT
	{	// test for regular file
	return (is_regular_file(status(_Path, _Code)));
	}

inline bool is_socket(file_status _Stat) _NOEXCEPT
	{	// test for socket file
	return (_Stat.type() == _File_type::socket);
	}

inline bool is_socket(const path& _Path)
	{	// test for socket file
	return (is_socket(status(_Path)));
	}

inline bool is_socket(const path& _Path,
	error_code& _Code) _NOEXCEPT
	{	// test for socket file
	return (is_socket(status(_Path, _Code)));
	}

inline bool is_symlink(file_status _Stat) _NOEXCEPT
	{	// test for symlink
	return (_Stat.type() == _File_type::symlink);
	}

inline bool is_symlink(const path& _Path)
	{	// test for symlink
	return (is_symlink(status(_Path)));
	}

inline bool is_symlink(const path& _Path,
	error_code& _Code) _NOEXCEPT
	{	// test for symlink
	return (is_symlink(status(_Path, _Code)));
	}

inline file_time_type last_write_time(const path& _Path)
	{	// get last write time
	error_code _Code;
	file_time_type _Ans = last_write_time(_Path, _Code);
	if (_Code != error_code())
		_THROW_NCEE(filesystem_error,
			"last_write_time(p): invalid argument");
	return (_Ans);
	}

inline file_time_type last_write_time(const path& _Path,
	error_code& _Code) _NOEXCEPT
	{	// get last write time
	int64_t _Ticks = _Last_write_time(_Path.c_str());

	_Code = error_code();
	if (_Ticks == -1)
		{	// report error
		_Code = make_error_code(errc::operation_not_permitted);
		return ((file_time_type::min)());
		}
	return (file_time_type(chrono::system_clock::duration(_Ticks)));
	}

inline void last_write_time(const path& _Path, file_time_type _Newtime)
	{	// set last write time
	error_code _Code;
	last_write_time(_Path, _Newtime, _Code);
	if (_Code != error_code())
		_THROW_NCEE(filesystem_error,
			"last_write_time(p, new_time): invalid arguments");
	}

inline void last_write_time(const path& _Path, file_time_type _Newtime,
	error_code& _Code) _NOEXCEPT
	{	// set last write time
	_Code = error_code();
	if (_Set_last_write_time(_Path.c_str(),
		_Newtime.time_since_epoch().count()) == 0)
		_Code = make_error_code(errc::operation_not_permitted);
	}

inline void permissions(const path& _Path, _Perms _Mask)
	{	// set access permissions
	error_code _Code;
	permissions(_Path, _Mask, _Code);
	if (_Code != error_code())
		_THROW_NCEE(filesystem_error,
			"permissions(p, prms): can't set permissions");
	}

inline void permissions(const path& _Path, _Perms _Mask,
	error_code& _Code) _NOEXCEPT
	{	// set access permissions
	_Perms _Todo = _Mask;
	_Mask = (_Perms)(_Mask & perms::mask);
	bool _Ok = true;

	_Code = error_code();
	if ((_Todo & perms::add_perms) == perms::none)
		if ((_Todo & perms::remove_perms) == perms::none)
			;
		else
			_Mask = (_Perms)(status(_Path).permissions() & ~_Mask);
	else
		if ((_Todo & perms::remove_perms) == perms::none)
			_Mask = (_Perms)(_Mask | status(_Path).permissions());
		else
			_Ok = false;
	if (!_Ok || _Chmod(_Path.c_str(), _Mask) != 0)
		_Code = make_error_code(errc::operation_not_permitted);
	}

inline path read_symlink(const path& _Path)
	{	// read symbolic link
	error_code _Code;
	path _Sympath = read_symlink(_Path, _Code);
	if (_Code != error_code())
		_THROW_NCEE(filesystem_error,
			"read_symlink(p): can't read symbolic link");
	return (_Sympath);
	}

inline path read_symlink(const path& _Path,
	error_code& _Code)
	{	// read symbolic link
	_Code = error_code();
	if (!is_symlink(_Path))
		{	// report error
		_Code = make_error_code(errc::no_such_file_or_directory);
		return (path());
		}
	_Pchar _Dest[_MAX_FILESYS_NAME];
	return (path(_Symlink_get(_Dest, _Path.c_str())));
	}

inline bool remove(const path& _Path)
	{	// remove a file
	error_code _Code;
	bool _Ans = remove(_Path, _Code);

	if (_Code != error_code())
		_THROW_NCEE(filesystem_error,
			"remove(p): invalid argument");
	return (_Ans);
	}

inline bool remove(const path& _Path,
	error_code& _Code) _NOEXCEPT
	{	// remove a file
	_Code = error_code();
	if (!exists(symlink_status(_Path)))
		return (false);
	else if (is_directory(_Path, _Code))
		if (_Remove_dir(_Path.c_str()))
			return (true);
		else
			{	// couldn't remove emptied directory, report it
			_Code = make_error_code(errc::operation_not_permitted);
			return (false);
			}
	else
		{	// file exists, try to remove it
		bool _Ans = _Unlink(_Path.c_str()) == 0;
		if (!_Ans)
			_Code = make_error_code(errc::operation_not_permitted);
		return (_Ans);
		}
	}

inline bool _Remove_all(const path& _Path, uintmax_t& _Ans,
	error_code& _Code) _NOEXCEPT
	{	// recursively remove a file or directory, count removed files
	_Code = error_code();
	if (is_directory(_Path))
		{	// empty and remove a directory
		typedef _Directory_iterator<false_type> _Myit;
		_Myit _Last;

		for (; ; )
			{	// remove a directory entry
			_Myit _First(_Path);
			if (_First == _Last)
				break;
			if (!_Remove_all(_Path / _First->path(), _Ans, _Code))
				return (false);
			}
		if (_Remove_dir(_Path.c_str()))
			return (true);
		else
			{	// couldn't remove emptied directory, report it
			_Code = make_error_code(errc::operation_not_permitted);
			return (false);
			}
		}
	else if (!remove(_Path, _Code))
		return (false);
	else
		{	// file remove succeeded, count it
		++_Ans;
		return (true);
		}
	}

inline uintmax_t remove_all(const path& _Path)
	{	// recursively remove a directory
	error_code _Code;
	uintmax_t _Ans = remove_all(_Path, _Code);
	if (_Code != error_code())
		_THROW_NCEE(filesystem_error,
			"remove_all(p): invalid argument");
	return (_Ans);
	}

inline uintmax_t remove_all(const path& _Path,
	error_code& _Code) _NOEXCEPT
	{	// recursively remove a directory
	uintmax_t _Ans = 0;
	_Remove_all(_Path, _Ans, _Code);
	if (_Code)
		return (static_cast<uintmax_t>(-1));
	return (_Ans);
	}

inline void rename(const path& _Oldpval, const path& _Newpval)
	{	// rename _Oldpval as _Newpval
	error_code _Code;
	rename(_Oldpval, _Newpval, _Code);
	if (_Code != error_code())
		_THROW_NCEE(filesystem_error,
			"rename(p1, p2): invalid arguments");
	}

inline void rename(const path& _Oldpval, const path& _Newpval,
	error_code& _Code) _NOEXCEPT
	{	// rename _Oldpval as _Newpval
	_Code = error_code();

	if (!exists(_Oldpval))
		{	// fail immediately without modifying the filesystem
		_Code = make_error_code(errc::operation_not_permitted);
		return;
		}

	if (exists(_Newpval))
		{	// both exist; there can be only one
		if (equivalent(_Oldpval, _Newpval, _Code) || _Code != error_code())
			return;	// successful no-op, or report equivalent() failure

		if (!remove(_Newpval, _Code))
			return;	// report remove() failure
		}

	if (_Rename(_Oldpval.c_str(), _Newpval.c_str()) != 0)
		_Code = make_error_code(errc::operation_not_permitted);
	}

inline void resize_file(const path& _Path, uintmax_t _Newsize)
	{	// change file size
	error_code _Code;
	resize_file(_Path, _Newsize, _Code);
	if (_Code != error_code())
		_THROW_NCEE(filesystem_error,
			"resize_file(p, n): invalid arguments");
	};

inline void resize_file(const path& _Path, uintmax_t _Newsize,
	error_code& _Code) _NOEXCEPT
	{	// change file size
	int _Errno = _Resize(_Path.c_str(), _Newsize);

	_Code = error_code();
	if (_Errno != 0)
		_Code = error_code(_Errno, _STD system_category());
	}

inline space_info space(const path& _Path)
	{	// get space information for volume
	return (_Statvfs(_Path.c_str()));
	}

inline space_info space(const path& _Path,
	error_code& _Code) _NOEXCEPT
	{	// get space information for volume
	_Code = error_code();
	return (_Statvfs(_Path.c_str()));
	}

inline file_status status(const path& _Path)
	{	// get file status
	error_code _Code;
	return (status(_Path, _Code));
	}

inline file_status status(const path& _Path,
	error_code& _Code) _NOEXCEPT
	{	// get file status
	_Code = error_code();
	_Perms _Mode;
	_File_type _Ftype = _Stat(_Path.c_str(), &_Mode);
	return (file_status(_Ftype, _Mode));
	}

inline bool status_known(file_status _Stat) _NOEXCEPT
	{	// test if status known
	return (_Stat.type() != _File_type::unknown);
	}

inline file_status symlink_status(const path& _Path)
	{	// get symlink file status
	error_code _Code;
	return (symlink_status(_Path, _Code));
	}

inline file_status symlink_status(const path& _Path,
	error_code& _Code) _NOEXCEPT
	{	// get symlink file status
	_Perms _Mode;
	_File_type _Ftype = _Lstat(_Path.c_str(), &_Mode);

	_Code = error_code();
	return (file_status(_Ftype, _Mode));
	}

inline path system_complete(const path& _Path)
	{	// get completely specified path
	if (_Path.is_absolute())
		return (_Path);
	path _Curpath = current_path();
	if (!_Path.has_root_name() || _Path.root_name() == _Curpath.root_name())
		return (absolute(_Path));
	current_path(_Path.root_name());	// switch to new device
	path _Altpath = current_path();	// get its full current path
	current_path(_Curpath);	// revert to original current path
	return (_Altpath / _Path.relative_path());
	}

inline path system_complete(const path& _Path,
	error_code& _Code)
	{	// get completely specified path
	_Code = error_code();

	path _Ans;

	_TRY_BEGIN
	_Ans = system_complete(_Path);
	_CATCH_ALL
	_Code = make_error_code(errc::operation_not_permitted);
	return (path());
	_CATCH_END

	return (_Ans);
	}

inline path temp_directory_path()
	{	// get temporary directory
	error_code _Code;
	path _Ans = temp_directory_path(_Code);
	if (_Code != error_code())
		_THROW_NCEE(filesystem_error,
			"temp_directory_path(): can't find temp directory");
	return (_Ans);
	}

inline path temp_directory_path(
	error_code& _Code)
	{	// get temporary directory
	_Pchar _Dest[_MAX_FILESYS_NAME];
	_Temp_get(_Dest);
	path _Ans(_Dest);

	_Code = error_code();
	if (!exists(_Ans) || !is_directory(_Ans))
		{	// report error
		_Code = make_error_code(errc::no_such_file_or_directory);
		return (path());
		}
	return (_Ans);
	}
		}	// inline namespace v1
	}	// namespace filesystem
}	// namespace experimental

namespace tr2 {
	namespace sys {
using namespace _STD experimental::filesystem::v1;	// retained
	}	// namespace sys
}	// namespace tr2
_STD_END
 #pragma pop_macro("new")
 #pragma warning(pop)
 #pragma pack(pop)
#endif /* RC_INVOKED */
#endif /* _FILESYSTEM_ */

/*
 * Copyright (c) by P.J. Plauger. All rights reserved.
 * Consult your license regarding permissions and restrictions.
V6.50:0009 */