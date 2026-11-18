/* A Bison parser, made by GNU Bison 3.5.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Substitute the type names.  */
#define YYSTYPE         PARSER_STYPE
#define YYLTYPE         PARSER_LTYPE
/* Substitute the variable and function names.  */
#define yyparse         parser_parse
#define yylex           parser_lex
#define yyerror         parser_error
#define yydebug         parser_debug
#define yynerrs         parser_nerrs

/* First part of user prologue.  */
#line 19 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"


#include "jscript.h"
#include "engine.h"
#include "parser.h"

#include "wine/debug.h"

WINE_DEFAULT_DEBUG_CHANNEL(jscript);

static int parser_error(unsigned*,parser_ctx_t*,const char*);
static void set_error(parser_ctx_t*,unsigned,HRESULT);
static BOOL explicit_error(parser_ctx_t*,void*,WCHAR);
static BOOL allow_auto_semicolon(parser_ctx_t*);

static literal_t *new_string_literal(parser_ctx_t*,jsstr_t*);
static literal_t *new_null_literal(parser_ctx_t*);

typedef struct _property_list_t {
    property_definition_t *head;
    property_definition_t *tail;
} property_list_t;

static property_definition_t *new_property_definition(parser_ctx_t *ctx, property_definition_type_t,
                                                      literal_t *name, expression_t *value);
static property_list_t *new_property_list(parser_ctx_t*,property_definition_t*);
static property_list_t *property_list_add(parser_ctx_t*,property_list_t*,property_definition_t*);

typedef struct _element_list_t {
    array_element_t *head;
    array_element_t *tail;
} element_list_t;

static element_list_t *new_element_list(parser_ctx_t*,int,expression_t*);
static element_list_t *element_list_add(parser_ctx_t*,element_list_t*,int,expression_t*);

typedef struct _argument_list_t {
    argument_t *head;
    argument_t *tail;
} argument_list_t;

static argument_list_t *new_argument_list(parser_ctx_t*,expression_t*);
static argument_list_t *argument_list_add(parser_ctx_t*,argument_list_t*,expression_t*);

typedef struct _case_list_t {
    case_clausule_t *head;
    case_clausule_t *tail;
} case_list_t;

typedef struct _statement_list_t {
   statement_t *head;
   statement_t *tail;
} statement_list_t;

static catch_block_t *new_catch_block(parser_ctx_t*,const WCHAR*,statement_t*);
static case_clausule_t *new_case_clausule(parser_ctx_t*,unsigned,expression_t*,statement_list_t*);
static case_list_t *new_case_list(parser_ctx_t*,case_clausule_t*);
static case_list_t *case_list_add(parser_ctx_t*,case_list_t*,case_clausule_t*);
static case_clausule_t *new_case_block(parser_ctx_t*,case_list_t*,case_clausule_t*,case_list_t*);

typedef struct _variable_list_t {
    variable_declaration_t *head;
    variable_declaration_t *tail;
} variable_list_t;

static variable_declaration_t *new_variable_declaration(parser_ctx_t*,const WCHAR*,expression_t*);
static variable_list_t *new_variable_list(parser_ctx_t*,variable_declaration_t*);
static variable_list_t *variable_list_add(parser_ctx_t*,variable_list_t*,variable_declaration_t*);

static void *new_statement(parser_ctx_t*,statement_type_t,size_t,unsigned);
static statement_t *new_block_statement(parser_ctx_t*,unsigned,statement_list_t*);
static statement_t *new_var_statement(parser_ctx_t*,BOOL,BOOL,unsigned,variable_list_t*);
static statement_t *new_expression_statement(parser_ctx_t*,unsigned,expression_t*);
static statement_t *new_if_statement(parser_ctx_t*,unsigned,expression_t*,statement_t*,statement_t*);
static statement_t *new_while_statement(parser_ctx_t*,unsigned,BOOL,expression_t*,statement_t*);
static statement_t *new_for_statement(parser_ctx_t*,unsigned,variable_declaration_t*,expression_t*,expression_t*,unsigned,
                                      expression_t*,unsigned,statement_t*);
static statement_t *new_forin_statement(parser_ctx_t*,unsigned,variable_declaration_t*,expression_t*,expression_t*,statement_t*);
static statement_t *new_continue_statement(parser_ctx_t*,unsigned,const WCHAR*);
static statement_t *new_break_statement(parser_ctx_t*,unsigned,const WCHAR*);
static statement_t *new_return_statement(parser_ctx_t*,unsigned,expression_t*);
static statement_t *new_with_statement(parser_ctx_t*,unsigned,expression_t*,statement_t*);
static statement_t *new_labelled_statement(parser_ctx_t*,unsigned,const WCHAR*,statement_t*);
static statement_t *new_switch_statement(parser_ctx_t*,unsigned,expression_t*,case_clausule_t*);
static statement_t *new_throw_statement(parser_ctx_t*,unsigned,expression_t*);
static statement_t *new_try_statement(parser_ctx_t*,statement_t*,catch_block_t*,statement_t*,unsigned);

static statement_list_t *new_statement_list(parser_ctx_t*,statement_t*);
static statement_list_t *statement_list_add(statement_list_t*,statement_t*);

typedef struct _parameter_list_t {
    parameter_t *head;
    parameter_t *tail;
} parameter_list_t;

static parameter_list_t *new_parameter_list(parser_ctx_t*,const WCHAR*);
static parameter_list_t *parameter_list_add(parser_ctx_t*,parameter_list_t*,const WCHAR*);

static void *new_expression(parser_ctx_t *ctx,expression_type_t,size_t);
static expression_t *new_function_expression(parser_ctx_t*,const WCHAR*,parameter_list_t*,
        statement_list_t*,const WCHAR*,const WCHAR*,DWORD);
static expression_t *new_binary_expression(parser_ctx_t*,expression_type_t,expression_t*,expression_t*);
static expression_t *new_unary_expression(parser_ctx_t*,expression_type_t,expression_t*);
static expression_t *new_conditional_expression(parser_ctx_t*,expression_t*,expression_t*,expression_t*);
static expression_t *new_member_expression(parser_ctx_t*,expression_t*,const WCHAR*);
static expression_t *new_new_expression(parser_ctx_t*,expression_t*,argument_list_t*);
static expression_t *new_call_expression(parser_ctx_t*,expression_t*,argument_list_t*);
static expression_t *new_identifier_expression(parser_ctx_t*,const WCHAR*);
static expression_t *new_literal_expression(parser_ctx_t*,literal_t*);
static expression_t *new_array_literal_expression(parser_ctx_t*,element_list_t*,int);
static expression_t *new_prop_and_value_expression(parser_ctx_t*,property_list_t*);

#define PARSER_LTYPE unsigned
#define YYLLOC_DEFAULT(Cur, Rhs, N) Cur = YYRHSLOC((Rhs), (N) ? 1 : 0)


#line 194 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_PARSER_E_KNSOFT_KUNDOWS_BUILD_REACTOSPARSERSOURCES_JSCRIPT_PARSER_TAB_H_INCLUDED
# define YY_PARSER_E_KNSOFT_KUNDOWS_BUILD_REACTOSPARSERSOURCES_JSCRIPT_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef PARSER_DEBUG
# if defined YYDEBUG
#if YYDEBUG
#   define PARSER_DEBUG 1
#  else
#   define PARSER_DEBUG 0
#  endif
# else /* ! defined YYDEBUG */
#  define PARSER_DEBUG 0
# endif /* ! defined YYDEBUG */
#endif  /* ! defined PARSER_DEBUG */
#if PARSER_DEBUG
extern int parser_debug;
#endif

/* Token type.  */
#ifndef PARSER_TOKENTYPE
# define PARSER_TOKENTYPE
  enum parser_tokentype
  {
    kBREAK = 258,
    kCASE = 259,
    kCATCH = 260,
    kCONST = 261,
    kCONTINUE = 262,
    kDEFAULT = 263,
    kDELETE = 264,
    kDO = 265,
    kELSE = 266,
    kFUNCTION = 267,
    kIF = 268,
    kFINALLY = 269,
    kFOR = 270,
    kGET = 271,
    kIN = 272,
    kLET = 273,
    kSET = 274,
    kINSTANCEOF = 275,
    kNEW = 276,
    kNULL = 277,
    kRETURN = 278,
    kSWITCH = 279,
    kTHIS = 280,
    kTHROW = 281,
    kTRUE = 282,
    kFALSE = 283,
    kTRY = 284,
    kTYPEOF = 285,
    kVAR = 286,
    kVOID = 287,
    kWHILE = 288,
    kWITH = 289,
    tANDAND = 290,
    tOROR = 291,
    tINC = 292,
    tDEC = 293,
    tHTMLCOMMENT = 294,
    kDIVEQ = 295,
    kDCOL = 296,
    tIdentifier = 297,
    tAssignOper = 298,
    tEqOper = 299,
    tShiftOper = 300,
    tRelOper = 301,
    tNumericLiteral = 302,
    tBooleanLiteral = 303,
    tStringLiteral = 304,
    LOWER_THAN_ELSE = 305
  };
#endif

/* Value type.  */
#if ! defined PARSER_STYPE && ! defined PARSER_STYPE_IS_DECLARED
union PARSER_STYPE
{
#line 142 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"

    int                     ival;
    jsstr_t                 *str;
    literal_t               *literal;
    struct _argument_list_t *argument_list;
    case_clausule_t         *case_clausule;
    struct _case_list_t     *case_list;
    catch_block_t           *catch_block;
    struct _element_list_t  *element_list;
    expression_t            *expr;
    const WCHAR            *identifier;
    struct _parameter_list_t *parameter_list;
    struct _property_list_t *property_list;
    property_definition_t   *property_definition;
    statement_t             *statement;
    struct _statement_list_t *statement_list;
    struct _variable_list_t *variable_list;
    variable_declaration_t  *variable_declaration;

#line 325 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"

};
typedef union PARSER_STYPE PARSER_STYPE;
# define PARSER_STYPE_IS_TRIVIAL 1
# define PARSER_STYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined PARSER_LTYPE && ! defined PARSER_LTYPE_IS_DECLARED
typedef struct PARSER_LTYPE PARSER_LTYPE;
struct PARSER_LTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define PARSER_LTYPE_IS_DECLARED 1
# define PARSER_LTYPE_IS_TRIVIAL 1
#endif



int parser_parse (parser_ctx_t *ctx);

#endif /* !YY_PARSER_E_KNSOFT_KUNDOWS_BUILD_REACTOSPARSERSOURCES_JSCRIPT_PARSER_TAB_H_INCLUDED  */



#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))

/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined PARSER_LTYPE_IS_TRIVIAL && PARSER_LTYPE_IS_TRIVIAL \
             && defined PARSER_STYPE_IS_TRIVIAL && PARSER_STYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  189
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1639

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  73
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  107
/* YYNRULES -- Number of rules.  */
#define YYNRULES  274
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  498

#define YYUNDEFTOK  2
#define YYMAXUTOK   305


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    67,     2,     2,     2,    65,    60,     2,
      71,    72,    63,    61,    54,    62,    70,    64,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    51,    56,
       2,    55,     2,    57,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    68,     2,    69,    59,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    52,    58,    53,    66,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50
};

#if PARSER_DEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   254,   254,   259,   262,   263,   267,   271,   273,   275,
     280,   284,   285,   290,   291,   295,   296,   297,   298,   299,
     300,   301,   302,   303,   304,   305,   306,   307,   308,   309,
     313,   317,   318,   322,   323,   328,   329,   333,   334,   338,
     340,   353,   355,   368,   373,   374,   379,   381,   386,   391,
     396,   397,   401,   406,   407,   411,   416,   420,   425,   427,
     432,   434,   437,   439,   436,   443,   445,   442,   448,   450,
     453,   455,   452,   462,   467,   472,   477,   482,   489,   490,
     491,   495,   500,   502,   507,   508,   512,   513,   518,   523,
     528,   533,   534,   535,   540,   545,   549,   550,   553,   554,
     558,   559,   564,   565,   569,   571,   575,   576,   580,   581,
     583,   588,   590,   592,   597,   598,   603,   605,   610,   611,
     616,   618,   623,   624,   629,   631,   636,   637,   642,   644,
     649,   650,   655,   657,   662,   663,   668,   670,   675,   676,
     681,   682,   687,   688,   690,   692,   697,   698,   700,   705,
     706,   711,   713,   715,   720,   721,   723,   725,   730,   731,
     733,   734,   736,   737,   738,   739,   740,   741,   745,   747,
     749,   755,   756,   760,   761,   765,   766,   767,   769,   771,
     776,   778,   780,   782,   787,   788,   792,   793,   798,   800,
     801,   802,   803,   804,   805,   806,   807,   811,   812,   813,
     814,   819,   821,   826,   827,   831,   832,   836,   837,   839,
     851,   852,   857,   859,   861,   865,   870,   871,   872,   876,
     877,   880,   881,   885,   886,   898,   899,   900,   901,   902,
     903,   904,   905,   906,   907,   908,   909,   910,   911,   912,
     913,   914,   915,   916,   917,   918,   919,   920,   921,   922,
     923,   924,   925,   926,   927,   930,   931,   932,   936,   937,
     938,   939,   940,   942,   947,   948,   949,   952,   953,   956,
     957,   960,   961,   964,   965
};
#endif

#if PARSER_DEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "kBREAK", "kCASE", "kCATCH", "kCONST",
  "kCONTINUE", "kDEFAULT", "kDELETE", "kDO", "kELSE", "kFUNCTION", "kIF",
  "kFINALLY", "kFOR", "kGET", "kIN", "kLET", "kSET", "kINSTANCEOF", "kNEW",
  "kNULL", "kRETURN", "kSWITCH", "kTHIS", "kTHROW", "kTRUE", "kFALSE",
  "kTRY", "kTYPEOF", "kVAR", "kVOID", "kWHILE", "kWITH", "tANDAND",
  "tOROR", "tINC", "tDEC", "tHTMLCOMMENT", "kDIVEQ", "kDCOL",
  "tIdentifier", "tAssignOper", "tEqOper", "tShiftOper", "tRelOper",
  "tNumericLiteral", "tBooleanLiteral", "tStringLiteral",
  "LOWER_THAN_ELSE", "':'", "'{'", "'}'", "','", "'='", "';'", "'?'",
  "'|'", "'^'", "'&'", "'+'", "'-'", "'*'", "'/'", "'%'", "'~'", "'!'",
  "'['", "']'", "'.'", "'('", "')'", "$accept", "Script", "ScriptBody",
  "HtmlComment", "FunctionStatementList", "FunctionExpression",
  "FunctionBody", "FormalParameterList", "FormalParameterList_opt",
  "Statement", "Declaration", "StatementListItem", "StatementList",
  "StatementList_opt", "Block", "LexicalDeclaration",
  "LexicalDeclarationNoIn", "VariableStatement", "VariableDeclarationList",
  "VariableDeclarationListNoIn", "VariableDeclaration",
  "VariableDeclarationNoIn", "Initialiser_opt", "Initialiser",
  "InitialiserNoIn_opt", "InitialiserNoIn", "EmptyStatement",
  "ExpressionStatement", "IfStatement", "IterationStatement", "$@1", "$@2",
  "$@3", "$@4", "$@5", "$@6", "ContinueStatement", "BreakStatement",
  "ReturnStatement", "WithStatement", "LabelledStatement",
  "SwitchStatement", "CaseBlock", "CaseClausules_opt", "CaseClausules",
  "CaseClausule", "DefaultClausule", "ThrowStatement", "TryStatement",
  "Catch", "Finally", "Expression_opt", "Expression_err", "Expression",
  "ExpressionNoIn_opt", "ExpressionNoIn", "AssignOper",
  "AssignmentExpression", "AssignmentExpressionNoIn",
  "ConditionalExpression", "ConditionalExpressionNoIn",
  "LogicalORExpression", "LogicalORExpressionNoIn", "LogicalANDExpression",
  "LogicalANDExpressionNoIn", "BitwiseORExpression",
  "BitwiseORExpressionNoIn", "BitwiseXORExpression",
  "BitwiseXORExpressionNoIn", "BitwiseANDExpression",
  "BitwiseANDExpressionNoIn", "EqualityExpression",
  "EqualityExpressionNoIn", "RelationalExpression",
  "RelationalExpressionNoIn", "ShiftExpression", "AdditiveExpression",
  "MultiplicativeExpression", "UnaryExpression", "PostfixExpression",
  "LeftHandSideExpression", "NewExpression", "MemberExpression",
  "CallExpression", "Arguments", "ArgumentList", "PrimaryExpression",
  "ArrayLiteral", "ElementList", "Elision", "Elision_opt", "ObjectLiteral",
  "PropertyNameAndValueList", "PropertyDefinition", "GetterSetterMethod",
  "PropertyName", "Identifier_opt", "Identifier", "IdentifierName",
  "ReservedAsIdentifier", "ES5Keyword", "Literal", "BooleanLiteral",
  "semicolon_opt", "left_bracket", "right_bracket", "semicolon", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,    58,   123,   125,    44,    61,    59,    63,   124,    94,
      38,    43,    45,    42,    47,    37,   126,    33,    91,    93,
      46,    40,    41
};
# endif

#define YYPACT_NINF (-376)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-258)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     853,   315,   315,   315,  1224,   919,   266,    18,    18,   -27,
     258,    58,  1281,  -376,  1224,    18,  -376,  1224,  -376,  -376,
      63,  1224,   315,  1224,    18,    18,  1224,  1224,  -376,   100,
    -376,  -376,  -376,   655,  -376,  1224,  1224,  -376,  1224,  1224,
     206,  1224,   163,    41,   540,  -376,  -376,  -376,   853,  -376,
    -376,  -376,  -376,  -376,  -376,  -376,  -376,  -376,  -376,  -376,
    -376,  -376,  -376,  -376,  -376,    73,  -376,  -376,   125,   132,
     113,   118,   119,   150,   210,   180,    -6,   311,  -376,  -376,
     273,  -376,   278,   333,  -376,  -376,  -376,  -376,  -376,  -376,
    -376,  -376,  -376,    56,  -376,  -376,    80,  -376,   209,    56,
    -376,  -376,  -376,  -376,  1350,  -376,  -376,    35,   177,   228,
    -376,  -376,    30,   315,   506,   982,   919,   919,    80,   919,
    -376,   278,    56,   211,  1224,    73,   721,   254,  -376,    80,
    -376,   506,  1224,  -376,  -376,   919,   288,  -376,  -376,   315,
     296,  -376,  1224,   919,  -376,   173,    31,  -376,    62,  1452,
    -376,   303,  1501,  -376,  1281,   218,  1103,    95,   245,  1224,
     248,   250,    63,  1224,   315,  1224,   167,   175,   100,   272,
     290,   587,   787,    50,  -376,   291,  -376,  -376,  -376,  -376,
    -376,  -376,  -376,  -376,  -376,   249,   251,  1224,   160,  -376,
    -376,  -376,  -376,  -376,  1224,  -376,  -376,  1224,  1224,  1224,
    1224,  1224,  1224,  1224,  1224,  1224,  1224,  1224,  1224,  1224,
    1224,  1224,  1224,  -376,  -376,  -376,  -376,  1224,  1224,  1224,
    1597,  1042,  -376,  1224,  1597,  -376,  -376,   315,  -376,  1224,
    -376,  -376,  -376,  -376,  -376,  -376,  -376,  -376,  -376,  -376,
    -376,  1550,  1550,  -376,  -376,  -376,  -376,  -376,  -376,  -376,
    -376,  -376,  -376,  -376,  -376,  -376,  -376,  -376,  -376,  -376,
    -376,    18,   315,   315,   270,    27,  -376,  -376,    27,   211,
     315,   315,   315,  -376,  -376,   289,  -376,  -376,   133,   318,
     292,   299,   309,   340,    72,   180,   199,  -376,  -376,  -376,
    -376,  -376,  -376,    22,  -376,  -376,    18,    63,   371,  -376,
    -376,    27,    22,  -376,    18,    18,  -376,  -376,  1401,  1224,
     336,  -376,  -376,  -376,  -376,  -376,  -376,   132,   342,   113,
     118,   119,   150,   210,   180,   180,   180,    -6,   311,   311,
    -376,  -376,  -376,  -376,  -376,   263,  -376,  -376,  -376,   194,
     271,  -376,  -376,  -376,   506,    18,    27,   315,  -376,  -376,
     343,   919,   127,  -376,   345,   127,   351,   389,  1224,   156,
    1224,  1224,  1224,  1224,  1224,  1224,  1224,  1224,  1224,  1224,
     506,  1224,  1224,   356,   315,  -376,  -376,   919,   919,  -376,
     315,  -376,  -376,  -376,  -376,   357,  1163,  1224,  -376,  1224,
    -376,  -376,    27,   315,   358,  -376,   853,   401,   315,  -376,
    1224,  -376,  -376,  -376,   156,   506,  -376,  -376,  -376,  1224,
    -376,   354,   318,   362,   292,   299,   309,   340,    72,   180,
     180,    27,  -376,  -376,   410,  -376,    27,  -376,  -376,    27,
    -376,  -376,  -376,  -376,    56,    27,   853,  -376,   363,  -376,
     919,  -376,  -376,  1224,    27,   156,  -376,  1224,   919,  1224,
      44,   410,  -376,    63,   365,  -376,   366,   367,  -376,  -376,
    -376,   919,  1224,   156,  -376,  -376,   169,   364,  -376,   410,
    -376,  -376,   853,   853,  -376,   156,  -376,    27,  1224,   853,
     853,   368,   369,   370,  1224,   919,    27,  -376,  -376,  -376,
    -376,  -376,    27,  -376,   919,   919,  -376,  -376
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int16 yydefact[] =
{
      35,   219,     0,   219,     0,     0,     0,     0,     0,   190,
     192,   191,     0,   258,    96,     0,   188,     0,   264,   265,
       0,     0,     0,     0,     0,     0,     0,     0,   263,   189,
     260,   266,   261,     0,    56,     0,     0,   262,     0,     0,
     205,     0,     0,     5,    18,    31,    32,    33,    36,     3,
      15,    30,    16,    17,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,     0,   100,   108,   114,   118,
     122,   126,   130,   134,   138,   142,   149,   151,   154,   158,
     168,   171,   173,   172,   175,   194,   195,   193,   259,   255,
     256,   257,   221,     0,   220,   222,     0,    44,    50,     0,
     190,   192,   191,   189,     0,   176,   159,   168,   192,     0,
     270,   269,     0,    13,     0,   102,     0,     0,     0,     0,
     174,   173,     0,    97,     0,     0,     0,     0,   161,     0,
     160,     0,     0,   162,   163,     0,   219,   226,   227,   228,
     219,   230,   231,   232,   233,     0,     0,   235,     0,   190,
     239,   192,   191,   240,   241,   258,    96,     0,   188,   246,
     264,   265,   248,   249,   250,   251,     0,     0,   189,   260,
     261,    38,     0,     0,   210,     0,   216,   224,   254,   164,
     165,   166,   167,   203,   197,     0,   206,     0,     0,     1,
       4,     2,    34,   268,     0,   267,    57,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   169,   170,   107,   106,     0,     0,     0,
       0,     0,   180,     0,     0,   181,    74,     0,    40,     0,
      48,    51,    73,   225,   228,   229,   231,   232,   237,   238,
     236,   255,   257,   241,   242,   243,   244,   245,   246,   247,
     234,   248,   249,   250,   251,   252,   253,   223,   218,   217,
     207,     0,     0,    13,    14,     0,    11,    99,     0,    98,
       0,   192,     0,    70,    62,   103,   104,   111,   116,   120,
     124,   128,   132,   136,   140,   146,   168,    78,    80,    39,
      79,   179,    75,     0,    90,    38,     0,     0,    91,    92,
      43,     0,     0,    77,     0,     0,    37,   208,     0,     0,
     205,   199,   204,   198,   201,   196,   101,   119,     0,   123,
     127,   131,   135,   139,   145,   144,   143,   150,   152,   153,
     155,   156,   157,   109,   110,     0,   178,   184,   186,     0,
       0,   183,    45,    52,     0,     0,     0,     0,   272,   271,
       0,     0,     0,    46,    53,     0,    65,    46,    96,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    95,    93,     0,     0,   213,
      13,   214,   209,   211,   212,   206,     0,     0,   177,     0,
     185,   182,     0,    13,     0,    12,    35,    59,     0,    42,
       0,    49,    54,    41,     0,     0,    71,   274,   273,    96,
     105,   168,   121,     0,   125,   129,   133,   137,   141,   148,
     147,     0,   112,   113,    84,    81,     0,    61,    76,     0,
     200,   202,   115,   187,     0,     0,    35,    10,     0,     6,
       0,    47,    55,    96,     0,     0,    63,     0,     0,     0,
       0,    85,    86,     0,     0,    60,     0,     0,     7,    58,
      66,     0,    96,     0,   117,    68,     0,     0,    82,    84,
      87,    94,    35,    35,     8,     0,    69,     0,    96,    35,
      35,     0,     0,     0,    96,     0,     0,    88,    89,    83,
     215,     9,     0,    72,     0,     0,    64,    67
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -376,  -376,  -376,  -376,  -376,     0,  -251,  -376,  -256,     4,
    -376,   -32,    32,     2,   -14,  -376,  -376,  -376,    29,  -186,
     192,  -242,  -376,  -376,  -376,  -376,  -376,  -376,  -376,  -376,
    -376,  -376,  -376,  -376,  -376,  -376,  -376,  -376,  -376,  -376,
    -376,  -376,  -376,   -45,  -376,   -26,  -376,  -376,  -376,  -376,
     129,  -265,  -127,    28,  -376,  -376,   349,  -134,  -294,  -376,
    -376,  -376,  -376,   233,    70,   235,    69,   238,    71,   240,
      77,   241,    79,   244,    82,   -74,   243,    49,    23,  -376,
      -1,    -4,     1,  -376,   -16,  -376,  -376,  -376,  -376,   141,
     143,  -376,  -376,   146,   151,  -131,    -2,     9,  -145,  -376,
      21,  -376,  -376,   -82,    83,    34,  -375
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    42,    43,   191,   437,   105,   438,   264,   265,    45,
      46,    47,    48,   439,    50,    51,   273,    52,    96,   352,
      97,   353,   230,   231,   401,   402,    53,    54,    55,    56,
     359,   463,   404,   475,   358,   445,    57,    58,    59,    60,
      61,    62,   425,   450,   451,   452,   469,    63,    64,   298,
     299,   122,   268,    65,   274,   275,   372,    66,   276,    67,
     277,    68,   278,    69,   279,    70,   280,    71,   281,    72,
     282,    73,   283,    74,   284,    75,    76,    77,    78,    79,
      80,    81,    82,    83,   222,   339,    84,    85,   185,   186,
     187,    86,   173,   174,   379,   175,    93,    98,   176,   177,
      95,    87,    88,   196,   113,   350,   409
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      44,    99,    49,   107,   301,    44,   127,   346,   120,   109,
      94,   226,    94,   121,   228,   112,   192,   232,   304,   110,
     107,   305,   107,   348,   116,   107,   107,   106,   348,   443,
     357,   110,   110,    44,   107,   107,   289,   107,   107,   118,
     292,   285,   123,   294,   128,   125,   130,   300,    44,   133,
     134,   129,   467,   314,   178,   208,   209,   193,   179,   180,
     316,   181,   182,   110,   318,   172,   410,   225,   413,   188,
     462,   262,   213,   214,   193,   336,   194,   422,   423,   341,
     190,   193,  -238,   333,   334,   355,   356,   338,   478,   111,
     114,   115,   368,   406,   349,   343,   110,   468,   124,   349,
     484,   111,   111,   307,   308,   291,   442,   131,   132,   119,
     304,   305,   195,  -236,   286,   126,    44,    44,   369,    44,
     287,   288,   266,   290,   429,   178,    44,   194,   193,   195,
     324,   325,   326,   111,   227,    44,   195,   435,    99,   303,
     192,   107,   269,    44,   446,    94,  -244,   109,   127,    94,
     120,   135,   293,   464,   112,   121,   441,   407,   172,   269,
     302,   197,   107,   189,   107,   106,   111,   199,   110,   361,
     178,   200,    44,   178,   110,   384,   110,   201,   460,   202,
     118,   398,   198,   195,   123,   457,   128,   125,   130,    89,
     362,    90,    91,   129,   203,   263,   107,   477,   107,   107,
     107,   107,   107,   107,   107,   107,   107,   107,   107,   107,
     107,   107,   408,   486,   194,    92,   370,   392,  -252,   492,
     479,   482,   483,   194,  -237,   207,  -253,   204,   117,   114,
     205,   115,   315,   330,   331,   332,   213,   214,   111,   215,
     124,   178,   216,   421,   111,   178,   111,   335,   389,   131,
     132,   340,   431,   432,   371,   433,   206,   328,   329,   296,
     183,   261,   178,   178,   229,   194,   390,   110,   297,  -242,
     399,   345,   266,   403,    89,   184,    90,    91,   444,   354,
     354,   354,    89,   375,    90,    91,   285,   285,   285,   285,
     285,   285,   285,   285,   419,   420,  -245,   285,   285,  -247,
      92,  -234,   351,   310,    89,   312,    90,    91,    92,   117,
     213,   214,    89,   215,    90,    91,   216,   194,   311,    89,
     313,    90,    91,  -218,   347,   194,   285,   373,   217,   178,
      92,    89,   388,    90,    91,   377,   378,   111,    92,  -225,
     391,  -217,   309,   360,   344,    92,   219,  -229,   220,   221,
     364,    44,   455,   363,  -256,   397,   395,    92,   365,   411,
     107,   411,   107,   107,   107,   107,   107,   107,   107,   366,
     411,   411,   269,   285,   210,   211,   212,    44,    44,   374,
     394,   427,   428,   426,   367,   297,   123,   380,   380,   266,
     183,   213,   214,   387,   215,   396,    44,   216,   269,   411,
     400,   223,   266,   224,   221,   398,   405,   354,   424,   371,
     436,   312,   440,   447,   449,   480,   458,   472,   473,   342,
     474,   489,   490,   491,   481,   470,   434,   376,   393,   218,
     317,   412,   414,   269,   319,   415,    44,   123,   320,   471,
      44,   321,   416,   322,   459,   417,   411,   323,    44,   418,
     327,   385,   465,   386,   383,   448,   381,     0,     0,     0,
     453,    44,     0,   454,     0,   476,     0,     0,     0,   456,
       0,   123,    44,    44,     0,     0,     0,   466,   461,    44,
      44,   487,   488,     0,     0,    44,     0,     0,     0,   493,
     123,     0,     0,     0,    44,    44,     0,     0,   496,   497,
       0,     0,     0,     0,     0,     0,   123,   267,     0,     0,
       0,   485,   123,     0,     0,     4,     0,     0,     6,     0,
     494,     0,   100,     0,   101,   102,   495,    12,    13,     0,
       0,    16,     0,    18,    19,     0,    21,     0,    23,     0,
       0,  -176,     0,    26,    27,     0,    28,     0,   103,     0,
       0,     0,     0,    30,    31,    32,     0,  -176,   104,     0,
    -176,     0,     0,     0,     0,     0,     0,    35,    36,     0,
      37,     0,    38,    39,    40,  -176,  -176,    41,     0,     0,
       0,     0,     0,  -176,  -176,  -176,  -176,     0,  -207,     0,
       0,     0,     0,     0,  -176,  -176,     0,  -176,  -176,  -176,
    -176,     0,     0,  -176,  -207,  -176,     0,  -207,     0,     0,
    -176,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -207,  -207,     0,     0,     0,     0,     0,     0,
    -207,  -207,  -207,  -207,     0,     0,     0,     0,     0,     0,
       0,  -207,  -207,     0,  -207,  -207,  -207,  -207,     0,     0,
    -207,     0,  -207,     0,     0,     0,     0,  -207,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
       0,     0,    26,    27,     0,    28,     0,   168,     0,     0,
       0,     0,   169,    31,   170,     0,     0,    33,   171,     0,
       0,    34,     0,     0,     0,     0,    35,    36,     0,    37,
       0,    38,    39,    40,     1,     0,    41,     2,     3,     0,
       4,     5,     0,     6,     7,     0,     8,     9,     0,    10,
      11,     0,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,     0,     0,    26,    27,
       0,    28,     0,    29,     0,     0,     0,     0,    30,    31,
      32,     0,     0,    33,   295,     0,     0,    34,     0,     0,
       0,     0,    35,    36,     0,    37,     0,    38,    39,    40,
       1,     0,    41,     2,     3,     0,     4,     5,     0,     6,
       7,     0,     8,     9,     0,    10,    11,     0,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,     0,     0,    26,    27,     0,    28,     0,    29,
       0,     0,     0,     0,    30,    31,    32,     0,     0,    33,
     306,     0,     0,    34,     0,     0,     0,     0,    35,    36,
       0,    37,     0,    38,    39,    40,     1,     0,    41,     2,
       3,     0,     4,     5,     0,     6,     7,     0,     8,     9,
       0,    10,    11,     0,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,     0,     0,
      26,    27,     0,    28,     0,    29,     0,     0,     0,     0,
      30,    31,    32,     0,     0,    33,     0,     0,     0,    34,
       0,     0,     0,     0,    35,    36,     0,    37,     0,    38,
      39,    40,     1,     0,    41,     0,     3,     0,     4,     5,
       0,     6,     7,     0,     8,     9,     0,   108,    11,     0,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,     0,     0,    26,    27,     0,    28,
       0,    29,     0,     0,     0,     0,    30,    31,    32,     0,
       0,    33,     0,     0,     0,    34,     0,     0,     0,     0,
      35,    36,     0,    37,     0,    38,    39,    40,   270,     0,
      41,     4,     0,     0,     6,     0,     0,     0,   100,     0,
     271,   102,     0,    12,    13,     0,     0,    16,     0,    18,
      19,     0,    21,   272,    23,     0,     0,     0,     0,    26,
      27,     0,    28,     0,   103,     0,     0,     0,     0,    30,
      31,    32,     0,     0,   104,     0,     0,     0,     0,     0,
       0,     0,     0,    35,    36,     0,    37,     0,    38,    39,
      40,     4,     0,    41,     6,     0,     0,     0,   100,     0,
     101,   102,     0,    12,    13,     0,     0,    16,     0,    18,
      19,     0,    21,     0,    23,     0,     0,     0,     0,    26,
      27,     0,    28,     0,   103,     0,     0,     0,     0,    30,
      31,    32,     0,     0,   104,     0,     0,     0,     0,     0,
       0,     0,     0,    35,    36,     0,    37,     0,    38,    39,
      40,     0,     4,    41,   337,     6,     0,     0,     0,   100,
       0,   101,   102,     0,    12,    13,     0,     0,    16,     0,
      18,    19,     0,    21,     0,    23,     0,     0,     0,     0,
      26,    27,     0,    28,     0,   103,     0,     0,     0,     0,
      30,    31,    32,     0,  -243,   104,     0,     0,     0,     0,
       0,     0,     0,     0,    35,    36,     0,    37,     0,    38,
      39,    40,     4,     0,    41,     6,     0,     0,     0,   100,
       0,   101,   102,     0,    12,    13,     0,     0,    16,     0,
      18,    19,     0,    21,     0,    23,     0,     0,     0,     0,
      26,    27,     0,    28,     0,   103,     0,     0,     0,     0,
      30,    31,    32,     0,     0,   104,     0,     0,     0,     0,
       0,     0,     0,     0,    35,    36,     0,    37,     0,    38,
      39,    40,   430,     4,    41,     0,     6,     0,     0,     0,
     100,     0,   101,   102,     0,    12,    13,     0,     0,    16,
       0,    18,    19,     0,    21,     0,    23,     0,     0,     0,
       0,    26,    27,     0,    28,     0,   103,     0,     0,     0,
       0,    30,    31,    32,     0,     0,   104,     0,     0,     0,
       0,     0,     0,     0,     0,    35,    36,     0,    37,     0,
      38,    39,    40,     6,     0,    41,     0,   100,     0,   101,
     102,     0,    12,    13,     0,     0,    16,     0,    18,    19,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    28,     0,   103,     0,     0,     0,     0,    30,    31,
      32,     0,     0,   104,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    37,     0,     0,     0,    40,
       0,     0,    41,   233,   137,   138,   234,   235,   141,   236,
     237,   144,   238,   239,   147,   240,   241,   150,    90,   242,
     153,   243,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   254,   255,   256,     0,     0,     0,     0,     0,
       0,     0,   257,     0,     0,     0,     0,   258,     0,   259,
       0,     0,     0,   260,   233,   137,   138,   234,   235,   141,
     236,   237,   144,   238,   239,   147,   240,   241,   150,    90,
     242,   153,   243,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,     0,     0,     0,     0,
       0,     0,     0,   257,     0,     0,     0,     0,   258,     0,
     259,     0,     0,     0,   382,   233,   137,   138,   234,   235,
     141,   236,   237,   144,   238,   239,   147,   240,    89,     0,
      90,    91,     0,   243,   244,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   254,   255,   256,     0,     0,     0,
       0,     0,     0,     0,   257,     0,     0,     0,     0,   258,
       0,   259,     0,  -255,   233,   137,   138,   234,   235,   141,
     236,   237,   144,   238,   239,   147,   240,    89,     0,    90,
      91,     0,   243,   244,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,     0,     0,     0,     0,
       0,     0,     0,   257,     0,     0,     0,     0,   258,     0,
     259,     0,  -257,   233,   137,   138,   234,   235,   141,   236,
     237,   144,   238,   239,   147,   240,    89,   150,    90,    91,
     153,   243,   244,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   254,   255,   256,     0,     0,     0,     0,     0,
       0,     0,   257,     0,     0,     0,     0,   258,     0,   259,
     233,   137,   138,   234,   235,   141,   236,   237,   144,   238,
     239,   147,   240,    89,   150,    90,    91,   153,   243,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,     0,     0,     0,     0,     0,     0,     0,   257
};

static const yytype_int16 yycheck[] =
{
       0,     3,     0,     4,   131,     5,    20,   263,    12,     5,
       1,    93,     3,    12,    96,     6,    48,    99,   149,     1,
      21,   152,    23,     1,    51,    26,    27,     4,     1,   404,
     272,     1,     1,    33,    35,    36,   118,    38,    39,    10,
     122,   115,    14,   125,    21,    17,    23,   129,    48,    26,
      27,    22,     8,   187,    33,    61,    62,     1,    35,    36,
     194,    38,    39,     1,   198,    33,   360,    83,   362,    41,
     445,    41,    37,    38,     1,   220,    54,   371,   372,   224,
      39,     1,    51,   217,   218,   271,   272,   221,   463,    71,
       7,     8,    20,   358,    72,   229,     1,    53,    15,    72,
     475,    71,    71,    53,    54,   121,   400,    24,    25,    51,
     241,   242,    56,    51,   115,    52,   116,   117,    46,   119,
     116,   117,   113,   119,   380,   104,   126,    54,     1,    56,
     204,   205,   206,    71,    54,   135,    56,   393,   140,   135,
     172,   142,   114,   143,   409,   136,    51,   143,   162,   140,
     154,    51,   124,   447,   145,   154,   398,     1,   126,   131,
     132,    36,   163,     0,   165,   142,    71,    35,     1,    36,
     149,    58,   172,   152,     1,   309,     1,    59,   443,    60,
     151,    54,    57,    56,   156,   436,   163,   159,   165,    16,
      57,    18,    19,   164,    44,   112,   197,   462,   199,   200,
     201,   202,   203,   204,   205,   206,   207,   208,   209,   210,
     211,   212,    56,   478,    54,    42,    17,   344,    51,   484,
      51,   472,   473,    54,    51,    45,    51,    17,    51,   146,
      20,   148,    72,   210,   211,   212,    37,    38,    71,    40,
     157,   220,    43,   370,    71,   224,    71,   219,    54,   166,
     167,   223,   386,   387,    55,   389,    46,   208,   209,     5,
      54,    33,   241,   242,    55,    54,    72,     1,    14,    51,
     352,   262,   263,   355,    16,    69,    18,    19,   405,   270,
     271,   272,    16,   297,    18,    19,   360,   361,   362,   363,
     364,   365,   366,   367,   368,   369,    51,   371,   372,    51,
      42,    51,   268,    54,    16,    54,    18,    19,    42,    51,
      37,    38,    16,    40,    18,    19,    43,    54,    69,    16,
      69,    18,    19,    51,    54,    54,   400,   293,    55,   308,
      42,    16,    69,    18,    19,   301,   302,    71,    42,    51,
      69,    51,    51,    54,   261,    42,    68,    51,    70,    71,
      58,   351,   434,    35,    51,   351,   347,    42,    59,   360,
     361,   362,   363,   364,   365,   366,   367,   368,   369,    60,
     371,   372,   344,   447,    63,    64,    65,   377,   378,   296,
     346,   377,   378,   374,    44,    14,   358,   304,   305,   380,
      54,    37,    38,    51,    40,    52,   396,    43,   370,   400,
      55,    68,   393,    70,    71,    54,    17,   398,    52,    55,
      52,    54,    11,    51,     4,    51,    53,    52,    52,   227,
      53,    53,    53,    53,   469,   451,   392,   298,   345,    80,
     197,   361,   363,   405,   199,   364,   436,   409,   200,   453,
     440,   201,   365,   202,   440,   366,   447,   203,   448,   367,
     207,   310,   448,   310,   308,   421,   305,    -1,    -1,    -1,
     426,   461,    -1,   429,    -1,   461,    -1,    -1,    -1,   435,
      -1,   443,   472,   473,    -1,    -1,    -1,   449,   444,   479,
     480,   479,   480,    -1,    -1,   485,    -1,    -1,    -1,   485,
     462,    -1,    -1,    -1,   494,   495,    -1,    -1,   494,   495,
      -1,    -1,    -1,    -1,    -1,    -1,   478,     1,    -1,    -1,
      -1,   477,   484,    -1,    -1,     9,    -1,    -1,    12,    -1,
     486,    -1,    16,    -1,    18,    19,   492,    21,    22,    -1,
      -1,    25,    -1,    27,    28,    -1,    30,    -1,    32,    -1,
      -1,     1,    -1,    37,    38,    -1,    40,    -1,    42,    -1,
      -1,    -1,    -1,    47,    48,    49,    -1,    17,    52,    -1,
      20,    -1,    -1,    -1,    -1,    -1,    -1,    61,    62,    -1,
      64,    -1,    66,    67,    68,    35,    36,    71,    -1,    -1,
      -1,    -1,    -1,    43,    44,    45,    46,    -1,     1,    -1,
      -1,    -1,    -1,    -1,    54,    55,    -1,    57,    58,    59,
      60,    -1,    -1,    63,    17,    65,    -1,    20,    -1,    -1,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    35,    36,    -1,    -1,    -1,    -1,    -1,    -1,
      43,    44,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    54,    55,    -1,    57,    58,    59,    60,    -1,    -1,
      63,    -1,    65,    -1,    -1,    -1,    -1,    70,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      -1,    -1,    37,    38,    -1,    40,    -1,    42,    -1,    -1,
      -1,    -1,    47,    48,    49,    -1,    -1,    52,    53,    -1,
      -1,    56,    -1,    -1,    -1,    -1,    61,    62,    -1,    64,
      -1,    66,    67,    68,     3,    -1,    71,     6,     7,    -1,
       9,    10,    -1,    12,    13,    -1,    15,    16,    -1,    18,
      19,    -1,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    -1,    -1,    37,    38,
      -1,    40,    -1,    42,    -1,    -1,    -1,    -1,    47,    48,
      49,    -1,    -1,    52,    53,    -1,    -1,    56,    -1,    -1,
      -1,    -1,    61,    62,    -1,    64,    -1,    66,    67,    68,
       3,    -1,    71,     6,     7,    -1,     9,    10,    -1,    12,
      13,    -1,    15,    16,    -1,    18,    19,    -1,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    -1,    -1,    37,    38,    -1,    40,    -1,    42,
      -1,    -1,    -1,    -1,    47,    48,    49,    -1,    -1,    52,
      53,    -1,    -1,    56,    -1,    -1,    -1,    -1,    61,    62,
      -1,    64,    -1,    66,    67,    68,     3,    -1,    71,     6,
       7,    -1,     9,    10,    -1,    12,    13,    -1,    15,    16,
      -1,    18,    19,    -1,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    -1,    -1,
      37,    38,    -1,    40,    -1,    42,    -1,    -1,    -1,    -1,
      47,    48,    49,    -1,    -1,    52,    -1,    -1,    -1,    56,
      -1,    -1,    -1,    -1,    61,    62,    -1,    64,    -1,    66,
      67,    68,     3,    -1,    71,    -1,     7,    -1,     9,    10,
      -1,    12,    13,    -1,    15,    16,    -1,    18,    19,    -1,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    -1,    -1,    37,    38,    -1,    40,
      -1,    42,    -1,    -1,    -1,    -1,    47,    48,    49,    -1,
      -1,    52,    -1,    -1,    -1,    56,    -1,    -1,    -1,    -1,
      61,    62,    -1,    64,    -1,    66,    67,    68,     6,    -1,
      71,     9,    -1,    -1,    12,    -1,    -1,    -1,    16,    -1,
      18,    19,    -1,    21,    22,    -1,    -1,    25,    -1,    27,
      28,    -1,    30,    31,    32,    -1,    -1,    -1,    -1,    37,
      38,    -1,    40,    -1,    42,    -1,    -1,    -1,    -1,    47,
      48,    49,    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    61,    62,    -1,    64,    -1,    66,    67,
      68,     9,    -1,    71,    12,    -1,    -1,    -1,    16,    -1,
      18,    19,    -1,    21,    22,    -1,    -1,    25,    -1,    27,
      28,    -1,    30,    -1,    32,    -1,    -1,    -1,    -1,    37,
      38,    -1,    40,    -1,    42,    -1,    -1,    -1,    -1,    47,
      48,    49,    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    61,    62,    -1,    64,    -1,    66,    67,
      68,    -1,     9,    71,    72,    12,    -1,    -1,    -1,    16,
      -1,    18,    19,    -1,    21,    22,    -1,    -1,    25,    -1,
      27,    28,    -1,    30,    -1,    32,    -1,    -1,    -1,    -1,
      37,    38,    -1,    40,    -1,    42,    -1,    -1,    -1,    -1,
      47,    48,    49,    -1,    51,    52,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    61,    62,    -1,    64,    -1,    66,
      67,    68,     9,    -1,    71,    12,    -1,    -1,    -1,    16,
      -1,    18,    19,    -1,    21,    22,    -1,    -1,    25,    -1,
      27,    28,    -1,    30,    -1,    32,    -1,    -1,    -1,    -1,
      37,    38,    -1,    40,    -1,    42,    -1,    -1,    -1,    -1,
      47,    48,    49,    -1,    -1,    52,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    61,    62,    -1,    64,    -1,    66,
      67,    68,    69,     9,    71,    -1,    12,    -1,    -1,    -1,
      16,    -1,    18,    19,    -1,    21,    22,    -1,    -1,    25,
      -1,    27,    28,    -1,    30,    -1,    32,    -1,    -1,    -1,
      -1,    37,    38,    -1,    40,    -1,    42,    -1,    -1,    -1,
      -1,    47,    48,    49,    -1,    -1,    52,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    61,    62,    -1,    64,    -1,
      66,    67,    68,    12,    -1,    71,    -1,    16,    -1,    18,
      19,    -1,    21,    22,    -1,    -1,    25,    -1,    27,    28,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    42,    -1,    -1,    -1,    -1,    47,    48,
      49,    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    68,
      -1,    -1,    71,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    42,    -1,    -1,    -1,    -1,    47,    -1,    49,
      -1,    -1,    -1,    53,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    42,    -1,    -1,    -1,    -1,    47,    -1,
      49,    -1,    -1,    -1,    53,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    -1,
      18,    19,    -1,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    42,    -1,    -1,    -1,    -1,    47,
      -1,    49,    -1,    51,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    -1,    18,
      19,    -1,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    42,    -1,    -1,    -1,    -1,    47,    -1,
      49,    -1,    51,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    42,    -1,    -1,    -1,    -1,    47,    -1,    49,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    42
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     6,     7,     9,    10,    12,    13,    15,    16,
      18,    19,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    37,    38,    40,    42,
      47,    48,    49,    52,    56,    61,    62,    64,    66,    67,
      68,    71,    74,    75,    78,    82,    83,    84,    85,    86,
      87,    88,    90,    99,   100,   101,   102,   109,   110,   111,
     112,   113,   114,   120,   121,   126,   130,   132,   134,   136,
     138,   140,   142,   144,   146,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   159,   160,   164,   174,   175,    16,
      18,    19,    42,   169,   170,   173,    91,    93,   170,   169,
      16,    18,    19,    42,    52,    78,   151,   153,    18,    82,
       1,    71,   170,   177,   177,   177,    51,    51,    91,    51,
     154,   155,   124,   126,   177,   126,    52,    87,   151,    91,
     151,   177,   177,   151,   151,    51,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    42,    47,
      49,    53,    85,   165,   166,   168,   171,   172,   173,   151,
     151,   151,   151,    54,    69,   161,   162,   163,   126,     0,
      39,    76,    84,     1,    54,    56,   176,    36,    57,    35,
      58,    59,    60,    44,    17,    20,    46,    45,    61,    62,
      63,    64,    65,    37,    38,    40,    43,    55,   129,    68,
      70,    71,   157,    68,    70,   157,   176,    54,   176,    55,
      95,    96,   176,     3,     6,     7,     9,    10,    12,    13,
      15,    16,    19,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    42,    47,    49,
      53,    33,    41,   177,    80,    81,   170,     1,   125,   126,
       6,    18,    31,    89,   127,   128,   131,   133,   135,   137,
     139,   141,   143,   145,   147,   148,   153,    82,    82,   176,
      82,   157,   176,   126,   176,    53,     5,    14,   122,   123,
     176,   125,   126,    82,   168,   168,    53,    53,    54,    51,
      54,    69,    54,    69,   130,    72,   130,   136,   130,   138,
     140,   142,   144,   146,   148,   148,   148,   149,   150,   150,
     151,   151,   151,   130,   130,   126,   171,    72,   130,   158,
     126,   171,    93,   130,   177,   170,    81,    54,     1,    72,
     178,   178,    92,    94,   170,    92,    92,    94,   107,   103,
      54,    36,    57,    35,    58,    59,    60,    44,    20,    46,
      17,    55,   129,   178,   177,    87,   123,   178,   178,   167,
     177,   167,    53,   166,   130,   162,   163,    51,    69,    54,
      72,    69,   125,   177,   178,   170,    52,    82,    54,   176,
      55,    97,    98,   176,   105,    17,   124,     1,    56,   179,
     131,   153,   137,   131,   139,   141,   143,   145,   147,   148,
     148,   125,   131,   131,    52,   115,   170,    82,    82,    81,
      69,   130,   130,   130,   178,    81,    52,    77,    79,    86,
      11,    94,   131,   179,   125,   108,   124,    51,   178,     4,
     116,   117,   118,   178,   178,   176,   178,    79,    53,    82,
     124,   178,   179,   104,   131,    82,   126,     8,    53,   119,
     118,    87,    52,    52,    53,   106,    82,   124,   179,    51,
      51,   116,    79,    79,   179,   178,   124,    86,    86,    53,
      53,    53,   124,    82,   178,   178,    82,    82
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    73,    74,    75,    76,    76,    77,    78,    78,    78,
      79,    80,    80,    81,    81,    82,    82,    82,    82,    82,
      82,    82,    82,    82,    82,    82,    82,    82,    82,    82,
      83,    84,    84,    85,    85,    86,    86,    87,    87,    88,
      88,    89,    89,    90,    91,    91,    92,    92,    93,    94,
      95,    95,    96,    97,    97,    98,    99,   100,   101,   101,
     102,   102,   103,   104,   102,   105,   106,   102,   102,   102,
     107,   108,   102,   109,   110,   111,   112,   113,   113,   113,
     113,   114,   115,   115,   116,   116,   117,   117,   118,   119,
     120,   121,   121,   121,   122,   123,   124,   124,   125,   125,
     126,   126,   127,   127,   128,   128,   129,   129,   130,   130,
     130,   131,   131,   131,   132,   132,   133,   133,   134,   134,
     135,   135,   136,   136,   137,   137,   138,   138,   139,   139,
     140,   140,   141,   141,   142,   142,   143,   143,   144,   144,
     145,   145,   146,   146,   146,   146,   147,   147,   147,   148,
     148,   149,   149,   149,   150,   150,   150,   150,   151,   151,
     151,   151,   151,   151,   151,   151,   151,   151,   152,   152,
     152,   153,   153,   154,   154,   155,   155,   155,   155,   155,
     156,   156,   156,   156,   157,   157,   158,   158,   159,   159,
     159,   159,   159,   159,   159,   159,   159,   160,   160,   160,
     160,   161,   161,   162,   162,   163,   163,   164,   164,   164,
     165,   165,   166,   166,   166,   167,   168,   168,   168,   169,
     169,   170,   170,   171,   171,   172,   172,   172,   172,   172,
     172,   172,   172,   172,   172,   172,   172,   172,   172,   172,
     172,   172,   172,   172,   172,   172,   172,   172,   172,   172,
     172,   172,   172,   172,   172,   173,   173,   173,   174,   174,
     174,   174,   174,   174,   175,   175,   175,   176,   176,   177,
     177,   178,   178,   179,   179
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     1,     1,     0,     1,     7,     8,    10,
       1,     1,     3,     0,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     0,     1,     3,     2,     3,
       3,     3,     3,     3,     1,     3,     1,     3,     2,     2,
       0,     1,     2,     0,     1,     2,     1,     2,     7,     5,
       7,     5,     0,     0,    11,     0,     0,    12,     7,     8,
       0,     0,    10,     3,     3,     3,     5,     3,     3,     3,
       3,     5,     3,     5,     0,     1,     1,     2,     4,     3,
       3,     3,     3,     4,     5,     2,     0,     1,     1,     1,
       1,     3,     0,     1,     1,     3,     1,     1,     1,     3,
       3,     1,     3,     3,     1,     5,     1,     5,     1,     3,
       1,     3,     1,     3,     1,     3,     1,     3,     1,     3,
       1,     3,     1,     3,     1,     3,     1,     3,     1,     3,
       1,     3,     1,     3,     3,     3,     1,     3,     3,     1,
       3,     1,     3,     3,     1,     3,     3,     3,     1,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     1,     2,
       2,     1,     1,     1,     2,     1,     1,     4,     3,     3,
       2,     2,     4,     3,     2,     3,     1,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     2,     3,     3,
       5,     2,     4,     1,     2,     0,     1,     2,     3,     4,
       1,     3,     3,     3,     3,     6,     1,     1,     1,     0,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (&yylloc, ctx, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if PARSER_DEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined PARSER_LTYPE_IS_TRIVIAL && PARSER_LTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
 }

#  define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value, Location, ctx); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, parser_ctx_t *ctx)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  YYUSE (yylocationp);
  YYUSE (ctx);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, parser_ctx_t *ctx)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  YY_LOCATION_PRINT (yyo, *yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yytype, yyvaluep, yylocationp, ctx);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule, parser_ctx_t *ctx)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                       , &(yylsp[(yyi + 1) - (yynrhs)])                       , ctx);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule, ctx); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !PARSER_DEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !PARSER_DEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, parser_ctx_t *ctx)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  YYUSE (ctx);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/*----------.
| yyparse.  |
`----------*/

int
yyparse (parser_ctx_t *ctx)
{
/* The lookahead symbol.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

/* Location data for the lookahead symbol.  */
static YYLTYPE yyloc_default
# if defined PARSER_LTYPE_IS_TRIVIAL && PARSER_LTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
YYLTYPE yylloc = yyloc_default;

    /* Number of syntax errors so far.  */
    int yynerrs;

    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.
       'yyls': related to locations.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[3];

    YYPTRDIFF_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yylsp = yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  yylsp[0] = yylloc;
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex (&yylval, &yylloc, ctx);
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2:
#line 254 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { ctx->source = (yyvsp[-1].statement_list) ? (yyvsp[-1].statement_list)->head : NULL;
                                  (void)parser_nerrs; /* avoid unused variable warning */ }
#line 2234 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 3:
#line 259 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.statement_list) = (yyvsp[0].statement_list); }
#line 2240 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 6:
#line 267 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.statement_list) = (yyvsp[0].statement_list); }
#line 2246 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 7:
#line 272 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = new_function_expression(ctx, NULL, (yyvsp[-4].parameter_list), (yyvsp[-1].statement_list), NULL, ctx->begin + (yylsp[-6]), (yylsp[0]) - (yylsp[-6]) + 1); }
#line 2252 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 8:
#line 274 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = new_function_expression(ctx, (yyvsp[-6].identifier), (yyvsp[-4].parameter_list), (yyvsp[-1].statement_list), NULL, ctx->begin + (yylsp[-7]), (yylsp[0]) - (yylsp[-7]) + 1); }
#line 2258 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 9:
#line 276 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = new_function_expression(ctx, (yyvsp[-6].identifier), (yyvsp[-4].parameter_list), (yyvsp[-1].statement_list), (yyvsp[-8].identifier), ctx->begin + (yylsp[-9]), (yylsp[0]) - (yylsp[-9]) + 1); }
#line 2264 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 10:
#line 280 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.statement_list) = (yyvsp[0].statement_list); }
#line 2270 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 11:
#line 284 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.parameter_list) = new_parameter_list(ctx, (yyvsp[0].identifier)); }
#line 2276 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 12:
#line 286 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.parameter_list) = parameter_list_add(ctx, (yyvsp[-2].parameter_list), (yyvsp[0].identifier)); }
#line 2282 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 13:
#line 290 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.parameter_list) = NULL; }
#line 2288 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 14:
#line 291 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.parameter_list) = (yyvsp[0].parameter_list); }
#line 2294 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 15:
#line 295 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.statement) = (yyvsp[0].statement); }
#line 2300 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 16:
#line 296 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.statement) = (yyvsp[0].statement); }
#line 2306 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 17:
#line 297 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.statement) = (yyvsp[0].statement); }
#line 2312 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 18:
#line 298 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.statement) = new_expression_statement(ctx, (yyloc), (yyvsp[0].expr)); }
#line 2318 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 19:
#line 299 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.statement) = (yyvsp[0].statement); }
#line 2324 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 20:
#line 300 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.statement) = (yyvsp[0].statement); }
#line 2330 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 21:
#line 301 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.statement) = (yyvsp[0].statement); }
#line 2336 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 22:
#line 302 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.statement) = (yyvsp[0].statement); }
#line 2342 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 23:
#line 303 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.statement) = (yyvsp[0].statement); }
#line 2348 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 24:
#line 304 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.statement) = (yyvsp[0].statement); }
#line 2354 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 25:
#line 305 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.statement) = (yyvsp[0].statement); }
#line 2360 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 26:
#line 306 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.statement) = (yyvsp[0].statement); }
#line 2366 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 27:
#line 307 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.statement) = (yyvsp[0].statement); }
#line 2372 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 28:
#line 308 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.statement) = (yyvsp[0].statement); }
#line 2378 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 29:
#line 309 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.statement) = (yyvsp[0].statement); }
#line 2384 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 30:
#line 313 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.statement) = (yyvsp[0].statement); }
#line 2390 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 31:
#line 317 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.statement) = (yyvsp[0].statement); }
#line 2396 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 32:
#line 318 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.statement) = (yyvsp[0].statement); }
#line 2402 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 33:
#line 322 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.statement_list) = new_statement_list(ctx, (yyvsp[0].statement)); }
#line 2408 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 34:
#line 324 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.statement_list) = statement_list_add((yyvsp[-1].statement_list), (yyvsp[0].statement)); }
#line 2414 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 35:
#line 328 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.statement_list) = NULL; }
#line 2420 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 36:
#line 329 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.statement_list) = (yyvsp[0].statement_list); }
#line 2426 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 37:
#line 333 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.statement) = new_block_statement(ctx, (yylsp[-1]), (yyvsp[-1].statement_list)); }
#line 2432 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 38:
#line 334 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.statement) = new_block_statement(ctx, (yyloc), NULL); }
#line 2438 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 39:
#line 339 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.statement) = new_var_statement(ctx, TRUE, FALSE, (yyloc), (yyvsp[-1].variable_list)); }
#line 2444 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 40:
#line 341 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                {
                                    if(ctx->script->version < SCRIPTLANGUAGEVERSION_ES5) {
                                        WARN("const var declaration %s in legacy mode.\n",
                                                debugstr_w((yyvsp[-2].identifier)));
                                        set_error(ctx, (yyloc), JS_E_SYNTAX);
                                        YYABORT;
                                    }
                                    (yyval.statement) = new_var_statement(ctx, TRUE, TRUE, (yyloc), (yyvsp[-1].variable_list));
                                }
#line 2458 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 41:
#line 354 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.statement) = new_var_statement(ctx, TRUE, FALSE, (yyloc), (yyvsp[-1].variable_list)); }
#line 2464 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 42:
#line 356 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                {
                                    if(ctx->script->version < SCRIPTLANGUAGEVERSION_ES5) {
                                        WARN("const var declaration %s in legacy mode.\n",
                                                debugstr_w((yyvsp[-2].identifier)));
                                        set_error(ctx, (yyloc), JS_E_SYNTAX);
                                        YYABORT;
                                    }
                                    (yyval.statement) = new_var_statement(ctx, TRUE, TRUE, (yyloc), (yyvsp[-1].variable_list));
                                }
#line 2478 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 43:
#line 369 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.statement) = new_var_statement(ctx, FALSE, FALSE, (yyloc), (yyvsp[-1].variable_list)); }
#line 2484 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 44:
#line 373 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.variable_list) = new_variable_list(ctx, (yyvsp[0].variable_declaration)); }
#line 2490 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 45:
#line 375 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.variable_list) = variable_list_add(ctx, (yyvsp[-2].variable_list), (yyvsp[0].variable_declaration)); }
#line 2496 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 46:
#line 380 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.variable_list) = new_variable_list(ctx, (yyvsp[0].variable_declaration)); }
#line 2502 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 47:
#line 382 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.variable_list) = variable_list_add(ctx, (yyvsp[-2].variable_list), (yyvsp[0].variable_declaration)); }
#line 2508 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 48:
#line 387 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.variable_declaration) = new_variable_declaration(ctx, (yyvsp[-1].identifier), (yyvsp[0].expr)); }
#line 2514 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 49:
#line 392 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.variable_declaration) = new_variable_declaration(ctx, (yyvsp[-1].identifier), (yyvsp[0].expr)); }
#line 2520 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 50:
#line 396 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = NULL; }
#line 2526 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 51:
#line 397 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 2532 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 52:
#line 402 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 2538 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 53:
#line 406 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = NULL; }
#line 2544 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 54:
#line 407 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 2550 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 55:
#line 412 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 2556 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 56:
#line 416 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.statement) = new_statement(ctx, STAT_EMPTY, 0, (yyloc)); }
#line 2562 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 57:
#line 421 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.statement) = new_expression_statement(ctx, (yyloc),  (yyvsp[-1].expr)); }
#line 2568 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 58:
#line 426 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.statement) = new_if_statement(ctx, (yyloc), (yyvsp[-4].expr), (yyvsp[-2].statement), (yyvsp[0].statement)); }
#line 2574 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 59:
#line 428 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.statement) = new_if_statement(ctx, (yyloc), (yyvsp[-2].expr), (yyvsp[0].statement), NULL); }
#line 2580 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 60:
#line 433 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.statement) = new_while_statement(ctx, (yylsp[-4]), TRUE, (yyvsp[-2].expr), (yyvsp[-5].statement)); }
#line 2586 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 61:
#line 435 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.statement) = new_while_statement(ctx, (yyloc), FALSE, (yyvsp[-2].expr), (yyvsp[0].statement)); }
#line 2592 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 62:
#line 437 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { if(!explicit_error(ctx, (yyvsp[0].expr), ';')) YYABORT; }
#line 2598 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 63:
#line 439 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { if(!explicit_error(ctx, (yyvsp[0].expr), ';')) YYABORT; }
#line 2604 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 64:
#line 441 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.statement) = new_for_statement(ctx, (yylsp[-8]), NULL, (yyvsp[-8].expr), (yyvsp[-5].expr), (yylsp[-5]), (yyvsp[-2].expr), (yylsp[-2]), (yyvsp[0].statement)); }
#line 2610 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 65:
#line 443 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { if(!explicit_error(ctx, (yyvsp[0].variable_list), ';')) YYABORT; }
#line 2616 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 66:
#line 445 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { if(!explicit_error(ctx, (yyvsp[0].expr), ';')) YYABORT; }
#line 2622 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 67:
#line 447 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.statement) = new_for_statement(ctx, (yylsp[-9]), (yyvsp[-8].variable_list) ? (yyvsp[-8].variable_list)->head : NULL, NULL, (yyvsp[-5].expr), (yylsp[-5]), (yyvsp[-2].expr), (yylsp[-2]), (yyvsp[0].statement)); }
#line 2628 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 68:
#line 449 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.statement) = new_forin_statement(ctx, (yyloc), NULL, (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[0].statement)); }
#line 2634 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 69:
#line 451 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.statement) = new_forin_statement(ctx, (yyloc),  (yyvsp[-4].variable_declaration), NULL, (yyvsp[-2].expr), (yyvsp[0].statement)); }
#line 2640 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 70:
#line 453 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { if(!explicit_error(ctx, (yyvsp[0].statement), ';')) YYABORT; }
#line 2646 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 71:
#line 455 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { if(!explicit_error(ctx, (yyvsp[0].expr), ';')) YYABORT; }
#line 2652 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 72:
#line 457 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.statement) = new_for_statement(ctx, (yylsp[-7]), ((var_statement_t *)(yyvsp[-7].statement))->variable_list,
                                  NULL, (yyvsp[-5].expr), (yylsp[-5]), (yyvsp[-2].expr), (yylsp[-2]), (yyvsp[0].statement)); }
#line 2659 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 73:
#line 463 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.statement) = new_continue_statement(ctx, (yyloc), (yyvsp[-1].identifier)); }
#line 2665 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 74:
#line 468 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.statement) = new_break_statement(ctx, (yyloc), (yyvsp[-1].identifier)); }
#line 2671 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 75:
#line 473 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.statement) = new_return_statement(ctx, (yyloc), (yyvsp[-1].expr)); }
#line 2677 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 76:
#line 478 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.statement) = new_with_statement(ctx, (yyloc), (yyvsp[-2].expr), (yyvsp[0].statement)); }
#line 2683 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 77:
#line 483 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.statement) = new_labelled_statement(ctx, (yyloc), (yyvsp[-2].identifier), (yyvsp[0].statement)); }
#line 2689 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 78:
#line 489 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.statement) = new_labelled_statement(ctx, (yyloc), (yyvsp[-2].identifier), (yyvsp[0].statement)); }
#line 2695 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 79:
#line 490 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.statement) = new_labelled_statement(ctx, (yyloc), (yyvsp[-2].identifier), (yyvsp[0].statement)); }
#line 2701 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 80:
#line 491 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.statement) = new_labelled_statement(ctx, (yyloc), (yyvsp[-2].identifier), (yyvsp[0].statement)); }
#line 2707 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 81:
#line 496 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.statement) = new_switch_statement(ctx, (yyloc), (yyvsp[-2].expr), (yyvsp[0].case_clausule)); }
#line 2713 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 82:
#line 501 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                 { (yyval.case_clausule) = new_case_block(ctx, (yyvsp[-1].case_list), NULL, NULL); }
#line 2719 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 83:
#line 503 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                 { (yyval.case_clausule) = new_case_block(ctx, (yyvsp[-3].case_list), (yyvsp[-2].case_clausule), (yyvsp[-1].case_list)); }
#line 2725 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 84:
#line 507 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                 { (yyval.case_list) = NULL; }
#line 2731 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 85:
#line 508 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                 { (yyval.case_list) = (yyvsp[0].case_list); }
#line 2737 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 86:
#line 512 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                 { (yyval.case_list) = new_case_list(ctx, (yyvsp[0].case_clausule)); }
#line 2743 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 87:
#line 514 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                 { (yyval.case_list) = case_list_add(ctx, (yyvsp[-1].case_list), (yyvsp[0].case_clausule)); }
#line 2749 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 88:
#line 519 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                 { (yyval.case_clausule) = new_case_clausule(ctx, (yyloc), (yyvsp[-2].expr), (yyvsp[0].statement_list)); }
#line 2755 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 89:
#line 524 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                 { (yyval.case_clausule) = new_case_clausule(ctx, (yyloc), NULL, (yyvsp[0].statement_list)); }
#line 2761 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 90:
#line 529 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.statement) = new_throw_statement(ctx, (yyloc), (yyvsp[-1].expr)); }
#line 2767 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 91:
#line 533 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.statement) = new_try_statement(ctx, (yyvsp[-1].statement), (yyvsp[0].catch_block), NULL, 0); }
#line 2773 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 92:
#line 534 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.statement) = new_try_statement(ctx, (yyvsp[-1].statement), NULL, (yyvsp[0].statement), (yylsp[0])); }
#line 2779 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 93:
#line 536 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.statement) = new_try_statement(ctx, (yyvsp[-2].statement), (yyvsp[-1].catch_block), (yyvsp[0].statement), (yylsp[0])); }
#line 2785 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 94:
#line 541 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.catch_block) = new_catch_block(ctx, (yyvsp[-2].identifier), (yyvsp[0].statement)); }
#line 2791 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 95:
#line 545 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyloc) = (yylsp[0]); (yyval.statement) = (yyvsp[0].statement); }
#line 2797 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 96:
#line 549 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = NULL; }
#line 2803 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 97:
#line 550 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 2809 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 98:
#line 553 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 2815 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 99:
#line 554 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { set_error(ctx, (yyloc), JS_E_SYNTAX); YYABORT; }
#line 2821 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 100:
#line 558 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 2827 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 101:
#line 560 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = new_binary_expression(ctx, EXPR_COMMA, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2833 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 102:
#line 564 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = NULL; }
#line 2839 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 103:
#line 565 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 2845 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 104:
#line 570 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 2851 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 105:
#line 572 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = new_binary_expression(ctx, EXPR_COMMA, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2857 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 106:
#line 575 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.ival) = (yyvsp[0].ival); }
#line 2863 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 107:
#line 576 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.ival) = EXPR_ASSIGNDIV; }
#line 2869 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 108:
#line 580 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 2875 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 109:
#line 582 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = new_binary_expression(ctx, EXPR_ASSIGN, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2881 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 110:
#line 584 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = new_binary_expression(ctx, (yyvsp[-1].ival), (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2887 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 111:
#line 589 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 2893 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 112:
#line 591 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = new_binary_expression(ctx, EXPR_ASSIGN, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2899 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 113:
#line 593 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = new_binary_expression(ctx, (yyvsp[-1].ival), (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2905 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 114:
#line 597 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 2911 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 115:
#line 599 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = new_conditional_expression(ctx, (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2917 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 116:
#line 604 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 2923 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 117:
#line 606 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = new_conditional_expression(ctx, (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2929 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 118:
#line 610 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 2935 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 119:
#line 612 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = new_binary_expression(ctx, EXPR_OR, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2941 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 120:
#line 617 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 2947 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 121:
#line 619 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = new_binary_expression(ctx, EXPR_OR, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2953 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 122:
#line 623 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 2959 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 123:
#line 625 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = new_binary_expression(ctx, EXPR_AND, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2965 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 124:
#line 630 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 2971 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 125:
#line 632 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = new_binary_expression(ctx, EXPR_AND, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2977 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 126:
#line 636 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 2983 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 127:
#line 638 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = new_binary_expression(ctx, EXPR_BOR, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 2989 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 128:
#line 643 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 2995 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 129:
#line 645 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = new_binary_expression(ctx, EXPR_BOR, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 3001 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 130:
#line 649 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 3007 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 131:
#line 651 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = new_binary_expression(ctx, EXPR_BXOR, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 3013 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 132:
#line 656 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 3019 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 133:
#line 658 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = new_binary_expression(ctx, EXPR_BXOR, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 3025 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 134:
#line 662 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 3031 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 135:
#line 664 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = new_binary_expression(ctx, EXPR_BAND, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 3037 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 136:
#line 669 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 3043 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 137:
#line 671 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = new_binary_expression(ctx, EXPR_BAND, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 3049 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 138:
#line 675 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 3055 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 139:
#line 677 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = new_binary_expression(ctx, (yyvsp[-1].ival), (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 3061 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 140:
#line 681 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                    { (yyval.expr) = (yyvsp[0].expr); }
#line 3067 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 141:
#line 683 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = new_binary_expression(ctx, (yyvsp[-1].ival), (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 3073 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 142:
#line 687 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 3079 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 143:
#line 689 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = new_binary_expression(ctx, (yyvsp[-1].ival), (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 3085 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 144:
#line 691 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = new_binary_expression(ctx, EXPR_INSTANCEOF, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 3091 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 145:
#line 693 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = new_binary_expression(ctx, EXPR_IN, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 3097 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 146:
#line 697 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 3103 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 147:
#line 699 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = new_binary_expression(ctx, (yyvsp[-1].ival), (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 3109 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 148:
#line 701 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = new_binary_expression(ctx, EXPR_INSTANCEOF, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 3115 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 149:
#line 705 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 3121 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 150:
#line 707 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = new_binary_expression(ctx, (yyvsp[-1].ival), (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 3127 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 151:
#line 712 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 3133 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 152:
#line 714 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = new_binary_expression(ctx, EXPR_ADD, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 3139 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 153:
#line 716 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = new_binary_expression(ctx, EXPR_SUB, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 3145 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 154:
#line 720 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 3151 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 155:
#line 722 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = new_binary_expression(ctx, EXPR_MUL, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 3157 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 156:
#line 724 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = new_binary_expression(ctx, EXPR_DIV, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 3163 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 157:
#line 726 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = new_binary_expression(ctx, EXPR_MOD, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 3169 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 158:
#line 730 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 3175 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 159:
#line 732 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = new_unary_expression(ctx, EXPR_DELETE, (yyvsp[0].expr)); }
#line 3181 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 160:
#line 733 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = new_unary_expression(ctx, EXPR_VOID, (yyvsp[0].expr)); }
#line 3187 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 161:
#line 735 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = new_unary_expression(ctx, EXPR_TYPEOF, (yyvsp[0].expr)); }
#line 3193 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 162:
#line 736 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = new_unary_expression(ctx, EXPR_PREINC, (yyvsp[0].expr)); }
#line 3199 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 163:
#line 737 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = new_unary_expression(ctx, EXPR_PREDEC, (yyvsp[0].expr)); }
#line 3205 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 164:
#line 738 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = new_unary_expression(ctx, EXPR_PLUS, (yyvsp[0].expr)); }
#line 3211 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 165:
#line 739 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = new_unary_expression(ctx, EXPR_MINUS, (yyvsp[0].expr)); }
#line 3217 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 166:
#line 740 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = new_unary_expression(ctx, EXPR_BITNEG, (yyvsp[0].expr)); }
#line 3223 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 167:
#line 741 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = new_unary_expression(ctx, EXPR_LOGNEG, (yyvsp[0].expr)); }
#line 3229 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 168:
#line 746 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 3235 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 169:
#line 748 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = new_unary_expression(ctx, EXPR_POSTINC, (yyvsp[-1].expr)); }
#line 3241 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 170:
#line 750 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = new_unary_expression(ctx, EXPR_POSTDEC, (yyvsp[-1].expr)); }
#line 3247 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 171:
#line 755 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 3253 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 172:
#line 756 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 3259 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 173:
#line 760 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 3265 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 174:
#line 761 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = new_new_expression(ctx, (yyvsp[0].expr), NULL); }
#line 3271 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 175:
#line 765 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 3277 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 176:
#line 766 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 3283 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 177:
#line 768 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = new_binary_expression(ctx, EXPR_ARRAY, (yyvsp[-3].expr), (yyvsp[-1].expr)); }
#line 3289 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 178:
#line 770 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = new_member_expression(ctx, (yyvsp[-2].expr), (yyvsp[0].identifier)); }
#line 3295 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 179:
#line 772 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = new_new_expression(ctx, (yyvsp[-1].expr), (yyvsp[0].argument_list)); }
#line 3301 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 180:
#line 777 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = new_call_expression(ctx, (yyvsp[-1].expr), (yyvsp[0].argument_list)); }
#line 3307 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 181:
#line 779 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = new_call_expression(ctx, (yyvsp[-1].expr), (yyvsp[0].argument_list)); }
#line 3313 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 182:
#line 781 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = new_binary_expression(ctx, EXPR_ARRAY, (yyvsp[-3].expr), (yyvsp[-1].expr)); }
#line 3319 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 183:
#line 783 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = new_member_expression(ctx, (yyvsp[-2].expr), (yyvsp[0].identifier)); }
#line 3325 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 184:
#line 787 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.argument_list) = NULL; }
#line 3331 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 185:
#line 788 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.argument_list) = (yyvsp[-1].argument_list); }
#line 3337 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 186:
#line 792 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.argument_list) = new_argument_list(ctx, (yyvsp[0].expr)); }
#line 3343 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 187:
#line 794 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.argument_list) = argument_list_add(ctx, (yyvsp[-2].argument_list), (yyvsp[0].expr)); }
#line 3349 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 188:
#line 798 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = new_expression(ctx, EXPR_THIS, 0); }
#line 3355 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 189:
#line 800 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = new_identifier_expression(ctx, (yyvsp[0].identifier)); }
#line 3361 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 190:
#line 801 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = new_identifier_expression(ctx, (yyvsp[0].identifier)); }
#line 3367 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 191:
#line 802 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = new_identifier_expression(ctx, (yyvsp[0].identifier)); }
#line 3373 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 192:
#line 803 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = new_identifier_expression(ctx, (yyvsp[0].identifier)); }
#line 3379 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 193:
#line 804 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = new_literal_expression(ctx, (yyvsp[0].literal)); }
#line 3385 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 194:
#line 805 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 3391 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 195:
#line 806 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 3397 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 196:
#line 807 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = (yyvsp[-1].expr); }
#line 3403 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 197:
#line 811 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = new_array_literal_expression(ctx, NULL, 0); }
#line 3409 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 198:
#line 812 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = new_array_literal_expression(ctx, NULL, (yyvsp[-1].ival)+1); }
#line 3415 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 199:
#line 813 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = new_array_literal_expression(ctx, (yyvsp[-1].element_list), 0); }
#line 3421 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 200:
#line 815 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = new_array_literal_expression(ctx, (yyvsp[-3].element_list), (yyvsp[-1].ival)+1); }
#line 3427 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 201:
#line 820 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.element_list) = new_element_list(ctx, (yyvsp[-1].ival), (yyvsp[0].expr)); }
#line 3433 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 202:
#line 822 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.element_list) = element_list_add(ctx, (yyvsp[-3].element_list), (yyvsp[-1].ival), (yyvsp[0].expr)); }
#line 3439 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 203:
#line 826 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.ival) = 1; }
#line 3445 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 204:
#line 827 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.ival) = (yyvsp[-1].ival) + 1; }
#line 3451 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 205:
#line 831 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.ival) = 0; }
#line 3457 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 206:
#line 832 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.ival) = (yyvsp[0].ival); }
#line 3463 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 207:
#line 836 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = new_prop_and_value_expression(ctx, NULL); }
#line 3469 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 208:
#line 838 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = new_prop_and_value_expression(ctx, (yyvsp[-1].property_list)); }
#line 3475 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 209:
#line 840 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
        {
            if(ctx->script->version < 2) {
                WARN("Trailing comma in object literal is illegal in legacy mode.\n");
                set_error(ctx, (yylsp[-1]), JS_E_SYNTAX);
                YYABORT;
            }
            (yyval.expr) = new_prop_and_value_expression(ctx, (yyvsp[-2].property_list));
        }
#line 3488 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 210:
#line 851 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.property_list) = new_property_list(ctx, (yyvsp[0].property_definition)); }
#line 3494 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 211:
#line 853 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.property_list) = property_list_add(ctx, (yyvsp[-2].property_list), (yyvsp[0].property_definition)); }
#line 3500 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 212:
#line 858 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.property_definition) = new_property_definition(ctx, PROPERTY_DEFINITION_VALUE, (yyvsp[-2].literal), (yyvsp[0].expr)); }
#line 3506 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 213:
#line 860 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.property_definition) = new_property_definition(ctx, PROPERTY_DEFINITION_GETTER, (yyvsp[-1].literal), (yyvsp[0].expr)); }
#line 3512 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 214:
#line 862 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.property_definition) = new_property_definition(ctx, PROPERTY_DEFINITION_SETTER, (yyvsp[-1].literal), (yyvsp[0].expr)); }
#line 3518 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 215:
#line 866 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.expr) = new_function_expression(ctx, NULL, (yyvsp[-4].parameter_list), (yyvsp[-1].statement_list), NULL, ctx->begin + (yylsp[-5]), (yylsp[0]) - (yylsp[-5]) + 1); }
#line 3524 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 216:
#line 870 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.literal) = new_string_literal(ctx, compiler_alloc_string_len(ctx->compiler, (yyvsp[0].identifier), lstrlenW((yyvsp[0].identifier)))); }
#line 3530 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 217:
#line 871 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.literal) = new_string_literal(ctx, (yyvsp[0].str)); }
#line 3536 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 218:
#line 872 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.literal) = (yyvsp[0].literal); }
#line 3542 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 219:
#line 876 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.identifier) = NULL; }
#line 3548 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 220:
#line 877 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.identifier) = (yyvsp[0].identifier); }
#line 3554 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 221:
#line 880 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.identifier) = (yyvsp[0].identifier); }
#line 3560 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 222:
#line 881 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.identifier) = (yyvsp[0].identifier); }
#line 3566 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 223:
#line 885 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.identifier) = (yyvsp[0].identifier); }
#line 3572 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 224:
#line 887 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
        {
            if(ctx->script->version < SCRIPTLANGUAGEVERSION_ES5) {
                WARN("%s keyword used as an identifier in legacy mode.\n",
                     debugstr_w((yyvsp[0].identifier)));
                set_error(ctx, (yyloc), JS_E_SYNTAX);
                YYABORT;
            }
            (yyval.identifier) = (yyvsp[0].identifier);
        }
#line 3586 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 225:
#line 898 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.identifier) = (yyvsp[0].identifier); }
#line 3592 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 226:
#line 899 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.identifier) = (yyvsp[0].identifier); }
#line 3598 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 227:
#line 900 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.identifier) = (yyvsp[0].identifier); }
#line 3604 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 228:
#line 901 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.identifier) = (yyvsp[0].identifier); }
#line 3610 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 229:
#line 902 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.identifier) = (yyvsp[0].identifier); }
#line 3616 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 230:
#line 903 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.identifier) = (yyvsp[0].identifier); }
#line 3622 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 231:
#line 904 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.identifier) = (yyvsp[0].identifier); }
#line 3628 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 232:
#line 905 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.identifier) = (yyvsp[0].identifier); }
#line 3634 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 233:
#line 906 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.identifier) = (yyvsp[0].identifier); }
#line 3640 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 234:
#line 907 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.identifier) = (yyvsp[0].identifier); }
#line 3646 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 235:
#line 908 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.identifier) = (yyvsp[0].identifier); }
#line 3652 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 236:
#line 909 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.identifier) = (yyvsp[0].identifier); }
#line 3658 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 237:
#line 910 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.identifier) = (yyvsp[0].identifier); }
#line 3664 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 238:
#line 911 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.identifier) = (yyvsp[0].identifier); }
#line 3670 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 239:
#line 912 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.identifier) = (yyvsp[0].identifier); }
#line 3676 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 240:
#line 913 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.identifier) = (yyvsp[0].identifier); }
#line 3682 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 241:
#line 914 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.identifier) = (yyvsp[0].identifier); }
#line 3688 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 242:
#line 915 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.identifier) = (yyvsp[0].identifier); }
#line 3694 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 243:
#line 916 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.identifier) = (yyvsp[0].identifier); }
#line 3700 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 244:
#line 917 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.identifier) = (yyvsp[0].identifier); }
#line 3706 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 245:
#line 918 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.identifier) = (yyvsp[0].identifier); }
#line 3712 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 246:
#line 919 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.identifier) = (yyvsp[0].identifier); }
#line 3718 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 247:
#line 920 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.identifier) = (yyvsp[0].identifier); }
#line 3724 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 248:
#line 921 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.identifier) = (yyvsp[0].identifier); }
#line 3730 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 249:
#line 922 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.identifier) = (yyvsp[0].identifier); }
#line 3736 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 250:
#line 923 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.identifier) = (yyvsp[0].identifier); }
#line 3742 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 251:
#line 924 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.identifier) = (yyvsp[0].identifier); }
#line 3748 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 252:
#line 925 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.identifier) = (yyvsp[0].identifier); }
#line 3754 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 253:
#line 926 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.identifier) = (yyvsp[0].identifier); }
#line 3760 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 254:
#line 927 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.identifier) = (yyvsp[0].identifier); }
#line 3766 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 255:
#line 930 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.identifier) = (yyvsp[0].identifier); }
#line 3772 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 256:
#line 931 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.identifier) = (yyvsp[0].identifier); }
#line 3778 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 257:
#line 932 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.identifier) = (yyvsp[0].identifier); }
#line 3784 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 258:
#line 936 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.literal) = new_null_literal(ctx); }
#line 3790 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 259:
#line 937 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.literal) = (yyvsp[0].literal); }
#line 3796 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 260:
#line 938 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.literal) = (yyvsp[0].literal); }
#line 3802 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 261:
#line 939 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.literal) = new_string_literal(ctx, (yyvsp[0].str)); }
#line 3808 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 262:
#line 940 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.literal) = parse_regexp(ctx);
                                  if(!(yyval.literal)) YYABORT; }
#line 3815 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 263:
#line 942 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.literal) = parse_regexp(ctx);
                                  if(!(yyval.literal)) YYABORT; }
#line 3822 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 264:
#line 947 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.literal) = new_boolean_literal(ctx, VARIANT_TRUE); }
#line 3828 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 265:
#line 948 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.literal) = new_boolean_literal(ctx, VARIANT_FALSE); }
#line 3834 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 266:
#line 949 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { (yyval.literal) = (yyvsp[0].literal); }
#line 3840 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 268:
#line 953 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { if(!allow_auto_semicolon(ctx)) {YYABORT;} else { ctx->hres = S_OK; ctx->error_loc = -1; } }
#line 3846 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 270:
#line 957 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { set_error(ctx, (yyloc), JS_E_MISSING_LBRACKET); YYABORT; }
#line 3852 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 272:
#line 961 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { set_error(ctx, (yyloc), JS_E_MISSING_RBRACKET); YYABORT; }
#line 3858 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;

  case 274:
#line 965 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"
                                { set_error(ctx,  (yyloc), JS_E_MISSING_SEMICOLON); YYABORT; }
#line 3864 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"
    break;


#line 3868 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Jscript\\parser.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (&yylloc, ctx, YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (&yylloc, ctx, yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }

  yyerror_range[1] = yylloc;

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc, ctx);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp, yylsp, ctx);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
  *++yylsp = yyloc;

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (&yylloc, ctx, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc, ctx);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[+*yyssp], yyvsp, yylsp, ctx);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 967 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\dll\\win32\\jscript\\parser.y"


static BOOL allow_auto_semicolon(parser_ctx_t *ctx)
{
    return ctx->nl || ctx->ptr == ctx->end || *(ctx->ptr-1) == '}';
}

static void *new_statement(parser_ctx_t *ctx, statement_type_t type, size_t size, unsigned loc)
{
    statement_t *stat;

    stat = parser_alloc(ctx, size ? size : sizeof(*stat));
    if(!stat)
        return NULL;

    stat->type = type;
    stat->loc  = loc;
    stat->next = NULL;

    return stat;
}

static literal_t *new_string_literal(parser_ctx_t *ctx, jsstr_t *str)
{
    literal_t *ret = parser_alloc(ctx, sizeof(literal_t));

    ret->type = LT_STRING;
    ret->u.str = str;

    return ret;
}

static literal_t *new_null_literal(parser_ctx_t *ctx)
{
    literal_t *ret = parser_alloc(ctx, sizeof(literal_t));

    ret->type = LT_NULL;

    return ret;
}

static property_definition_t *new_property_definition(parser_ctx_t *ctx, property_definition_type_t type,
                                                      literal_t *name, expression_t *value)
{
    property_definition_t *ret = parser_alloc(ctx, sizeof(property_definition_t));

    ret->type = type;
    ret->name = name;
    ret->value = value;
    ret->next = NULL;

    return ret;
}

static property_list_t *new_property_list(parser_ctx_t *ctx, property_definition_t *prop)
{
    property_list_t *ret = parser_alloc_tmp(ctx, sizeof(property_list_t));
    ret->head = ret->tail = prop;
    return ret;
}

static property_list_t *property_list_add(parser_ctx_t *ctx, property_list_t *list, property_definition_t *prop)
{
    list->tail = list->tail->next = prop;
    return list;
}

static array_element_t *new_array_element(parser_ctx_t *ctx, int elision, expression_t *expr)
{
    array_element_t *ret = parser_alloc(ctx, sizeof(array_element_t));

    ret->elision = elision;
    ret->expr = expr;
    ret->next = NULL;

    return ret;
}

static element_list_t *new_element_list(parser_ctx_t *ctx, int elision, expression_t *expr)
{
    element_list_t *ret = parser_alloc_tmp(ctx, sizeof(element_list_t));

    ret->head = ret->tail = new_array_element(ctx, elision, expr);

    return ret;
}

static element_list_t *element_list_add(parser_ctx_t *ctx, element_list_t *list, int elision, expression_t *expr)
{
    list->tail = list->tail->next = new_array_element(ctx, elision, expr);

    return list;
}

static argument_t *new_argument(parser_ctx_t *ctx, expression_t *expr)
{
    argument_t *ret = parser_alloc(ctx, sizeof(argument_t));

    ret->expr = expr;
    ret->next = NULL;

    return ret;
}

static argument_list_t *new_argument_list(parser_ctx_t *ctx, expression_t *expr)
{
    argument_list_t *ret = parser_alloc_tmp(ctx, sizeof(argument_list_t));

    ret->head = ret->tail = new_argument(ctx, expr);

    return ret;
}

static argument_list_t *argument_list_add(parser_ctx_t *ctx, argument_list_t *list, expression_t *expr)
{
    list->tail = list->tail->next = new_argument(ctx, expr);

    return list;
}

static catch_block_t *new_catch_block(parser_ctx_t *ctx, const WCHAR *identifier, statement_t *statement)
{
    catch_block_t *ret = parser_alloc(ctx, sizeof(catch_block_t));

    ret->identifier = identifier;
    ret->statement = statement;

    return ret;
}

static case_clausule_t *new_case_clausule(parser_ctx_t *ctx, unsigned loc, expression_t *expr, statement_list_t *stat_list)
{
    case_clausule_t *ret = parser_alloc(ctx, sizeof(case_clausule_t));

    ret->expr = expr;
    ret->stat = stat_list ? stat_list->head : NULL;
    ret->loc = loc;
    ret->next = NULL;

    return ret;
}

static case_list_t *new_case_list(parser_ctx_t *ctx, case_clausule_t *case_clausule)
{
    case_list_t *ret = parser_alloc_tmp(ctx, sizeof(case_list_t));

    ret->head = ret->tail = case_clausule;

    return ret;
}

static case_list_t *case_list_add(parser_ctx_t *ctx, case_list_t *list, case_clausule_t *case_clausule)
{
    list->tail = list->tail->next = case_clausule;

    return list;
}

static case_clausule_t *new_case_block(parser_ctx_t *ctx, case_list_t *case_list1,
        case_clausule_t *default_clausule, case_list_t *case_list2)
{
    case_clausule_t *ret = NULL, *iter = NULL, *iter2;
    statement_t *stat = NULL;

    if(case_list1) {
        ret = case_list1->head;
        iter = case_list1->tail;
    }

    if(default_clausule) {
        if(ret)
            iter = iter->next = default_clausule;
        else
            ret = iter = default_clausule;
    }

    if(case_list2) {
        if(ret)
            iter->next = case_list2->head;
        else
            ret = case_list2->head;
    }

    if(!ret)
        return NULL;

    for(iter = ret; iter; iter = iter->next) {
        for(iter2 = iter; iter2 && !iter2->stat; iter2 = iter2->next);
        if(!iter2)
            break;

        while(iter != iter2) {
            iter->stat = iter2->stat;
            iter = iter->next;
        }

        if(stat) {
            while(stat->next)
                stat = stat->next;
            stat->next = iter->stat;
        }else {
            stat = iter->stat;
        }
    }

    return ret;
}

static statement_t *new_block_statement(parser_ctx_t *ctx, unsigned loc, statement_list_t *list)
{
    block_statement_t *ret;

    ret = new_statement(ctx, STAT_BLOCK, sizeof(*ret), loc);
    if(!ret)
        return NULL;

    ret->scope_index = 0;
    ret->stat_list = list ? list->head : NULL;

    return &ret->stat;
}

static variable_declaration_t *new_variable_declaration(parser_ctx_t *ctx, const WCHAR *identifier, expression_t *expr)
{
    variable_declaration_t *ret = parser_alloc(ctx, sizeof(variable_declaration_t));

    ret->identifier = identifier;
    ret->expr = expr;
    ret->next = NULL;
    ret->global_next = NULL;
    ret->block_scope = FALSE;
    ret->constant = FALSE;

    return ret;
}

static variable_list_t *new_variable_list(parser_ctx_t *ctx, variable_declaration_t *decl)
{
    variable_list_t *ret = parser_alloc_tmp(ctx, sizeof(variable_list_t));

    ret->head = ret->tail = decl;

    return ret;
}

static variable_list_t *variable_list_add(parser_ctx_t *ctx, variable_list_t *list, variable_declaration_t *decl)
{
    list->tail = list->tail->next = decl;

    return list;
}

static statement_t *new_var_statement(parser_ctx_t *ctx, BOOL block_scope, BOOL constant, unsigned loc,
        variable_list_t *variable_list)
{
    variable_declaration_t *var;
    var_statement_t *ret;

    ret = new_statement(ctx, STAT_VAR, sizeof(*ret), loc);
    if(!ret)
        return NULL;

    ret->variable_list = variable_list->head;
    for (var = ret->variable_list; var; var = var->next)
    {
        var->block_scope = block_scope;
        var->constant = constant;
    }

    return &ret->stat;
}

static statement_t *new_expression_statement(parser_ctx_t *ctx, unsigned loc, expression_t *expr)
{
    expression_statement_t *ret;

    ret = new_statement(ctx, STAT_EXPR, sizeof(*ret), loc);
    if(!ret)
        return NULL;

    ret->expr = expr;

    return &ret->stat;
}

static statement_t *new_if_statement(parser_ctx_t *ctx, unsigned loc, expression_t *expr,
                                     statement_t *if_stat, statement_t *else_stat)
{
    if_statement_t *ret;

    ret = new_statement(ctx, STAT_IF, sizeof(*ret), loc);
    if(!ret)
        return NULL;

    ret->expr = expr;
    ret->if_stat = if_stat;
    ret->else_stat = else_stat;

    return &ret->stat;
}

static statement_t *new_while_statement(parser_ctx_t *ctx, unsigned loc, BOOL dowhile, expression_t *expr, statement_t *stat)
{
    while_statement_t *ret;

    ret = new_statement(ctx, STAT_WHILE, sizeof(*ret), loc);
    if(!ret)
        return NULL;

    ret->do_while = dowhile;
    ret->expr = expr;
    ret->statement = stat;

    return &ret->stat;
}

static statement_t *new_for_statement(parser_ctx_t *ctx, unsigned loc, variable_declaration_t *variable_list, expression_t *begin_expr,
        expression_t *expr, unsigned expr_loc, expression_t *end_expr, unsigned end_loc, statement_t *statement)
{
    for_statement_t *ret;

    ret = new_statement(ctx, STAT_FOR, sizeof(*ret), loc);
    if(!ret)
        return NULL;

    ret->variable_list = variable_list;
    ret->begin_expr = begin_expr;
    ret->expr = expr;
    ret->expr_loc = expr_loc;
    ret->end_expr = end_expr;
    ret->end_loc = end_loc;
    ret->statement = statement;
    ret->scope_index = 0;

    return &ret->stat;
}

static statement_t *new_forin_statement(parser_ctx_t *ctx, unsigned loc, variable_declaration_t *variable, expression_t *expr,
        expression_t *in_expr, statement_t *statement)
{
    forin_statement_t *ret;

    ret = new_statement(ctx, STAT_FORIN, sizeof(*ret), loc);
    if(!ret)
        return NULL;

    ret->variable = variable;
    ret->expr = expr;
    ret->in_expr = in_expr;
    ret->statement = statement;

    return &ret->stat;
}

static statement_t *new_continue_statement(parser_ctx_t *ctx, unsigned loc, const WCHAR *identifier)
{
    branch_statement_t *ret;

    ret = new_statement(ctx, STAT_CONTINUE, sizeof(*ret), loc);
    if(!ret)
        return NULL;

    ret->identifier = identifier;

    return &ret->stat;
}

static statement_t *new_break_statement(parser_ctx_t *ctx, unsigned loc, const WCHAR *identifier)
{
    branch_statement_t *ret;

    ret = new_statement(ctx, STAT_BREAK, sizeof(*ret), loc);
    if(!ret)
        return NULL;

    ret->identifier = identifier;

    return &ret->stat;
}

static statement_t *new_return_statement(parser_ctx_t *ctx, unsigned loc, expression_t *expr)
{
    expression_statement_t *ret;

    ret = new_statement(ctx, STAT_RETURN, sizeof(*ret), loc);
    if(!ret)
        return NULL;

    ret->expr = expr;

    return &ret->stat;
}

static statement_t *new_with_statement(parser_ctx_t *ctx, unsigned loc, expression_t *expr, statement_t *statement)
{
    with_statement_t *ret;

    ret = new_statement(ctx, STAT_WITH, sizeof(*ret), loc);
    if(!ret)
        return NULL;

    ret->expr = expr;
    ret->statement = statement;

    return &ret->stat;
}

static statement_t *new_labelled_statement(parser_ctx_t *ctx, unsigned loc, const WCHAR *identifier, statement_t *statement)
{
    labelled_statement_t *ret;

    ret = new_statement(ctx, STAT_LABEL, sizeof(*ret), loc);
    if(!ret)
        return NULL;

    ret->identifier = identifier;
    ret->statement = statement;

    return &ret->stat;
}

static statement_t *new_switch_statement(parser_ctx_t *ctx, unsigned loc, expression_t *expr, case_clausule_t *case_list)
{
    switch_statement_t *ret;

    ret = new_statement(ctx, STAT_SWITCH, sizeof(*ret), loc);
    if(!ret)
        return NULL;

    ret->expr = expr;
    ret->case_list = case_list;

    return &ret->stat;
}

static statement_t *new_throw_statement(parser_ctx_t *ctx, unsigned loc, expression_t *expr)
{
    expression_statement_t *ret;

    ret = new_statement(ctx, STAT_THROW, sizeof(*ret), loc);
    if(!ret)
        return NULL;

    ret->expr = expr;

    return &ret->stat;
}

static statement_t *new_try_statement(parser_ctx_t *ctx, statement_t *try_statement,
        catch_block_t *catch_block, statement_t *finally_statement, unsigned finally_loc)
{
    try_statement_t *ret;

    ret = new_statement(ctx, STAT_TRY, sizeof(*ret), try_statement->loc);
    if(!ret)
        return NULL;

    ret->try_statement = try_statement;
    ret->catch_block = catch_block;
    ret->finally_statement = finally_statement;
    ret->finally_loc = finally_loc;

    return &ret->stat;
}

static parameter_t *new_parameter(parser_ctx_t *ctx, const WCHAR *identifier)
{
    parameter_t *ret = parser_alloc(ctx, sizeof(parameter_t));

    ret->identifier = identifier;
    ret->next = NULL;

    return ret;
}

static parameter_list_t *new_parameter_list(parser_ctx_t *ctx, const WCHAR *identifier)
{
    parameter_list_t *ret = parser_alloc_tmp(ctx, sizeof(parameter_list_t));

    ret->head = ret->tail = new_parameter(ctx, identifier);

    return ret;
}

static parameter_list_t *parameter_list_add(parser_ctx_t *ctx, parameter_list_t *list, const WCHAR *identifier)
{
    list->tail = list->tail->next = new_parameter(ctx, identifier);

    return list;
}

static expression_t *new_function_expression(parser_ctx_t *ctx, const WCHAR *identifier, parameter_list_t *parameter_list,
    statement_list_t *statement_list, const WCHAR *event_target, const WCHAR *src_str, DWORD src_len)
{
    function_expression_t *ret = new_expression(ctx, EXPR_FUNC, sizeof(*ret));

    ret->identifier = identifier;
    ret->parameter_list = parameter_list ? parameter_list->head : NULL;
    ret->statement_list = statement_list ? statement_list->head : NULL;
    ret->event_target = event_target;
    ret->src_str = src_str;
    ret->src_len = src_len;
    ret->is_statement = FALSE;
    ret->scope_index = 0;
    ret->next = NULL;

    return &ret->expr;
}

static void *new_expression(parser_ctx_t *ctx, expression_type_t type, size_t size)
{
    expression_t *ret = parser_alloc(ctx, size ? size : sizeof(*ret));

    ret->type = type;

    return ret;
}

static expression_t *new_binary_expression(parser_ctx_t *ctx, expression_type_t type,
       expression_t *expression1, expression_t *expression2)
{
    binary_expression_t *ret = new_expression(ctx, type, sizeof(*ret));

    ret->expression1 = expression1;
    ret->expression2 = expression2;

    return &ret->expr;
}

static expression_t *new_unary_expression(parser_ctx_t *ctx, expression_type_t type, expression_t *expression)
{
    unary_expression_t *ret = new_expression(ctx, type, sizeof(*ret));

    ret->expression = expression;

    return &ret->expr;
}

static expression_t *new_conditional_expression(parser_ctx_t *ctx, expression_t *expression,
       expression_t *true_expression, expression_t *false_expression)
{
    conditional_expression_t *ret = new_expression(ctx, EXPR_COND, sizeof(*ret));

    ret->expression = expression;
    ret->true_expression = true_expression;
    ret->false_expression = false_expression;

    return &ret->expr;
}

static expression_t *new_member_expression(parser_ctx_t *ctx, expression_t *expression, const WCHAR *identifier)
{
    member_expression_t *ret = new_expression(ctx, EXPR_MEMBER, sizeof(*ret));

    ret->expression = expression;
    ret->identifier = identifier;

    return &ret->expr;
}

static expression_t *new_new_expression(parser_ctx_t *ctx, expression_t *expression, argument_list_t *argument_list)
{
    call_expression_t *ret = new_expression(ctx, EXPR_NEW, sizeof(*ret));

    ret->expression = expression;
    ret->argument_list = argument_list ? argument_list->head : NULL;

    return &ret->expr;
}

static expression_t *new_call_expression(parser_ctx_t *ctx, expression_t *expression, argument_list_t *argument_list)
{
    call_expression_t *ret = new_expression(ctx, EXPR_CALL, sizeof(*ret));

    ret->expression = expression;
    ret->argument_list = argument_list ? argument_list->head : NULL;

    return &ret->expr;
}

static int parser_error(unsigned *loc, parser_ctx_t *ctx, const char *str)
{
    if(ctx->error_loc == -1)
        ctx->error_loc = *loc;
    if(ctx->hres == S_OK)
        ctx->hres = JS_E_SYNTAX;
    return 0;
}

static void set_error(parser_ctx_t *ctx, unsigned loc, HRESULT error)
{
    ctx->hres = error;
    ctx->error_loc = loc;
}

static BOOL explicit_error(parser_ctx_t *ctx, void *obj, WCHAR next)
{
    if(obj || *(ctx->ptr-1)==next) return TRUE;

    set_error(ctx, ctx->ptr - ctx->begin /* FIXME */, JS_E_SYNTAX);
    return FALSE;
}


static expression_t *new_identifier_expression(parser_ctx_t *ctx, const WCHAR *identifier)
{
    identifier_expression_t *ret = new_expression(ctx, EXPR_IDENT, sizeof(*ret));

    ret->identifier = identifier;

    return &ret->expr;
}

static expression_t *new_array_literal_expression(parser_ctx_t *ctx, element_list_t *element_list, int length)
{
    array_literal_expression_t *ret = new_expression(ctx, EXPR_ARRAYLIT, sizeof(*ret));

    ret->element_list = element_list ? element_list->head : NULL;
    ret->length = length;

    return &ret->expr;
}

static expression_t *new_prop_and_value_expression(parser_ctx_t *ctx, property_list_t *property_list)
{
    property_value_expression_t *ret = new_expression(ctx, EXPR_PROPVAL, sizeof(*ret));

    ret->property_list = property_list ? property_list->head : NULL;

    return &ret->expr;
}

static expression_t *new_literal_expression(parser_ctx_t *ctx, literal_t *literal)
{
    literal_expression_t *ret = new_expression(ctx, EXPR_LITERAL, sizeof(*ret));

    ret->literal = literal;

    return &ret->expr;
}

static statement_list_t *new_statement_list(parser_ctx_t *ctx, statement_t *statement)
{
    statement_list_t *ret =  parser_alloc_tmp(ctx, sizeof(statement_list_t));

    ret->head = ret->tail = statement;

    return ret;
}

static statement_list_t *statement_list_add(statement_list_t *list, statement_t *statement)
{
    list->tail = list->tail->next = statement;

    return list;
}

void parser_release(parser_ctx_t *ctx)
{
    script_release(ctx->script);
    heap_pool_free(&ctx->heap);
    free(ctx);
}

HRESULT script_parse(script_ctx_t *ctx, struct _compiler_ctx_t *compiler, bytecode_t *code, const WCHAR *delimiter, BOOL from_eval,
        parser_ctx_t **ret)
{
    parser_ctx_t *parser_ctx;
    heap_pool_t *mark;
    HRESULT hres;

    parser_ctx = calloc(1, sizeof(parser_ctx_t));
    if(!parser_ctx)
        return E_OUTOFMEMORY;

    parser_ctx->error_loc = -1;
    parser_ctx->is_html = delimiter && !wcsicmp(delimiter, L"</script>");

    parser_ctx->begin = parser_ctx->ptr = code->source;
    parser_ctx->end = parser_ctx->begin + lstrlenW(parser_ctx->begin);

    script_addref(ctx);
    parser_ctx->script = ctx;

    mark = heap_pool_mark(&ctx->tmp_heap);
    heap_pool_init(&parser_ctx->heap);

    parser_ctx->compiler = compiler;
    parser_parse(parser_ctx);
    parser_ctx->compiler = NULL;

    heap_pool_clear(mark);
    hres = parser_ctx->hres;
    if(FAILED(hres)) {
        unsigned int error_loc = parser_ctx->error_loc == -1 ? 0 : parser_ctx->error_loc;
        const WCHAR *line_start = code->source + error_loc, *line_end = line_start;
        jsstr_t *line_str;

        while(line_start > code->source && line_start[-1] != '\n')
            line_start--;
        while(*line_end && *line_end != '\n')
            line_end++;
        line_str = jsstr_alloc_len(line_start, line_end - line_start);

        WARN("parser failed around %s in line %s\n",
             debugstr_w(parser_ctx->begin+20 > parser_ctx->ptr ? parser_ctx->begin : parser_ctx->ptr-20),
             debugstr_jsstr(line_str));

        throw_error(ctx, hres, NULL);
        set_error_location(ctx->ei, code, error_loc, IDS_COMPILATION_ERROR, line_str);
        parser_release(parser_ctx);
        if(line_str)
            jsstr_release(line_str);
        return DISP_E_EXCEPTION;
    }

    *ret = parser_ctx;
    return S_OK;
}
