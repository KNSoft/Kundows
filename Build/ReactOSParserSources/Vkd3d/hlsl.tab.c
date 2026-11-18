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
#define YYPURE 2

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Substitute the type names.  */
#define YYSTYPE         HLSL_YYSTYPE
#define YYLTYPE         HLSL_YYLTYPE
/* Substitute the variable and function names.  */
#define yyparse         hlsl_yyparse
#define yylex           hlsl_yylex
#define yyerror         hlsl_yyerror
#define yydebug         hlsl_yydebug
#define yynerrs         hlsl_yynerrs


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
# define YYERROR_VERBOSE 1
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_HLSL_YY_E_KNSOFT_KUNDOWS_BUILD_REACTOSPARSERSOURCES_VKD3D_HLSL_TAB_H_INCLUDED
# define YY_HLSL_YY_E_KNSOFT_KUNDOWS_BUILD_REACTOSPARSERSOURCES_VKD3D_HLSL_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef HLSL_YYDEBUG
# if defined YYDEBUG
#if YYDEBUG
#   define HLSL_YYDEBUG 1
#  else
#   define HLSL_YYDEBUG 0
#  endif
# else /* ! defined YYDEBUG */
#  define HLSL_YYDEBUG 0
# endif /* ! defined YYDEBUG */
#endif  /* ! defined HLSL_YYDEBUG */
#if HLSL_YYDEBUG
extern int hlsl_yydebug;
#endif
/* "%code requires" blocks.  */
#line 24 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"


#include "hlsl.h"
#include <stdio.h>

#define HLSL_YYLTYPE struct vkd3d_shader_location

struct parse_fields
{
    struct hlsl_struct_field *fields;
    size_t count, capacity;
};

struct parse_initializer
{
    struct hlsl_ir_node **args;
    unsigned int args_count;
    struct hlsl_block *instrs;
    bool braces;
    struct vkd3d_shader_location loc;
};

struct parse_parameter
{
    struct hlsl_type *type;
    const char *name;
    struct hlsl_semantic semantic;
    struct hlsl_reg_reservation reg_reservation;
    uint32_t modifiers;
    struct parse_initializer initializer;
};

struct parse_colon_attributes
{
    struct hlsl_semantic semantic;
    struct hlsl_reg_reservation reg_reservation;
};

struct parse_array_sizes
{
    uint32_t *sizes; /* innermost first */
    unsigned int count;
};

struct parse_variable_def
{
    struct list entry;
    struct vkd3d_shader_location loc;

    char *name;
    struct parse_array_sizes arrays;
    struct hlsl_semantic semantic;
    struct hlsl_reg_reservation reg_reservation;
    struct parse_initializer initializer;
    struct hlsl_scope *annotations;

    struct hlsl_type *basic_type;
    uint32_t modifiers;
    struct vkd3d_shader_location modifiers_loc;

    struct hlsl_state_block **state_blocks;
    unsigned int state_block_count;
    size_t state_block_capacity;
};

struct parse_function
{
    struct hlsl_ir_function_decl *decl;
    struct hlsl_func_parameters parameters;
    struct hlsl_semantic return_semantic;
    bool first;
};

struct parse_if_body
{
    struct hlsl_block *then_block;
    struct hlsl_block *else_block;
};

enum parse_assign_op
{
    ASSIGN_OP_ASSIGN,
    ASSIGN_OP_ADD,
    ASSIGN_OP_SUB,
    ASSIGN_OP_MUL,
    ASSIGN_OP_DIV,
    ASSIGN_OP_MOD,
    ASSIGN_OP_LSHIFT,
    ASSIGN_OP_RSHIFT,
    ASSIGN_OP_AND,
    ASSIGN_OP_OR,
    ASSIGN_OP_XOR,
};

struct parse_attribute_list
{
    unsigned int count;
    const struct hlsl_attribute **attrs;
};

struct state_block_index
{
    bool has_index;
    unsigned int index;
};


#line 234 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"

/* Token type.  */
#ifndef HLSL_YYTOKENTYPE
# define HLSL_YYTOKENTYPE
  enum hlsl_yytokentype
  {
    KW_BLENDSTATE = 258,
    KW_BREAK = 259,
    KW_BUFFER = 260,
    KW_BYTEADDRESSBUFFER = 261,
    KW_CASE = 262,
    KW_CONSTANTBUFFER = 263,
    KW_CBUFFER = 264,
    KW_CENTROID = 265,
    KW_COLUMN_MAJOR = 266,
    KW_COMPILE = 267,
    KW_COMPILESHADER = 268,
    KW_COMPUTESHADER = 269,
    KW_CONST = 270,
    KW_CONSTRUCTGSWITHSO = 271,
    KW_CONTINUE = 272,
    KW_DEFAULT = 273,
    KW_DEPTHSTENCILSTATE = 274,
    KW_DEPTHSTENCILVIEW = 275,
    KW_DISCARD = 276,
    KW_DO = 277,
    KW_DOMAINSHADER = 278,
    KW_ELSE = 279,
    KW_EXPORT = 280,
    KW_EXTERN = 281,
    KW_FALSE = 282,
    KW_FOR = 283,
    KW_FXGROUP = 284,
    KW_GEOMETRYSHADER = 285,
    KW_GROUPSHARED = 286,
    KW_HULLSHADER = 287,
    KW_IF = 288,
    KW_IN = 289,
    KW_INLINE = 290,
    KW_INOUT = 291,
    KW_LINEAR = 292,
    KW_MATRIX = 293,
    KW_NAMESPACE = 294,
    KW_NOINTERPOLATION = 295,
    KW_NOPERSPECTIVE = 296,
    KW_NULL = 297,
    KW_OUT = 298,
    KW_PACKOFFSET = 299,
    KW_PASS = 300,
    KW_PIXELSHADER = 301,
    KW_RASTERIZERORDEREDBUFFER = 302,
    KW_RASTERIZERORDEREDSTRUCTUREDBUFFER = 303,
    KW_RASTERIZERORDEREDTEXTURE1D = 304,
    KW_RASTERIZERORDEREDTEXTURE1DARRAY = 305,
    KW_RASTERIZERORDEREDTEXTURE2D = 306,
    KW_RASTERIZERORDEREDTEXTURE2DARRAY = 307,
    KW_RASTERIZERORDEREDTEXTURE3D = 308,
    KW_RASTERIZERSTATE = 309,
    KW_RENDERTARGETVIEW = 310,
    KW_RETURN = 311,
    KW_REGISTER = 312,
    KW_ROW_MAJOR = 313,
    KW_RWBUFFER = 314,
    KW_RWBYTEADDRESSBUFFER = 315,
    KW_RWSTRUCTUREDBUFFER = 316,
    KW_RWTEXTURE1D = 317,
    KW_RWTEXTURE1DARRAY = 318,
    KW_RWTEXTURE2D = 319,
    KW_RWTEXTURE2DARRAY = 320,
    KW_RWTEXTURE3D = 321,
    KW_SAMPLER = 322,
    KW_SAMPLER1D = 323,
    KW_SAMPLER2D = 324,
    KW_SAMPLER3D = 325,
    KW_SAMPLERCUBE = 326,
    KW_SAMPLER_STATE = 327,
    KW_SAMPLERCOMPARISONSTATE = 328,
    KW_SHARED = 329,
    KW_SNORM = 330,
    KW_STATEBLOCK = 331,
    KW_STATEBLOCK_STATE = 332,
    KW_STATIC = 333,
    KW_STRING = 334,
    KW_STRUCT = 335,
    KW_SWITCH = 336,
    KW_TBUFFER = 337,
    KW_TECHNIQUE = 338,
    KW_TECHNIQUE10 = 339,
    KW_TECHNIQUE11 = 340,
    KW_TEXTURE = 341,
    KW_TEXTURE1D = 342,
    KW_TEXTURE1DARRAY = 343,
    KW_TEXTURE2D = 344,
    KW_TEXTURE2DARRAY = 345,
    KW_TEXTURE2DMS = 346,
    KW_TEXTURE2DMSARRAY = 347,
    KW_TEXTURE3D = 348,
    KW_TEXTURECUBE = 349,
    KW_TEXTURECUBEARRAY = 350,
    KW_TRUE = 351,
    KW_TYPEDEF = 352,
    KW_UNSIGNED = 353,
    KW_UNIFORM = 354,
    KW_UNORM = 355,
    KW_VECTOR = 356,
    KW_VERTEXSHADER = 357,
    KW_VOID = 358,
    KW_VOLATILE = 359,
    KW_WHILE = 360,
    OP_INC = 361,
    OP_DEC = 362,
    OP_AND = 363,
    OP_OR = 364,
    OP_EQ = 365,
    OP_LEFTSHIFT = 366,
    OP_LEFTSHIFTASSIGN = 367,
    OP_RIGHTSHIFT = 368,
    OP_RIGHTSHIFTASSIGN = 369,
    OP_LE = 370,
    OP_GE = 371,
    OP_NE = 372,
    OP_ADDASSIGN = 373,
    OP_SUBASSIGN = 374,
    OP_MULASSIGN = 375,
    OP_DIVASSIGN = 376,
    OP_MODASSIGN = 377,
    OP_ANDASSIGN = 378,
    OP_ORASSIGN = 379,
    OP_XORASSIGN = 380,
    C_FLOAT = 381,
    C_INTEGER = 382,
    C_UNSIGNED = 383,
    PRE_LINE = 384,
    VAR_IDENTIFIER = 385,
    NEW_IDENTIFIER = 386,
    STRING = 387,
    TYPE_IDENTIFIER = 388
  };
#endif

/* Value type.  */
#if ! defined HLSL_YYSTYPE && ! defined HLSL_YYSTYPE_IS_DECLARED
union HLSL_YYSTYPE
{
#line 6532 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"

    struct hlsl_type *type;
    INT intval;
    FLOAT floatval;
    bool boolval;
    char *name;
    uint32_t modifiers;
    struct hlsl_ir_node *instr;
    struct hlsl_block *block;
    struct list *list;
    struct parse_fields fields;
    struct parse_function function;
    struct parse_parameter parameter;
    struct hlsl_func_parameters parameters;
    struct parse_initializer initializer;
    struct parse_array_sizes arrays;
    struct parse_variable_def *variable_def;
    struct parse_if_body if_body;
    enum parse_assign_op assign_op;
    struct hlsl_reg_reservation reg_reservation;
    struct parse_colon_attributes colon_attributes;
    struct hlsl_semantic semantic;
    enum hlsl_buffer_type buffer_type;
    enum hlsl_sampler_dim sampler_dim;
    struct hlsl_attribute *attr;
    struct parse_attribute_list attr_list;
    struct hlsl_ir_switch_case *switch_case;
    struct hlsl_scope *scope;
    struct hlsl_state_block *state_block;
    struct state_block_index state_block_index;

#line 411 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"

};
typedef union HLSL_YYSTYPE HLSL_YYSTYPE;
# define HLSL_YYSTYPE_IS_TRIVIAL 1
# define HLSL_YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined HLSL_YYLTYPE && ! defined HLSL_YYLTYPE_IS_DECLARED
typedef struct HLSL_YYLTYPE HLSL_YYLTYPE;
struct HLSL_YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define HLSL_YYLTYPE_IS_DECLARED 1
# define HLSL_YYLTYPE_IS_TRIVIAL 1
#endif



int hlsl_yyparse (void *scanner, struct hlsl_ctx *ctx);
/* "%code provides" blocks.  */
#line 133 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"


int yylex(HLSL_YYSTYPE *yylval_param, HLSL_YYLTYPE *yylloc_param, void *yyscanner);


#line 443 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"

#endif /* !YY_HLSL_YY_E_KNSOFT_KUNDOWS_BUILD_REACTOSPARSERSOURCES_VKD3D_HLSL_TAB_H_INCLUDED  */


/* Unqualified %code blocks.  */
#line 140 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"


#define YYLLOC_DEFAULT(cur, rhs, n) (cur) = YYRHSLOC(rhs, !!n)

static void yyerror(YYLTYPE *loc, void *scanner, struct hlsl_ctx *ctx, const char *s)
{
    hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_INVALID_SYNTAX, "%s", s);
}

static struct hlsl_ir_node *node_from_block(struct hlsl_block *block)
{
    return block->value;
}

static struct hlsl_block *make_empty_block(struct hlsl_ctx *ctx)
{
    struct hlsl_block *block;

    if ((block = hlsl_alloc(ctx, sizeof(*block))))
        hlsl_block_init(block);
    return block;
}

static struct list *make_empty_list(struct hlsl_ctx *ctx)
{
    struct list *list;

    if ((list = hlsl_alloc(ctx, sizeof(*list))))
        list_init(list);
    return list;
}

static void destroy_block(struct hlsl_block *block)
{
    if (!block)
        return;

    hlsl_block_cleanup(block);
    vkd3d_free(block);
}

static void destroy_switch_cases(struct list *cases)
{
    hlsl_cleanup_ir_switch_cases(cases);
    vkd3d_free(cases);
}

static bool hlsl_types_are_componentwise_compatible(struct hlsl_ctx *ctx, struct hlsl_type *src,
        struct hlsl_type *dst)
{
    unsigned int k, count = hlsl_type_component_count(dst);

    if (count > hlsl_type_component_count(src))
        return false;

    for (k = 0; k < count; ++k)
    {
        struct hlsl_type *src_comp_type, *dst_comp_type;

        src_comp_type = hlsl_type_get_component_type(ctx, src, k);
        dst_comp_type = hlsl_type_get_component_type(ctx, dst, k);

        if ((src_comp_type->class != HLSL_CLASS_SCALAR || dst_comp_type->class != HLSL_CLASS_SCALAR)
                && !hlsl_types_are_equal(src_comp_type, dst_comp_type))
            return false;
    }
    return true;
}

static bool hlsl_types_are_componentwise_equal(struct hlsl_ctx *ctx, struct hlsl_type *src,
        struct hlsl_type *dst)
{
    unsigned int k, count = hlsl_type_component_count(src);

    if (count != hlsl_type_component_count(dst))
        return false;

    for (k = 0; k < count; ++k)
    {
        struct hlsl_type *src_comp_type, *dst_comp_type;

        src_comp_type = hlsl_type_get_component_type(ctx, src, k);
        dst_comp_type = hlsl_type_get_component_type(ctx, dst, k);

        if (!hlsl_types_are_equal(src_comp_type, dst_comp_type))
            return false;
    }
    return true;
}

static bool type_contains_only_numerics(const struct hlsl_type *type)
{
    unsigned int i;

    if (type->class == HLSL_CLASS_ARRAY)
        return type_contains_only_numerics(type->e.array.type);
    if (type->class == HLSL_CLASS_STRUCT)
    {
        for (i = 0; i < type->e.record.field_count; ++i)
        {
            if (!type_contains_only_numerics(type->e.record.fields[i].type))
                return false;
        }
        return true;
    }
    return hlsl_is_numeric_type(type);
}

static bool explicit_compatible_data_types(struct hlsl_ctx *ctx, struct hlsl_type *src, struct hlsl_type *dst)
{
    if (hlsl_is_numeric_type(src) && src->dimx == 1 && src->dimy == 1 && type_contains_only_numerics(dst))
        return true;

    if (src->class == HLSL_CLASS_MATRIX && dst->class == HLSL_CLASS_MATRIX
            && src->dimx >= dst->dimx && src->dimy >= dst->dimy)
        return true;

    if ((src->class == HLSL_CLASS_MATRIX && src->dimx > 1 && src->dimy > 1)
            && hlsl_type_component_count(src) != hlsl_type_component_count(dst))
        return false;

    if ((dst->class == HLSL_CLASS_MATRIX && dst->dimy > 1)
            && hlsl_type_component_count(src) != hlsl_type_component_count(dst))
        return false;

    return hlsl_types_are_componentwise_compatible(ctx, src, dst);
}

static bool implicit_compatible_data_types(struct hlsl_ctx *ctx, struct hlsl_type *src, struct hlsl_type *dst)
{
    if (hlsl_is_numeric_type(src) != hlsl_is_numeric_type(dst))
        return false;

    if (hlsl_is_numeric_type(src))
    {
        /* Scalar vars can be converted to any other numeric data type */
        if (src->dimx == 1 && src->dimy == 1)
            return true;
        /* The other way around is true too */
        if (dst->dimx == 1 && dst->dimy == 1)
            return true;

        if (src->class == HLSL_CLASS_MATRIX || dst->class == HLSL_CLASS_MATRIX)
        {
            if (src->class == HLSL_CLASS_MATRIX && dst->class == HLSL_CLASS_MATRIX)
                return src->dimx >= dst->dimx && src->dimy >= dst->dimy;

            /* Matrix-vector conversion is apparently allowed if they have
            * the same components count, or if the matrix is 1xN or Nx1
            * and we are reducing the component count */
            if (src->class == HLSL_CLASS_VECTOR || dst->class == HLSL_CLASS_VECTOR)
            {
                if (hlsl_type_component_count(src) == hlsl_type_component_count(dst))
                    return true;

                if ((src->class == HLSL_CLASS_VECTOR || src->dimx == 1 || src->dimy == 1) &&
                        (dst->class == HLSL_CLASS_VECTOR || dst->dimx == 1 || dst->dimy == 1))
                    return hlsl_type_component_count(src) >= hlsl_type_component_count(dst);
            }

            return false;
        }
        else
        {
            return src->dimx >= dst->dimx;
        }
    }

    if (src->class == HLSL_CLASS_NULL)
    {
        switch (dst->class)
        {
            case HLSL_CLASS_DEPTH_STENCIL_STATE:
            case HLSL_CLASS_DEPTH_STENCIL_VIEW:
            case HLSL_CLASS_PIXEL_SHADER:
            case HLSL_CLASS_RASTERIZER_STATE:
            case HLSL_CLASS_RENDER_TARGET_VIEW:
            case HLSL_CLASS_SAMPLER:
            case HLSL_CLASS_STRING:
            case HLSL_CLASS_TEXTURE:
            case HLSL_CLASS_UAV:
            case HLSL_CLASS_VERTEX_SHADER:
                return true;
            default:
                break;
        }
    }

    return hlsl_types_are_componentwise_equal(ctx, src, dst);
}

static void check_condition_type(struct hlsl_ctx *ctx, const struct hlsl_ir_node *cond)
{
    const struct hlsl_type *type = cond->data_type;

    if (type->class == HLSL_CLASS_ERROR)
        return;

    if (type->class > HLSL_CLASS_LAST_NUMERIC || type->dimx > 1 || type->dimy > 1)
    {
        struct vkd3d_string_buffer *string;

        if ((string = hlsl_type_to_string(ctx, type)))
            hlsl_error(ctx, &cond->loc, VKD3D_SHADER_ERROR_HLSL_INVALID_TYPE,
                    "Condition type '%s' is not a scalar numeric type.", string->buffer);
        hlsl_release_string_buffer(ctx, string);
    }
}

static struct hlsl_ir_node *add_cast(struct hlsl_ctx *ctx, struct hlsl_block *block,
        struct hlsl_ir_node *node, struct hlsl_type *dst_type, const struct vkd3d_shader_location *loc)
{
    struct hlsl_type *src_type = node->data_type;
    struct hlsl_ir_node *cast;

    if (hlsl_types_are_equal(src_type, dst_type))
        return node;

    if (src_type->class == HLSL_CLASS_NULL)
        return node;

    if (src_type->class > HLSL_CLASS_VECTOR || dst_type->class > HLSL_CLASS_VECTOR)
    {
        unsigned int src_comp_count = hlsl_type_component_count(src_type);
        unsigned int dst_comp_count = hlsl_type_component_count(dst_type);
        struct hlsl_deref var_deref;
        bool broadcast, matrix_cast;
        struct hlsl_ir_load *load;
        struct hlsl_ir_var *var;
        unsigned int dst_idx;

        broadcast = hlsl_is_numeric_type(src_type) && src_type->dimx == 1 && src_type->dimy == 1;
        matrix_cast = !broadcast && dst_comp_count != src_comp_count
                && src_type->class == HLSL_CLASS_MATRIX && dst_type->class == HLSL_CLASS_MATRIX;
        VKD3D_ASSERT(src_comp_count >= dst_comp_count || broadcast);
        if (matrix_cast)
        {
            VKD3D_ASSERT(dst_type->dimx <= src_type->dimx);
            VKD3D_ASSERT(dst_type->dimy <= src_type->dimy);
        }

        if (!(var = hlsl_new_synthetic_var(ctx, "cast", dst_type, loc)))
            return NULL;
        hlsl_init_simple_deref_from_var(&var_deref, var);

        for (dst_idx = 0; dst_idx < dst_comp_count; ++dst_idx)
        {
            struct hlsl_ir_node *component_load;
            struct hlsl_type *dst_comp_type;
            struct hlsl_block store_block;
            unsigned int src_idx;

            if (broadcast)
            {
                src_idx = 0;
            }
            else if (matrix_cast)
            {
                unsigned int x = dst_idx % dst_type->dimx, y = dst_idx / dst_type->dimx;

                src_idx = y * src_type->dimx + x;
            }
            else
            {
                src_idx = dst_idx;
            }

            dst_comp_type = hlsl_type_get_component_type(ctx, dst_type, dst_idx);

            if (!(component_load = hlsl_add_load_component(ctx, block, node, src_idx, loc)))
                return NULL;

            if (!(cast = hlsl_new_cast(ctx, component_load, dst_comp_type, loc)))
                return NULL;
            hlsl_block_add_instr(block, cast);

            if (!hlsl_new_store_component(ctx, &store_block, &var_deref, dst_idx, cast))
                return NULL;
            hlsl_block_add_block(block, &store_block);
        }

        if (!(load = hlsl_new_var_load(ctx, var, loc)))
            return NULL;
        hlsl_block_add_instr(block, &load->node);

        return &load->node;
    }
    else
    {
        if (!(cast = hlsl_new_cast(ctx, node, dst_type, loc)))
            return NULL;
        hlsl_block_add_instr(block, cast);
        return cast;
    }
}

static struct hlsl_ir_node *add_implicit_conversion(struct hlsl_ctx *ctx, struct hlsl_block *block,
        struct hlsl_ir_node *node, struct hlsl_type *dst_type, const struct vkd3d_shader_location *loc)
{
    struct hlsl_type *src_type = node->data_type;

    if (hlsl_types_are_equal(src_type, dst_type))
        return node;

    if (node->type == HLSL_IR_SAMPLER_STATE && dst_type->class == HLSL_CLASS_SAMPLER)
        return node;

    if (!implicit_compatible_data_types(ctx, src_type, dst_type))
    {
        struct vkd3d_string_buffer *src_string, *dst_string;

        src_string = hlsl_type_to_string(ctx, src_type);
        dst_string = hlsl_type_to_string(ctx, dst_type);
        if (src_string && dst_string)
            hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_INVALID_TYPE,
                    "Can't implicitly convert from %s to %s.", src_string->buffer, dst_string->buffer);
        hlsl_release_string_buffer(ctx, src_string);
        hlsl_release_string_buffer(ctx, dst_string);
        return NULL;
    }

    if (dst_type->dimx * dst_type->dimy < src_type->dimx * src_type->dimy && ctx->warn_implicit_truncation)
        hlsl_warning(ctx, loc, VKD3D_SHADER_WARNING_HLSL_IMPLICIT_TRUNCATION, "Implicit truncation of %s type.",
                src_type->class == HLSL_CLASS_VECTOR ? "vector" : "matrix");

    return add_cast(ctx, block, node, dst_type, loc);
}

static bool add_explicit_conversion(struct hlsl_ctx *ctx, struct hlsl_block *block,
        struct hlsl_type *dst_type, const struct parse_array_sizes *arrays, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_node *instr = node_from_block(block);
    struct hlsl_type *src_type = instr->data_type;
    unsigned int i;

    for (i = 0; i < arrays->count; ++i)
    {
        if (arrays->sizes[i] == HLSL_ARRAY_ELEMENTS_COUNT_IMPLICIT)
            hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_INVALID_TYPE, "Implicit size arrays not allowed in casts.");
        dst_type = hlsl_new_array_type(ctx, dst_type, arrays->sizes[i]);
    }

    if (instr->data_type->class == HLSL_CLASS_ERROR)
        return true;

    if (!explicit_compatible_data_types(ctx, src_type, dst_type))
    {
        struct vkd3d_string_buffer *src_string, *dst_string;

        src_string = hlsl_type_to_string(ctx, src_type);
        dst_string = hlsl_type_to_string(ctx, dst_type);
        if (src_string && dst_string)
            hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_INVALID_TYPE, "Can't cast from %s to %s.",
                    src_string->buffer, dst_string->buffer);
        hlsl_release_string_buffer(ctx, src_string);
        hlsl_release_string_buffer(ctx, dst_string);
        return false;
    }

    return add_cast(ctx, block, instr, dst_type, loc);
}

static uint32_t add_modifiers(struct hlsl_ctx *ctx, uint32_t modifiers, uint32_t mod,
        const struct vkd3d_shader_location *loc)
{
    if (modifiers & mod)
    {
        struct vkd3d_string_buffer *string;

        if ((string = hlsl_modifiers_to_string(ctx, mod)))
            hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_INVALID_MODIFIER,
                    "Modifier '%s' was already specified.", string->buffer);
        hlsl_release_string_buffer(ctx, string);
        return modifiers;
    }
    return modifiers | mod;
}

static bool append_conditional_break(struct hlsl_ctx *ctx, struct hlsl_block *cond_block)
{
    struct hlsl_ir_node *condition, *cast, *not, *iff, *jump;
    struct hlsl_block then_block;
    struct hlsl_type *bool_type;

    /* E.g. "for (i = 0; ; ++i)". */
    if (list_empty(&cond_block->instrs))
        return true;

    condition = node_from_block(cond_block);

    check_condition_type(ctx, condition);

    bool_type = hlsl_get_scalar_type(ctx, HLSL_TYPE_BOOL);
    /* We already checked for a 1-component numeric type, so
     * add_implicit_conversion() is equivalent to add_cast() here. */
    if (!(cast = add_cast(ctx, cond_block, condition, bool_type, &condition->loc)))
        return false;

    if (!(not = hlsl_new_unary_expr(ctx, HLSL_OP1_LOGIC_NOT, cast, &condition->loc)))
        return false;
    hlsl_block_add_instr(cond_block, not);

    hlsl_block_init(&then_block);

    if (!(jump = hlsl_new_jump(ctx, HLSL_IR_JUMP_BREAK, NULL, &condition->loc)))
        return false;
    hlsl_block_add_instr(&then_block, jump);

    if (!(iff = hlsl_new_if(ctx, not, &then_block, NULL, &condition->loc)))
        return false;
    hlsl_block_add_instr(cond_block, iff);
    return true;
}

enum loop_type
{
    LOOP_FOR,
    LOOP_WHILE,
    LOOP_DO_WHILE
};

static void check_attribute_list_for_duplicates(struct hlsl_ctx *ctx, const struct parse_attribute_list *attrs)
{
    unsigned int i, j;

    for (i = 0; i < attrs->count; ++i)
    {
        for (j = i + 1; j < attrs->count; ++j)
        {
            if (!strcmp(attrs->attrs[i]->name, attrs->attrs[j]->name))
                hlsl_error(ctx, &attrs->attrs[j]->loc, VKD3D_SHADER_ERROR_HLSL_INVALID_SYNTAX,
                        "Found duplicate attribute \"%s\".", attrs->attrs[j]->name);
        }
    }
}

static void resolve_loop_continue(struct hlsl_ctx *ctx, struct hlsl_block *block, enum loop_type type,
        struct hlsl_block *cond, struct hlsl_block *iter)
{
    struct hlsl_ir_node *instr, *next;

    LIST_FOR_EACH_ENTRY_SAFE(instr, next, &block->instrs, struct hlsl_ir_node, entry)
    {
        if (instr->type == HLSL_IR_IF)
        {
            struct hlsl_ir_if *iff = hlsl_ir_if(instr);

            resolve_loop_continue(ctx, &iff->then_block, type, cond, iter);
            resolve_loop_continue(ctx, &iff->else_block, type, cond, iter);
        }
        else if (instr->type == HLSL_IR_JUMP)
        {
            struct hlsl_ir_jump *jump = hlsl_ir_jump(instr);
            struct hlsl_block cond_block;

            if (jump->type != HLSL_IR_JUMP_UNRESOLVED_CONTINUE)
                continue;

            if (type == LOOP_DO_WHILE)
            {
                if (!hlsl_clone_block(ctx, &cond_block, cond))
                    return;
                if (!append_conditional_break(ctx, &cond_block))
                {
                    hlsl_block_cleanup(&cond_block);
                    return;
                }
                list_move_before(&instr->entry, &cond_block.instrs);
            }
            else if (type == LOOP_FOR)
            {
                if (!hlsl_clone_block(ctx, &cond_block, iter))
                    return;
                list_move_before(&instr->entry, &cond_block.instrs);
            }
            jump->type = HLSL_IR_JUMP_CONTINUE;
        }
    }
}

static void check_loop_attributes(struct hlsl_ctx *ctx, const struct parse_attribute_list *attributes,
        const struct vkd3d_shader_location *loc)
{
    bool has_unroll = false, has_loop = false, has_fastopt = false;
    unsigned int i;

    for (i = 0; i < attributes->count; ++i)
    {
        const char *name = attributes->attrs[i]->name;

        has_loop |= !strcmp(name, "loop");
        has_unroll |= !strcmp(name, "unroll");
        has_fastopt |= !strcmp(name, "fastopt");
    }

    if (has_unroll && has_loop)
        hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_INVALID_SYNTAX, "Unroll attribute can't be used with 'loop' attribute.");

    if (has_unroll && has_fastopt)
        hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_INVALID_SYNTAX, "Unroll attribute can't be used with 'fastopt' attribute.");
}

static struct hlsl_default_value evaluate_static_expression(struct hlsl_ctx *ctx,
        struct hlsl_block *block, struct hlsl_type *dst_type, const struct vkd3d_shader_location *loc)
{
    struct hlsl_default_value ret = {0};
    struct hlsl_ir_node *node;
    struct hlsl_block expr;
    struct hlsl_src src;

    if (node_from_block(block)->data_type->class == HLSL_CLASS_ERROR)
        return ret;

    LIST_FOR_EACH_ENTRY(node, &block->instrs, struct hlsl_ir_node, entry)
    {
        switch (node->type)
        {
            case HLSL_IR_COMPILE:
            case HLSL_IR_CONSTANT:
            case HLSL_IR_EXPR:
            case HLSL_IR_SAMPLER_STATE:
            case HLSL_IR_STRING_CONSTANT:
            case HLSL_IR_SWIZZLE:
            case HLSL_IR_LOAD:
            case HLSL_IR_INDEX:
                continue;
            case HLSL_IR_STORE:
                if (hlsl_ir_store(node)->lhs.var->is_synthetic)
                    break;
                /* fall-through */
            case HLSL_IR_CALL:
            case HLSL_IR_IF:
            case HLSL_IR_LOOP:
            case HLSL_IR_JUMP:
            case HLSL_IR_RESOURCE_LOAD:
            case HLSL_IR_RESOURCE_STORE:
            case HLSL_IR_SWITCH:
            case HLSL_IR_STATEBLOCK_CONSTANT:
                hlsl_error(ctx, &node->loc, VKD3D_SHADER_ERROR_HLSL_INVALID_SYNTAX,
                        "Expected literal expression.");
                break;
            case HLSL_IR_VSIR_INSTRUCTION_REF:
                vkd3d_unreachable();
        }
    }

    if (!hlsl_clone_block(ctx, &expr, &ctx->static_initializers))
        return ret;
    hlsl_block_add_block(&expr, block);

    if (!(node = add_implicit_conversion(ctx, &expr, node_from_block(&expr), dst_type, loc)))
    {
        hlsl_block_cleanup(&expr);
        return ret;
    }

    /* Wrap the node into a src to allow the reference to survive the multiple const passes. */
    hlsl_src_from_node(&src, node);
    hlsl_lower_index_loads(ctx, &expr);
    hlsl_run_const_passes(ctx, &expr);
    node = src.node;
    hlsl_src_remove(&src);

    if (node->type == HLSL_IR_CONSTANT)
    {
        struct hlsl_ir_constant *constant = hlsl_ir_constant(node);

        ret.number = constant->value.u[0];
    }
    else if (node->type == HLSL_IR_STRING_CONSTANT)
    {
        struct hlsl_ir_string_constant *string = hlsl_ir_string_constant(node);

        if (!(ret.string = vkd3d_strdup(string->string)))
            return ret;
    }
    else if (node->type == HLSL_IR_STRING_CONSTANT)
    {
        hlsl_fixme(ctx, &node->loc, "Evaluate string constants as static expressions.");
    }
    else
    {
        hlsl_error(ctx, &node->loc, VKD3D_SHADER_ERROR_HLSL_INVALID_SYNTAX,
                "Failed to evaluate constant expression.");
    }

    hlsl_block_cleanup(&expr);

    return ret;
}

static unsigned int evaluate_static_expression_as_uint(struct hlsl_ctx *ctx, struct hlsl_block *block,
        const struct vkd3d_shader_location *loc)
{
    struct hlsl_default_value res;

    res = evaluate_static_expression(ctx, block, hlsl_get_scalar_type(ctx, HLSL_TYPE_UINT), loc);
    VKD3D_ASSERT(!res.string);
    return res.number.u;
}

static struct hlsl_block *create_loop(struct hlsl_ctx *ctx, enum loop_type type,
        const struct parse_attribute_list *attributes, struct hlsl_block *init, struct hlsl_block *cond,
        struct hlsl_block *iter, struct hlsl_block *body, const struct vkd3d_shader_location *loc)
{
    enum hlsl_ir_loop_unroll_type unroll_type = HLSL_IR_LOOP_UNROLL;
    unsigned int i, unroll_limit = 0;
    struct hlsl_ir_node *loop;

    check_attribute_list_for_duplicates(ctx, attributes);
    check_loop_attributes(ctx, attributes, loc);

    /* Ignore unroll(0) attribute, and any invalid attribute. */
    for (i = 0; i < attributes->count; ++i)
    {
        const struct hlsl_attribute *attr = attributes->attrs[i];
        if (!strcmp(attr->name, "unroll"))
        {
            if (attr->args_count > 1)
            {
                hlsl_warning(ctx, &attr->loc, VKD3D_SHADER_WARNING_HLSL_IGNORED_ATTRIBUTE,
                        "Ignoring 'unroll' attribute with more than 1 argument.");
                continue;
            }

            if (attr->args_count == 1)
            {
                struct hlsl_block expr;
                hlsl_block_init(&expr);
                if (!hlsl_clone_block(ctx, &expr, &attr->instrs))
                    return NULL;

                unroll_limit = evaluate_static_expression_as_uint(ctx, &expr, loc);
                hlsl_block_cleanup(&expr);
            }

            unroll_type = HLSL_IR_LOOP_FORCE_UNROLL;
        }
        else if (!strcmp(attr->name, "loop"))
        {
            unroll_type = HLSL_IR_LOOP_FORCE_LOOP;
        }
        else if (!strcmp(attr->name, "fastopt")
                || !strcmp(attr->name, "allow_uav_condition"))
        {
            hlsl_fixme(ctx, loc, "Unhandled attribute '%s'.", attr->name);
        }
        else
        {
            hlsl_warning(ctx, loc, VKD3D_SHADER_WARNING_HLSL_UNKNOWN_ATTRIBUTE, "Unrecognized attribute '%s'.", attr->name);
        }
    }

    resolve_loop_continue(ctx, body, type, cond, iter);

    if (!init && !(init = make_empty_block(ctx)))
        goto oom;

    if (!append_conditional_break(ctx, cond))
        goto oom;

    if (iter)
        hlsl_block_add_block(body, iter);

    if (type == LOOP_DO_WHILE)
        list_move_tail(&body->instrs, &cond->instrs);
    else
        list_move_head(&body->instrs, &cond->instrs);

    if (!(loop = hlsl_new_loop(ctx, body, unroll_type, unroll_limit, loc)))
        goto oom;
    hlsl_block_add_instr(init, loop);

    destroy_block(cond);
    destroy_block(body);
    destroy_block(iter);
    return init;

oom:
    destroy_block(init);
    destroy_block(cond);
    destroy_block(iter);
    destroy_block(body);
    return NULL;
}

static unsigned int initializer_size(const struct parse_initializer *initializer)
{
    unsigned int count = 0, i;

    for (i = 0; i < initializer->args_count; ++i)
    {
        count += hlsl_type_component_count(initializer->args[i]->data_type);
    }
    return count;
}

static void cleanup_parse_attribute_list(struct parse_attribute_list *attr_list)
{
    unsigned int i = 0;

    VKD3D_ASSERT(attr_list);
    for (i = 0; i < attr_list->count; ++i)
        hlsl_free_attribute((struct hlsl_attribute *) attr_list->attrs[i]);
    vkd3d_free(attr_list->attrs);
}

static void free_parse_initializer(struct parse_initializer *initializer)
{
    destroy_block(initializer->instrs);
    vkd3d_free(initializer->args);
}

static struct hlsl_ir_node *get_swizzle(struct hlsl_ctx *ctx, struct hlsl_ir_node *value, const char *swizzle,
        struct vkd3d_shader_location *loc)
{
    unsigned int len = strlen(swizzle), component = 0;
    unsigned int i, set, swiz = 0;
    bool valid;

    if (value->data_type->class == HLSL_CLASS_MATRIX)
    {
        /* Matrix swizzle */
        bool m_swizzle;
        unsigned int inc, x, y;

        if (len < 3 || swizzle[0] != '_')
            return NULL;
        m_swizzle = swizzle[1] == 'm';
        inc = m_swizzle ? 4 : 3;

        if (len % inc || len > inc * 4)
            return NULL;

        for (i = 0; i < len; i += inc)
        {
            if (swizzle[i] != '_')
                return NULL;
            if (m_swizzle)
            {
                if (swizzle[i + 1] != 'm')
                    return NULL;
                y = swizzle[i + 2] - '0';
                x = swizzle[i + 3] - '0';
            }
            else
            {
                y = swizzle[i + 1] - '1';
                x = swizzle[i + 2] - '1';
            }

            if (x >= value->data_type->dimx || y >= value->data_type->dimy)
                return NULL;
            swiz |= (y << 4 | x) << component * 8;
            component++;
        }
        return hlsl_new_swizzle(ctx, swiz, component, value, loc);
    }

    /* Vector swizzle */
    if (len > 4)
        return NULL;

    for (set = 0; set < 2; ++set)
    {
        valid = true;
        component = 0;
        for (i = 0; i < len; ++i)
        {
            char c[2][4] = {{'x', 'y', 'z', 'w'}, {'r', 'g', 'b', 'a'}};
            unsigned int s = 0;

            for (s = 0; s < 4; ++s)
            {
                if (swizzle[i] == c[set][s])
                    break;
            }
            if (s == 4)
            {
                valid = false;
                break;
            }

            if (s >= value->data_type->dimx)
                return NULL;
            swiz |= s << component * 2;
            component++;
        }
        if (valid)
            return hlsl_new_swizzle(ctx, swiz, component, value, loc);
    }

    return NULL;
}

static bool add_return(struct hlsl_ctx *ctx, struct hlsl_block *block,
        struct hlsl_ir_node *return_value, const struct vkd3d_shader_location *loc)
{
    struct hlsl_type *return_type = ctx->cur_function->return_type;
    struct hlsl_ir_node *jump;

    if (ctx->cur_function->return_var)
    {
        if (return_value)
        {
            struct hlsl_ir_node *store;

            if (return_value->data_type->class == HLSL_CLASS_ERROR)
                return true;

            if (!(return_value = add_implicit_conversion(ctx, block, return_value, return_type, loc)))
                return false;

            if (!(store = hlsl_new_simple_store(ctx, ctx->cur_function->return_var, return_value)))
                return false;
            list_add_after(&return_value->entry, &store->entry);
        }
        else
        {
            hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_INVALID_RETURN, "Non-void functions must return a value.");
            return false;
        }
    }
    else
    {
        if (return_value)
            hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_INVALID_RETURN, "Void functions cannot return a value.");
    }

    if (!(jump = hlsl_new_jump(ctx, HLSL_IR_JUMP_RETURN, NULL, loc)))
        return false;
    hlsl_block_add_instr(block, jump);

    return true;
}

struct hlsl_ir_node *hlsl_add_load_component(struct hlsl_ctx *ctx, struct hlsl_block *block,
        struct hlsl_ir_node *var_instr, unsigned int comp, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_node *load, *store;
    struct hlsl_block load_block;
    struct hlsl_ir_var *var;
    struct hlsl_deref src;

    if (!(var = hlsl_new_synthetic_var(ctx, "deref", var_instr->data_type, &var_instr->loc)))
        return NULL;

    if (!(store = hlsl_new_simple_store(ctx, var, var_instr)))
        return NULL;
    hlsl_block_add_instr(block, store);

    hlsl_init_simple_deref_from_var(&src, var);
    if (!(load = hlsl_new_load_component(ctx, &load_block, &src, comp, loc)))
        return NULL;
    hlsl_block_add_block(block, &load_block);

    return load;
}

static bool add_record_access(struct hlsl_ctx *ctx, struct hlsl_block *block, struct hlsl_ir_node *record,
        unsigned int idx, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_node *index, *c;

    VKD3D_ASSERT(idx < record->data_type->e.record.field_count);

    if (!(c = hlsl_new_uint_constant(ctx, idx, loc)))
        return false;
    hlsl_block_add_instr(block, c);

    if (!(index = hlsl_new_index(ctx, record, c, loc)))
        return false;
    hlsl_block_add_instr(block, index);

    return true;
}

static struct hlsl_ir_node *add_binary_arithmetic_expr(struct hlsl_ctx *ctx, struct hlsl_block *block,
        enum hlsl_ir_expr_op op, struct hlsl_ir_node *arg1, struct hlsl_ir_node *arg2,
        const struct vkd3d_shader_location *loc);

static bool add_array_access(struct hlsl_ctx *ctx, struct hlsl_block *block, struct hlsl_ir_node *array,
        struct hlsl_ir_node *index, const struct vkd3d_shader_location *loc)
{
    const struct hlsl_type *expr_type = array->data_type, *index_type = index->data_type;
    struct hlsl_ir_node *return_index, *cast;

    if (array->data_type->class == HLSL_CLASS_ERROR || index->data_type->class == HLSL_CLASS_ERROR)
    {
        block->value = ctx->error_instr;
        return true;
    }

    if ((expr_type->class == HLSL_CLASS_TEXTURE || expr_type->class == HLSL_CLASS_UAV)
            && expr_type->sampler_dim != HLSL_SAMPLER_DIM_GENERIC)
    {
        unsigned int dim_count = hlsl_sampler_dim_count(expr_type->sampler_dim);

        if (index_type->class > HLSL_CLASS_VECTOR || index_type->dimx != dim_count)
        {
            struct vkd3d_string_buffer *string;

            if ((string = hlsl_type_to_string(ctx, expr_type)))
                hlsl_error(ctx, &index->loc, VKD3D_SHADER_ERROR_HLSL_INVALID_TYPE,
                        "Array index of type '%s' must be of type 'uint%u'.", string->buffer, dim_count);
            hlsl_release_string_buffer(ctx, string);
            return false;
        }

        if (!(index = add_implicit_conversion(ctx, block, index,
                hlsl_get_vector_type(ctx, HLSL_TYPE_UINT, dim_count), &index->loc)))
            return false;

        if (!(return_index = hlsl_new_index(ctx, array, index, loc)))
            return false;
        hlsl_block_add_instr(block, return_index);

        return true;
    }

    if (index_type->class != HLSL_CLASS_SCALAR)
    {
        hlsl_error(ctx, &index->loc, VKD3D_SHADER_ERROR_HLSL_INVALID_TYPE, "Array index is not scalar.");
        return false;
    }

    if (!(cast = hlsl_new_cast(ctx, index, hlsl_get_scalar_type(ctx, HLSL_TYPE_UINT), &index->loc)))
        return false;
    hlsl_block_add_instr(block, cast);
    index = cast;

    if (expr_type->class != HLSL_CLASS_ARRAY && expr_type->class != HLSL_CLASS_VECTOR && expr_type->class != HLSL_CLASS_MATRIX)
    {
        if (expr_type->class == HLSL_CLASS_SCALAR)
            hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_INVALID_INDEX, "Scalar expressions cannot be array-indexed.");
        else
            hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_INVALID_INDEX, "Expression cannot be array-indexed.");
        return false;
    }

    if (!(return_index = hlsl_new_index(ctx, array, index, loc)))
        return false;
    hlsl_block_add_instr(block, return_index);

    return true;
}

static const struct hlsl_struct_field *get_struct_field(const struct hlsl_struct_field *fields,
        size_t count, const char *name)
{
    size_t i;

    for (i = 0; i < count; ++i)
    {
        if (!strcmp(fields[i].name, name))
            return &fields[i];
    }
    return NULL;
}

static struct hlsl_type *apply_type_modifiers(struct hlsl_ctx *ctx, struct hlsl_type *type,
        uint32_t *modifiers, bool force_majority, const struct vkd3d_shader_location *loc)
{
    unsigned int default_majority = 0;
    struct hlsl_type *new_type;

    if (!(*modifiers & HLSL_MODIFIERS_MAJORITY_MASK)
            && !(type->modifiers & HLSL_MODIFIERS_MAJORITY_MASK)
            && type->class == HLSL_CLASS_MATRIX)
    {
        if (!(default_majority = ctx->matrix_majority) && force_majority)
            default_majority = HLSL_MODIFIER_COLUMN_MAJOR;
    }
    else if (type->class != HLSL_CLASS_MATRIX && (*modifiers & HLSL_MODIFIERS_MAJORITY_MASK))
    {
        hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_INVALID_MODIFIER,
                "'row_major' and 'column_major' modifiers are only allowed for matrices.");
    }

    if (!default_majority && !(*modifiers & HLSL_TYPE_MODIFIERS_MASK))
        return type;

    if (!(new_type = hlsl_type_clone(ctx, type, default_majority, *modifiers & HLSL_TYPE_MODIFIERS_MASK)))
        return NULL;

    *modifiers &= ~HLSL_TYPE_MODIFIERS_MASK;

    if ((new_type->modifiers & HLSL_MODIFIER_ROW_MAJOR) && (new_type->modifiers & HLSL_MODIFIER_COLUMN_MAJOR))
        hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_INVALID_MODIFIER,
                "'row_major' and 'column_major' modifiers are mutually exclusive.");

    return new_type;
}

static void free_parse_variable_def(struct parse_variable_def *v)
{
    free_parse_initializer(&v->initializer);
    vkd3d_free(v->arrays.sizes);
    vkd3d_free(v->name);
    hlsl_cleanup_semantic(&v->semantic);
    VKD3D_ASSERT(!v->state_blocks);
    vkd3d_free(v);
}

static bool gen_struct_fields(struct hlsl_ctx *ctx, struct parse_fields *fields,
        struct hlsl_type *type, uint32_t modifiers, struct list *defs)
{
    struct parse_variable_def *v, *v_next;
    size_t i = 0;

    if (type->class == HLSL_CLASS_MATRIX)
        VKD3D_ASSERT(type->modifiers & HLSL_MODIFIERS_MAJORITY_MASK);

    memset(fields, 0, sizeof(*fields));
    fields->count = list_count(defs);
    if (!hlsl_array_reserve(ctx, (void **)&fields->fields, &fields->capacity, fields->count, sizeof(*fields->fields)))
        return false;

    LIST_FOR_EACH_ENTRY_SAFE(v, v_next, defs, struct parse_variable_def, entry)
    {
        struct hlsl_struct_field *field = &fields->fields[i++];
        bool unbounded_res_array = false;
        unsigned int k;

        field->type = type;

        if (hlsl_version_ge(ctx, 5, 1) && hlsl_type_is_resource(type))
        {
            for (k = 0; k < v->arrays.count; ++k)
                unbounded_res_array |= (v->arrays.sizes[k] == HLSL_ARRAY_ELEMENTS_COUNT_IMPLICIT);
        }

        if (unbounded_res_array)
        {
            if (v->arrays.count == 1)
            {
                hlsl_fixme(ctx, &v->loc, "Unbounded resource arrays as struct fields.");
                free_parse_variable_def(v);
                vkd3d_free(field);
                continue;
            }
            else
            {
                hlsl_error(ctx, &v->loc, VKD3D_SHADER_ERROR_HLSL_INVALID_TYPE,
                        "Unbounded resource arrays cannot be multi-dimensional.");
            }
        }
        else
        {
            for (k = 0; k < v->arrays.count; ++k)
            {
                if (v->arrays.sizes[k] == HLSL_ARRAY_ELEMENTS_COUNT_IMPLICIT)
                {
                    hlsl_error(ctx, &v->loc, VKD3D_SHADER_ERROR_HLSL_INVALID_TYPE,
                            "Implicit size arrays not allowed in struct fields.");
                }

                field->type = hlsl_new_array_type(ctx, field->type, v->arrays.sizes[k]);
            }
        }

        if (hlsl_version_ge(ctx, 5, 1) && field->type->class == HLSL_CLASS_ARRAY && hlsl_type_is_resource(field->type))
            hlsl_fixme(ctx, &v->loc, "Shader model 5.1+ resource array.");

        vkd3d_free(v->arrays.sizes);
        field->loc = v->loc;
        field->name = v->name;
        field->semantic = v->semantic;
        field->storage_modifiers = modifiers;
        if (v->initializer.args_count)
        {
            hlsl_error(ctx, &v->loc, VKD3D_SHADER_ERROR_HLSL_INVALID_SYNTAX, "Illegal initializer on a struct field.");
            free_parse_initializer(&v->initializer);
        }
        if (v->reg_reservation.offset_type)
            hlsl_error(ctx, &v->loc, VKD3D_SHADER_ERROR_HLSL_INVALID_RESERVATION,
                "packoffset() is not allowed inside struct definitions.");
        vkd3d_free(v);
    }
    vkd3d_free(defs);
    return true;
}

static bool add_record_access_recurse(struct hlsl_ctx *ctx, struct hlsl_block *block,
        const char *name, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_node *record = node_from_block(block);
    const struct hlsl_type *type = record->data_type;
    const struct hlsl_struct_field *field, *base;

    if ((field = get_struct_field(type->e.record.fields, type->e.record.field_count, name)))
    {
        unsigned int field_idx = field - type->e.record.fields;

        return add_record_access(ctx, block, record, field_idx, loc);
    }
    else if ((base = get_struct_field(type->e.record.fields, type->e.record.field_count, "$super")))
    {
        unsigned int base_idx = base - type->e.record.fields;

        if (!add_record_access(ctx, block, record, base_idx, loc))
            return false;
        return add_record_access_recurse(ctx, block, name, loc);
    }

    hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_NOT_DEFINED, "Field \"%s\" is not defined.", name);
    block->value = ctx->error_instr;
    return true;
}

static bool add_typedef(struct hlsl_ctx *ctx, struct hlsl_type *const orig_type, struct list *list)
{
    struct parse_variable_def *v, *v_next;
    struct hlsl_type *type;
    unsigned int i;
    bool ret;

    LIST_FOR_EACH_ENTRY_SAFE(v, v_next, list, struct parse_variable_def, entry)
    {
        if (!v->arrays.count)
        {
            if (!(type = hlsl_type_clone(ctx, orig_type, 0, 0)))
            {
                free_parse_variable_def(v);
                continue;
            }
        }
        else
        {
            uint32_t var_modifiers = 0;

            if (!(type = apply_type_modifiers(ctx, orig_type, &var_modifiers, true, &v->loc)))
            {
                free_parse_variable_def(v);
                continue;
            }
        }

        ret = true;
        for (i = 0; i < v->arrays.count; ++i)
        {
            if (v->arrays.sizes[i] == HLSL_ARRAY_ELEMENTS_COUNT_IMPLICIT)
            {
                hlsl_error(ctx, &v->loc, VKD3D_SHADER_ERROR_HLSL_INVALID_TYPE,
                        "Implicit size arrays not allowed in typedefs.");
            }

            if (!(type = hlsl_new_array_type(ctx, type, v->arrays.sizes[i])))
            {
                free_parse_variable_def(v);
                ret = false;
                break;
            }
        }
        if (!ret)
            continue;
        vkd3d_free(v->arrays.sizes);

        vkd3d_free((void *)type->name);
        type->name = v->name;

        ret = hlsl_scope_add_type(ctx->cur_scope, type);
        if (!ret)
            hlsl_error(ctx, &v->loc, VKD3D_SHADER_ERROR_HLSL_REDEFINED,
                    "Type '%s' is already defined.", v->name);
        free_parse_initializer(&v->initializer);
        vkd3d_free(v);
    }
    vkd3d_free(list);
    return true;
}

static void initialize_var_components(struct hlsl_ctx *ctx, struct hlsl_block *instrs,
        struct hlsl_ir_var *dst, unsigned int *store_index, struct hlsl_ir_node *src,
        bool is_default_values_initializer);

static bool add_func_parameter(struct hlsl_ctx *ctx, struct hlsl_func_parameters *parameters,
        struct parse_parameter *param, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_var *var;

    if (param->type->class == HLSL_CLASS_MATRIX)
        VKD3D_ASSERT(param->type->modifiers & HLSL_MODIFIERS_MAJORITY_MASK);

    if ((param->modifiers & HLSL_STORAGE_OUT) && (param->modifiers & HLSL_STORAGE_UNIFORM))
        hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_INVALID_MODIFIER,
                "Parameter '%s' is declared as both \"out\" and \"uniform\".", param->name);

    if (param->reg_reservation.offset_type)
        hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_INVALID_RESERVATION,
                "packoffset() is not allowed on function parameters.");

    if (parameters->count && parameters->vars[parameters->count - 1]->default_values
                && !param->initializer.args_count)
        hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_MISSING_INITIALIZER,
                "Missing default value for parameter '%s'.", param->name);

    if (param->initializer.args_count && (param->modifiers & HLSL_STORAGE_OUT))
        hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_INVALID_MODIFIER,
                "Output parameter '%s' has a default value.", param->name);

    if (!(var = hlsl_new_var(ctx, param->name, param->type, loc, &param->semantic, param->modifiers,
            &param->reg_reservation)))
        return false;
    var->is_param = 1;

    if (param->initializer.args_count)
    {
        unsigned int component_count = hlsl_type_component_count(param->type);
        unsigned int store_index = 0;
        unsigned int size, i;

        if (!(var->default_values = hlsl_calloc(ctx, component_count, sizeof(*var->default_values))))
            return false;

        if (!param->initializer.braces)
        {
            if (!(add_implicit_conversion(ctx, param->initializer.instrs, param->initializer.args[0], param->type, loc)))
                return false;

            param->initializer.args[0] = node_from_block(param->initializer.instrs);
        }

        size = initializer_size(&param->initializer);
        if (component_count != size)
        {
            hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_WRONG_PARAMETER_COUNT,
                    "Expected %u components in initializer, but got %u.", component_count, size);
        }

        for (i = 0; i < param->initializer.args_count; ++i)
        {
            initialize_var_components(ctx, param->initializer.instrs, var,
                    &store_index, param->initializer.args[i], true);
        }

        free_parse_initializer(&param->initializer);
    }

    if (!hlsl_add_var(ctx, var, false))
    {
        hlsl_free_var(var);
        return false;
    }

    if (!hlsl_array_reserve(ctx, (void **)&parameters->vars, &parameters->capacity,
            parameters->count + 1, sizeof(*parameters->vars)))
        return false;
    parameters->vars[parameters->count++] = var;
    return true;
}

static bool add_pass(struct hlsl_ctx *ctx, const char *name, struct hlsl_scope *annotations,
        struct hlsl_state_block *state_block, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_var *var;
    struct hlsl_type *type;

    type = hlsl_get_type(ctx->globals, "pass", false, false);
    if (!(var = hlsl_new_var(ctx, name, type, loc, NULL, 0, NULL)))
        return false;
    var->annotations = annotations;

    var->state_blocks = hlsl_alloc(ctx, sizeof(*var->state_blocks));
    var->state_blocks[0] = state_block;
    var->state_block_count = 1;
    var->state_block_capacity = 1;

    if (!hlsl_add_var(ctx, var, false))
    {
        struct hlsl_ir_var *old = hlsl_get_var(ctx->cur_scope, var->name);

        hlsl_error(ctx, &var->loc, VKD3D_SHADER_ERROR_HLSL_REDEFINED,
                "Identifier \"%s\" was already declared in this scope.", var->name);
        hlsl_note(ctx, &old->loc, VKD3D_SHADER_LOG_ERROR, "\"%s\" was previously declared here.", old->name);
        hlsl_free_var(var);
        return false;
    }

    return true;
}

static bool add_technique(struct hlsl_ctx *ctx, const char *name, struct hlsl_scope *scope,
        struct hlsl_scope *annotations, const char *typename, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_var *var;
    struct hlsl_type *type;

    type = hlsl_get_type(ctx->globals, typename, false, false);
    if (!(var = hlsl_new_var(ctx, name, type, loc, NULL, 0, NULL)))
        return false;
    var->scope = scope;
    var->annotations = annotations;

    if (!hlsl_add_var(ctx, var, false))
    {
        struct hlsl_ir_var *old = hlsl_get_var(ctx->cur_scope, var->name);

        hlsl_error(ctx, &var->loc, VKD3D_SHADER_ERROR_HLSL_REDEFINED,
                "Identifier \"%s\" was already declared in this scope.", var->name);
        hlsl_note(ctx, &old->loc, VKD3D_SHADER_LOG_ERROR, "\"%s\" was previously declared here.", old->name);
        hlsl_free_var(var);
        return false;
    }

    return true;
}

static bool add_effect_group(struct hlsl_ctx *ctx, const char *name, struct hlsl_scope *scope,
        struct hlsl_scope *annotations, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_var *var;
    struct hlsl_type *type;

    type = hlsl_get_type(ctx->globals, "fxgroup", false, false);
    if (!(var = hlsl_new_var(ctx, name, type, loc, NULL, 0, NULL)))
        return false;
    var->scope = scope;
    var->annotations = annotations;

    if (!hlsl_add_var(ctx, var, false))
    {
        struct hlsl_ir_var *old = hlsl_get_var(ctx->cur_scope, var->name);

        hlsl_error(ctx, &var->loc, VKD3D_SHADER_ERROR_HLSL_REDEFINED,
                "Identifier \"%s\" was already declared in this scope.", var->name);
        hlsl_note(ctx, &old->loc, VKD3D_SHADER_LOG_ERROR, "\"%s\" was previously declared here.", old->name);
        hlsl_free_var(var);
        return false;
    }

    return true;
}

static bool parse_reservation_index(struct hlsl_ctx *ctx, const char *string, unsigned int bracket_offset,
        struct hlsl_reg_reservation *reservation)
{
    char *endptr;

    reservation->reg_type = ascii_tolower(string[0]);

    /* Prior to SM5.1, fxc simply ignored bracket offsets for 'b' types. */
    if (reservation->reg_type == 'b' && hlsl_version_lt(ctx, 5, 1))
    {
        bracket_offset = 0;
    }

    if (string[1] == '\0')
    {
        reservation->reg_index = bracket_offset;
        return true;
    }

    reservation->reg_index = strtoul(string + 1, &endptr, 10) + bracket_offset;

    if (*endptr)
    {
        /* fxc for SM >= 4 treats all parse failures for 'b' types as successes,
         * setting index to -1. It will later fail while validating slot limits. */
        if (reservation->reg_type == 'b' && hlsl_version_ge(ctx, 4, 0))
        {
            reservation->reg_index = -1;
            return true;
        }

        /* All other types tolerate leftover characters. */
        if (endptr == string + 1)
            return false;
    }

    return true;
}

static bool parse_reservation_space(const char *string, uint32_t *space)
{
    return !ascii_strncasecmp(string, "space", 5) && sscanf(string + 5, "%u", space);
}

static struct hlsl_reg_reservation parse_packoffset(struct hlsl_ctx *ctx, const char *reg_string,
        const char *swizzle, const struct vkd3d_shader_location *loc)
{
    struct hlsl_reg_reservation reservation = {0};
    char *endptr;

    if (hlsl_version_lt(ctx, 4, 0))
        return reservation;

    reservation.offset_index = strtoul(reg_string + 1, &endptr, 10);
    if (*endptr)
    {
        hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_INVALID_RESERVATION,
                "Invalid packoffset() syntax.");
        return reservation;
    }

    reservation.offset_type = ascii_tolower(reg_string[0]);
    if (reservation.offset_type != 'c')
    {
        hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_INVALID_RESERVATION,
                "Only 'c' registers are allowed in packoffset().");
        return reservation;
    }

    reservation.offset_index *= 4;

    if (swizzle)
    {
        if (strlen(swizzle) != 1)
            hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_INVALID_RESERVATION,
                    "Invalid packoffset() component \"%s\".", swizzle);

        if (swizzle[0] == 'x' || swizzle[0] == 'r')
            reservation.offset_index += 0;
        else if (swizzle[0] == 'y' || swizzle[0] == 'g')
            reservation.offset_index += 1;
        else if (swizzle[0] == 'z' || swizzle[0] == 'b')
            reservation.offset_index += 2;
        else if (swizzle[0] == 'w' || swizzle[0] == 'a')
            reservation.offset_index += 3;
        else
            hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_INVALID_RESERVATION,
                    "Invalid packoffset() component \"%s\".", swizzle);
    }

    return reservation;
}

static struct hlsl_block *make_block(struct hlsl_ctx *ctx, struct hlsl_ir_node *instr)
{
    struct hlsl_block *block;

    if (!(block = make_empty_block(ctx)))
    {
        hlsl_free_instr(instr);
        return NULL;
    }
    hlsl_block_add_instr(block, instr);
    return block;
}

static bool expr_compatible_data_types(struct hlsl_type *t1, struct hlsl_type *t2)
{
    /* Scalar vars can be converted to pretty much everything */
    if ((t1->dimx == 1 && t1->dimy == 1) || (t2->dimx == 1 && t2->dimy == 1))
        return true;

    if (t1->class == HLSL_CLASS_VECTOR && t2->class == HLSL_CLASS_VECTOR)
        return true;

    if (t1->class == HLSL_CLASS_MATRIX || t2->class == HLSL_CLASS_MATRIX)
    {
        /* Matrix-vector conversion is apparently allowed if either they have the same components
           count or the matrix is nx1 or 1xn */
        if (t1->class == HLSL_CLASS_VECTOR || t2->class == HLSL_CLASS_VECTOR)
        {
            if (hlsl_type_component_count(t1) == hlsl_type_component_count(t2))
                return true;

            return (t1->class == HLSL_CLASS_MATRIX && (t1->dimx == 1 || t1->dimy == 1))
                    || (t2->class == HLSL_CLASS_MATRIX && (t2->dimx == 1 || t2->dimy == 1));
        }

        /* Both matrices */
        if ((t1->dimx >= t2->dimx && t1->dimy >= t2->dimy)
                || (t1->dimx <= t2->dimx && t1->dimy <= t2->dimy))
            return true;
    }

    return false;
}

static enum hlsl_base_type expr_common_base_type(enum hlsl_base_type t1, enum hlsl_base_type t2)
{
    if (t1 == t2)
        return t1 == HLSL_TYPE_BOOL ? HLSL_TYPE_INT : t1;
    if (t1 == HLSL_TYPE_DOUBLE || t2 == HLSL_TYPE_DOUBLE)
        return HLSL_TYPE_DOUBLE;
    if (t1 == HLSL_TYPE_FLOAT || t2 == HLSL_TYPE_FLOAT
            || t1 == HLSL_TYPE_HALF || t2 == HLSL_TYPE_HALF)
        return HLSL_TYPE_FLOAT;
    if (t1 == HLSL_TYPE_UINT || t2 == HLSL_TYPE_UINT)
        return HLSL_TYPE_UINT;
    return HLSL_TYPE_INT;
}

static bool expr_common_shape(struct hlsl_ctx *ctx, struct hlsl_type *t1, struct hlsl_type *t2,
        const struct vkd3d_shader_location *loc, enum hlsl_type_class *type, unsigned int *dimx, unsigned int *dimy)
{
    if (!hlsl_is_numeric_type(t1))
    {
        struct vkd3d_string_buffer *string;

        if ((string = hlsl_type_to_string(ctx, t1)))
            hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_INVALID_TYPE,
                    "Expression of type \"%s\" cannot be used in a numeric expression.", string->buffer);
        hlsl_release_string_buffer(ctx, string);
        return false;
    }

    if (!hlsl_is_numeric_type(t2))
    {
        struct vkd3d_string_buffer *string;

        if ((string = hlsl_type_to_string(ctx, t2)))
            hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_INVALID_TYPE,
                    "Expression of type \"%s\" cannot be used in a numeric expression.", string->buffer);
        hlsl_release_string_buffer(ctx, string);
        return false;
    }

    if (!expr_compatible_data_types(t1, t2))
    {
        struct vkd3d_string_buffer *t1_string = hlsl_type_to_string(ctx, t1);
        struct vkd3d_string_buffer *t2_string = hlsl_type_to_string(ctx, t2);

        if (t1_string && t2_string)
            hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_INVALID_TYPE,
                    "Expression data types \"%s\" and \"%s\" are incompatible.",
                    t1_string->buffer, t2_string->buffer);
        hlsl_release_string_buffer(ctx, t1_string);
        hlsl_release_string_buffer(ctx, t2_string);
        return false;
    }

    if (t1->dimx == 1 && t1->dimy == 1)
    {
        *type = t2->class;
        *dimx = t2->dimx;
        *dimy = t2->dimy;
    }
    else if (t2->dimx == 1 && t2->dimy == 1)
    {
        *type = t1->class;
        *dimx = t1->dimx;
        *dimy = t1->dimy;
    }
    else if (t1->class == HLSL_CLASS_MATRIX && t2->class == HLSL_CLASS_MATRIX)
    {
        *type = HLSL_CLASS_MATRIX;
        *dimx = min(t1->dimx, t2->dimx);
        *dimy = min(t1->dimy, t2->dimy);
    }
    else
    {
        if (t1->dimx * t1->dimy <= t2->dimx * t2->dimy)
        {
            *type = t1->class;
            *dimx = t1->dimx;
            *dimy = t1->dimy;
        }
        else
        {
            *type = t2->class;
            *dimx = t2->dimx;
            *dimy = t2->dimy;
        }
    }

    return true;
}

static struct hlsl_ir_node *add_expr(struct hlsl_ctx *ctx, struct hlsl_block *block,
        enum hlsl_ir_expr_op op, struct hlsl_ir_node *operands[HLSL_MAX_OPERANDS],
        struct hlsl_type *type, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_node *expr;
    unsigned int i;

    if (type->class == HLSL_CLASS_MATRIX)
    {
        struct hlsl_type *scalar_type;
        struct hlsl_ir_load *var_load;
        struct hlsl_deref var_deref;
        struct hlsl_ir_node *load;
        struct hlsl_ir_var *var;

        scalar_type = hlsl_get_scalar_type(ctx, type->e.numeric.type);

        if (!(var = hlsl_new_synthetic_var(ctx, "split_op", type, loc)))
            return NULL;
        hlsl_init_simple_deref_from_var(&var_deref, var);

        for (i = 0; i < type->dimy * type->dimx; ++i)
        {
            struct hlsl_ir_node *value, *cell_operands[HLSL_MAX_OPERANDS] = { NULL };
            struct hlsl_block store_block;
            unsigned int j;

            for (j = 0; j < HLSL_MAX_OPERANDS; j++)
            {
                if (operands[j])
                {
                    if (!(load = hlsl_add_load_component(ctx, block, operands[j], i, loc)))
                        return NULL;

                    cell_operands[j] = load;
                }
            }

            if (!(value = add_expr(ctx, block, op, cell_operands, scalar_type, loc)))
                return NULL;

            if (!hlsl_new_store_component(ctx, &store_block, &var_deref, i, value))
                return NULL;
            hlsl_block_add_block(block, &store_block);
        }

        if (!(var_load = hlsl_new_var_load(ctx, var, loc)))
            return NULL;
        hlsl_block_add_instr(block, &var_load->node);

        return &var_load->node;
    }

    if (!(expr = hlsl_new_expr(ctx, op, operands, type, loc)))
        return NULL;
    hlsl_block_add_instr(block, expr);

    return expr;
}

static bool type_is_integer(enum hlsl_base_type type)
{
    switch (type)
    {
        case HLSL_TYPE_BOOL:
        case HLSL_TYPE_INT:
        case HLSL_TYPE_UINT:
            return true;

        case HLSL_TYPE_DOUBLE:
        case HLSL_TYPE_FLOAT:
        case HLSL_TYPE_HALF:
            return false;
    }

    vkd3d_unreachable();
}

static void check_integer_type(struct hlsl_ctx *ctx, const struct hlsl_ir_node *instr)
{
    const struct hlsl_type *type = instr->data_type;
    struct vkd3d_string_buffer *string;

    if (type_is_integer(type->e.numeric.type))
        return;

    if ((string = hlsl_type_to_string(ctx, type)))
        hlsl_error(ctx, &instr->loc, VKD3D_SHADER_ERROR_HLSL_INVALID_TYPE,
                "Expression type '%s' is not integer.", string->buffer);
    hlsl_release_string_buffer(ctx, string);
}

static struct hlsl_ir_node *add_unary_arithmetic_expr(struct hlsl_ctx *ctx, struct hlsl_block *block,
        enum hlsl_ir_expr_op op, struct hlsl_ir_node *arg, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_node *args[HLSL_MAX_OPERANDS] = {arg};

    if (arg->data_type->class == HLSL_CLASS_ERROR)
        return arg;

    return add_expr(ctx, block, op, args, arg->data_type, loc);
}

static struct hlsl_ir_node *add_unary_bitwise_expr(struct hlsl_ctx *ctx, struct hlsl_block *block,
        enum hlsl_ir_expr_op op, struct hlsl_ir_node *arg, const struct vkd3d_shader_location *loc)
{
    if (arg->data_type->class == HLSL_CLASS_ERROR)
        return arg;

    check_integer_type(ctx, arg);

    return add_unary_arithmetic_expr(ctx, block, op, arg, loc);
}

static struct hlsl_ir_node *add_unary_logical_expr(struct hlsl_ctx *ctx, struct hlsl_block *block,
        enum hlsl_ir_expr_op op, struct hlsl_ir_node *arg, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_node *args[HLSL_MAX_OPERANDS] = {0};
    struct hlsl_type *bool_type;

    if (arg->data_type->class == HLSL_CLASS_ERROR)
        return arg;

    bool_type = hlsl_get_numeric_type(ctx, arg->data_type->class, HLSL_TYPE_BOOL,
            arg->data_type->dimx, arg->data_type->dimy);

    if (!(args[0] = add_implicit_conversion(ctx, block, arg, bool_type, loc)))
        return NULL;

    return add_expr(ctx, block, op, args, bool_type, loc);
}

static struct hlsl_type *get_common_numeric_type(struct hlsl_ctx *ctx, const struct hlsl_ir_node *arg1,
        const struct hlsl_ir_node *arg2, const struct vkd3d_shader_location *loc)
{
    enum hlsl_type_class type;
    enum hlsl_base_type base;
    unsigned int dimx, dimy;

    if (!expr_common_shape(ctx, arg1->data_type, arg2->data_type, loc, &type, &dimx, &dimy))
        return NULL;
    base = expr_common_base_type(arg1->data_type->e.numeric.type, arg2->data_type->e.numeric.type);
    return hlsl_get_numeric_type(ctx, type, base, dimx, dimy);
}

static struct hlsl_ir_node *add_binary_arithmetic_expr(struct hlsl_ctx *ctx, struct hlsl_block *block,
        enum hlsl_ir_expr_op op, struct hlsl_ir_node *arg1, struct hlsl_ir_node *arg2,
        const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_node *args[HLSL_MAX_OPERANDS] = {0};
    struct hlsl_type *common_type;

    if (!(common_type = get_common_numeric_type(ctx, arg1, arg2, loc)))
    {
        block->value = ctx->error_instr;
        return block->value;
    }

    if (!(args[0] = add_implicit_conversion(ctx, block, arg1, common_type, loc)))
        return NULL;

    if (!(args[1] = add_implicit_conversion(ctx, block, arg2, common_type, loc)))
        return NULL;

    return add_expr(ctx, block, op, args, common_type, loc);
}

static struct hlsl_ir_node *add_binary_bitwise_expr(struct hlsl_ctx *ctx, struct hlsl_block *block,
        enum hlsl_ir_expr_op op, struct hlsl_ir_node *arg1, struct hlsl_ir_node *arg2,
        const struct vkd3d_shader_location *loc)
{
    check_integer_type(ctx, arg1);
    check_integer_type(ctx, arg2);

    return add_binary_arithmetic_expr(ctx, block, op, arg1, arg2, loc);
}

static struct hlsl_ir_node *add_binary_comparison_expr(struct hlsl_ctx *ctx, struct hlsl_block *block,
        enum hlsl_ir_expr_op op, struct hlsl_ir_node *arg1, struct hlsl_ir_node *arg2,
        const struct vkd3d_shader_location *loc)
{
    struct hlsl_type *common_type, *return_type;
    enum hlsl_type_class type;
    enum hlsl_base_type base;
    unsigned int dimx, dimy;
    struct hlsl_ir_node *args[HLSL_MAX_OPERANDS] = {0};

    if (!expr_common_shape(ctx, arg1->data_type, arg2->data_type, loc, &type, &dimx, &dimy))
        return NULL;

    base = expr_common_base_type(arg1->data_type->e.numeric.type, arg2->data_type->e.numeric.type);
    common_type = hlsl_get_numeric_type(ctx, type, base, dimx, dimy);
    return_type = hlsl_get_numeric_type(ctx, type, HLSL_TYPE_BOOL, dimx, dimy);

    if (!(args[0] = add_implicit_conversion(ctx, block, arg1, common_type, loc)))
        return NULL;

    if (!(args[1] = add_implicit_conversion(ctx, block, arg2, common_type, loc)))
        return NULL;

    return add_expr(ctx, block, op, args, return_type, loc);
}

static struct hlsl_ir_node *add_binary_logical_expr(struct hlsl_ctx *ctx, struct hlsl_block *block,
        enum hlsl_ir_expr_op op, struct hlsl_ir_node *arg1, struct hlsl_ir_node *arg2,
        const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_node *args[HLSL_MAX_OPERANDS] = {0};
    struct hlsl_type *common_type;
    enum hlsl_type_class type;
    unsigned int dimx, dimy;

    if (!expr_common_shape(ctx, arg1->data_type, arg2->data_type, loc, &type, &dimx, &dimy))
        return NULL;

    common_type = hlsl_get_numeric_type(ctx, type, HLSL_TYPE_BOOL, dimx, dimy);

    if (!(args[0] = add_implicit_conversion(ctx, block, arg1, common_type, loc)))
        return NULL;

    if (!(args[1] = add_implicit_conversion(ctx, block, arg2, common_type, loc)))
        return NULL;

    return add_expr(ctx, block, op, args, common_type, loc);
}

static struct hlsl_ir_node *add_binary_shift_expr(struct hlsl_ctx *ctx, struct hlsl_block *block,
        enum hlsl_ir_expr_op op, struct hlsl_ir_node *arg1, struct hlsl_ir_node *arg2,
        const struct vkd3d_shader_location *loc)
{
    enum hlsl_base_type base = arg1->data_type->e.numeric.type;
    struct hlsl_ir_node *args[HLSL_MAX_OPERANDS] = {0};
    struct hlsl_type *return_type, *integer_type;
    enum hlsl_type_class type;
    unsigned int dimx, dimy;

    check_integer_type(ctx, arg1);
    check_integer_type(ctx, arg2);

    if (base == HLSL_TYPE_BOOL)
        base = HLSL_TYPE_INT;

    if (!expr_common_shape(ctx, arg1->data_type, arg2->data_type, loc, &type, &dimx, &dimy))
        return NULL;

    return_type = hlsl_get_numeric_type(ctx, type, base, dimx, dimy);
    integer_type = hlsl_get_numeric_type(ctx, type, HLSL_TYPE_INT, dimx, dimy);

    if (!(args[0] = add_implicit_conversion(ctx, block, arg1, return_type, loc)))
        return NULL;

    if (!(args[1] = add_implicit_conversion(ctx, block, arg2, integer_type, loc)))
        return NULL;

    return add_expr(ctx, block, op, args, return_type, loc);
}

static struct hlsl_ir_node *add_binary_dot_expr(struct hlsl_ctx *ctx, struct hlsl_block *instrs,
        struct hlsl_ir_node *arg1, struct hlsl_ir_node *arg2, const struct vkd3d_shader_location *loc)
{
    enum hlsl_base_type base = expr_common_base_type(arg1->data_type->e.numeric.type, arg2->data_type->e.numeric.type);
    struct hlsl_ir_node *args[HLSL_MAX_OPERANDS] = {0};
    struct hlsl_type *common_type, *ret_type;
    enum hlsl_ir_expr_op op;
    unsigned dim;

    if (arg1->data_type->class == HLSL_CLASS_MATRIX)
    {
        struct vkd3d_string_buffer *string;

        if ((string = hlsl_type_to_string(ctx, arg1->data_type)))
            hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_INVALID_TYPE, "Invalid type %s.", string->buffer);
        hlsl_release_string_buffer(ctx, string);
        return NULL;
    }

    if (arg2->data_type->class == HLSL_CLASS_MATRIX)
    {
        struct vkd3d_string_buffer *string;

        if ((string = hlsl_type_to_string(ctx, arg2->data_type)))
            hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_INVALID_TYPE, "Invalid type %s.", string->buffer);
        hlsl_release_string_buffer(ctx, string);
        return NULL;
    }

    if (arg1->data_type->class == HLSL_CLASS_SCALAR)
        dim = arg2->data_type->dimx;
    else if (arg2->data_type->class == HLSL_CLASS_SCALAR)
        dim = arg1->data_type->dimx;
    else
        dim = min(arg1->data_type->dimx, arg2->data_type->dimx);

    if (dim == 1)
        op = HLSL_OP2_MUL;
    else
        op = HLSL_OP2_DOT;

    common_type = hlsl_get_vector_type(ctx, base, dim);
    ret_type = hlsl_get_scalar_type(ctx, base);

    if (!(args[0] = add_implicit_conversion(ctx, instrs, arg1, common_type, loc)))
        return NULL;

    if (!(args[1] = add_implicit_conversion(ctx, instrs, arg2, common_type, loc)))
        return NULL;

    return add_expr(ctx, instrs, op, args, ret_type, loc);
}

static struct hlsl_ir_node *add_binary_expr(struct hlsl_ctx *ctx, struct hlsl_block *block, enum hlsl_ir_expr_op op,
        struct hlsl_ir_node *lhs, struct hlsl_ir_node *rhs, const struct vkd3d_shader_location *loc)
{
    switch (op)
    {
        case HLSL_OP2_ADD:
        case HLSL_OP2_DIV:
        case HLSL_OP2_MOD:
        case HLSL_OP2_MUL:
            return add_binary_arithmetic_expr(ctx, block, op, lhs, rhs, loc);

        case HLSL_OP2_BIT_AND:
        case HLSL_OP2_BIT_OR:
        case HLSL_OP2_BIT_XOR:
            return add_binary_bitwise_expr(ctx, block, op, lhs, rhs, loc);

        case HLSL_OP2_LESS:
        case HLSL_OP2_GEQUAL:
        case HLSL_OP2_EQUAL:
        case HLSL_OP2_NEQUAL:
            return add_binary_comparison_expr(ctx, block, op, lhs, rhs, loc);

        case HLSL_OP2_LOGIC_AND:
        case HLSL_OP2_LOGIC_OR:
            return add_binary_logical_expr(ctx, block, op, lhs, rhs, loc);

        case HLSL_OP2_LSHIFT:
        case HLSL_OP2_RSHIFT:
            return add_binary_shift_expr(ctx, block, op, lhs, rhs, loc);

        default:
            vkd3d_unreachable();
    }
}

static struct hlsl_block *add_binary_expr_merge(struct hlsl_ctx *ctx, struct hlsl_block *block1,
        struct hlsl_block *block2, enum hlsl_ir_expr_op op, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_node *arg1 = node_from_block(block1), *arg2 = node_from_block(block2);

    hlsl_block_add_block(block1, block2);
    destroy_block(block2);

    if (arg1->data_type->class == HLSL_CLASS_ERROR || arg2->data_type->class == HLSL_CLASS_ERROR)
    {
        block1->value = ctx->error_instr;
        return block1;
    }

    if (add_binary_expr(ctx, block1, op, arg1, arg2, loc) == NULL)
        return NULL;

    return block1;
}

static enum hlsl_ir_expr_op op_from_assignment(enum parse_assign_op op)
{
    static const enum hlsl_ir_expr_op ops[] =
    {
        0,
        HLSL_OP2_ADD,
        0,
        HLSL_OP2_MUL,
        HLSL_OP2_DIV,
        HLSL_OP2_MOD,
        HLSL_OP2_LSHIFT,
        HLSL_OP2_RSHIFT,
        HLSL_OP2_BIT_AND,
        HLSL_OP2_BIT_OR,
        HLSL_OP2_BIT_XOR,
    };

    return ops[op];
}

static bool invert_swizzle(uint32_t *swizzle, unsigned int *writemask, unsigned int *ret_width)
{
    unsigned int i, j, bit = 0, inverted = 0, width, new_writemask = 0, new_swizzle = 0;

    /* Apply the writemask to the swizzle to get a new writemask and swizzle. */
    for (i = 0; i < 4; ++i)
    {
        if (*writemask & (1 << i))
        {
            unsigned int s = (*swizzle >> (i * 2)) & 3;
            new_swizzle |= s << (bit++ * 2);
            if (new_writemask & (1 << s))
                return false;
            new_writemask |= 1 << s;
        }
    }
    width = bit;

    /* Invert the swizzle. */
    bit = 0;
    for (i = 0; i < 4; ++i)
    {
        for (j = 0; j < width; ++j)
        {
            unsigned int s = (new_swizzle >> (j * 2)) & 3;
            if (s == i)
                inverted |= j << (bit++ * 2);
        }
    }

    *swizzle = inverted;
    *writemask = new_writemask;
    *ret_width = width;
    return true;
}

static bool invert_swizzle_matrix(uint32_t *swizzle, unsigned int *writemask, unsigned int *ret_width)
{
    /* swizzle is 8 bits per component, each component is (from LSB) 4 bits X, then 4 bits Y.
     * components are indexed by their sources. i.e. the first component comes from the first
     * component of the rhs. */
    unsigned int i, j, bit = 0, inverted = 0, width, new_writemask = 0, new_swizzle = 0;

    /* First, we filter the swizzle to remove components that aren't enabled by writemask. */
    for (i = 0; i < 4; ++i)
    {
        if (*writemask & (1 << i))
        {
            unsigned int s = (*swizzle >> (i * 8)) & 0xff;
            unsigned int x = s & 0xf, y = (s >> 4) & 0xf;
            unsigned int idx = x + y * 4;
            new_swizzle |= s << (bit++ * 8);
            if (new_writemask & (1 << idx))
                return false;
            new_writemask |= 1 << idx;
        }
    }
    width = bit;

    /* Then we invert the swizzle. The resulting swizzle has 2 bits per component, because it's for the
     * incoming vector. */
    bit = 0;
    for (i = 0; i < 16; ++i)
    {
        for (j = 0; j < width; ++j)
        {
            unsigned int s = (new_swizzle >> (j * 8)) & 0xff;
            unsigned int x = s & 0xf, y = (s >> 4) & 0xf;
            unsigned int idx = x + y * 4;
            if (idx == i)
                inverted |= j << (bit++ * 2);
        }
    }

    *swizzle = inverted;
    *writemask = new_writemask;
    *ret_width = width;
    return true;
}

static bool add_assignment(struct hlsl_ctx *ctx, struct hlsl_block *block, struct hlsl_ir_node *lhs,
        enum parse_assign_op assign_op, struct hlsl_ir_node *rhs)
{
    struct hlsl_type *lhs_type = lhs->data_type;
    unsigned int writemask = 0, width = 0;
    bool matrix_writemask = false;

    if (lhs->data_type->class == HLSL_CLASS_ERROR || rhs->data_type->class == HLSL_CLASS_ERROR)
    {
        block->value = ctx->error_instr;
        return true;
    }

    if (assign_op == ASSIGN_OP_SUB)
    {
        if (!(rhs = add_unary_arithmetic_expr(ctx, block, HLSL_OP1_NEG, rhs, &rhs->loc)))
            return false;
        assign_op = ASSIGN_OP_ADD;
    }
    if (assign_op != ASSIGN_OP_ASSIGN)
    {
        enum hlsl_ir_expr_op op = op_from_assignment(assign_op);

        VKD3D_ASSERT(op);
        if (!(rhs = add_binary_expr(ctx, block, op, lhs, rhs, &rhs->loc)))
            return false;
    }

    if (hlsl_is_numeric_type(lhs_type))
    {
        writemask = (1 << lhs_type->dimx) - 1;
        width = lhs_type->dimx;
    }

    if (!(rhs = add_implicit_conversion(ctx, block, rhs, lhs_type, &rhs->loc)))
        return false;

    while (lhs->type != HLSL_IR_LOAD && lhs->type != HLSL_IR_INDEX)
    {
        if (lhs->type == HLSL_IR_EXPR && hlsl_ir_expr(lhs)->op == HLSL_OP1_CAST)
        {
            hlsl_fixme(ctx, &lhs->loc, "Cast on the LHS.");
            return false;
        }
        else if (lhs->type == HLSL_IR_SWIZZLE)
        {
            struct hlsl_ir_swizzle *swizzle = hlsl_ir_swizzle(lhs);
            struct hlsl_ir_node *new_swizzle;
            uint32_t s = swizzle->swizzle;

            VKD3D_ASSERT(!matrix_writemask);

            if (swizzle->val.node->data_type->class == HLSL_CLASS_MATRIX)
            {
                if (swizzle->val.node->type != HLSL_IR_LOAD && swizzle->val.node->type != HLSL_IR_INDEX)
                {
                    hlsl_fixme(ctx, &lhs->loc, "Unhandled source of matrix swizzle.");
                    return false;
                }
                if (!invert_swizzle_matrix(&s, &writemask, &width))
                {
                    hlsl_error(ctx, &lhs->loc, VKD3D_SHADER_ERROR_HLSL_INVALID_WRITEMASK, "Invalid writemask for matrix.");
                    return false;
                }
                matrix_writemask = true;
            }
            else if (!invert_swizzle(&s, &writemask, &width))
            {
                hlsl_error(ctx, &lhs->loc, VKD3D_SHADER_ERROR_HLSL_INVALID_WRITEMASK, "Invalid writemask.");
                return false;
            }

            if (!(new_swizzle = hlsl_new_swizzle(ctx, s, width, rhs, &swizzle->node.loc)))
                return false;
            hlsl_block_add_instr(block, new_swizzle);

            lhs = swizzle->val.node;
            rhs = new_swizzle;
        }
        else
        {
            hlsl_error(ctx, &lhs->loc, VKD3D_SHADER_ERROR_HLSL_INVALID_LVALUE, "Invalid lvalue.");
            return false;
        }
    }

    if (lhs->type == HLSL_IR_INDEX && hlsl_index_chain_has_resource_access(hlsl_ir_index(lhs)))
    {
        struct hlsl_ir_node *coords = hlsl_ir_index(lhs)->idx.node;
        struct hlsl_deref resource_deref;
        struct hlsl_type *resource_type;
        struct hlsl_ir_node *store;
        unsigned int dim_count;

        if (!hlsl_index_is_resource_access(hlsl_ir_index(lhs)))
        {
            hlsl_fixme(ctx, &lhs->loc, "Non-direct structured resource store.");
            return false;
        }

        if (!hlsl_init_deref_from_index_chain(ctx, &resource_deref, hlsl_ir_index(lhs)->val.node))
            return false;

        resource_type = hlsl_deref_get_type(ctx, &resource_deref);
        VKD3D_ASSERT(resource_type->class == HLSL_CLASS_TEXTURE || resource_type->class == HLSL_CLASS_UAV);

        if (resource_type->class != HLSL_CLASS_UAV)
            hlsl_error(ctx, &lhs->loc, VKD3D_SHADER_ERROR_HLSL_INVALID_TYPE,
                    "Read-only resources cannot be stored to.");

        dim_count = hlsl_sampler_dim_count(resource_type->sampler_dim);

        if (width != resource_type->e.resource.format->dimx * resource_type->e.resource.format->dimy)
            hlsl_error(ctx, &lhs->loc, VKD3D_SHADER_ERROR_HLSL_INVALID_WRITEMASK,
                    "Resource store expressions must write to all components.");

        VKD3D_ASSERT(coords->data_type->class == HLSL_CLASS_VECTOR);
        VKD3D_ASSERT(coords->data_type->e.numeric.type == HLSL_TYPE_UINT);
        VKD3D_ASSERT(coords->data_type->dimx == dim_count);

        if (!(store = hlsl_new_resource_store(ctx, &resource_deref, coords, rhs, &lhs->loc)))
        {
            hlsl_cleanup_deref(&resource_deref);
            return false;
        }
        hlsl_block_add_instr(block, store);
        hlsl_cleanup_deref(&resource_deref);
    }
    else if (matrix_writemask)
    {
        struct hlsl_deref deref;
        unsigned int i, j, k = 0;

        hlsl_init_deref_from_index_chain(ctx, &deref, lhs);

        for (i = 0; i < lhs->data_type->dimy; ++i)
        {
            for (j = 0; j < lhs->data_type->dimx; ++j)
            {
                struct hlsl_ir_node *load;
                struct hlsl_block store_block;
                const unsigned int idx = i * 4 + j;
                const unsigned int component = i * lhs->data_type->dimx + j;

                if (!(writemask & (1 << idx)))
                    continue;

                if (!(load = hlsl_add_load_component(ctx, block, rhs, k++, &rhs->loc)))
                {
                    hlsl_cleanup_deref(&deref);
                    return false;
                }

                if (!hlsl_new_store_component(ctx, &store_block, &deref, component, load))
                {
                    hlsl_cleanup_deref(&deref);
                    return false;
                }
                hlsl_block_add_block(block, &store_block);
            }
        }

        hlsl_cleanup_deref(&deref);
    }
    else if (lhs->type == HLSL_IR_INDEX && hlsl_index_is_noncontiguous(hlsl_ir_index(lhs)))
    {
        struct hlsl_ir_index *row = hlsl_ir_index(lhs);
        struct hlsl_ir_node *mat = row->val.node;
        unsigned int i, k = 0;

        VKD3D_ASSERT(!matrix_writemask);

        for (i = 0; i < mat->data_type->dimx; ++i)
        {
            struct hlsl_ir_node *cell, *load, *store, *c;
            struct hlsl_deref deref;

            if (!(writemask & (1 << i)))
                continue;

            if (!(c = hlsl_new_uint_constant(ctx, i, &lhs->loc)))
                return false;
            hlsl_block_add_instr(block, c);

            if (!(cell = hlsl_new_index(ctx, &row->node, c, &lhs->loc)))
                return false;
            hlsl_block_add_instr(block, cell);

            if (!(load = hlsl_add_load_component(ctx, block, rhs, k++, &rhs->loc)))
                return false;

            if (!hlsl_init_deref_from_index_chain(ctx, &deref, cell))
                return false;

            if (!(store = hlsl_new_store_index(ctx, &deref, NULL, load, 0, &rhs->loc)))
            {
                hlsl_cleanup_deref(&deref);
                return false;
            }
            hlsl_block_add_instr(block, store);
            hlsl_cleanup_deref(&deref);
        }
    }
    else
    {
        struct hlsl_ir_node *store;
        struct hlsl_deref deref;

        if (!hlsl_init_deref_from_index_chain(ctx, &deref, lhs))
            return false;

        if (!(store = hlsl_new_store_index(ctx, &deref, NULL, rhs, writemask, &rhs->loc)))
        {
            hlsl_cleanup_deref(&deref);
            return false;
        }
        hlsl_block_add_instr(block, store);
        hlsl_cleanup_deref(&deref);
    }

    block->value = rhs;
    return true;
}

static bool add_increment(struct hlsl_ctx *ctx, struct hlsl_block *block, bool decrement, bool post,
        const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_node *lhs = node_from_block(block);
    struct hlsl_ir_node *one;

    if (lhs->data_type->class == HLSL_CLASS_ERROR)
        return true;

    if (lhs->data_type->modifiers & HLSL_MODIFIER_CONST)
        hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_MODIFIES_CONST,
                "Argument to %s%screment operator is const.", post ? "post" : "pre", decrement ? "de" : "in");

    if (!(one = hlsl_new_int_constant(ctx, 1, loc)))
        return false;
    hlsl_block_add_instr(block, one);

    if (!add_assignment(ctx, block, lhs, decrement ? ASSIGN_OP_SUB : ASSIGN_OP_ADD, one))
        return false;

    if (post)
    {
        struct hlsl_ir_node *copy;

        if (!(copy = hlsl_new_copy(ctx, lhs)))
            return false;
        hlsl_block_add_instr(block, copy);

        /* Post increment/decrement expressions are considered const. */
        if (!(copy->data_type = hlsl_type_clone(ctx, copy->data_type, 0, HLSL_MODIFIER_CONST)))
            return false;
    }

    return true;
}

static void initialize_var_components(struct hlsl_ctx *ctx, struct hlsl_block *instrs,
        struct hlsl_ir_var *dst, unsigned int *store_index, struct hlsl_ir_node *src,
        bool is_default_values_initializer)
{
    unsigned int src_comp_count = hlsl_type_component_count(src->data_type);
    struct hlsl_deref dst_deref;
    unsigned int k;

    hlsl_init_simple_deref_from_var(&dst_deref, dst);

    for (k = 0; k < src_comp_count; ++k)
    {
        struct hlsl_ir_node *conv, *load;
        struct hlsl_type *dst_comp_type;
        struct hlsl_block block;

        if (!(load = hlsl_add_load_component(ctx, instrs, src, k, &src->loc)))
            return;

        dst_comp_type = hlsl_type_get_component_type(ctx, dst->data_type, *store_index);

        if (is_default_values_initializer)
        {
            struct hlsl_default_value default_value = {0};

            if (src->type == HLSL_IR_COMPILE || src->type == HLSL_IR_SAMPLER_STATE)
            {
                if (hlsl_is_numeric_type(dst_comp_type))
                {
                    /* Default values are discarded if they contain an object
                     * literal expression for a numeric component. */
                    if (dst->default_values)
                    {
                        hlsl_warning(ctx, &src->loc, VKD3D_SHADER_WARNING_HLSL_IGNORED_DEFAULT_VALUE,
                                "Component %u in variable '%s' initializer is object literal. Default values discarded.",
                                k, dst->name);
                        vkd3d_free(dst->default_values);
                        dst->default_values = NULL;
                    }
                }
            }
            else
            {
                if (!hlsl_clone_block(ctx, &block, instrs))
                    return;
                default_value = evaluate_static_expression(ctx, &block, dst_comp_type, &src->loc);

                if (dst->default_values)
                    dst->default_values[*store_index] = default_value;

                hlsl_block_cleanup(&block);
            }
        }
        else
        {
            if (src->type == HLSL_IR_SAMPLER_STATE)
            {
                /* Sampler states end up in the variable's state_blocks instead of
                 * being used to initialize its value. */
                struct hlsl_ir_sampler_state *sampler_state = hlsl_ir_sampler_state(src);

                if (dst_comp_type->class != HLSL_CLASS_SAMPLER)
                {
                    struct vkd3d_string_buffer *dst_string;

                    dst_string = hlsl_type_to_string(ctx, dst_comp_type);
                    if (dst_string)
                        hlsl_error(ctx, &src->loc, VKD3D_SHADER_ERROR_HLSL_INVALID_TYPE,
                                "Cannot assign sampler_state to %s.", dst_string->buffer);
                    hlsl_release_string_buffer(ctx, dst_string);
                    return;
                }

                if (!hlsl_array_reserve(ctx, (void **)&dst->state_blocks, &dst->state_block_capacity,
                        dst->state_block_count + 1, sizeof(*dst->state_blocks)))
                    return;

                dst->state_blocks[dst->state_block_count] = sampler_state->state_block;
                sampler_state->state_block = NULL;
                ++dst->state_block_count;
            }
            else
            {
                if (!(conv = add_implicit_conversion(ctx, instrs, load, dst_comp_type, &src->loc)))
                    return;

                if (!hlsl_new_store_component(ctx, &block, &dst_deref, *store_index, conv))
                    return;
                hlsl_block_add_block(instrs, &block);
            }
        }

        ++*store_index;
    }
}

static void initialize_var(struct hlsl_ctx *ctx, struct hlsl_ir_var *dst,
        const struct parse_initializer *initializer, bool is_default_values_initializer)
{
    unsigned int store_index = 0;

    /* If any of the elements has an error type, then initializer_size() is not
     * meaningful. */
    for (unsigned int i = 0; i < initializer->args_count; ++i)
    {
        if (initializer->args[i]->data_type->class == HLSL_CLASS_ERROR)
            return;
    }

    if (initializer_size(initializer) != hlsl_type_component_count(dst->data_type))
    {
        hlsl_error(ctx, &initializer->loc, VKD3D_SHADER_ERROR_HLSL_WRONG_PARAMETER_COUNT,
                "Expected %u components in initializer, but got %u.",
                hlsl_type_component_count(dst->data_type), initializer_size(initializer));
        return;
    }

    for (unsigned int i = 0; i < initializer->args_count; ++i)
        initialize_var_components(ctx, initializer->instrs, dst, &store_index,
                initializer->args[i], is_default_values_initializer);
}

static bool type_has_object_components(const struct hlsl_type *type)
{
    if (type->class == HLSL_CLASS_ARRAY)
        return type_has_object_components(type->e.array.type);

    if (type->class == HLSL_CLASS_STRUCT)
    {
        for (unsigned int i = 0; i < type->e.record.field_count; ++i)
        {
            if (type_has_object_components(type->e.record.fields[i].type))
                return true;
        }

        return false;
    }

    return !hlsl_is_numeric_type(type);
}

static bool type_has_numeric_components(struct hlsl_type *type)
{
    if (hlsl_is_numeric_type(type))
        return true;
    if (type->class == HLSL_CLASS_ARRAY)
        return type_has_numeric_components(type->e.array.type);

    if (type->class == HLSL_CLASS_STRUCT)
    {
        unsigned int i;

        for (i = 0; i < type->e.record.field_count; ++i)
        {
            if (type_has_numeric_components(type->e.record.fields[i].type))
                return true;
        }
    }
    return false;
}

static void check_invalid_in_out_modifiers(struct hlsl_ctx *ctx, unsigned int modifiers,
        const struct vkd3d_shader_location *loc)
{
    modifiers &= (HLSL_STORAGE_IN | HLSL_STORAGE_OUT);
    if (modifiers)
    {
        struct vkd3d_string_buffer *string;

        if ((string = hlsl_modifiers_to_string(ctx, modifiers)))
            hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_INVALID_MODIFIER,
                    "Modifiers '%s' are not allowed on non-parameter variables.", string->buffer);
        hlsl_release_string_buffer(ctx, string);
    }
}

static void check_invalid_object_fields(struct hlsl_ctx *ctx, const struct hlsl_ir_var *var)
{
    const struct hlsl_type *type = var->data_type;

    while (type->class == HLSL_CLASS_ARRAY)
        type = type->e.array.type;

    if (type->class == HLSL_CLASS_STRUCT && type_has_object_components(type))
        hlsl_error(ctx, &var->loc, VKD3D_SHADER_ERROR_HLSL_INVALID_TYPE,
                "Target profile doesn't support objects as struct members in uniform variables.");
}

static void declare_var(struct hlsl_ctx *ctx, struct parse_variable_def *v)
{
    struct hlsl_type *basic_type = v->basic_type;
    struct hlsl_ir_function_decl *func;
    struct hlsl_semantic new_semantic;
    uint32_t modifiers = v->modifiers;
    bool unbounded_res_array = false;
    bool constant_buffer = false;
    struct hlsl_ir_var *var;
    struct hlsl_type *type;
    bool local = true;
    char *var_name;
    unsigned int i;

    VKD3D_ASSERT(basic_type);

    if (basic_type->class == HLSL_CLASS_MATRIX)
        VKD3D_ASSERT(basic_type->modifiers & HLSL_MODIFIERS_MAJORITY_MASK);

    type = basic_type;

    if (hlsl_version_ge(ctx, 5, 1) && hlsl_type_is_resource(type))
    {
        for (i = 0; i < v->arrays.count; ++i)
            unbounded_res_array |= (v->arrays.sizes[i] == HLSL_ARRAY_ELEMENTS_COUNT_IMPLICIT);
    }

    if (type->class == HLSL_CLASS_CONSTANT_BUFFER)
    {
        type = type->e.resource.format;
        constant_buffer = true;
    }

    if (unbounded_res_array)
    {
        if (v->arrays.count == 1)
        {
            hlsl_fixme(ctx, &v->loc, "Unbounded resource arrays.");
            return;
        }
        else
        {
            hlsl_error(ctx, &v->loc, VKD3D_SHADER_ERROR_HLSL_INVALID_TYPE,
                    "Unbounded resource arrays cannot be multi-dimensional.");
        }
    }
    else
    {
        for (i = 0; i < v->arrays.count; ++i)
        {
            if (v->arrays.sizes[i] == HLSL_ARRAY_ELEMENTS_COUNT_IMPLICIT)
            {
                unsigned int size = initializer_size(&v->initializer);
                unsigned int elem_components = hlsl_type_component_count(type);

                if (i < v->arrays.count - 1)
                {
                    hlsl_error(ctx, &v->loc, VKD3D_SHADER_ERROR_HLSL_INVALID_TYPE,
                            "Only innermost array size can be implicit.");
                    v->initializer.args_count = 0;
                }
                else if (elem_components == 0)
                {
                    hlsl_error(ctx, &v->loc, VKD3D_SHADER_ERROR_HLSL_INVALID_TYPE,
                            "Cannot declare an implicit size array of a size 0 type.");
                    v->initializer.args_count = 0;
                }
                else if (size == 0)
                {
                    hlsl_error(ctx, &v->loc, VKD3D_SHADER_ERROR_HLSL_INVALID_TYPE,
                            "Implicit size arrays need to be initialized.");
                    v->initializer.args_count = 0;
                }
                else if (size % elem_components != 0)
                {
                    hlsl_error(ctx, &v->loc, VKD3D_SHADER_ERROR_HLSL_WRONG_PARAMETER_COUNT,
                            "Cannot initialize implicit size array with %u components, expected a multiple of %u.",
                            size, elem_components);
                    v->initializer.args_count = 0;
                }
                else
                {
                    v->arrays.sizes[i] = size / elem_components;
                }
            }
            type = hlsl_new_array_type(ctx, type, v->arrays.sizes[i]);
        }
    }

    if (hlsl_version_ge(ctx, 5, 1) && type->class == HLSL_CLASS_ARRAY && hlsl_type_is_resource(type))
    {
        /* SM 5.1/6.x descriptor arrays act differently from previous versions.
         * Not only are they treated as a single object in reflection, but they
         * act as a single component for the purposes of assignment and
         * initialization. */
        hlsl_fixme(ctx, &v->loc, "Shader model 5.1+ resource array.");
    }

    if (!(var_name = vkd3d_strdup(v->name)))
        return;

    if (!hlsl_clone_semantic(ctx, &new_semantic, &v->semantic))
    {
        vkd3d_free(var_name);
        return;
    }

    if (!(var = hlsl_new_var(ctx, var_name, type, &v->loc, &new_semantic, modifiers, &v->reg_reservation)))
    {
        hlsl_cleanup_semantic(&new_semantic);
        vkd3d_free(var_name);
        return;
    }

    var->annotations = v->annotations;

    if (constant_buffer && ctx->cur_scope == ctx->globals)
    {
        if (!(var_name = vkd3d_strdup(v->name)))
            return;
        var->buffer = hlsl_new_buffer(ctx, HLSL_BUFFER_CONSTANT, var_name, modifiers, &v->reg_reservation, NULL, &v->loc);
    }
    else
    {
        var->buffer = ctx->cur_buffer;
    }

    if (var->buffer == ctx->globals_buffer)
    {
        if (var->reg_reservation.offset_type)
            hlsl_error(ctx, &var->loc, VKD3D_SHADER_ERROR_HLSL_INVALID_RESERVATION,
                    "packoffset() is only allowed inside constant buffer declarations.");
    }

    if (ctx->cur_scope == ctx->globals)
    {
        local = false;

        if ((modifiers & HLSL_STORAGE_UNIFORM) && (modifiers & HLSL_STORAGE_STATIC))
            hlsl_error(ctx, &var->loc, VKD3D_SHADER_ERROR_HLSL_INVALID_MODIFIER,
                    "Variable '%s' is declared as both \"uniform\" and \"static\".", var->name);

        /* Mark it as uniform. We need to do this here since synthetic
            * variables also get put in the global scope, but shouldn't be
            * considered uniforms, and we have no way of telling otherwise. */
        if (!(modifiers & HLSL_STORAGE_STATIC))
            var->storage_modifiers |= HLSL_STORAGE_UNIFORM;

        if ((ctx->profile->major_version < 5 || ctx->profile->type == VKD3D_SHADER_TYPE_EFFECT)
                && (var->storage_modifiers & HLSL_STORAGE_UNIFORM))
        {
            check_invalid_object_fields(ctx, var);
        }

        if ((func = hlsl_get_first_func_decl(ctx, var->name)))
        {
            hlsl_error(ctx, &var->loc, VKD3D_SHADER_ERROR_HLSL_REDEFINED,
                    "'%s' is already defined as a function.", var->name);
            hlsl_note(ctx, &func->loc, VKD3D_SHADER_LOG_ERROR,
                    "'%s' was previously defined here.", var->name);
        }
    }
    else
    {
        static const unsigned int invalid = HLSL_STORAGE_EXTERN | HLSL_STORAGE_SHARED
                | HLSL_STORAGE_GROUPSHARED | HLSL_STORAGE_UNIFORM;

        if (modifiers & invalid)
        {
            struct vkd3d_string_buffer *string;

            if ((string = hlsl_modifiers_to_string(ctx, modifiers & invalid)))
                hlsl_error(ctx, &var->loc, VKD3D_SHADER_ERROR_HLSL_INVALID_MODIFIER,
                        "Modifiers '%s' are not allowed on local variables.", string->buffer);
            hlsl_release_string_buffer(ctx, string);
        }
        if (var->semantic.name)
            hlsl_error(ctx, &var->loc, VKD3D_SHADER_ERROR_HLSL_INVALID_SEMANTIC,
                    "Semantics are not allowed on local variables.");

        if ((type->modifiers & HLSL_MODIFIER_CONST) && !v->initializer.args_count && !(modifiers & HLSL_STORAGE_STATIC))
        {
            hlsl_error(ctx, &v->loc, VKD3D_SHADER_ERROR_HLSL_MISSING_INITIALIZER,
                "Const variable \"%s\" is missing an initializer.", var->name);
        }

        if (var->annotations)
        {
            hlsl_error(ctx, &v->loc, VKD3D_SHADER_ERROR_HLSL_INVALID_SYNTAX,
                    "Annotations are only allowed for objects in the global scope.");
        }
    }

    if ((var->storage_modifiers & HLSL_STORAGE_STATIC) && type_has_numeric_components(var->data_type)
            && type_has_object_components(var->data_type))
    {
        hlsl_error(ctx, &var->loc, VKD3D_SHADER_ERROR_HLSL_INVALID_TYPE,
                "Static variables cannot have both numeric and resource components.");
    }

    if (!hlsl_add_var(ctx, var, local))
    {
        struct hlsl_ir_var *old = hlsl_get_var(ctx->cur_scope, var->name);

        hlsl_error(ctx, &var->loc, VKD3D_SHADER_ERROR_HLSL_REDEFINED,
                "Variable \"%s\" was already declared in this scope.", var->name);
        hlsl_note(ctx, &old->loc, VKD3D_SHADER_LOG_ERROR, "\"%s\" was previously declared here.", old->name);
        hlsl_free_var(var);
        return;
    }
}

static struct hlsl_block *initialize_vars(struct hlsl_ctx *ctx, struct list *var_list)
{
    struct parse_variable_def *v, *v_next;
    struct hlsl_block *initializers;
    unsigned int component_count;
    struct hlsl_ir_var *var;
    struct hlsl_type *type;

    if (!(initializers = make_empty_block(ctx)))
    {
        LIST_FOR_EACH_ENTRY_SAFE(v, v_next, var_list, struct parse_variable_def, entry)
        {
            free_parse_variable_def(v);
        }
        vkd3d_free(var_list);
        return NULL;
    }

    LIST_FOR_EACH_ENTRY_SAFE(v, v_next, var_list, struct parse_variable_def, entry)
    {
        /* If this fails, the variable failed to be declared. */
        if (!(var = hlsl_get_var(ctx->cur_scope, v->name)))
        {
            free_parse_variable_def(v);
            continue;
        }

        type = var->data_type;
        component_count = hlsl_type_component_count(type);

        var->state_blocks = v->state_blocks;
        var->state_block_count = v->state_block_count;
        var->state_block_capacity = v->state_block_capacity;
        v->state_block_count = 0;
        v->state_block_capacity = 0;
        v->state_blocks = NULL;

        if (var->state_blocks && component_count != var->state_block_count)
        {
            hlsl_error(ctx, &v->loc, VKD3D_SHADER_ERROR_HLSL_WRONG_PARAMETER_COUNT,
                    "Expected %u state blocks, but got %u.", component_count, var->state_block_count);
            free_parse_variable_def(v);
            continue;
        }

        if (v->initializer.args_count)
        {
            bool is_default_values_initializer;

            is_default_values_initializer = (ctx->cur_buffer != ctx->globals_buffer)
                    || (var->storage_modifiers & HLSL_STORAGE_UNIFORM)
                    || ctx->cur_scope->annotations;
            if (hlsl_get_multiarray_element_type(type)->class == HLSL_CLASS_SAMPLER)
                is_default_values_initializer = false;
            if (hlsl_type_is_shader(type))
                is_default_values_initializer = false;

            if (is_default_values_initializer)
            {
                /* Default values might have been allocated already for another variable of the same name,
                   in the same scope. */
                if (var->default_values)
                {
                    free_parse_variable_def(v);
                    continue;
                }

                if (!(var->default_values = hlsl_calloc(ctx, component_count, sizeof(*var->default_values))))
                {
                    free_parse_variable_def(v);
                    continue;
                }
            }

            if (!v->initializer.braces)
            {
                if (!(add_implicit_conversion(ctx, v->initializer.instrs, v->initializer.args[0], type, &v->loc)))
                {
                    free_parse_variable_def(v);
                    continue;
                }

                v->initializer.args[0] = node_from_block(v->initializer.instrs);
            }

            initialize_var(ctx, var, &v->initializer, is_default_values_initializer);

            if (is_default_values_initializer)
            {
                hlsl_dump_var_default_values(var);
            }
            else if (var->storage_modifiers & HLSL_STORAGE_STATIC)
            {
                hlsl_block_add_block(&ctx->static_initializers, v->initializer.instrs);
            }
            else
            {
                hlsl_block_add_block(initializers, v->initializer.instrs);
            }

            if (var->state_blocks)
                TRACE("Variable %s has %u state blocks.\n", var->name, var->state_block_count);
        }
        else if (var->storage_modifiers & HLSL_STORAGE_STATIC)
        {
            struct hlsl_ir_node *cast, *store, *zero;

            /* Initialize statics to zero by default. */

            if (type_has_object_components(var->data_type))
            {
                free_parse_variable_def(v);
                continue;
            }

            if (!(zero = hlsl_new_uint_constant(ctx, 0, &var->loc)))
            {
                free_parse_variable_def(v);
                continue;
            }
            hlsl_block_add_instr(&ctx->static_initializers, zero);

            if (!(cast = add_cast(ctx, &ctx->static_initializers, zero, var->data_type, &var->loc)))
            {
                free_parse_variable_def(v);
                continue;
            }

            if (!(store = hlsl_new_simple_store(ctx, var, cast)))
            {
                free_parse_variable_def(v);
                continue;
            }
            hlsl_block_add_instr(&ctx->static_initializers, store);
        }
        free_parse_variable_def(v);
    }

    vkd3d_free(var_list);
    return initializers;
}

static bool func_is_compatible_match(struct hlsl_ctx *ctx, const struct hlsl_ir_function_decl *decl,
        bool is_compile, const struct parse_initializer *args)
{
    unsigned int i, k;

    k = 0;
    for (i = 0; i < decl->parameters.count; ++i)
    {
        if (is_compile && !(decl->parameters.vars[i]->storage_modifiers & HLSL_STORAGE_UNIFORM))
            continue;

        if (k >= args->args_count)
        {
            if (!decl->parameters.vars[i]->default_values)
                return false;
            return true;
        }

        if (!implicit_compatible_data_types(ctx, args->args[k]->data_type, decl->parameters.vars[i]->data_type))
            return false;

        ++k;
    }
    if (k < args->args_count)
        return false;
    return true;
}

static struct hlsl_ir_function_decl *find_function_call(struct hlsl_ctx *ctx,
        const char *name, const struct parse_initializer *args, bool is_compile,
        const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_function_decl *decl, *compatible_match = NULL;
    struct hlsl_ir_function *func;
    struct rb_entry *entry;

    if (!(entry = rb_get(&ctx->functions, name)))
        return NULL;
    func = RB_ENTRY_VALUE(entry, struct hlsl_ir_function, entry);

    LIST_FOR_EACH_ENTRY(decl, &func->overloads, struct hlsl_ir_function_decl, entry)
    {
        if (func_is_compatible_match(ctx, decl, is_compile, args))
        {
            if (compatible_match)
            {
                hlsl_fixme(ctx, loc, "Prioritize between multiple compatible function overloads.");
                break;
            }
            compatible_match = decl;
        }
    }

    return compatible_match;
}

static struct hlsl_ir_node *hlsl_new_void_expr(struct hlsl_ctx *ctx, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_node *operands[HLSL_MAX_OPERANDS] = {0};

    return hlsl_new_expr(ctx, HLSL_OP0_VOID, operands, ctx->builtin_types.Void, loc);
}

static struct hlsl_ir_node *add_user_call(struct hlsl_ctx *ctx,
        struct hlsl_ir_function_decl *func, const struct parse_initializer *args,
        bool is_compile, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_node *call;
    unsigned int i, j, k;

    VKD3D_ASSERT(args->args_count <= func->parameters.count);

    k = 0;
    for (i = 0; i < func->parameters.count; ++i)
    {
        struct hlsl_ir_var *param = func->parameters.vars[i];
        struct hlsl_ir_node *arg;

        if (is_compile && !(param->storage_modifiers & HLSL_STORAGE_UNIFORM))
            continue;

        if (k >= args->args_count)
            break;
        arg = args->args[k];

        if (!hlsl_types_are_equal(arg->data_type, param->data_type))
        {
            struct hlsl_ir_node *cast;

            if (!(cast = add_cast(ctx, args->instrs, arg, param->data_type, &arg->loc)))
                return NULL;
            args->args[k] = cast;
            arg = cast;
        }

        if (param->storage_modifiers & HLSL_STORAGE_IN)
        {
            struct hlsl_ir_node *store;

            if (!(store = hlsl_new_simple_store(ctx, param, arg)))
                return NULL;
            hlsl_block_add_instr(args->instrs, store);
        }

        ++k;
    }

    /* Add default values for the remaining parameters. */
    for (; i < func->parameters.count; ++i)
    {
        struct hlsl_ir_var *param = func->parameters.vars[i];
        unsigned int comp_count = hlsl_type_component_count(param->data_type);
        struct hlsl_deref param_deref;

        VKD3D_ASSERT(param->default_values);

        if (is_compile && !(param->storage_modifiers & HLSL_STORAGE_UNIFORM))
            continue;

        hlsl_init_simple_deref_from_var(&param_deref, param);

        for (j = 0; j < comp_count; ++j)
        {
            struct hlsl_type *type = hlsl_type_get_component_type(ctx, param->data_type, j);
            struct hlsl_constant_value value;
            struct hlsl_ir_node *comp;
            struct hlsl_block store_block;

            if (!param->default_values[j].string)
            {
                value.u[0] = param->default_values[j].number;
                if (!(comp = hlsl_new_constant(ctx, type, &value, loc)))
                    return NULL;
                hlsl_block_add_instr(args->instrs, comp);

                if (!hlsl_new_store_component(ctx, &store_block, &param_deref, j, comp))
                    return NULL;
                hlsl_block_add_block(args->instrs, &store_block);
            }
        }
    }

    if (!(call = hlsl_new_call(ctx, func, loc)))
        return NULL;
    hlsl_block_add_instr(args->instrs, call);

    if (is_compile)
        return call;

    for (i = 0; i < args->args_count; ++i)
    {
        struct hlsl_ir_var *param = func->parameters.vars[i];
        struct hlsl_ir_node *arg = args->args[i];

        if (param->storage_modifiers & HLSL_STORAGE_OUT)
        {
            struct hlsl_ir_load *load;

            if (arg->data_type->modifiers & HLSL_MODIFIER_CONST)
                hlsl_error(ctx, &arg->loc, VKD3D_SHADER_ERROR_HLSL_MODIFIES_CONST,
                        "Output argument to \"%s\" is const.", func->func->name);

            if (!(load = hlsl_new_var_load(ctx, param, &arg->loc)))
                return NULL;
            hlsl_block_add_instr(args->instrs, &load->node);

            if (!add_assignment(ctx, args->instrs, arg, ASSIGN_OP_ASSIGN, &load->node))
                return NULL;
        }
    }

    if (func->return_var)
    {
        struct hlsl_ir_load *load;

        if (!(load = hlsl_new_var_load(ctx, func->return_var, loc)))
            return false;
        hlsl_block_add_instr(args->instrs, &load->node);
    }
    else
    {
        struct hlsl_ir_node *expr;

        if (!(expr = hlsl_new_void_expr(ctx, loc)))
            return false;
        hlsl_block_add_instr(args->instrs, expr);
    }

    return call;
}

static struct hlsl_ir_node *intrinsic_float_convert_arg(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, struct hlsl_ir_node *arg, const struct vkd3d_shader_location *loc)
{
    struct hlsl_type *type = arg->data_type;

    if (!type_is_integer(type->e.numeric.type))
        return arg;

    type = hlsl_get_numeric_type(ctx, type->class, HLSL_TYPE_FLOAT, type->dimx, type->dimy);
    return add_implicit_conversion(ctx, params->instrs, arg, type, loc);
}

static bool convert_args(struct hlsl_ctx *ctx, const struct parse_initializer *params,
        struct hlsl_type *type, const struct vkd3d_shader_location *loc)
{
    unsigned int i;

    for (i = 0; i < params->args_count; ++i)
    {
        struct hlsl_ir_node *new_arg;

        if (!(new_arg = add_implicit_conversion(ctx, params->instrs, params->args[i], type, loc)))
            return false;
        params->args[i] = new_arg;
    }

    return true;
}

static struct hlsl_type *elementwise_intrinsic_get_common_type(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    enum hlsl_base_type base = params->args[0]->data_type->e.numeric.type;
    bool vectors = false, matrices = false;
    unsigned int dimx = 4, dimy = 4;
    struct hlsl_type *common_type;
    unsigned int i;

    for (i = 0; i < params->args_count; ++i)
    {
        struct hlsl_type *arg_type = params->args[i]->data_type;

        base = expr_common_base_type(base, arg_type->e.numeric.type);

        if (arg_type->class == HLSL_CLASS_VECTOR)
        {
            vectors = true;
            dimx = min(dimx, arg_type->dimx);
        }
        else if (arg_type->class == HLSL_CLASS_MATRIX)
        {
            matrices = true;
            dimx = min(dimx, arg_type->dimx);
            dimy = min(dimy, arg_type->dimy);
        }
    }

    if (matrices && vectors)
    {
        hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_INVALID_TYPE,
                "Cannot use both matrices and vectors in an elementwise intrinsic.");
        return NULL;
    }
    else if (matrices)
    {
        common_type = hlsl_get_matrix_type(ctx, base, dimx, dimy);
    }
    else if (vectors)
    {
        common_type = hlsl_get_vector_type(ctx, base, dimx);
    }
    else
    {
        common_type = hlsl_get_scalar_type(ctx, base);
    }

    return common_type;
}

static bool elementwise_intrinsic_convert_args(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    struct hlsl_type *common_type;

    if (!(common_type = elementwise_intrinsic_get_common_type(ctx, params, loc)))
        return false;

    return convert_args(ctx, params, common_type, loc);
}

static bool elementwise_intrinsic_float_convert_args(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    struct hlsl_type *type;

    if (!(type = elementwise_intrinsic_get_common_type(ctx, params, loc)))
        return false;
    if (type_is_integer(type->e.numeric.type))
        type = hlsl_get_numeric_type(ctx, type->class, HLSL_TYPE_FLOAT, type->dimx, type->dimy);

    return convert_args(ctx, params, type, loc);
}

static bool elementwise_intrinsic_uint_convert_args(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    struct hlsl_type *type;

    if (!(type = elementwise_intrinsic_get_common_type(ctx, params, loc)))
        return false;

    type = hlsl_get_numeric_type(ctx, type->class, HLSL_TYPE_UINT, type->dimx, type->dimy);

    return convert_args(ctx, params, type, loc);
}

static bool intrinsic_abs(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    return !!add_unary_arithmetic_expr(ctx, params->instrs, HLSL_OP1_ABS, params->args[0], loc);
}

static bool write_acos_or_asin(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc, bool asin_mode)
{
    struct hlsl_ir_function_decl *func;
    struct hlsl_ir_node *arg;
    struct hlsl_type *type;
    char *body;

    static const char template[] =
            "%s %s(%s x)\n"
            "{\n"
            "    %s abs_arg = abs(x);\n"
            "    %s poly_approx = (((-0.018729\n"
            "        * abs_arg + 0.074261)\n"
            "        * abs_arg - 0.212114)\n"
            "        * abs_arg + 1.570729);\n"
            "    %s correction = sqrt(1.0 - abs_arg);\n"
            "    %s zero_flip = (x < 0.0) * (-2.0 * correction * poly_approx + 3.141593);\n"
            "    %s result = poly_approx * correction + zero_flip;\n"
            "    return %s;\n"
            "}";
    static const char fn_name_acos[] = "acos";
    static const char fn_name_asin[] = "asin";
    static const char return_stmt_acos[] = "result";
    static const char return_stmt_asin[] = "-result + 1.570796";

    const char *fn_name = asin_mode ? fn_name_asin : fn_name_acos;

    if (!(arg = intrinsic_float_convert_arg(ctx, params, params->args[0], loc)))
        return false;
    type = arg->data_type;

    if (!(body = hlsl_sprintf_alloc(ctx, template,
            type->name, fn_name, type->name,
            type->name, type->name, type->name, type->name, type->name,
            (asin_mode ? return_stmt_asin : return_stmt_acos))))
        return false;
    func = hlsl_compile_internal_function(ctx, fn_name, body);
    vkd3d_free(body);
    if (!func)
        return false;

    return !!add_user_call(ctx, func, params, false, loc);
}

static bool intrinsic_acos(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    return write_acos_or_asin(ctx, params, loc, false);
}

/* Find the type corresponding to the given source type, with the same
 * dimensions but a different base type. */
static struct hlsl_type *convert_numeric_type(const struct hlsl_ctx *ctx,
        const struct hlsl_type *type, enum hlsl_base_type base_type)
{
    return hlsl_get_numeric_type(ctx, type->class, base_type, type->dimx, type->dimy);
}

static bool add_combine_components(struct hlsl_ctx *ctx, const struct parse_initializer *params,
        struct hlsl_ir_node *arg, enum hlsl_ir_expr_op op, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_node *res, *load;
    unsigned int i, count;

    count = hlsl_type_component_count(arg->data_type);

    if (!(res = hlsl_add_load_component(ctx, params->instrs, arg, 0, loc)))
        return false;

    for (i = 1; i < count; ++i)
    {
        if (!(load = hlsl_add_load_component(ctx, params->instrs, arg, i, loc)))
            return false;

        if (!(res = hlsl_new_binary_expr(ctx, op, res, load)))
                return false; //REACTOS: previous return NULL;
        hlsl_block_add_instr(params->instrs, res);
    }

    return true;
}

static bool intrinsic_all(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_node *arg = params->args[0], *cast;
    struct hlsl_type *bool_type;

    bool_type = convert_numeric_type(ctx, arg->data_type, HLSL_TYPE_BOOL);
    if (!(cast = add_cast(ctx, params->instrs, arg, bool_type, loc)))
        return false;

    return add_combine_components(ctx, params, cast, HLSL_OP2_LOGIC_AND, loc);
}

static bool intrinsic_any(struct hlsl_ctx *ctx, const struct parse_initializer *params,
        const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_node *arg = params->args[0], *cast;
    struct hlsl_type *bool_type;

    bool_type = convert_numeric_type(ctx, arg->data_type, HLSL_TYPE_BOOL);
    if (!(cast = add_cast(ctx, params->instrs, arg, bool_type, loc)))
        return false;

    return add_combine_components(ctx, params, cast, HLSL_OP2_LOGIC_OR, loc);
}

static bool intrinsic_asin(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    return write_acos_or_asin(ctx, params, loc, true);
}

static bool write_atan_or_atan2(struct hlsl_ctx *ctx,
        const struct parse_initializer *params,
        const struct vkd3d_shader_location *loc, bool atan2_mode)
{
    struct hlsl_ir_function_decl *func;
    struct hlsl_type *type;
    struct vkd3d_string_buffer *buf;
    int ret;

    static const char atan2_name[] = "atan2";
    static const char atan_name[] = "atan";

    static const char atan2_header_template[] =
            "%s atan2(%s y, %s x)\n"
            "{\n"
            "    %s in_y, in_x;\n"
            "    in_y = y;\n"
            "    in_x = x;\n";
    static const char atan_header_template[] =
            "%s atan(%s y)\n"
            "{\n"
            "    %s in_y, in_x;\n"
            "    in_y = y;\n"
            "    in_x = 1.0;\n";

    static const char body_template[] =
            "    %s recip, input, x2, poly_approx, flipped;"
            "    recip = 1.0 / max(abs(in_y), abs(in_x));\n"
            "    input = recip * min(abs(in_y), abs(in_x));\n"
            "    x2 = input * input;\n"
            "    poly_approx = ((((0.020835\n"
            "        * x2 - 0.085133)\n"
            "        * x2 + 0.180141)\n"
            "        * x2 - 0.330299)\n"
            "        * x2 + 0.999866)\n"
            "        * input;\n"
            "    flipped = poly_approx * -2.0 + 1.570796;\n"
            "    poly_approx += abs(in_x) < abs(in_y) ? flipped : 0.0;\n"
            "    poly_approx += in_x < 0.0 ? -3.1415927 : 0.0;\n"
            "    return (min(in_x, in_y) < 0.0 && max(in_x, in_y) >= 0.0)\n"
            "        ? -poly_approx\n"
            "        : poly_approx;\n"
            "}";

    if (!elementwise_intrinsic_float_convert_args(ctx, params, loc))
        return false;
    type = params->args[0]->data_type;

    if (!(buf = hlsl_get_string_buffer(ctx)))
        return false;

    if (atan2_mode)
        ret = vkd3d_string_buffer_printf(buf, atan2_header_template,
                type->name, type->name, type->name, type->name);
    else
        ret = vkd3d_string_buffer_printf(buf, atan_header_template,
                type->name, type->name, type->name);
    if (ret < 0)
    {
        hlsl_release_string_buffer(ctx, buf);
        return false;
    }

    ret = vkd3d_string_buffer_printf(buf, body_template, type->name);
    if (ret < 0)
    {
        hlsl_release_string_buffer(ctx, buf);
        return false;
    }

    func = hlsl_compile_internal_function(ctx,
            atan2_mode ? atan2_name : atan_name, buf->buffer);
    hlsl_release_string_buffer(ctx, buf);
    if (!func)
        return false;

    return !!add_user_call(ctx, func, params, false, loc);
}

static bool intrinsic_atan(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    return write_atan_or_atan2(ctx, params, loc, false);
}


static bool intrinsic_atan2(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    return write_atan_or_atan2(ctx, params, loc, true);
}

static bool intrinsic_asfloat(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_node *operands[HLSL_MAX_OPERANDS] = {0};
    struct hlsl_type *data_type;

    data_type = params->args[0]->data_type;
    if (data_type->e.numeric.type == HLSL_TYPE_BOOL || data_type->e.numeric.type == HLSL_TYPE_DOUBLE)
    {
        struct vkd3d_string_buffer *string;

        if ((string = hlsl_type_to_string(ctx, data_type)))
            hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_INVALID_TYPE,
                    "Wrong argument type of asfloat(): expected 'int', 'uint', 'float', or 'half', but got '%s'.",
                    string->buffer);
        hlsl_release_string_buffer(ctx, string);
    }
    data_type = convert_numeric_type(ctx, data_type, HLSL_TYPE_FLOAT);

    operands[0] = params->args[0];
    return add_expr(ctx, params->instrs, HLSL_OP1_REINTERPRET, operands, data_type, loc);
}

static bool intrinsic_asint(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_node *operands[HLSL_MAX_OPERANDS] = {0};
    struct hlsl_type *data_type;

    data_type = params->args[0]->data_type;
    if (data_type->e.numeric.type == HLSL_TYPE_BOOL || data_type->e.numeric.type == HLSL_TYPE_DOUBLE)
    {
        struct vkd3d_string_buffer *string;

        if ((string = hlsl_type_to_string(ctx, data_type)))
            hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_INVALID_TYPE,
                    "Wrong argument type of asint(): expected 'int', 'uint', 'float', or 'half', but got '%s'.",
                    string->buffer);
        hlsl_release_string_buffer(ctx, string);
    }
    data_type = convert_numeric_type(ctx, data_type, HLSL_TYPE_INT);

    operands[0] = params->args[0];
    return add_expr(ctx, params->instrs, HLSL_OP1_REINTERPRET, operands, data_type, loc);
}

static bool intrinsic_asuint(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_node *operands[HLSL_MAX_OPERANDS] = {0};
    struct hlsl_type *data_type;

    if (params->args_count != 1 && params->args_count != 3)
    {
        hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_WRONG_PARAMETER_COUNT,
                "Wrong number of arguments to function 'asuint': expected 1 or 3, but got %u.", params->args_count);
        return false;
    }

    if (params->args_count == 3)
    {
        hlsl_fixme(ctx, loc, "Double-to-integer conversion.");
        return false;
    }

    data_type = params->args[0]->data_type;
    if (data_type->e.numeric.type == HLSL_TYPE_BOOL || data_type->e.numeric.type == HLSL_TYPE_DOUBLE)
    {
        struct vkd3d_string_buffer *string;

        if ((string = hlsl_type_to_string(ctx, data_type)))
            hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_INVALID_TYPE,
                    "Wrong type for argument 0 of asuint(): expected 'int', 'uint', 'float', or 'half', but got '%s'.",
                    string->buffer);
        hlsl_release_string_buffer(ctx, string);
    }
    data_type = convert_numeric_type(ctx, data_type, HLSL_TYPE_UINT);

    operands[0] = params->args[0];
    return add_expr(ctx, params->instrs, HLSL_OP1_REINTERPRET, operands, data_type, loc);
}

static bool intrinsic_ceil(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_node *arg;

    if (!(arg = intrinsic_float_convert_arg(ctx, params, params->args[0], loc)))
        return false;

    return !!add_unary_arithmetic_expr(ctx, params->instrs, HLSL_OP1_CEIL, arg, loc);
}

static bool intrinsic_clamp(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_node *max;

    if (!elementwise_intrinsic_convert_args(ctx, params, loc))
        return false;

    if (!(max = add_binary_arithmetic_expr(ctx, params->instrs, HLSL_OP2_MAX, params->args[0], params->args[1], loc)))
        return false;

    return !!add_binary_arithmetic_expr(ctx, params->instrs, HLSL_OP2_MIN, max, params->args[2], loc);
}

static bool intrinsic_clip(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_node *condition, *jump;

    if (!elementwise_intrinsic_float_convert_args(ctx, params, loc))
        return false;

    condition = params->args[0];

    if (ctx->profile->major_version < 4 && hlsl_type_component_count(condition->data_type) > 4)
    {
        struct vkd3d_string_buffer *string;

        if ((string = hlsl_type_to_string(ctx, condition->data_type)))
            hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_INVALID_TYPE,
                    "Argument type cannot exceed 4 components, got type \"%s\".", string->buffer);
        hlsl_release_string_buffer(ctx, string);
        return false;
    }

    if (!(jump = hlsl_new_jump(ctx, HLSL_IR_JUMP_DISCARD_NEG, condition, loc)))
        return false;
    hlsl_block_add_instr(params->instrs, jump);

    return true;
}

static bool intrinsic_cos(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_node *arg;

    if (!(arg = intrinsic_float_convert_arg(ctx, params, params->args[0], loc)))
        return false;

    return !!add_unary_arithmetic_expr(ctx, params->instrs, HLSL_OP1_COS, arg, loc);
}

static bool write_cosh_or_sinh(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc, bool sinh_mode)
{
    struct hlsl_ir_function_decl *func;
    struct hlsl_ir_node *arg;
    const char *fn_name, *type_name;
    char *body;

    static const char template[] =
            "%s %s(%s x)\n"
            "{\n"
            "    return (exp(x) %s exp(-x)) / 2;\n"
            "}\n";
    static const char fn_name_sinh[] = "sinh";
    static const char fn_name_cosh[] = "cosh";

    if (!(arg = intrinsic_float_convert_arg(ctx, params, params->args[0], loc)))
        return false;

    type_name = arg->data_type->name;
    fn_name = sinh_mode ? fn_name_sinh : fn_name_cosh;

    if (!(body = hlsl_sprintf_alloc(ctx, template,
            type_name, fn_name, type_name, sinh_mode ? "-" : "+")))
        return false;

    func = hlsl_compile_internal_function(ctx, fn_name, body);
    vkd3d_free(body);
    if (!func)
        return false;

    return !!add_user_call(ctx, func, params, false, loc);
}

static bool intrinsic_cosh(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    return write_cosh_or_sinh(ctx, params, loc, false);
}

static bool intrinsic_cross(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_node *arg1_swzl1, *arg1_swzl2, *arg2_swzl1, *arg2_swzl2;
    struct hlsl_ir_node *arg1 = params->args[0], *arg2 = params->args[1];
    struct hlsl_ir_node *arg1_cast, *arg2_cast, *mul1_neg, *mul1, *mul2;
    struct hlsl_type *cast_type;
    enum hlsl_base_type base;

    base = expr_common_base_type(arg1->data_type->e.numeric.type, arg2->data_type->e.numeric.type);
    if (type_is_integer(base))
        base = HLSL_TYPE_FLOAT;

    cast_type = hlsl_get_vector_type(ctx, base, 3);

    if (!(arg1_cast = add_implicit_conversion(ctx, params->instrs, arg1, cast_type, loc)))
        return false;

    if (!(arg2_cast = add_implicit_conversion(ctx, params->instrs, arg2, cast_type, loc)))
        return false;

    if (!(arg1_swzl1 = hlsl_new_swizzle(ctx, HLSL_SWIZZLE(Z, X, Y, Z), 3, arg1_cast, loc)))
        return false;
    hlsl_block_add_instr(params->instrs, arg1_swzl1);

    if (!(arg2_swzl1 = hlsl_new_swizzle(ctx, HLSL_SWIZZLE(Y, Z, X, Y), 3, arg2_cast, loc)))
        return false;
    hlsl_block_add_instr(params->instrs, arg2_swzl1);

    if (!(mul1 = add_binary_arithmetic_expr(ctx, params->instrs, HLSL_OP2_MUL, arg1_swzl1, arg2_swzl1, loc)))
        return false;

    if (!(mul1_neg = hlsl_new_unary_expr(ctx, HLSL_OP1_NEG, mul1, loc)))
        return false;
    hlsl_block_add_instr(params->instrs, mul1_neg);

    if (!(arg1_swzl2 = hlsl_new_swizzle(ctx, HLSL_SWIZZLE(Y, Z, X, Y), 3, arg1_cast, loc)))
        return false;
    hlsl_block_add_instr(params->instrs, arg1_swzl2);

    if (!(arg2_swzl2 = hlsl_new_swizzle(ctx, HLSL_SWIZZLE(Z, X, Y, Z), 3, arg2_cast, loc)))
        return false;
    hlsl_block_add_instr(params->instrs, arg2_swzl2);

    if (!(mul2 = add_binary_arithmetic_expr(ctx, params->instrs, HLSL_OP2_MUL, arg1_swzl2, arg2_swzl2, loc)))
        return false;

    return !!add_binary_arithmetic_expr(ctx, params->instrs, HLSL_OP2_ADD, mul2, mul1_neg, loc);
}

static bool intrinsic_ddx(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_node *arg;

    if (!(arg = intrinsic_float_convert_arg(ctx, params, params->args[0], loc)))
        return false;

    return !!add_unary_arithmetic_expr(ctx, params->instrs, HLSL_OP1_DSX, arg, loc);
}

static bool intrinsic_ddx_coarse(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_node *arg;

    if (!(arg = intrinsic_float_convert_arg(ctx, params, params->args[0], loc)))
        return false;

    return !!add_unary_arithmetic_expr(ctx, params->instrs, HLSL_OP1_DSX_COARSE, arg, loc);
}

static bool intrinsic_ddx_fine(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_node *arg;

    if (!(arg = intrinsic_float_convert_arg(ctx, params, params->args[0], loc)))
        return false;

    return !!add_unary_arithmetic_expr(ctx, params->instrs, HLSL_OP1_DSX_FINE, arg, loc);
}

static bool intrinsic_ddy(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_node *arg;

    if (!(arg = intrinsic_float_convert_arg(ctx, params, params->args[0], loc)))
        return false;

    return !!add_unary_arithmetic_expr(ctx, params->instrs, HLSL_OP1_DSY, arg, loc);
}

static bool intrinsic_ddy_coarse(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_node *arg;

    if (!(arg = intrinsic_float_convert_arg(ctx, params, params->args[0], loc)))
        return false;

    return !!add_unary_arithmetic_expr(ctx, params->instrs, HLSL_OP1_DSY_COARSE, arg, loc);
}

static bool intrinsic_degrees(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_node *arg, *deg;

    if (!(arg = intrinsic_float_convert_arg(ctx, params, params->args[0], loc)))
        return false;

    /* 1 rad = 180/pi degree = 57.2957795 degree */
    if (!(deg = hlsl_new_float_constant(ctx, 57.2957795f, loc)))
        return false;
    hlsl_block_add_instr(params->instrs, deg);

    return !!add_binary_arithmetic_expr(ctx, params->instrs, HLSL_OP2_MUL, arg, deg, loc);
}

static bool intrinsic_ddy_fine(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_node *arg;

    if (!(arg = intrinsic_float_convert_arg(ctx, params, params->args[0], loc)))
        return false;

    return !!add_unary_arithmetic_expr(ctx, params->instrs, HLSL_OP1_DSY_FINE, arg, loc);
}

static bool intrinsic_determinant(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    static const char determinant2x2[] =
            "%s determinant(%s2x2 m)\n"
            "{\n"
            "    return m._11 * m._22 - m._12 * m._21;\n"
            "}";
    static const char determinant3x3[] =
            "%s determinant(%s3x3 m)\n"
            "{\n"
            "    %s2x2 m1 = { m._22, m._23, m._32, m._33 };\n"
            "    %s2x2 m2 = { m._21, m._23, m._31, m._33 };\n"
            "    %s2x2 m3 = { m._21, m._22, m._31, m._32 };\n"
            "    %s3 v1 = { m._11, -m._12, m._13 };\n"
            "    %s3 v2 = { determinant(m1), determinant(m2), determinant(m3) };\n"
            "    return dot(v1, v2);\n"
            "}";
    static const char determinant4x4[] =
            "%s determinant(%s4x4 m)\n"
            "{\n"
            "    %s3x3 m1 = { m._22, m._23, m._24, m._32, m._33, m._34, m._42, m._43, m._44 };\n"
            "    %s3x3 m2 = { m._21, m._23, m._24, m._31, m._33, m._34, m._41, m._43, m._44 };\n"
            "    %s3x3 m3 = { m._21, m._22, m._24, m._31, m._32, m._34, m._41, m._42, m._44 };\n"
            "    %s3x3 m4 = { m._21, m._22, m._23, m._31, m._32, m._33, m._41, m._42, m._43 };\n"
            "    %s4 v1 = { m._11, -m._12, m._13, -m._14 };\n"
            "    %s4 v2 = { determinant(m1), determinant(m2), determinant(m3), determinant(m4) };\n"
            "    return dot(v1, v2);\n"
            "}";
    static const char *templates[] =
    {
        [2] = determinant2x2,
        [3] = determinant3x3,
        [4] = determinant4x4,
    };

    struct hlsl_ir_node *arg = params->args[0];
    const struct hlsl_type *type = arg->data_type;
    struct hlsl_ir_function_decl *func;
    const char *typename, *template;
    unsigned int dim;
    char *body;

    if (type->class != HLSL_CLASS_SCALAR && type->class != HLSL_CLASS_MATRIX)
    {
        hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_INVALID_TYPE, "Invalid argument type.");
        return false;
    }

    if (!(arg = intrinsic_float_convert_arg(ctx, params, arg, loc)))
        return false;

    dim = min(type->dimx, type->dimy);
    if (dim == 1)
        return hlsl_add_load_component(ctx, params->instrs, arg, 0, loc);

    typename = hlsl_get_scalar_type(ctx, arg->data_type->e.numeric.type)->name;
    template = templates[dim];

    switch (dim)
    {
        case 2:
            body = hlsl_sprintf_alloc(ctx, template, typename, typename);
            break;
        case 3:
            body = hlsl_sprintf_alloc(ctx, template, typename, typename, typename,
                    typename, typename, typename, typename);
            break;
        case 4:
            body = hlsl_sprintf_alloc(ctx, template, typename, typename, typename,
                    typename, typename, typename, typename, typename);
            break;
        default:
            vkd3d_unreachable();
    }

    if (!body)
        return false;

    func = hlsl_compile_internal_function(ctx, "determinant", body);
    vkd3d_free(body);
    if (!func)
        return false;

    return !!add_user_call(ctx, func, params, false, loc);
}

static bool intrinsic_distance(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_node *arg1, *arg2, *neg, *add, *dot;

    if (!(arg1 = intrinsic_float_convert_arg(ctx, params, params->args[0], loc)))
        return false;

    if (!(arg2 = intrinsic_float_convert_arg(ctx, params, params->args[1], loc)))
        return false;

    if (!(neg = add_unary_arithmetic_expr(ctx, params->instrs, HLSL_OP1_NEG, arg2, loc)))
        return false;

    if (!(add = add_binary_arithmetic_expr(ctx, params->instrs, HLSL_OP2_ADD, arg1, neg, loc)))
        return false;

    if (!(dot = add_binary_dot_expr(ctx, params->instrs, add, add, loc)))
        return false;

    return !!add_unary_arithmetic_expr(ctx, params->instrs, HLSL_OP1_SQRT, dot, loc);
}

static bool intrinsic_dot(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    return !!add_binary_dot_expr(ctx, params->instrs, params->args[0], params->args[1], loc);
}

static bool intrinsic_dst(struct hlsl_ctx *ctx, const struct parse_initializer *params,
        const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_function_decl *func;
    struct hlsl_type *type, *vec4_type;
    char *body;

    static const char template[] =
            "%s dst(%s i0, %s i1)\n"
            "{\n"
            /* Scalars and vector-4s are both valid inputs, so promote scalars
             * if necessary. */
            "    %s src0 = i0, src1 = i1;\n"
            "    return %s(1, src0.y * src1.y, src0.z, src1.w);\n"
            "}";

    if (!elementwise_intrinsic_convert_args(ctx, params, loc))
        return false;
    type = params->args[0]->data_type;
    if (!(type->class == HLSL_CLASS_SCALAR
            || (type->class == HLSL_CLASS_VECTOR && type->dimx == 4)))
    {
        struct vkd3d_string_buffer *string;
        if ((string = hlsl_type_to_string(ctx, type)))
            hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_INVALID_TYPE,
                    "Wrong dimension for dst(): expected scalar or 4-dimensional vector, but got %s.",
                    string->buffer);
        hlsl_release_string_buffer(ctx, string);
    }
    vec4_type = hlsl_get_vector_type(ctx, type->e.numeric.type, 4);

    if (!(body = hlsl_sprintf_alloc(ctx, template,
            vec4_type->name, type->name, type->name,
            vec4_type->name,
            vec4_type->name)))
        return false;
    func = hlsl_compile_internal_function(ctx, "dst", body);
    vkd3d_free(body);
    if (!func)
        return false;

    return !!add_user_call(ctx, func, params, false, loc);
}

static bool intrinsic_exp(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_node *arg, *mul, *coeff;

    if (!(arg = intrinsic_float_convert_arg(ctx, params, params->args[0], loc)))
        return false;

    /* 1/ln(2) */
    if (!(coeff = hlsl_new_float_constant(ctx, 1.442695f, loc)))
        return false;
    hlsl_block_add_instr(params->instrs, coeff);

    if (!(mul = add_binary_arithmetic_expr(ctx, params->instrs, HLSL_OP2_MUL, coeff, params->args[0], loc)))
        return false;

    return !!add_unary_arithmetic_expr(ctx, params->instrs, HLSL_OP1_EXP2, mul, loc);
}

static bool intrinsic_exp2(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_node *arg;

    if (!(arg = intrinsic_float_convert_arg(ctx, params, params->args[0], loc)))
        return false;

    return !!add_unary_arithmetic_expr(ctx, params->instrs, HLSL_OP1_EXP2, arg, loc);
}

static bool intrinsic_faceforward(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_function_decl *func;
    struct hlsl_type *type;
    char *body;

    static const char template[] =
            "%s faceforward(%s n, %s i, %s ng)\n"
            "{\n"
            "    return dot(i, ng) < 0 ? n : -n;\n"
            "}\n";

    if (!elementwise_intrinsic_float_convert_args(ctx, params, loc))
        return false;
    type = params->args[0]->data_type;

    if (!(body = hlsl_sprintf_alloc(ctx, template,
            type->name, type->name, type->name, type->name)))
        return false;
    func = hlsl_compile_internal_function(ctx, "faceforward", body);
    vkd3d_free(body);
    if (!func)
        return false;

    return !!add_user_call(ctx, func, params, false, loc);
}

static bool intrinsic_f16tof32(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_node *operands[HLSL_MAX_OPERANDS] = {0};
    struct hlsl_type *type;

    if (!elementwise_intrinsic_uint_convert_args(ctx, params, loc))
        return false;

    type = convert_numeric_type(ctx, params->args[0]->data_type, HLSL_TYPE_FLOAT);

    operands[0] = params->args[0];
    return add_expr(ctx, params->instrs, HLSL_OP1_F16TOF32, operands, type, loc);
}

static bool intrinsic_f32tof16(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_node *operands[HLSL_MAX_OPERANDS] = {0};
    struct hlsl_type *type;

    if (!elementwise_intrinsic_float_convert_args(ctx, params, loc))
        return false;

    type = convert_numeric_type(ctx, params->args[0]->data_type, HLSL_TYPE_UINT);

    operands[0] = params->args[0];
    return add_expr(ctx, params->instrs, HLSL_OP1_F32TOF16, operands, type, loc);
}

static bool intrinsic_floor(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_node *arg;

    if (!(arg = intrinsic_float_convert_arg(ctx, params, params->args[0], loc)))
        return false;

    return !!add_unary_arithmetic_expr(ctx, params->instrs, HLSL_OP1_FLOOR, arg, loc);
}

static bool intrinsic_fmod(struct hlsl_ctx *ctx, const struct parse_initializer *params,
        const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_node *x, *y, *div, *abs, *frac, *neg_frac, *ge, *select, *zero;
    struct hlsl_ir_node *operands[HLSL_MAX_OPERANDS] = { 0 };
    static const struct hlsl_constant_value zero_value;

    if (!(x = intrinsic_float_convert_arg(ctx, params, params->args[0], loc)))
        return false;

    if (!(y = intrinsic_float_convert_arg(ctx, params, params->args[1], loc)))
        return false;

    if (!(div = add_binary_arithmetic_expr(ctx, params->instrs, HLSL_OP2_DIV, x, y, loc)))
        return false;

    if (!(zero = hlsl_new_constant(ctx, div->data_type, &zero_value, loc)))
        return false;
    hlsl_block_add_instr(params->instrs, zero);

    if (!(abs = add_unary_arithmetic_expr(ctx, params->instrs, HLSL_OP1_ABS, div, loc)))
        return false;

    if (!(frac = add_unary_arithmetic_expr(ctx, params->instrs, HLSL_OP1_FRACT, abs, loc)))
        return false;

    if (!(neg_frac = add_unary_arithmetic_expr(ctx, params->instrs, HLSL_OP1_NEG, frac, loc)))
        return false;

    if (!(ge = add_binary_comparison_expr(ctx, params->instrs, HLSL_OP2_GEQUAL, div, zero, loc)))
        return false;

    operands[0] = ge;
    operands[1] = frac;
    operands[2] = neg_frac;
    if (!(select = add_expr(ctx, params->instrs, HLSL_OP3_TERNARY, operands, x->data_type, loc)))
        return false;

    return !!add_binary_arithmetic_expr(ctx, params->instrs, HLSL_OP2_MUL, select, y, loc);
}

static bool intrinsic_frac(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_node *arg;

    if (!(arg = intrinsic_float_convert_arg(ctx, params, params->args[0], loc)))
        return false;

    return !!add_unary_arithmetic_expr(ctx, params->instrs, HLSL_OP1_FRACT, arg, loc);
}

static bool intrinsic_fwidth(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_function_decl *func;
    struct hlsl_type *type;
    char *body;

    static const char template[] =
            "%s fwidth(%s x)\n"
            "{\n"
            "    return abs(ddx(x)) + abs(ddy(x));\n"
            "}";

    if (!elementwise_intrinsic_float_convert_args(ctx, params, loc))
        return false;
    type = params->args[0]->data_type;

    if (!(body = hlsl_sprintf_alloc(ctx, template, type->name, type->name)))
        return false;
    func = hlsl_compile_internal_function(ctx, "fwidth", body);
    vkd3d_free(body);
    if (!func)
        return false;

    return !!add_user_call(ctx, func, params, false, loc);
}

static bool intrinsic_ldexp(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_node *arg;

    if (!elementwise_intrinsic_float_convert_args(ctx, params, loc))
        return false;

    if (!(arg = add_unary_arithmetic_expr(ctx, params->instrs, HLSL_OP1_EXP2, params->args[1], loc)))
        return false;

    return !!add_binary_arithmetic_expr(ctx, params->instrs, HLSL_OP2_MUL, params->args[0], arg, loc);
}

static bool intrinsic_length(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    struct hlsl_type *type = params->args[0]->data_type;
    struct hlsl_ir_node *arg, *dot;

    if (type->class == HLSL_CLASS_MATRIX)
    {
        struct vkd3d_string_buffer *string;

        if ((string = hlsl_type_to_string(ctx, type)))
            hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_INVALID_TYPE,
                    "Invalid type %s.", string->buffer);
        hlsl_release_string_buffer(ctx, string);
    }

    if (!(arg = intrinsic_float_convert_arg(ctx, params, params->args[0], loc)))
        return false;

    if (!(dot = add_binary_dot_expr(ctx, params->instrs, arg, arg, loc)))
        return false;

    return !!add_unary_arithmetic_expr(ctx, params->instrs, HLSL_OP1_SQRT, dot, loc);
}

static bool intrinsic_lerp(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_node *neg, *add, *mul;

    if (!elementwise_intrinsic_float_convert_args(ctx, params, loc))
        return false;

    if (!(neg = add_unary_arithmetic_expr(ctx, params->instrs, HLSL_OP1_NEG, params->args[0], loc)))
        return false;

    if (!(add = add_binary_arithmetic_expr(ctx, params->instrs, HLSL_OP2_ADD, params->args[1], neg, loc)))
        return false;

    if (!(mul = add_binary_arithmetic_expr(ctx, params->instrs, HLSL_OP2_MUL, params->args[2], add, loc)))
        return false;

    return !!add_binary_arithmetic_expr(ctx, params->instrs, HLSL_OP2_ADD, params->args[0], mul, loc);
}

static struct hlsl_ir_node * add_pow_expr(struct hlsl_ctx *ctx,
        struct hlsl_block *instrs, struct hlsl_ir_node *arg1, struct hlsl_ir_node *arg2,
        const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_node *log, *mul;

    if (!(log = add_unary_arithmetic_expr(ctx, instrs, HLSL_OP1_LOG2, arg1, loc)))
        return NULL;

    if (!(mul = add_binary_arithmetic_expr(ctx, instrs, HLSL_OP2_MUL, arg2, log, loc)))
        return NULL;

    return add_unary_arithmetic_expr(ctx, instrs, HLSL_OP1_EXP2, mul, loc);
}

static bool intrinsic_lit(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_function_decl *func;

    static const char body[] =
            "float4 lit(float n_l, float n_h, float m)\n"
            "{\n"
            "    float4 ret;\n"
            "    ret.xw = 1.0;\n"
            "    ret.y = max(n_l, 0);\n"
            "    ret.z = (n_l < 0 || n_h < 0) ? 0 : pow(n_h, m);\n"
            "    return ret;\n"
            "}";

    if (params->args[0]->data_type->class != HLSL_CLASS_SCALAR
            || params->args[1]->data_type->class != HLSL_CLASS_SCALAR
            || params->args[2]->data_type->class != HLSL_CLASS_SCALAR)
    {
        hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_INVALID_TYPE, "Invalid argument type.");
        return false;
    }

    if (!(func = hlsl_compile_internal_function(ctx, "lit", body)))
        return false;

    return !!add_user_call(ctx, func, params, false, loc);
}

static bool intrinsic_log(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_node *log, *arg, *coeff;

    if (!(arg = intrinsic_float_convert_arg(ctx, params, params->args[0], loc)))
        return false;

    if (!(log = add_unary_arithmetic_expr(ctx, params->instrs, HLSL_OP1_LOG2, arg, loc)))
        return false;

    /* ln(2) */
    if (!(coeff = hlsl_new_float_constant(ctx, 0.69314718055f, loc)))
        return false;
    hlsl_block_add_instr(params->instrs, coeff);

    return !!add_binary_arithmetic_expr(ctx, params->instrs, HLSL_OP2_MUL, log, coeff, loc);
}

static bool intrinsic_log10(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_node *log, *arg, *coeff;

    if (!(arg = intrinsic_float_convert_arg(ctx, params, params->args[0], loc)))
        return false;

    if (!(log = add_unary_arithmetic_expr(ctx, params->instrs, HLSL_OP1_LOG2, arg, loc)))
        return false;

    /* 1 / log2(10) */
    if (!(coeff = hlsl_new_float_constant(ctx, 0.301029996f, loc)))
        return false;
    hlsl_block_add_instr(params->instrs, coeff);

    return !!add_binary_arithmetic_expr(ctx, params->instrs, HLSL_OP2_MUL, log, coeff, loc);
}

static bool intrinsic_log2(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_node *arg;

    if (!(arg = intrinsic_float_convert_arg(ctx, params, params->args[0], loc)))
        return false;

    return !!add_unary_arithmetic_expr(ctx, params->instrs, HLSL_OP1_LOG2, arg, loc);
}

static bool intrinsic_mad(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_node *args[HLSL_MAX_OPERANDS] = {0};

    if (!elementwise_intrinsic_convert_args(ctx, params, loc))
        return false;

    args[0] = params->args[0];
    args[1] = params->args[1];
    args[2] = params->args[2];
    return add_expr(ctx, params->instrs, HLSL_OP3_MAD, args, args[0]->data_type, loc);
}

static bool intrinsic_max(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    if (!elementwise_intrinsic_convert_args(ctx, params, loc))
        return false;

    return !!add_binary_arithmetic_expr(ctx, params->instrs, HLSL_OP2_MAX, params->args[0], params->args[1], loc);
}

static bool intrinsic_min(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    if (!elementwise_intrinsic_convert_args(ctx, params, loc))
        return false;

    return !!add_binary_arithmetic_expr(ctx, params->instrs, HLSL_OP2_MIN, params->args[0], params->args[1], loc);
}

static bool intrinsic_modf(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_function_decl *func;
    struct hlsl_type *type;
    char *body;

    static const char template[] =
            "%s modf(%s x, out %s ip)\n"
            "{\n"
            "    ip = trunc(x);\n"
            "    return x - ip;\n"
            "}";

    if (!elementwise_intrinsic_float_convert_args(ctx, params, loc))
        return false;
    type = params->args[0]->data_type;

    if (!(body = hlsl_sprintf_alloc(ctx, template,
            type->name, type->name, type->name)))
        return false;
    func = hlsl_compile_internal_function(ctx, "modf", body);
    vkd3d_free(body);
    if (!func)
        return false;

    return !!add_user_call(ctx, func, params, false, loc);
}

static bool intrinsic_mul(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_node *arg1 = params->args[0], *arg2 = params->args[1], *cast1, *cast2;
    enum hlsl_base_type base = expr_common_base_type(arg1->data_type->e.numeric.type, arg2->data_type->e.numeric.type);
    struct hlsl_type *cast_type1 = arg1->data_type, *cast_type2 = arg2->data_type, *matrix_type, *ret_type;
    unsigned int i, j, k, vect_count = 0;
    struct hlsl_deref var_deref;
    struct hlsl_ir_load *load;
    struct hlsl_ir_var *var;

    if (arg1->data_type->class == HLSL_CLASS_SCALAR || arg2->data_type->class == HLSL_CLASS_SCALAR)
        return !!add_binary_arithmetic_expr(ctx, params->instrs, HLSL_OP2_MUL, arg1, arg2, loc);

    if (arg1->data_type->class == HLSL_CLASS_VECTOR)
    {
        vect_count++;
        cast_type1 = hlsl_get_matrix_type(ctx, base, arg1->data_type->dimx, 1);
    }
    if (arg2->data_type->class == HLSL_CLASS_VECTOR)
    {
        vect_count++;
        cast_type2 = hlsl_get_matrix_type(ctx, base, 1, arg2->data_type->dimx);
    }

    matrix_type = hlsl_get_matrix_type(ctx, base, cast_type2->dimx, cast_type1->dimy);

    if (vect_count == 0)
    {
        ret_type = matrix_type;
    }
    else if (vect_count == 1)
    {
        VKD3D_ASSERT(matrix_type->dimx == 1 || matrix_type->dimy == 1);
        ret_type = hlsl_get_vector_type(ctx, base, matrix_type->dimx * matrix_type->dimy);
    }
    else
    {
        VKD3D_ASSERT(matrix_type->dimx == 1 && matrix_type->dimy == 1);
        ret_type = hlsl_get_scalar_type(ctx, base);
    }

    if (!(cast1 = add_implicit_conversion(ctx, params->instrs, arg1, cast_type1, loc)))
        return false;

    if (!(cast2 = add_implicit_conversion(ctx, params->instrs, arg2, cast_type2, loc)))
        return false;

    if (!(var = hlsl_new_synthetic_var(ctx, "mul", matrix_type, loc)))
        return false;
    hlsl_init_simple_deref_from_var(&var_deref, var);

    for (i = 0; i < matrix_type->dimx; ++i)
    {
        for (j = 0; j < matrix_type->dimy; ++j)
        {
            struct hlsl_ir_node *instr = NULL;
            struct hlsl_block block;

            for (k = 0; k < cast_type1->dimx && k < cast_type2->dimy; ++k)
            {
                struct hlsl_ir_node *value1, *value2, *mul;

                if (!(value1 = hlsl_add_load_component(ctx, params->instrs,
                        cast1, j * cast1->data_type->dimx + k, loc)))
                    return false;

                if (!(value2 = hlsl_add_load_component(ctx, params->instrs,
                        cast2, k * cast2->data_type->dimx + i, loc)))
                    return false;

                if (!(mul = add_binary_arithmetic_expr(ctx, params->instrs, HLSL_OP2_MUL, value1, value2, loc)))
                    return false;

                if (instr)
                {
                    if (!(instr = add_binary_arithmetic_expr(ctx, params->instrs, HLSL_OP2_ADD, instr, mul, loc)))
                        return false;
                }
                else
                {
                    instr = mul;
                }
            }

            if (!hlsl_new_store_component(ctx, &block, &var_deref, j * matrix_type->dimx + i, instr))
                return false;
            hlsl_block_add_block(params->instrs, &block);
        }
    }

    if (!(load = hlsl_new_var_load(ctx, var, loc)))
        return false;
    hlsl_block_add_instr(params->instrs, &load->node);

    return !!add_implicit_conversion(ctx, params->instrs, &load->node, ret_type, loc);
}

static bool intrinsic_normalize(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    struct hlsl_type *type = params->args[0]->data_type;
    struct hlsl_ir_node *dot, *rsq, *arg;

    if (type->class == HLSL_CLASS_MATRIX)
    {
        struct vkd3d_string_buffer *string;

        if ((string = hlsl_type_to_string(ctx, type)))
            hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_INVALID_TYPE,
                    "Invalid type %s.", string->buffer);
        hlsl_release_string_buffer(ctx, string);
    }

    if (!(arg = intrinsic_float_convert_arg(ctx, params, params->args[0], loc)))
        return false;

    if (!(dot = add_binary_dot_expr(ctx, params->instrs, arg, arg, loc)))
        return false;

    if (!(rsq = add_unary_arithmetic_expr(ctx, params->instrs, HLSL_OP1_RSQ, dot, loc)))
        return false;

    return !!add_binary_arithmetic_expr(ctx, params->instrs, HLSL_OP2_MUL, rsq, arg, loc);
}

static bool intrinsic_pow(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    if (!elementwise_intrinsic_float_convert_args(ctx, params, loc))
        return false;

    return !!add_pow_expr(ctx, params->instrs, params->args[0], params->args[1], loc);
}

static bool intrinsic_radians(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_node *arg, *rad;

    if (!(arg = intrinsic_float_convert_arg(ctx, params, params->args[0], loc)))
        return false;

    /* 1 degree = pi/180 rad = 0.0174532925f rad */
    if (!(rad = hlsl_new_float_constant(ctx, 0.0174532925f, loc)))
        return false;
    hlsl_block_add_instr(params->instrs, rad);

    return !!add_binary_arithmetic_expr(ctx, params->instrs, HLSL_OP2_MUL, arg, rad, loc);
}

static bool intrinsic_rcp(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_node *arg;

    if (!(arg = intrinsic_float_convert_arg(ctx, params, params->args[0], loc)))
        return false;

    return !!add_unary_arithmetic_expr(ctx, params->instrs, HLSL_OP1_RCP, arg, loc);
}

static bool intrinsic_reflect(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_node *i = params->args[0], *n = params->args[1];
    struct hlsl_ir_node *dot, *mul_n, *two_dot, *neg;

    if (!(dot = add_binary_dot_expr(ctx, params->instrs, i, n, loc)))
        return false;

    if (!(two_dot = add_binary_arithmetic_expr(ctx, params->instrs, HLSL_OP2_ADD, dot, dot, loc)))
        return false;

    if (!(mul_n = add_binary_arithmetic_expr(ctx, params->instrs, HLSL_OP2_MUL, n, two_dot, loc)))
        return false;

    if (!(neg = add_unary_arithmetic_expr(ctx, params->instrs, HLSL_OP1_NEG, mul_n, loc)))
        return false;

    return !!add_binary_arithmetic_expr(ctx, params->instrs, HLSL_OP2_ADD, i, neg, loc);
}

static bool intrinsic_refract(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    struct hlsl_type *type, *scalar_type;
    struct hlsl_ir_function_decl *func;
    struct hlsl_ir_node *index;
    char *body;

    static const char template[] =
            "%s refract(%s r, %s n, %s i)\n"
            "{\n"
            "    %s d, t;\n"
            "    d = dot(r, n);\n"
            "    t = 1 - i.x * i.x * (1 - d * d);\n"
            "    return t >= 0.0 ? i.x * r - (i.x * d + sqrt(t)) * n : 0;\n"
            "}";

    if (params->args[0]->data_type->class == HLSL_CLASS_MATRIX
            || params->args[1]->data_type->class == HLSL_CLASS_MATRIX
            || params->args[2]->data_type->class == HLSL_CLASS_MATRIX)
    {
        hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_INVALID_TYPE, "Matrix arguments are not supported.");
        return false;
    }

    /* This is technically not an elementwise intrinsic, but the first two
     * arguments are.
     * The third argument is a scalar, but can be passed as a vector,
     * which should generate an implicit truncation warning.
     * Cast down to scalar explicitly, then we can just use
     * elementwise_intrinsic_float_convert_args().
     * This may result in casting the scalar back to a vector,
     * which we will only use the first component of. */

    scalar_type = hlsl_get_scalar_type(ctx, params->args[2]->data_type->e.numeric.type);
    if (!(index = add_implicit_conversion(ctx, params->instrs, params->args[2], scalar_type, loc)))
        return false;
    params->args[2] = index;

    if (!elementwise_intrinsic_float_convert_args(ctx, params, loc))
        return false;
    type = params->args[0]->data_type;

    if (!(body = hlsl_sprintf_alloc(ctx, template, type->name, type->name,
            type->name, type->name, scalar_type->name)))
        return false;

    func = hlsl_compile_internal_function(ctx, "refract", body);
    vkd3d_free(body);
    if (!func)
        return false;

    return !!add_user_call(ctx, func, params, false, loc);
}

static bool intrinsic_round(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_node *arg;

    if (!(arg = intrinsic_float_convert_arg(ctx, params, params->args[0], loc)))
        return false;

    return !!add_unary_arithmetic_expr(ctx, params->instrs, HLSL_OP1_ROUND, arg, loc);
}

static bool intrinsic_rsqrt(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_node *arg;

    if (!(arg = intrinsic_float_convert_arg(ctx, params, params->args[0], loc)))
        return false;

    return !!add_unary_arithmetic_expr(ctx, params->instrs, HLSL_OP1_RSQ, arg, loc);
}

static bool intrinsic_saturate(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_node *arg;

    if (!(arg = intrinsic_float_convert_arg(ctx, params, params->args[0], loc)))
        return false;

    return !!add_unary_arithmetic_expr(ctx, params->instrs, HLSL_OP1_SAT, arg, loc);
}

static bool intrinsic_sign(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_node *lt, *neg, *op1, *op2, *zero, *arg = params->args[0];
    static const struct hlsl_constant_value zero_value;

    struct hlsl_type *int_type = hlsl_get_numeric_type(ctx, arg->data_type->class, HLSL_TYPE_INT,
            arg->data_type->dimx, arg->data_type->dimy);

    if (!(zero = hlsl_new_constant(ctx, hlsl_get_scalar_type(ctx, arg->data_type->e.numeric.type), &zero_value, loc)))
        return false;
    hlsl_block_add_instr(params->instrs, zero);

    /* Check if 0 < arg, cast bool to int */

    if (!(lt = add_binary_comparison_expr(ctx, params->instrs, HLSL_OP2_LESS, zero, arg, loc)))
        return false;

    if (!(op1 = add_implicit_conversion(ctx, params->instrs, lt, int_type, loc)))
        return false;

    /* Check if arg < 0, cast bool to int and invert (meaning true is -1) */

    if (!(lt = add_binary_comparison_expr(ctx, params->instrs, HLSL_OP2_LESS, arg, zero, loc)))
        return false;

    if (!(op2 = add_implicit_conversion(ctx, params->instrs, lt, int_type, loc)))
        return false;

    if (!(neg = add_unary_arithmetic_expr(ctx, params->instrs, HLSL_OP1_NEG, op2, loc)))
        return false;

    /* Adding these two together will make 1 when > 0, -1 when < 0, and 0 when neither */
    return !!add_binary_arithmetic_expr(ctx, params->instrs, HLSL_OP2_ADD, neg, op1, loc);
}

static bool intrinsic_sin(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_node *arg;

    if (!(arg = intrinsic_float_convert_arg(ctx, params, params->args[0], loc)))
        return false;

    return !!add_unary_arithmetic_expr(ctx, params->instrs, HLSL_OP1_SIN, arg, loc);
}

static bool intrinsic_sincos(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_function_decl *func;
    struct hlsl_type *type;
    char *body;

    static const char template[] =
            "void sincos(%s f, out %s s, out %s c)\n"
            "{\n"
            "    s = sin(f);\n"
            "    c = cos(f);\n"
            "}";

    if (!elementwise_intrinsic_float_convert_args(ctx, params, loc))
        return false;
    type = params->args[0]->data_type;

    if (!(body = hlsl_sprintf_alloc(ctx, template,
            type->name, type->name, type->name)))
        return false;
    func = hlsl_compile_internal_function(ctx, "sincos", body);
    vkd3d_free(body);
    if (!func)
        return false;

    return !!add_user_call(ctx, func, params, false, loc);
}

static bool intrinsic_sinh(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    return write_cosh_or_sinh(ctx, params, loc, true);
}

/* smoothstep(a, b, x) = p^2 (3 - 2p), where p = saturate((x - a)/(b - a)) */
static bool intrinsic_smoothstep(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_function_decl *func;
    struct hlsl_type *type;
    char *body;

    static const char template[] =
            "%s smoothstep(%s low, %s high, %s x)\n"
            "{\n"
            "    %s p = saturate((x - low) / (high - low));\n"
            "    return (p * p) * (3 - 2 * p);\n"
            "}";

    if (!elementwise_intrinsic_float_convert_args(ctx, params, loc))
        return false;
    type = params->args[0]->data_type;

    if (!(body = hlsl_sprintf_alloc(ctx, template, type->name, type->name, type->name, type->name, type->name)))
        return false;
    func = hlsl_compile_internal_function(ctx, "smoothstep", body);
    vkd3d_free(body);
    if (!func)
        return false;

    return !!add_user_call(ctx, func, params, false, loc);
}

static bool intrinsic_sqrt(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_node *arg;

    if (!(arg = intrinsic_float_convert_arg(ctx, params, params->args[0], loc)))
        return false;

    return !!add_unary_arithmetic_expr(ctx, params->instrs, HLSL_OP1_SQRT, arg, loc);
}

static bool intrinsic_step(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_node *ge;
    struct hlsl_type *type;

    if (!elementwise_intrinsic_float_convert_args(ctx, params, loc))
        return false;
    type = params->args[0]->data_type;

    if (!(ge = add_binary_comparison_expr(ctx, params->instrs, HLSL_OP2_GEQUAL,
            params->args[1], params->args[0], loc)))
        return false;

    return !!add_implicit_conversion(ctx, params->instrs, ge, type, loc);
}

static bool intrinsic_tan(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_node *arg = params->args[0], *sin, *cos;

    if (!(sin = add_unary_arithmetic_expr(ctx, params->instrs, HLSL_OP1_SIN, arg, loc)))
        return false;

    if (!(cos = add_unary_arithmetic_expr(ctx, params->instrs, HLSL_OP1_COS, arg, loc)))
        return false;

    return !!add_binary_arithmetic_expr(ctx, params->instrs, HLSL_OP2_DIV, sin, cos, loc);
}

static bool intrinsic_tanh(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_function_decl *func;
    struct hlsl_ir_node *arg;
    struct hlsl_type *type;
    char *body;

    static const char template[] =
            "%s tanh(%s x)\n"
            "{\n"
            "    %s exp_pos, exp_neg;\n"
            "    exp_pos = exp(x);\n"
            "    exp_neg = exp(-x);\n"
            "    return (exp_pos - exp_neg) / (exp_pos + exp_neg);\n"
            "}\n";

    if (!(arg = intrinsic_float_convert_arg(ctx, params, params->args[0], loc)))
        return false;
    type = arg->data_type;

    if (!(body = hlsl_sprintf_alloc(ctx, template,
            type->name, type->name, type->name)))
        return false;

    func = hlsl_compile_internal_function(ctx, "tanh", body);
    vkd3d_free(body);
    if (!func)
        return false;

    return !!add_user_call(ctx, func, params, false, loc);
}

static bool intrinsic_tex(struct hlsl_ctx *ctx, const struct parse_initializer *params,
        const struct vkd3d_shader_location *loc, const char *name, enum hlsl_sampler_dim dim)
{
    unsigned int sampler_dim = hlsl_sampler_dim_count(dim);
    struct hlsl_resource_load_params load_params = { 0 };
    const struct hlsl_type *sampler_type;
    struct hlsl_ir_node *coords, *sample;

    if (params->args_count != 2 && params->args_count != 4)
    {
        hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_WRONG_PARAMETER_COUNT,
                "Wrong number of arguments to function '%s': expected 2 or 4, but got %u.", name, params->args_count);
        return false;
    }

    sampler_type = params->args[0]->data_type;
    if (sampler_type->class != HLSL_CLASS_SAMPLER
            || (sampler_type->sampler_dim != dim && sampler_type->sampler_dim != HLSL_SAMPLER_DIM_GENERIC))
    {
        struct vkd3d_string_buffer *string;

        if ((string = hlsl_type_to_string(ctx, sampler_type)))
            hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_INVALID_TYPE,
                    "Wrong type for argument 1 of '%s': expected 'sampler' or '%s', but got '%s'.",
                    name, ctx->builtin_types.sampler[dim]->name, string->buffer);
        hlsl_release_string_buffer(ctx, string);
    }

    if (!strcmp(name, "tex2Dbias")
            || !strcmp(name, "tex2Dlod"))
    {
        struct hlsl_ir_node *lod, *c;

        if (!strcmp(name, "tex2Dlod"))
            load_params.type = HLSL_RESOURCE_SAMPLE_LOD;
        else
            load_params.type = HLSL_RESOURCE_SAMPLE_LOD_BIAS;

        if (!(c = hlsl_new_swizzle(ctx, HLSL_SWIZZLE(X, Y, Z, W), sampler_dim, params->args[1], loc)))
            return false;
        hlsl_block_add_instr(params->instrs, c);

        if (!(coords = add_implicit_conversion(ctx, params->instrs, c,
                hlsl_get_vector_type(ctx, HLSL_TYPE_FLOAT, sampler_dim), loc)))
        {
            return false;
        }

        if (!(lod = hlsl_new_swizzle(ctx, HLSL_SWIZZLE(W, W, W, W), 1, params->args[1], loc)))
            return false;
        hlsl_block_add_instr(params->instrs, lod);

        if (!(load_params.lod = add_implicit_conversion(ctx, params->instrs, lod,
                hlsl_get_scalar_type(ctx, HLSL_TYPE_FLOAT), loc)))
        {
            return false;
        }
    }
    else if (!strcmp(name, "tex2Dproj")
            || !strcmp(name, "tex3Dproj")
            || !strcmp(name, "texCUBEproj"))
    {
        if (!(coords = add_implicit_conversion(ctx, params->instrs, params->args[1],
                hlsl_get_vector_type(ctx, HLSL_TYPE_FLOAT, 4), loc)))
        {
            return false;
        }

        if (hlsl_version_ge(ctx, 4, 0))
        {
            struct hlsl_ir_node *divisor;

            if (!(divisor = hlsl_new_swizzle(ctx, HLSL_SWIZZLE(W, W, W, W), sampler_dim, coords, loc)))
                return false;
            hlsl_block_add_instr(params->instrs, divisor);

            if (!(coords = hlsl_new_swizzle(ctx, HLSL_SWIZZLE(X, Y, Z, W), sampler_dim, coords, loc)))
                return false;
            hlsl_block_add_instr(params->instrs, coords);

            if (!(coords = add_binary_arithmetic_expr(ctx, params->instrs, HLSL_OP2_DIV, coords, divisor, loc)))
                return false;

            load_params.type = HLSL_RESOURCE_SAMPLE;
        }
        else
        {
            load_params.type = HLSL_RESOURCE_SAMPLE_PROJ;
        }
    }
    else if (params->args_count == 4) /* Gradient sampling. */
    {
        if (!(coords = add_implicit_conversion(ctx, params->instrs, params->args[1],
                hlsl_get_vector_type(ctx, HLSL_TYPE_FLOAT, sampler_dim), loc)))
        {
            return false;
        }

        if (!(load_params.ddx = add_implicit_conversion(ctx, params->instrs, params->args[2],
                hlsl_get_vector_type(ctx, HLSL_TYPE_FLOAT, sampler_dim), loc)))
        {
            return false;
        }

        if (!(load_params.ddy = add_implicit_conversion(ctx, params->instrs, params->args[3],
                hlsl_get_vector_type(ctx, HLSL_TYPE_FLOAT, sampler_dim), loc)))
        {
            return false;
        }

        load_params.type = HLSL_RESOURCE_SAMPLE_GRAD;
    }
    else
    {
        load_params.type = HLSL_RESOURCE_SAMPLE;

        if (!(coords = add_implicit_conversion(ctx, params->instrs, params->args[1],
                hlsl_get_vector_type(ctx, HLSL_TYPE_FLOAT, sampler_dim), loc)))
        {
            return false;
        }
    }

    /* tex1D() functions never produce 1D resource declarations. For newer profiles half offset
       is used for the second coordinate, while older ones appear to replicate first coordinate.*/
    if (dim == HLSL_SAMPLER_DIM_1D)
    {
        struct hlsl_ir_load *load;
        struct hlsl_ir_node *half;
        struct hlsl_ir_var *var;
        unsigned int idx = 0;

        if (!(var = hlsl_new_synthetic_var(ctx, "coords", hlsl_get_vector_type(ctx, HLSL_TYPE_FLOAT, 2), loc)))
            return false;

        initialize_var_components(ctx, params->instrs, var, &idx, coords, false);
        if (hlsl_version_ge(ctx, 4, 0))
        {
            if (!(half = hlsl_new_float_constant(ctx, 0.5f, loc)))
                return false;
            hlsl_block_add_instr(params->instrs, half);

            initialize_var_components(ctx, params->instrs, var, &idx, half, false);
        }
        else
            initialize_var_components(ctx, params->instrs, var, &idx, coords, false);

        if (!(load = hlsl_new_var_load(ctx, var, loc)))
            return false;
        hlsl_block_add_instr(params->instrs, &load->node);

        coords = &load->node;

        dim = HLSL_SAMPLER_DIM_2D;
    }

    load_params.coords = coords;
    load_params.resource = params->args[0];
    load_params.format = hlsl_get_vector_type(ctx, HLSL_TYPE_FLOAT, 4);
    load_params.sampling_dim = dim;

    if (!(sample = hlsl_new_resource_load(ctx, &load_params, loc)))
        return false;
    hlsl_block_add_instr(params->instrs, sample);
    return true;
}

static bool intrinsic_tex1D(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    return intrinsic_tex(ctx, params, loc, "tex1D", HLSL_SAMPLER_DIM_1D);
}

static bool intrinsic_tex1Dgrad(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    return intrinsic_tex(ctx, params, loc, "tex1Dgrad", HLSL_SAMPLER_DIM_1D);
}

static bool intrinsic_tex2D(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    return intrinsic_tex(ctx, params, loc, "tex2D", HLSL_SAMPLER_DIM_2D);
}

static bool intrinsic_tex2Dbias(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    return intrinsic_tex(ctx, params, loc, "tex2Dbias", HLSL_SAMPLER_DIM_2D);
}

static bool intrinsic_tex2Dgrad(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    return intrinsic_tex(ctx, params, loc, "tex2Dgrad", HLSL_SAMPLER_DIM_2D);
}

static bool intrinsic_tex2Dlod(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    return intrinsic_tex(ctx, params, loc, "tex2Dlod", HLSL_SAMPLER_DIM_2D);
}

static bool intrinsic_tex2Dproj(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    return intrinsic_tex(ctx, params, loc, "tex2Dproj", HLSL_SAMPLER_DIM_2D);
}

static bool intrinsic_tex3D(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    return intrinsic_tex(ctx, params, loc, "tex3D", HLSL_SAMPLER_DIM_3D);
}

static bool intrinsic_tex3Dgrad(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    return intrinsic_tex(ctx, params, loc, "tex3Dgrad", HLSL_SAMPLER_DIM_3D);
}

static bool intrinsic_tex3Dproj(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    return intrinsic_tex(ctx, params, loc, "tex3Dproj", HLSL_SAMPLER_DIM_3D);
}

static bool intrinsic_texCUBE(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    return intrinsic_tex(ctx, params, loc, "texCUBE", HLSL_SAMPLER_DIM_CUBE);
}

static bool intrinsic_texCUBEgrad(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    return intrinsic_tex(ctx, params, loc, "texCUBEgrad", HLSL_SAMPLER_DIM_CUBE);
}

static bool intrinsic_texCUBEproj(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    return intrinsic_tex(ctx, params, loc, "texCUBEproj", HLSL_SAMPLER_DIM_CUBE);
}

static bool intrinsic_transpose(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_node *arg = params->args[0];
    struct hlsl_type *arg_type = arg->data_type;
    struct hlsl_ir_load *var_load;
    struct hlsl_deref var_deref;
    struct hlsl_type *mat_type;
    struct hlsl_ir_node *load;
    struct hlsl_ir_var *var;
    unsigned int i, j;

    if (arg_type->class != HLSL_CLASS_SCALAR && arg_type->class != HLSL_CLASS_MATRIX)
    {
        struct vkd3d_string_buffer *string;

        if ((string = hlsl_type_to_string(ctx, arg_type)))
            hlsl_error(ctx, &arg->loc, VKD3D_SHADER_ERROR_HLSL_INVALID_TYPE,
                   "Wrong type for argument 1 of transpose(): expected a matrix or scalar type, but got '%s'.",
                   string->buffer);
        hlsl_release_string_buffer(ctx, string);
        return false;
    }

    if (arg_type->class == HLSL_CLASS_SCALAR)
    {
        hlsl_block_add_instr(params->instrs, arg);
        return true;
    }

    mat_type = hlsl_get_matrix_type(ctx, arg_type->e.numeric.type, arg_type->dimy, arg_type->dimx);

    if (!(var = hlsl_new_synthetic_var(ctx, "transpose", mat_type, loc)))
        return false;
    hlsl_init_simple_deref_from_var(&var_deref, var);

    for (i = 0; i < arg_type->dimx; ++i)
    {
        for (j = 0; j < arg_type->dimy; ++j)
        {
            struct hlsl_block block;

            if (!(load = hlsl_add_load_component(ctx, params->instrs, arg, j * arg->data_type->dimx + i, loc)))
                return false;

            if (!hlsl_new_store_component(ctx, &block, &var_deref, i * var->data_type->dimx + j, load))
                return false;
            hlsl_block_add_block(params->instrs, &block);
        }
    }

    if (!(var_load = hlsl_new_var_load(ctx, var, loc)))
        return false;
    hlsl_block_add_instr(params->instrs, &var_load->node);

    return true;
}

static bool intrinsic_trunc(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_node *arg;

    if (!(arg = intrinsic_float_convert_arg(ctx, params, params->args[0], loc)))
        return false;

    return !!add_unary_arithmetic_expr(ctx, params->instrs, HLSL_OP1_TRUNC, arg, loc);
}

static bool intrinsic_d3dcolor_to_ubyte4(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_node *arg = params->args[0], *ret, *c, *swizzle;
    struct hlsl_type *arg_type = arg->data_type;

    if (arg_type->class != HLSL_CLASS_SCALAR && !(arg_type->class == HLSL_CLASS_VECTOR && arg_type->dimx == 4))
    {
        struct vkd3d_string_buffer *string;

        if ((string = hlsl_type_to_string(ctx, arg_type)))
        {
            hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_INVALID_TYPE, "Wrong argument type '%s'.", string->buffer);
            hlsl_release_string_buffer(ctx, string);
        }

        return false;
    }

    if (!(arg = intrinsic_float_convert_arg(ctx, params, arg, loc)))
        return false;

    if (!(c = hlsl_new_float_constant(ctx, 255.0f + (0.5f / 256.0f), loc)))
        return false;
    hlsl_block_add_instr(params->instrs, c);

    if (arg_type->class == HLSL_CLASS_VECTOR)
    {
        if (!(swizzle = hlsl_new_swizzle(ctx, HLSL_SWIZZLE(Z, Y, X, W), 4, arg, loc)))
            return false;
        hlsl_block_add_instr(params->instrs, swizzle);

        arg = swizzle;
    }

    if (!(ret = add_binary_arithmetic_expr(ctx, params->instrs, HLSL_OP2_MUL, arg, c, loc)))
        return false;

    if (hlsl_version_ge(ctx, 4, 0))
        return !!add_unary_arithmetic_expr(ctx, params->instrs, HLSL_OP1_TRUNC, ret, loc);

    return true;
}

static bool intrinsic_GetRenderTargetSampleCount(struct hlsl_ctx *ctx,
        const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_node *operands[HLSL_MAX_OPERANDS] = {0};
    struct hlsl_ir_node *expr;

    if (ctx->profile->type != VKD3D_SHADER_TYPE_PIXEL || hlsl_version_lt(ctx, 4, 1))
        hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_INCOMPATIBLE_PROFILE,
                "GetRenderTargetSampleCount() can only be used from a pixel shader using version 4.1 or higher.");

    if (!(expr = hlsl_new_expr(ctx, HLSL_OP0_RASTERIZER_SAMPLE_COUNT,
            operands, hlsl_get_scalar_type(ctx, HLSL_TYPE_UINT), loc)))
        return false;
    hlsl_block_add_instr(params->instrs, expr);

    return true;
}

static const struct intrinsic_function
{
    const char *name;
    int param_count;
    bool check_numeric;
    bool (*handler)(struct hlsl_ctx *ctx, const struct parse_initializer *params,
            const struct vkd3d_shader_location *loc);
}
intrinsic_functions[] =
{
    /* Note: these entries should be kept in alphabetical order. */
    {"D3DCOLORtoUBYTE4",                    1, true,  intrinsic_d3dcolor_to_ubyte4},
    {"GetRenderTargetSampleCount",          0, true,  intrinsic_GetRenderTargetSampleCount},
    {"abs",                                 1, true,  intrinsic_abs},
    {"acos",                                1, true,  intrinsic_acos},
    {"all",                                 1, true,  intrinsic_all},
    {"any",                                 1, true,  intrinsic_any},
    {"asfloat",                             1, true,  intrinsic_asfloat},
    {"asin",                                1, true,  intrinsic_asin},
    {"asint",                               1, true,  intrinsic_asint},
    {"asuint",                             -1, true,  intrinsic_asuint},
    {"atan",                                1, true,  intrinsic_atan},
    {"atan2",                               2, true,  intrinsic_atan2},
    {"ceil",                                1, true,  intrinsic_ceil},
    {"clamp",                               3, true,  intrinsic_clamp},
    {"clip",                                1, true,  intrinsic_clip},
    {"cos",                                 1, true,  intrinsic_cos},
    {"cosh",                                1, true,  intrinsic_cosh},
    {"cross",                               2, true,  intrinsic_cross},
    {"ddx",                                 1, true,  intrinsic_ddx},
    {"ddx_coarse",                          1, true,  intrinsic_ddx_coarse},
    {"ddx_fine",                            1, true,  intrinsic_ddx_fine},
    {"ddy",                                 1, true,  intrinsic_ddy},
    {"ddy_coarse",                          1, true,  intrinsic_ddy_coarse},
    {"ddy_fine",                            1, true,  intrinsic_ddy_fine},
    {"degrees",                             1, true,  intrinsic_degrees},
    {"determinant",                         1, true,  intrinsic_determinant},
    {"distance",                            2, true,  intrinsic_distance},
    {"dot",                                 2, true,  intrinsic_dot},
    {"dst",                                 2, true,  intrinsic_dst},
    {"exp",                                 1, true,  intrinsic_exp},
    {"exp2",                                1, true,  intrinsic_exp2},
    {"f16tof32",                            1, true,  intrinsic_f16tof32},
    {"f32tof16",                            1, true,  intrinsic_f32tof16},
    {"faceforward",                         3, true,  intrinsic_faceforward},
    {"floor",                               1, true,  intrinsic_floor},
    {"fmod",                                2, true,  intrinsic_fmod},
    {"frac",                                1, true,  intrinsic_frac},
    {"fwidth",                              1, true,  intrinsic_fwidth},
    {"ldexp",                               2, true,  intrinsic_ldexp},
    {"length",                              1, true,  intrinsic_length},
    {"lerp",                                3, true,  intrinsic_lerp},
    {"lit",                                 3, true,  intrinsic_lit},
    {"log",                                 1, true,  intrinsic_log},
    {"log10",                               1, true,  intrinsic_log10},
    {"log2",                                1, true,  intrinsic_log2},
    {"mad",                                 3, true,  intrinsic_mad},
    {"max",                                 2, true,  intrinsic_max},
    {"min",                                 2, true,  intrinsic_min},
    {"modf",                                2, true,  intrinsic_modf},
    {"mul",                                 2, true,  intrinsic_mul},
    {"normalize",                           1, true,  intrinsic_normalize},
    {"pow",                                 2, true,  intrinsic_pow},
    {"radians",                             1, true,  intrinsic_radians},
    {"rcp",                                 1, true,  intrinsic_rcp},
    {"reflect",                             2, true,  intrinsic_reflect},
    {"refract",                             3, true,  intrinsic_refract},
    {"round",                               1, true,  intrinsic_round},
    {"rsqrt",                               1, true,  intrinsic_rsqrt},
    {"saturate",                            1, true,  intrinsic_saturate},
    {"sign",                                1, true,  intrinsic_sign},
    {"sin",                                 1, true,  intrinsic_sin},
    {"sincos",                              3, true,  intrinsic_sincos},
    {"sinh",                                1, true,  intrinsic_sinh},
    {"smoothstep",                          3, true,  intrinsic_smoothstep},
    {"sqrt",                                1, true,  intrinsic_sqrt},
    {"step",                                2, true,  intrinsic_step},
    {"tan",                                 1, true,  intrinsic_tan},
    {"tanh",                                1, true,  intrinsic_tanh},
    {"tex1D",                              -1, false, intrinsic_tex1D},
    {"tex1Dgrad",                           4, false, intrinsic_tex1Dgrad},
    {"tex2D",                              -1, false, intrinsic_tex2D},
    {"tex2Dbias",                           2, false, intrinsic_tex2Dbias},
    {"tex2Dgrad",                           4, false, intrinsic_tex2Dgrad},
    {"tex2Dlod",                            2, false, intrinsic_tex2Dlod},
    {"tex2Dproj",                           2, false, intrinsic_tex2Dproj},
    {"tex3D",                              -1, false, intrinsic_tex3D},
    {"tex3Dgrad",                           4, false, intrinsic_tex3Dgrad},
    {"tex3Dproj",                           2, false, intrinsic_tex3Dproj},
    {"texCUBE",                            -1, false, intrinsic_texCUBE},
    {"texCUBEgrad",                         4, false, intrinsic_texCUBEgrad},
    {"texCUBEproj",                         2, false, intrinsic_texCUBEproj},
    {"transpose",                           1, true,  intrinsic_transpose},
    {"trunc",                               1, true,  intrinsic_trunc},
};

static int intrinsic_function_name_compare(const void *a, const void *b)
{
    const struct intrinsic_function *func = b;

    return strcmp(a, func->name);
}

static struct hlsl_block *add_call(struct hlsl_ctx *ctx, const char *name,
        struct parse_initializer *args, const struct vkd3d_shader_location *loc)
{
    struct intrinsic_function *intrinsic;
    struct hlsl_ir_function_decl *decl;

    for (unsigned int i = 0; i < args->args_count; ++i)
    {
        if (args->args[i]->data_type->class == HLSL_CLASS_ERROR)
        {
            args->instrs->value = ctx->error_instr;
            return args->instrs;
        }
    }

    if ((decl = find_function_call(ctx, name, args, false, loc)))
    {
        if (!add_user_call(ctx, decl, args, false, loc))
            goto fail;
    }
    else if ((intrinsic = bsearch(name, intrinsic_functions, ARRAY_SIZE(intrinsic_functions),
            sizeof(*intrinsic_functions), intrinsic_function_name_compare)))
    {
        if (intrinsic->param_count >= 0 && args->args_count != intrinsic->param_count)
        {
            hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_WRONG_PARAMETER_COUNT,
                    "Wrong number of arguments to function '%s': expected %u, but got %u.",
                    name, intrinsic->param_count, args->args_count);
            goto fail;
        }

        if (intrinsic->check_numeric)
        {
            unsigned int i;

            for (i = 0; i < args->args_count; ++i)
            {
                if (!hlsl_is_numeric_type(args->args[i]->data_type))
                {
                    struct vkd3d_string_buffer *string;

                    if ((string = hlsl_type_to_string(ctx, args->args[i]->data_type)))
                        hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_INVALID_TYPE,
                                "Wrong type for argument %u of '%s': expected a numeric type, but got '%s'.",
                                i + 1, name, string->buffer);
                    hlsl_release_string_buffer(ctx, string);
                    goto fail;
                }
            }
        }

        if (!intrinsic->handler(ctx, args, loc))
            goto fail;
    }
    else if (rb_get(&ctx->functions, name))
    {
        hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_NOT_DEFINED, "No compatible %u parameter declaration for \"%s\" found.",
                args->args_count, name);
        goto fail;
    }
    else
    {
        hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_NOT_DEFINED, "Function \"%s\" is not defined.", name);
        goto fail;
    }
    vkd3d_free(args->args);
    return args->instrs;

fail:
    free_parse_initializer(args);
    return NULL;
}

static struct hlsl_block *add_shader_compilation(struct hlsl_ctx *ctx, const char *profile_name,
        const char *function_name, struct parse_initializer *args, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_node *compile, *call_to_compile = NULL;
    struct hlsl_ir_function_decl *decl;

    if (!ctx->in_state_block && ctx->cur_scope != ctx->globals)
    {
        hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_MISPLACED_COMPILE,
                "Shader compilation statements must be in global scope or a state block.");
        free_parse_initializer(args);
        return NULL;
    }

    if (!(decl = find_function_call(ctx, function_name, args, true, loc)))
    {
        if (rb_get(&ctx->functions, function_name))
        {
            hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_NOT_DEFINED,
                    "No compatible \"%s\" declaration with %u uniform parameters found.",
                    function_name, args->args_count);
        }
        else
        {
            hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_NOT_DEFINED,
                    "Function \"%s\" is not defined.", function_name);
        }
        free_parse_initializer(args);
        return NULL;
    }

    if (!(call_to_compile = add_user_call(ctx, decl, args, true, loc)))
    {
        free_parse_initializer(args);
        return NULL;
    }

    if (!(compile = hlsl_new_compile(ctx, HLSL_COMPILE_TYPE_COMPILE,
            profile_name, &call_to_compile, 1, args->instrs, loc)))
    {
        free_parse_initializer(args);
        return NULL;
    }

    free_parse_initializer(args);
    return make_block(ctx, compile);
}

static struct hlsl_block *add_compile_variant(struct hlsl_ctx *ctx, enum hlsl_compile_type compile_type,
        struct parse_initializer *args, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_node *compile;

    switch (compile_type)
    {
        case HLSL_COMPILE_TYPE_COMPILE:
            vkd3d_unreachable();

        case HLSL_COMPILE_TYPE_CONSTRUCTGSWITHSO:
            if (args->args_count != 2 && args->args_count != 6)
            {
                hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_WRONG_PARAMETER_COUNT,
                        "Wrong number of arguments to ConstructGSWithSO: expected 2 or 6, but got %u.",
                        args->args_count);
            }
            break;
    }

    if (!(compile = hlsl_new_compile(ctx, compile_type, NULL, args->args, args->args_count, args->instrs, loc)))
    {
        free_parse_initializer(args);
        return NULL;
    }

    free_parse_initializer(args);
    return make_block(ctx, compile);
}

static struct hlsl_block *add_constructor(struct hlsl_ctx *ctx, struct hlsl_type *type,
        struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_load *load;
    struct hlsl_ir_var *var;

    if (!(var = hlsl_new_synthetic_var(ctx, "constructor", type, loc)))
        return NULL;

    initialize_var(ctx, var, params, false);

    if (!(load = hlsl_new_var_load(ctx, var, loc)))
        return NULL;
    hlsl_block_add_instr(params->instrs, &load->node);

    vkd3d_free(params->args);
    return params->instrs;
}

static bool add_ternary(struct hlsl_ctx *ctx, struct hlsl_block *block,
        struct hlsl_ir_node *cond, struct hlsl_ir_node *first, struct hlsl_ir_node *second)
{
    struct hlsl_ir_node *args[HLSL_MAX_OPERANDS] = {0};
    struct hlsl_type *cond_type = cond->data_type;
    struct hlsl_type *common_type;

    if (cond->data_type->class == HLSL_CLASS_ERROR
            || first->data_type->class == HLSL_CLASS_ERROR
            || second->data_type->class == HLSL_CLASS_ERROR)
    {
        block->value = ctx->error_instr;
        return true;
    }

    if (cond_type->class > HLSL_CLASS_LAST_NUMERIC)
    {
        struct vkd3d_string_buffer *string;

        if ((string = hlsl_type_to_string(ctx, cond_type)))
            hlsl_error(ctx, &cond->loc, VKD3D_SHADER_ERROR_HLSL_INVALID_TYPE,
                    "Ternary condition type '%s' is not numeric.", string->buffer);
        hlsl_release_string_buffer(ctx, string);
    }

    if (first->data_type->class <= HLSL_CLASS_LAST_NUMERIC
            && second->data_type->class <= HLSL_CLASS_LAST_NUMERIC)
    {
        if (!(common_type = get_common_numeric_type(ctx, first, second, &first->loc)))
            return false;

        if (cond_type->dimx == 1 && cond_type->dimy == 1)
        {
            cond_type = hlsl_get_numeric_type(ctx, common_type->class,
                    HLSL_TYPE_BOOL, common_type->dimx, common_type->dimy);
            if (!(cond = add_implicit_conversion(ctx, block, cond, cond_type, &cond->loc)))
                return false;
        }
        else
        {
            if (common_type->dimx == 1 && common_type->dimy == 1)
            {
                common_type = hlsl_get_numeric_type(ctx, cond_type->class,
                        common_type->e.numeric.type, cond_type->dimx, cond_type->dimy);
            }
            else if (cond_type->dimx != common_type->dimx || cond_type->dimy != common_type->dimy)
            {
                /* This condition looks wrong but is correct.
                * floatN is compatible with float1xN, but not with floatNx1. */

                struct vkd3d_string_buffer *cond_string, *value_string;

                cond_string = hlsl_type_to_string(ctx, cond_type);
                value_string = hlsl_type_to_string(ctx, common_type);
                if (cond_string && value_string)
                    hlsl_error(ctx, &first->loc, VKD3D_SHADER_ERROR_HLSL_INVALID_TYPE,
                            "Ternary condition type '%s' is not compatible with value type '%s'.",
                            cond_string->buffer, value_string->buffer);
                hlsl_release_string_buffer(ctx, cond_string);
                hlsl_release_string_buffer(ctx, value_string);
            }

            cond_type = hlsl_get_numeric_type(ctx, common_type->class, HLSL_TYPE_BOOL,
                    common_type->dimx, common_type->dimy);
            if (!(cond = add_implicit_conversion(ctx, block, cond, cond_type, &cond->loc)))
                return false;
        }

        if (!(first = add_implicit_conversion(ctx, block, first, common_type, &first->loc)))
            return false;

        if (!(second = add_implicit_conversion(ctx, block, second, common_type, &second->loc)))
            return false;
    }
    else
    {
        struct vkd3d_string_buffer *first_string, *second_string;

        if (!hlsl_types_are_equal(first->data_type, second->data_type))
        {
            first_string = hlsl_type_to_string(ctx, first->data_type);
            second_string = hlsl_type_to_string(ctx, second->data_type);
            if (first_string && second_string)
                hlsl_error(ctx, &first->loc, VKD3D_SHADER_ERROR_HLSL_INVALID_TYPE,
                        "Ternary argument types '%s' and '%s' do not match.",
                        first_string->buffer, second_string->buffer);
            hlsl_release_string_buffer(ctx, first_string);
            hlsl_release_string_buffer(ctx, second_string);
        }

        cond_type = hlsl_get_numeric_type(ctx, cond_type->class, HLSL_TYPE_BOOL,
                cond_type->dimx, cond_type->dimy);
        if (!(cond = add_implicit_conversion(ctx, block, cond, cond_type, &cond->loc)))
            return false;

        common_type = first->data_type;
    }

    VKD3D_ASSERT(cond->data_type->e.numeric.type == HLSL_TYPE_BOOL);

    args[0] = cond;
    args[1] = first;
    args[2] = second;
    return add_expr(ctx, block, HLSL_OP3_TERNARY, args, common_type, &first->loc);
}

static unsigned int hlsl_offset_dim_count(enum hlsl_sampler_dim dim)
{
    switch (dim)
    {
        case HLSL_SAMPLER_DIM_1D:
        case HLSL_SAMPLER_DIM_1DARRAY:
            return 1;
        case HLSL_SAMPLER_DIM_2D:
        case HLSL_SAMPLER_DIM_2DMS:
        case HLSL_SAMPLER_DIM_2DARRAY:
        case HLSL_SAMPLER_DIM_2DMSARRAY:
            return 2;
        case HLSL_SAMPLER_DIM_3D:
            return 3;
        case HLSL_SAMPLER_DIM_CUBE:
        case HLSL_SAMPLER_DIM_CUBEARRAY:
        case HLSL_SAMPLER_DIM_BUFFER:
        case HLSL_SAMPLER_DIM_RAW_BUFFER:
            /* Offset parameters not supported for these types. */
            return 0;
        default:
            vkd3d_unreachable();
    }
}

static bool raise_invalid_method_object_type(struct hlsl_ctx *ctx, const struct hlsl_type *object_type,
        const char *method, const struct vkd3d_shader_location *loc)
{
    struct vkd3d_string_buffer *string;

    if ((string = hlsl_type_to_string(ctx, object_type)))
        hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_NOT_DEFINED,
                "Method '%s' is not defined on type '%s'.", method, string->buffer);
    hlsl_release_string_buffer(ctx, string);
    return false;
}

static bool add_raw_load_method_call(struct hlsl_ctx *ctx, struct hlsl_block *block, struct hlsl_ir_node *object,
        const char *name, const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    struct hlsl_resource_load_params load_params = {.type = HLSL_RESOURCE_LOAD};
    struct hlsl_ir_node *load;
    unsigned int value_dim;

    if (params->args_count != 1 && params->args_count != 2)
    {
        hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_WRONG_PARAMETER_COUNT,
                "Wrong number of arguments to method 'Load': expected between 1 and 2, but got %u.",
                params->args_count);
        return false;
    }

    if (params->args_count == 2)
    {
        hlsl_fixme(ctx, loc, "Tiled resource status argument.");
        return false;
    }

    if (params->args[0]->data_type->class != HLSL_CLASS_SCALAR)
    {
        hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_INVALID_TYPE, "Scalar address argument expected for '%s'.", name);
        return false;
    }

    if (!strcmp(name, "Load"))
        value_dim = 1;
    else if (!strcmp(name, "Load2"))
        value_dim = 2;
    else if (!strcmp(name, "Load3"))
        value_dim = 3;
    else
        value_dim = 4;

    if (!(load_params.coords = add_implicit_conversion(ctx, block, params->args[0],
            hlsl_get_scalar_type(ctx, HLSL_TYPE_UINT), loc)))
        return false;

    load_params.format = hlsl_get_vector_type(ctx, HLSL_TYPE_UINT, value_dim);
    load_params.resource = object;

    if (!(load = hlsl_new_resource_load(ctx, &load_params, loc)))
        return false;
    hlsl_block_add_instr(block, load);
    return true;
}

static bool add_load_method_call(struct hlsl_ctx *ctx, struct hlsl_block *block, struct hlsl_ir_node *object,
        const char *name, const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    const struct hlsl_type *object_type = object->data_type;
    struct hlsl_resource_load_params load_params = {.type = HLSL_RESOURCE_LOAD};
    unsigned int sampler_dim, offset_dim;
    struct hlsl_ir_node *load;
    bool multisampled;

    if (object_type->sampler_dim == HLSL_SAMPLER_DIM_RAW_BUFFER)
        return add_raw_load_method_call(ctx, block, object, name, params, loc);

    if (object_type->sampler_dim == HLSL_SAMPLER_DIM_STRUCTURED_BUFFER)
    {
        hlsl_fixme(ctx, loc, "Method '%s' for structured buffers.", name);
        return false;
    }

    sampler_dim = hlsl_sampler_dim_count(object_type->sampler_dim);
    offset_dim = hlsl_offset_dim_count(object_type->sampler_dim);

    multisampled = object_type->sampler_dim == HLSL_SAMPLER_DIM_2DMS
            || object_type->sampler_dim == HLSL_SAMPLER_DIM_2DMSARRAY;

    if (params->args_count < 1 + multisampled || params->args_count > 2 + multisampled + !!offset_dim)
    {
        hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_WRONG_PARAMETER_COUNT,
                "Wrong number of arguments to method 'Load': expected between %u and %u, but got %u.",
                1 + multisampled, 2 + multisampled + !!offset_dim, params->args_count);
        return false;
    }

    if (multisampled)
    {
        if (!(load_params.sample_index = add_implicit_conversion(ctx, block, params->args[1],
                hlsl_get_scalar_type(ctx, HLSL_TYPE_INT), loc)))
            return false;
    }

    if (!!offset_dim && params->args_count > 1 + multisampled)
    {
        if (!(load_params.texel_offset = add_implicit_conversion(ctx, block, params->args[1 + multisampled],
                hlsl_get_vector_type(ctx, HLSL_TYPE_INT, offset_dim), loc)))
            return false;
    }

    if (params->args_count > 1 + multisampled + !!offset_dim)
    {
        hlsl_fixme(ctx, loc, "Tiled resource status argument.");
    }

    /* +1 for the mipmap level for non-multisampled textures */
    if (!(load_params.coords = add_implicit_conversion(ctx, block, params->args[0],
            hlsl_get_vector_type(ctx, HLSL_TYPE_INT, sampler_dim + !multisampled), loc)))
        return false;

    load_params.format = object_type->e.resource.format;
    load_params.resource = object;

    if (!(load = hlsl_new_resource_load(ctx, &load_params, loc)))
        return false;
    hlsl_block_add_instr(block, load);
    return true;
}

static bool add_sample_method_call(struct hlsl_ctx *ctx, struct hlsl_block *block, struct hlsl_ir_node *object,
        const char *name, const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    const struct hlsl_type *object_type = object->data_type;
    struct hlsl_resource_load_params load_params = {.type = HLSL_RESOURCE_SAMPLE};
    unsigned int sampler_dim, offset_dim;
    const struct hlsl_type *sampler_type;
    struct hlsl_ir_node *load;

    sampler_dim = hlsl_sampler_dim_count(object_type->sampler_dim);
    offset_dim = hlsl_offset_dim_count(object_type->sampler_dim);

    if (params->args_count < 2 || params->args_count > 4 + !!offset_dim)
    {
        hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_WRONG_PARAMETER_COUNT,
                "Wrong number of arguments to method 'Sample': expected from 2 to %u, but got %u.",
                4 + !!offset_dim, params->args_count);
        return false;
    }

    sampler_type = params->args[0]->data_type;
    if (sampler_type->class != HLSL_CLASS_SAMPLER || sampler_type->sampler_dim != HLSL_SAMPLER_DIM_GENERIC)
    {
        struct vkd3d_string_buffer *string;

        if ((string = hlsl_type_to_string(ctx, sampler_type)))
            hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_INVALID_TYPE,
                    "Wrong type for argument 0 of Sample(): expected 'sampler', but got '%s'.", string->buffer);
        hlsl_release_string_buffer(ctx, string);
        return false;
    }

    if (!(load_params.coords = add_implicit_conversion(ctx, block, params->args[1],
            hlsl_get_vector_type(ctx, HLSL_TYPE_FLOAT, sampler_dim), loc)))
        return false;

    if (offset_dim && params->args_count > 2)
    {
        if (!(load_params.texel_offset = add_implicit_conversion(ctx, block, params->args[2],
                hlsl_get_vector_type(ctx, HLSL_TYPE_INT, offset_dim), loc)))
            return false;
    }

    if (params->args_count > 2 + !!offset_dim)
        hlsl_fixme(ctx, loc, "Sample() clamp parameter.");
    if (params->args_count > 3 + !!offset_dim)
        hlsl_fixme(ctx, loc, "Tiled resource status argument.");

    load_params.format = object_type->e.resource.format;
    load_params.resource = object;
    load_params.sampler = params->args[0];

    if (!(load = hlsl_new_resource_load(ctx, &load_params, loc)))
        return false;
    hlsl_block_add_instr(block, load);

    return true;
}

static bool add_sample_cmp_method_call(struct hlsl_ctx *ctx, struct hlsl_block *block, struct hlsl_ir_node *object,
        const char *name, const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    const struct hlsl_type *object_type = object->data_type;
    struct hlsl_resource_load_params load_params = { 0 };
    unsigned int sampler_dim, offset_dim;
    const struct hlsl_type *sampler_type;
    struct hlsl_ir_node *load;

    sampler_dim = hlsl_sampler_dim_count(object_type->sampler_dim);
    offset_dim = hlsl_offset_dim_count(object_type->sampler_dim);

    if (!strcmp(name, "SampleCmpLevelZero"))
        load_params.type = HLSL_RESOURCE_SAMPLE_CMP_LZ;
    else
        load_params.type = HLSL_RESOURCE_SAMPLE_CMP;

    if (params->args_count < 3 || params->args_count > 5 + !!offset_dim)
    {
        hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_WRONG_PARAMETER_COUNT,
                "Wrong number of arguments to method '%s': expected from 3 to %u, but got %u.",
                name, 5 + !!offset_dim, params->args_count);
        return false;
    }

    sampler_type = params->args[0]->data_type;
    if (sampler_type->class != HLSL_CLASS_SAMPLER || sampler_type->sampler_dim != HLSL_SAMPLER_DIM_COMPARISON)
    {
        struct vkd3d_string_buffer *string;

        if ((string = hlsl_type_to_string(ctx, sampler_type)))
            hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_INVALID_TYPE,
                    "Wrong type for argument 0 of %s(): expected 'SamplerComparisonState', but got '%s'.",
                    name, string->buffer);
        hlsl_release_string_buffer(ctx, string);
        return false;
    }

    if (!(load_params.coords = add_implicit_conversion(ctx, block, params->args[1],
            hlsl_get_vector_type(ctx, HLSL_TYPE_FLOAT, sampler_dim), loc)))
        return false;

    if (!(load_params.cmp = add_implicit_conversion(ctx, block, params->args[2],
            hlsl_get_scalar_type(ctx, HLSL_TYPE_FLOAT), loc)))
        load_params.cmp = params->args[2];

    if (offset_dim && params->args_count > 3)
    {
        if (!(load_params.texel_offset = add_implicit_conversion(ctx, block, params->args[2],
                hlsl_get_vector_type(ctx, HLSL_TYPE_INT, offset_dim), loc)))
            return false;
    }

    if (params->args_count > 3 + !!offset_dim)
        hlsl_fixme(ctx, loc, "%s() clamp parameter.", name);
    if (params->args_count > 4 + !!offset_dim)
        hlsl_fixme(ctx, loc, "Tiled resource status argument.");

    load_params.format = object_type->e.resource.format;
    load_params.resource = object;
    load_params.sampler = params->args[0];

    if (!(load = hlsl_new_resource_load(ctx, &load_params, loc)))
        return false;
    hlsl_block_add_instr(block, load);

    return true;
}

static bool add_gather_method_call(struct hlsl_ctx *ctx, struct hlsl_block *block, struct hlsl_ir_node *object,
        const char *name, const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    const struct hlsl_type *object_type = object->data_type;
    struct hlsl_resource_load_params load_params = {0};
    unsigned int sampler_dim, offset_dim;
    const struct hlsl_type *sampler_type;
    struct hlsl_ir_node *load;
    unsigned int read_channel;

    sampler_dim = hlsl_sampler_dim_count(object_type->sampler_dim);
    offset_dim = hlsl_offset_dim_count(object_type->sampler_dim);

    if (!strcmp(name, "GatherGreen"))
    {
        load_params.type = HLSL_RESOURCE_GATHER_GREEN;
        read_channel = 1;
    }
    else if (!strcmp(name, "GatherBlue"))
    {
        load_params.type = HLSL_RESOURCE_GATHER_BLUE;
        read_channel = 2;
    }
    else if (!strcmp(name, "GatherAlpha"))
    {
        load_params.type = HLSL_RESOURCE_GATHER_ALPHA;
        read_channel = 3;
    }
    else
    {
        load_params.type = HLSL_RESOURCE_GATHER_RED;
        read_channel = 0;
    }

    if (!strcmp(name, "Gather") || !offset_dim)
    {
        if (params->args_count < 2 || params->args_count > 3 + !!offset_dim)
        {
            hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_WRONG_PARAMETER_COUNT,
                    "Wrong number of arguments to method '%s': expected from 2 to %u, but got %u.",
                    name, 3 + !!offset_dim, params->args_count);
            return false;
        }
    }
    else if (params->args_count < 2 || params->args_count == 5 || params->args_count > 7)
    {
        hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_WRONG_PARAMETER_COUNT,
                "Wrong number of arguments to method '%s': expected 2, 3, 4, 6 or 7, but got %u.",
                name, params->args_count);
        return false;
    }

    if (params->args_count == 3 + !!offset_dim || params->args_count == 7)
        hlsl_fixme(ctx, loc, "Tiled resource status argument.");

    if (params->args_count == 6 || params->args_count == 7)
    {
        hlsl_fixme(ctx, loc, "Multiple %s() offset parameters.", name);
    }
    else if (offset_dim && params->args_count > 2)
    {
        if (!(load_params.texel_offset = add_implicit_conversion(ctx, block, params->args[2],
                hlsl_get_vector_type(ctx, HLSL_TYPE_INT, offset_dim), loc)))
            return false;
    }

    sampler_type = params->args[0]->data_type;
    if (sampler_type->class != HLSL_CLASS_SAMPLER || sampler_type->sampler_dim != HLSL_SAMPLER_DIM_GENERIC)
    {
        struct vkd3d_string_buffer *string;

        if ((string = hlsl_type_to_string(ctx, sampler_type)))
            hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_INVALID_TYPE,
                    "Wrong type for argument 1 of %s(): expected 'sampler', but got '%s'.", name, string->buffer);
        hlsl_release_string_buffer(ctx, string);
        return false;
    }

    if (read_channel >= object_type->e.resource.format->dimx)
    {
        hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_INVALID_TYPE,
                "Method %s() requires at least %u channels.", name, read_channel + 1);
        return false;
    }

    if (!(load_params.coords = add_implicit_conversion(ctx, block, params->args[1],
            hlsl_get_vector_type(ctx, HLSL_TYPE_FLOAT, sampler_dim), loc)))
        return false;

    load_params.format = hlsl_get_vector_type(ctx, object_type->e.resource.format->e.numeric.type, 4);
    load_params.resource = object;
    load_params.sampler = params->args[0];

    if (!(load = hlsl_new_resource_load(ctx, &load_params, loc)))
        return false;
    hlsl_block_add_instr(block, load);
    return true;
}

static bool add_assignment_from_component(struct hlsl_ctx *ctx, struct hlsl_block *instrs, struct hlsl_ir_node *dest,
        struct hlsl_ir_node *src, unsigned int component, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_node *load;

    if (!dest)
        return true;

    if (!(load = hlsl_add_load_component(ctx, instrs, src, component, loc)))
        return false;

    if (!add_assignment(ctx, instrs, dest, ASSIGN_OP_ASSIGN, load))
        return false;

    return true;
}

static bool add_getdimensions_method_call(struct hlsl_ctx *ctx, struct hlsl_block *block, struct hlsl_ir_node *object,
        const char *name, const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    const struct hlsl_type *object_type = object->data_type;
    bool uint_resinfo, has_uint_arg, has_float_arg;
    struct hlsl_resource_load_params load_params;
    struct hlsl_ir_node *sample_info, *res_info;
    struct hlsl_ir_node *zero = NULL, *void_ret;
    struct hlsl_type *uint_type, *float_type;
    unsigned int i, j;
    enum func_argument
    {
        ARG_MIP_LEVEL,
        ARG_WIDTH,
        ARG_HEIGHT,
        ARG_ELEMENT_COUNT,
        ARG_LEVEL_COUNT,
        ARG_SAMPLE_COUNT,
        ARG_MAX_ARGS,
    };
    struct hlsl_ir_node *args[ARG_MAX_ARGS] = { 0 };
    static const struct overload
    {
        enum hlsl_sampler_dim sampler_dim;
        unsigned int args_count;
        enum func_argument args[ARG_MAX_ARGS];
    }
    overloads[] =
    {
        { HLSL_SAMPLER_DIM_1D, 1, { ARG_WIDTH } },
        { HLSL_SAMPLER_DIM_1D, 3, { ARG_MIP_LEVEL, ARG_WIDTH, ARG_LEVEL_COUNT } },
        { HLSL_SAMPLER_DIM_1DARRAY, 2, { ARG_WIDTH, ARG_ELEMENT_COUNT } },
        { HLSL_SAMPLER_DIM_1DARRAY, 4, { ARG_MIP_LEVEL, ARG_WIDTH, ARG_ELEMENT_COUNT, ARG_LEVEL_COUNT } },
        { HLSL_SAMPLER_DIM_2D, 2, { ARG_WIDTH, ARG_HEIGHT } },
        { HLSL_SAMPLER_DIM_2D, 4, { ARG_MIP_LEVEL, ARG_WIDTH, ARG_HEIGHT, ARG_LEVEL_COUNT } },
        { HLSL_SAMPLER_DIM_2DARRAY, 3, { ARG_WIDTH, ARG_HEIGHT, ARG_ELEMENT_COUNT } },
        { HLSL_SAMPLER_DIM_2DARRAY, 5, { ARG_MIP_LEVEL, ARG_WIDTH, ARG_HEIGHT, ARG_ELEMENT_COUNT, ARG_LEVEL_COUNT } },
        { HLSL_SAMPLER_DIM_3D, 3, { ARG_WIDTH, ARG_HEIGHT, ARG_ELEMENT_COUNT } },
        { HLSL_SAMPLER_DIM_3D, 5, { ARG_MIP_LEVEL, ARG_WIDTH, ARG_HEIGHT, ARG_ELEMENT_COUNT, ARG_LEVEL_COUNT } },
        { HLSL_SAMPLER_DIM_CUBE, 2, { ARG_WIDTH, ARG_HEIGHT } },
        { HLSL_SAMPLER_DIM_CUBE, 4, { ARG_MIP_LEVEL, ARG_WIDTH, ARG_HEIGHT, ARG_LEVEL_COUNT } },
        { HLSL_SAMPLER_DIM_CUBEARRAY, 3, { ARG_WIDTH, ARG_HEIGHT, ARG_ELEMENT_COUNT } },
        { HLSL_SAMPLER_DIM_CUBEARRAY, 5, { ARG_MIP_LEVEL, ARG_WIDTH, ARG_HEIGHT, ARG_ELEMENT_COUNT, ARG_LEVEL_COUNT } },
        { HLSL_SAMPLER_DIM_2DMS, 3, { ARG_WIDTH, ARG_HEIGHT, ARG_SAMPLE_COUNT } },
        { HLSL_SAMPLER_DIM_2DMSARRAY, 4, { ARG_WIDTH, ARG_HEIGHT, ARG_ELEMENT_COUNT, ARG_SAMPLE_COUNT } },
        { HLSL_SAMPLER_DIM_BUFFER, 1, { ARG_WIDTH} },
    };
    const struct overload *o = NULL;

    if (object_type->sampler_dim == HLSL_SAMPLER_DIM_STRUCTURED_BUFFER)
    {
        hlsl_fixme(ctx, loc, "Method '%s' for structured buffers.", name);
        return false;
    }

    uint_type = hlsl_get_scalar_type(ctx, HLSL_TYPE_UINT);
    float_type = hlsl_get_scalar_type(ctx, HLSL_TYPE_FLOAT);
    has_uint_arg = has_float_arg = false;
    for (i = 0; i < ARRAY_SIZE(overloads); ++i)
    {
        const struct overload *iter = &overloads[i];

        if (iter->sampler_dim == object_type->sampler_dim && iter->args_count == params->args_count)
        {
            for (j = 0; j < params->args_count; ++j)
            {
                args[iter->args[j]] = params->args[j];

                /* Input parameter. */
                if (iter->args[j] == ARG_MIP_LEVEL)
                {
                    if (!(args[ARG_MIP_LEVEL] = add_implicit_conversion(ctx, block, args[ARG_MIP_LEVEL],
                            hlsl_get_scalar_type(ctx, HLSL_TYPE_UINT), loc)))
                    {
                        return false;
                    }

                    continue;
                }

                has_float_arg |= hlsl_types_are_equal(params->args[j]->data_type, float_type);
                has_uint_arg |= hlsl_types_are_equal(params->args[j]->data_type, uint_type);

                if (params->args[j]->data_type->class != HLSL_CLASS_SCALAR)
                {
                    hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_INVALID_TYPE, "Expected scalar arguments.");
                    break;
                }
            }
            o = iter;
            break;
        }
    }
    uint_resinfo = !has_float_arg && has_uint_arg;

    if (!o)
    {
        struct vkd3d_string_buffer *string;

        if ((string = hlsl_type_to_string(ctx, object_type)))
        {
            hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_INVALID_SYNTAX,
                    "Unexpected number of arguments %u for %s.%s().", params->args_count, string->buffer, name);
            hlsl_release_string_buffer(ctx, string);
        }
    }

    if (!args[ARG_MIP_LEVEL])
    {
        if (!(zero = hlsl_new_uint_constant(ctx, 0, loc)))
            return false;
        hlsl_block_add_instr(block, zero);
        args[ARG_MIP_LEVEL] = zero;
    }

    memset(&load_params, 0, sizeof(load_params));
    load_params.type = HLSL_RESOURCE_RESINFO;
    load_params.resource = object;
    load_params.lod = args[ARG_MIP_LEVEL];
    load_params.format = hlsl_get_vector_type(ctx, uint_resinfo ? HLSL_TYPE_UINT : HLSL_TYPE_FLOAT, 4);

    if (!(res_info = hlsl_new_resource_load(ctx, &load_params, loc)))
        return false;
    hlsl_block_add_instr(block, res_info);

    if (!add_assignment_from_component(ctx, block, args[ARG_WIDTH], res_info, 0, loc))
        return false;

    if (!add_assignment_from_component(ctx, block, args[ARG_HEIGHT], res_info, 1, loc))
        return false;

    if (!add_assignment_from_component(ctx, block, args[ARG_ELEMENT_COUNT], res_info,
            object_type->sampler_dim == HLSL_SAMPLER_DIM_1DARRAY ? 1 : 2, loc))
    {
        return false;
    }

    if (!add_assignment_from_component(ctx, block, args[ARG_LEVEL_COUNT], res_info, 3, loc))
        return false;

    if (args[ARG_SAMPLE_COUNT])
    {
        memset(&load_params, 0, sizeof(load_params));
        load_params.type = HLSL_RESOURCE_SAMPLE_INFO;
        load_params.resource = object;
        load_params.format = args[ARG_SAMPLE_COUNT]->data_type;
        if (!(sample_info = hlsl_new_resource_load(ctx, &load_params, loc)))
            return false;
        hlsl_block_add_instr(block, sample_info);

        if (!add_assignment(ctx, block, args[ARG_SAMPLE_COUNT], ASSIGN_OP_ASSIGN, sample_info))
            return false;
    }

    if (!(void_ret = hlsl_new_void_expr(ctx, loc)))
        return false;
    hlsl_block_add_instr(block, void_ret);

    return true;
}

static bool add_sample_lod_method_call(struct hlsl_ctx *ctx, struct hlsl_block *block, struct hlsl_ir_node *object,
        const char *name, const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    const struct hlsl_type *object_type = object->data_type;
    struct hlsl_resource_load_params load_params = { 0 };
    unsigned int sampler_dim, offset_dim;
    const struct hlsl_type *sampler_type;
    struct hlsl_ir_node *load;

    sampler_dim = hlsl_sampler_dim_count(object_type->sampler_dim);
    offset_dim = hlsl_offset_dim_count(object_type->sampler_dim);

    if (!strcmp(name, "SampleLevel"))
        load_params.type = HLSL_RESOURCE_SAMPLE_LOD;
    else
        load_params.type = HLSL_RESOURCE_SAMPLE_LOD_BIAS;

    if (params->args_count < 3 || params->args_count > 4 + !!offset_dim)
    {
        hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_WRONG_PARAMETER_COUNT,
                "Wrong number of arguments to method '%s': expected from 3 to %u, but got %u.",
                name, 4 + !!offset_dim, params->args_count);
        return false;
    }

    sampler_type = params->args[0]->data_type;
    if (sampler_type->class != HLSL_CLASS_SAMPLER || sampler_type->sampler_dim != HLSL_SAMPLER_DIM_GENERIC)
    {
        struct vkd3d_string_buffer *string;

        if ((string = hlsl_type_to_string(ctx, sampler_type)))
            hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_INVALID_TYPE,
                    "Wrong type for argument 0 of %s(): expected 'sampler', but got '%s'.", name, string->buffer);
        hlsl_release_string_buffer(ctx, string);
        return false;
    }

    if (!(load_params.coords = add_implicit_conversion(ctx, block, params->args[1],
            hlsl_get_vector_type(ctx, HLSL_TYPE_FLOAT, sampler_dim), loc)))
        load_params.coords = params->args[1];

    if (!(load_params.lod = add_implicit_conversion(ctx, block, params->args[2],
            hlsl_get_scalar_type(ctx, HLSL_TYPE_FLOAT), loc)))
        load_params.lod = params->args[2];

    if (offset_dim && params->args_count > 3)
    {
        if (!(load_params.texel_offset = add_implicit_conversion(ctx, block, params->args[3],
                hlsl_get_vector_type(ctx, HLSL_TYPE_INT, offset_dim), loc)))
            return false;
    }

    if (params->args_count > 3 + !!offset_dim)
        hlsl_fixme(ctx, loc, "Tiled resource status argument.");

    load_params.format = object_type->e.resource.format;
    load_params.resource = object;
    load_params.sampler = params->args[0];

    if (!(load = hlsl_new_resource_load(ctx, &load_params, loc)))
        return false;
    hlsl_block_add_instr(block, load);
    return true;
}

static bool add_sample_grad_method_call(struct hlsl_ctx *ctx, struct hlsl_block *block, struct hlsl_ir_node *object,
        const char *name, const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    const struct hlsl_type *object_type = object->data_type;
    struct hlsl_resource_load_params load_params = { 0 };
    unsigned int sampler_dim, offset_dim;
    const struct hlsl_type *sampler_type;
    struct hlsl_ir_node *load;

    sampler_dim = hlsl_sampler_dim_count(object_type->sampler_dim);
    offset_dim = hlsl_offset_dim_count(object_type->sampler_dim);

    load_params.type = HLSL_RESOURCE_SAMPLE_GRAD;

    if (params->args_count < 4 || params->args_count > 5 + !!offset_dim)
    {
        hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_WRONG_PARAMETER_COUNT,
                "Wrong number of arguments to method '%s': expected from 4 to %u, but got %u.",
                name, 5 + !!offset_dim, params->args_count);
        return false;
    }

    sampler_type = params->args[0]->data_type;
    if (sampler_type->class != HLSL_CLASS_SAMPLER || sampler_type->sampler_dim != HLSL_SAMPLER_DIM_GENERIC)
    {
        struct vkd3d_string_buffer *string;

        if ((string = hlsl_type_to_string(ctx, sampler_type)))
            hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_INVALID_TYPE,
                    "Wrong type for argument 0 of %s(): expected 'sampler', but got '%s'.", name, string->buffer);
        hlsl_release_string_buffer(ctx, string);
        return false;
    }

    if (!(load_params.coords = add_implicit_conversion(ctx, block, params->args[1],
            hlsl_get_vector_type(ctx, HLSL_TYPE_FLOAT, sampler_dim), loc)))
        load_params.coords = params->args[1];

    if (!(load_params.ddx = add_implicit_conversion(ctx, block, params->args[2],
            hlsl_get_vector_type(ctx, HLSL_TYPE_FLOAT, sampler_dim), loc)))
        load_params.ddx = params->args[2];

    if (!(load_params.ddy = add_implicit_conversion(ctx, block, params->args[3],
            hlsl_get_vector_type(ctx, HLSL_TYPE_FLOAT, sampler_dim), loc)))
        load_params.ddy = params->args[3];

    if (offset_dim && params->args_count > 4)
    {
        if (!(load_params.texel_offset = add_implicit_conversion(ctx, block, params->args[4],
                hlsl_get_vector_type(ctx, HLSL_TYPE_INT, offset_dim), loc)))
            return false;
    }

    if (params->args_count > 4 + !!offset_dim)
        hlsl_fixme(ctx, loc, "Tiled resource status argument.");

    load_params.format = object_type->e.resource.format;
    load_params.resource = object;
    load_params.sampler = params->args[0];

    if (!(load = hlsl_new_resource_load(ctx, &load_params, loc)))
        return false;
    hlsl_block_add_instr(block, load);
    return true;
}

static bool add_store_method_call(struct hlsl_ctx *ctx, struct hlsl_block *block, struct hlsl_ir_node *object,
        const char *name, const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    struct hlsl_ir_node *offset, *rhs, *store;
    struct hlsl_deref resource_deref;
    unsigned int value_dim;

    if (params->args_count != 2)
    {
        hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_WRONG_PARAMETER_COUNT,
                "Wrong number of arguments to method '%s': expected 2.", name);
        return false;
    }

    if (!strcmp(name, "Store"))
        value_dim = 1;
    else if (!strcmp(name, "Store2"))
        value_dim = 2;
    else if (!strcmp(name, "Store3"))
        value_dim = 3;
    else
        value_dim = 4;

    if (!(offset = add_implicit_conversion(ctx, block, params->args[0],
            hlsl_get_scalar_type(ctx, HLSL_TYPE_UINT), loc)))
        return false;

    if (!(rhs = add_implicit_conversion(ctx, block, params->args[1],
            hlsl_get_vector_type(ctx, HLSL_TYPE_UINT, value_dim), loc)))
        return false;

    if (!hlsl_init_deref_from_index_chain(ctx, &resource_deref, object))
        return false;

    if (!(store = hlsl_new_resource_store(ctx, &resource_deref, offset, rhs, loc)))
    {
        hlsl_cleanup_deref(&resource_deref);
        return false;
    }

    hlsl_block_add_instr(block, store);
    hlsl_cleanup_deref(&resource_deref);

    return true;
}

static const struct method_function
{
    const char *name;
    bool (*handler)(struct hlsl_ctx *ctx, struct hlsl_block *block, struct hlsl_ir_node *object,
            const char *name, const struct parse_initializer *params, const struct vkd3d_shader_location *loc);
    char valid_dims[HLSL_SAMPLER_DIM_MAX + 1];
}
texture_methods[] =
{
    { "Gather",             add_gather_method_call,        "00010101001000" },
    { "GatherAlpha",        add_gather_method_call,        "00010101001000" },
    { "GatherBlue",         add_gather_method_call,        "00010101001000" },
    { "GatherGreen",        add_gather_method_call,        "00010101001000" },
    { "GatherRed",          add_gather_method_call,        "00010101001000" },

    { "GetDimensions",      add_getdimensions_method_call, "00111111111110" },

    { "Load",               add_load_method_call,          "00111011110111" },
    { "Load2",              add_raw_load_method_call,      "00000000000001" },
    { "Load3",              add_raw_load_method_call,      "00000000000001" },
    { "Load4",              add_raw_load_method_call,      "00000000000001" },

    { "Sample",             add_sample_method_call,        "00111111001000" },
    { "SampleBias",         add_sample_lod_method_call,    "00111111001000" },
    { "SampleCmp",          add_sample_cmp_method_call,    "00111111001000" },
    { "SampleCmpLevelZero", add_sample_cmp_method_call,    "00111111001000" },
    { "SampleGrad",         add_sample_grad_method_call,   "00111111001000" },
    { "SampleLevel",        add_sample_lod_method_call,    "00111111001000" },
};

static const struct method_function uav_methods[] =
{
    { "Store",  add_store_method_call, "00000000000001" },
    { "Store2", add_store_method_call, "00000000000001" },
    { "Store3", add_store_method_call, "00000000000001" },
    { "Store4", add_store_method_call, "00000000000001" },
};

static int object_method_function_name_compare(const void *a, const void *b)
{
    const struct method_function *func = b;

    return strcmp(a, func->name);
}

static bool add_method_call(struct hlsl_ctx *ctx, struct hlsl_block *block, struct hlsl_ir_node *object,
        const char *name, const struct parse_initializer *params, const struct vkd3d_shader_location *loc)
{
    const struct hlsl_type *object_type = object->data_type;
    const struct method_function *method, *methods;
    unsigned int count;

    if (object_type->class == HLSL_CLASS_ERROR)
    {
        block->value = ctx->error_instr;
        return true;
    }

    for (unsigned int i = 0; i < params->args_count; ++i)
    {
        if (params->args[i]->data_type->class == HLSL_CLASS_ERROR)
        {
            block->value = ctx->error_instr;
            return true;
        }
    }

    if (object_type->class == HLSL_CLASS_TEXTURE)
    {
        count = ARRAY_SIZE(texture_methods);
        methods = texture_methods;
    }
    else if (object_type->class == HLSL_CLASS_UAV)
    {
        count = ARRAY_SIZE(uav_methods);
        methods = uav_methods;
    }
    else
    {
        struct vkd3d_string_buffer *string;

        if ((string = hlsl_type_to_string(ctx, object_type)))
            hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_INVALID_TYPE,
                    "Type '%s' does not have methods.", string->buffer);
        hlsl_release_string_buffer(ctx, string);
        return false;
    }

    method = bsearch(name, methods, count, sizeof(*method),
            object_method_function_name_compare);

    if (method && method->valid_dims[object_type->sampler_dim] == '1')
    {
        return method->handler(ctx, block, object, name, params, loc);
    }
    else
    {
        return raise_invalid_method_object_type(ctx, object_type, name, loc);
    }
}

static void validate_texture_format_type(struct hlsl_ctx *ctx, struct hlsl_type *format,
        const struct vkd3d_shader_location *loc)
{
    if (format->class > HLSL_CLASS_VECTOR)
    {
        struct vkd3d_string_buffer *string;

        string = hlsl_type_to_string(ctx, format);
        if (string)
            hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_INVALID_TYPE,
                    "Texture data type %s is not scalar or vector.", string->buffer);
        hlsl_release_string_buffer(ctx, string);
    }
}

static bool check_continue(struct hlsl_ctx *ctx, const struct hlsl_scope *scope, const struct vkd3d_shader_location *loc)
{
    if (scope->_switch)
    {
        hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_INVALID_SYNTAX,
                "The 'continue' statement is not allowed in 'switch' statements.");
        return false;
    }

    if (scope->loop)
        return true;

    if (scope->upper)
        return check_continue(ctx, scope->upper, loc);

    hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_INVALID_SYNTAX, "The 'continue' statement is only allowed in loops.");
    return false;
}

static bool is_break_allowed(const struct hlsl_scope *scope)
{
    if (scope->loop || scope->_switch)
        return true;

    return scope->upper ? is_break_allowed(scope->upper) : false;
}

static void check_duplicated_switch_cases(struct hlsl_ctx *ctx, const struct hlsl_ir_switch_case *check, struct list *cases)
{
    struct hlsl_ir_switch_case *c;
    bool found_duplicate = false;

    LIST_FOR_EACH_ENTRY(c, cases, struct hlsl_ir_switch_case, entry)
    {
        if (check->is_default)
        {
            if ((found_duplicate = c->is_default))
            {
                hlsl_error(ctx, &check->loc, VKD3D_SHADER_ERROR_HLSL_DUPLICATE_SWITCH_CASE,
                        "Found multiple 'default' statements.");
                hlsl_note(ctx, &c->loc, VKD3D_SHADER_LOG_ERROR, "The 'default' statement was previously found here.");
            }
        }
        else
        {
            if (c->is_default) continue;
            if ((found_duplicate = (c->value == check->value)))
            {
                hlsl_error(ctx, &check->loc, VKD3D_SHADER_ERROR_HLSL_DUPLICATE_SWITCH_CASE,
                        "Found duplicate 'case' statement.");
                hlsl_note(ctx, &c->loc, VKD3D_SHADER_LOG_ERROR, "The same 'case %d' statement was previously found here.",
                        c->value);
            }
        }

        if (found_duplicate)
            break;
    }
}

static void validate_uav_type(struct hlsl_ctx *ctx, enum hlsl_sampler_dim dim,
        struct hlsl_type *format, const struct vkd3d_shader_location* loc)
{
    struct vkd3d_string_buffer *string = hlsl_type_to_string(ctx, format);

    if (!type_contains_only_numerics(format))
    {
        if (string)
            hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_INVALID_TYPE,
                    "UAV type %s is not numeric.", string->buffer);
    }

    switch (dim)
    {
        case HLSL_SAMPLER_DIM_BUFFER:
        case HLSL_SAMPLER_DIM_1D:
        case HLSL_SAMPLER_DIM_1DARRAY:
        case HLSL_SAMPLER_DIM_2D:
        case HLSL_SAMPLER_DIM_2DARRAY:
        case HLSL_SAMPLER_DIM_3D:
            if (format->class == HLSL_CLASS_ARRAY)
            {
                if (string)
                    hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_INVALID_TYPE,
                            "This type of UAV does not support array type.");
            }
            else if (hlsl_type_component_count(format) > 4)
            {
                if (string)
                    hlsl_error(ctx, loc, VKD3D_SHADER_ERROR_HLSL_INVALID_TYPE,
                            "UAV data type %s size exceeds maximum size.", string->buffer);
            }
            break;
        case HLSL_SAMPLER_DIM_STRUCTURED_BUFFER:
            break;
        default:
            vkd3d_unreachable();
    }

    hlsl_release_string_buffer(ctx, string);
}


#line 6831 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"

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
         || (defined HLSL_YYLTYPE_IS_TRIVIAL && HLSL_YYLTYPE_IS_TRIVIAL \
             && defined HLSL_YYSTYPE_IS_TRIVIAL && HLSL_YYSTYPE_IS_TRIVIAL)))

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
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   3656

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  158
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  109
/* YYNRULES -- Number of rules.  */
#define YYNRULES  334
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  625

#define YYUNDEFTOK  2
#define YYMAXUTOK   388


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   150,     2,     2,     2,   153,   154,     2,
     142,   143,   151,   147,   144,   148,   145,   152,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   139,   134,
     137,   146,   138,   157,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   140,     2,   141,   155,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   135,   156,   136,   149,     2,     2,     2,
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
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133
};

#if HLSL_YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,  6812,  6812,  6813,  6814,  6815,  6820,  6821,  6822,  6823,
    6826,  6830,  6833,  6840,  6847,  6856,  6860,  6865,  6874,  6875,
    6878,  6879,  6882,  6892,  6902,  6916,  6917,  6918,  6921,  6922,
    6925,  6926,  6929,  6938,  6948,  6954,  6958,  6964,  6965,  6968,
    6990,  7005,  7006,  7009,  7047,  7053,  7054,  7055,  7059,  7063,
    7075,  7081,  7109,  7110,  7113,  7134,  7146,  7167,  7177,  7192,
    7197,  7200,  7256,  7263,  7380,  7381,  7398,  7403,  7410,  7416,
    7423,  7430,  7437,  7438,  7441,  7447,  7452,  7461,  7476,  7495,
    7504,  7516,  7538,  7556,  7571,  7588,  7610,  7616,  7625,  7629,
    7633,  7639,  7648,  7660,  7661,  7668,  7705,  7709,  7713,  7717,
    7721,  7725,  7729,  7733,  7739,  7743,  7749,  7753,  7757,  7761,
    7765,  7769,  7773,  7779,  7783,  7787,  7791,  7795,  7799,  7803,
    7809,  7818,  7841,  7845,  7874,  7878,  7882,  7886,  7890,  7894,
    7898,  7902,  7906,  7910,  7915,  7921,  7937,  7941,  7946,  7951,
    7955,  7959,  7976,  7996,  8003,  8007,  8011,  8015,  8019,  8023,
    8027,  8031,  8035,  8039,  8046,  8050,  8056,  8057,  8063,  8064,
    8065,  8072,  8073,  8076,  8104,  8110,  8117,  8126,  8133,  8139,
    8146,  8154,  8173,  8185,  8191,  8195,  8200,  8205,  8210,  8217,
    8222,  8235,  8240,  8264,  8294,  8304,  8315,  8316,  8321,  8331,
    8346,  8361,  8378,  8382,  8404,  8409,  8426,  8430,  8434,  8438,
    8442,  8446,  8450,  8454,  8458,  8462,  8466,  8470,  8474,  8478,
    8482,  8486,  8490,  8494,  8498,  8502,  8506,  8510,  8524,  8537,
    8542,  8549,  8550,  8571,  8574,  8587,  8605,  8609,  8615,  8616,
    8624,  8625,  8626,  8627,  8628,  8629,  8630,  8633,  8649,  8662,
    8668,  8675,  8692,  8742,  8747,  8754,  8760,  8766,  8772,  8780,
    8812,  8828,  8843,  8855,  8865,  8875,  8883,  8888,  8891,  8897,
    8906,  8909,  8918,  8927,  8936,  8948,  8965,  8977,  8994,  8999,
    9010,  9021,  9026,  9035,  9054,  9079,  9080,  9089,  9098,  9133,
    9149,  9176,  9194,  9195,  9204,  9213,  9217,  9222,  9227,  9233,
    9253,  9254,  9258,  9262,  9268,  9269,  9273,  9283,  9284,  9288,
    9294,  9295,  9299,  9303,  9307,  9313,  9314,  9318,  9324,  9325,
    9331,  9332,  9338,  9339,  9345,  9346,  9352,  9353,  9359,  9360,
    9378,  9379,  9396,  9400,  9404,  9408,  9412,  9416,  9420,  9424,
    9428,  9432,  9436,  9442,  9443
};
#endif

#if HLSL_YYDEBUG || YYERROR_VERBOSE || 1
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "KW_BLENDSTATE", "KW_BREAK", "KW_BUFFER",
  "KW_BYTEADDRESSBUFFER", "KW_CASE", "KW_CONSTANTBUFFER", "KW_CBUFFER",
  "KW_CENTROID", "KW_COLUMN_MAJOR", "KW_COMPILE", "KW_COMPILESHADER",
  "KW_COMPUTESHADER", "KW_CONST", "KW_CONSTRUCTGSWITHSO", "KW_CONTINUE",
  "KW_DEFAULT", "KW_DEPTHSTENCILSTATE", "KW_DEPTHSTENCILVIEW",
  "KW_DISCARD", "KW_DO", "KW_DOMAINSHADER", "KW_ELSE", "KW_EXPORT",
  "KW_EXTERN", "KW_FALSE", "KW_FOR", "KW_FXGROUP", "KW_GEOMETRYSHADER",
  "KW_GROUPSHARED", "KW_HULLSHADER", "KW_IF", "KW_IN", "KW_INLINE",
  "KW_INOUT", "KW_LINEAR", "KW_MATRIX", "KW_NAMESPACE",
  "KW_NOINTERPOLATION", "KW_NOPERSPECTIVE", "KW_NULL", "KW_OUT",
  "KW_PACKOFFSET", "KW_PASS", "KW_PIXELSHADER",
  "KW_RASTERIZERORDEREDBUFFER", "KW_RASTERIZERORDEREDSTRUCTUREDBUFFER",
  "KW_RASTERIZERORDEREDTEXTURE1D", "KW_RASTERIZERORDEREDTEXTURE1DARRAY",
  "KW_RASTERIZERORDEREDTEXTURE2D", "KW_RASTERIZERORDEREDTEXTURE2DARRAY",
  "KW_RASTERIZERORDEREDTEXTURE3D", "KW_RASTERIZERSTATE",
  "KW_RENDERTARGETVIEW", "KW_RETURN", "KW_REGISTER", "KW_ROW_MAJOR",
  "KW_RWBUFFER", "KW_RWBYTEADDRESSBUFFER", "KW_RWSTRUCTUREDBUFFER",
  "KW_RWTEXTURE1D", "KW_RWTEXTURE1DARRAY", "KW_RWTEXTURE2D",
  "KW_RWTEXTURE2DARRAY", "KW_RWTEXTURE3D", "KW_SAMPLER", "KW_SAMPLER1D",
  "KW_SAMPLER2D", "KW_SAMPLER3D", "KW_SAMPLERCUBE", "KW_SAMPLER_STATE",
  "KW_SAMPLERCOMPARISONSTATE", "KW_SHARED", "KW_SNORM", "KW_STATEBLOCK",
  "KW_STATEBLOCK_STATE", "KW_STATIC", "KW_STRING", "KW_STRUCT",
  "KW_SWITCH", "KW_TBUFFER", "KW_TECHNIQUE", "KW_TECHNIQUE10",
  "KW_TECHNIQUE11", "KW_TEXTURE", "KW_TEXTURE1D", "KW_TEXTURE1DARRAY",
  "KW_TEXTURE2D", "KW_TEXTURE2DARRAY", "KW_TEXTURE2DMS",
  "KW_TEXTURE2DMSARRAY", "KW_TEXTURE3D", "KW_TEXTURECUBE",
  "KW_TEXTURECUBEARRAY", "KW_TRUE", "KW_TYPEDEF", "KW_UNSIGNED",
  "KW_UNIFORM", "KW_UNORM", "KW_VECTOR", "KW_VERTEXSHADER", "KW_VOID",
  "KW_VOLATILE", "KW_WHILE", "OP_INC", "OP_DEC", "OP_AND", "OP_OR",
  "OP_EQ", "OP_LEFTSHIFT", "OP_LEFTSHIFTASSIGN", "OP_RIGHTSHIFT",
  "OP_RIGHTSHIFTASSIGN", "OP_LE", "OP_GE", "OP_NE", "OP_ADDASSIGN",
  "OP_SUBASSIGN", "OP_MULASSIGN", "OP_DIVASSIGN", "OP_MODASSIGN",
  "OP_ANDASSIGN", "OP_ORASSIGN", "OP_XORASSIGN", "C_FLOAT", "C_INTEGER",
  "C_UNSIGNED", "PRE_LINE", "VAR_IDENTIFIER", "NEW_IDENTIFIER", "STRING",
  "TYPE_IDENTIFIER", "';'", "'{'", "'}'", "'<'", "'>'", "':'", "'['",
  "']'", "'('", "')'", "','", "'.'", "'='", "'+'", "'-'", "'~'", "'!'",
  "'*'", "'/'", "'%'", "'&'", "'^'", "'|'", "'?'", "$accept", "hlsl_prog",
  "name_opt", "pass", "annotations_list", "annotations_opt", "pass_list",
  "passes", "technique9", "technique10", "technique11", "global_technique",
  "group_technique", "group_techniques", "effect_group",
  "buffer_declaration", "buffer_body", "buffer_type",
  "declaration_statement_list", "preproc_directive",
  "struct_declaration_without_vars", "struct_spec", "named_struct_spec",
  "unnamed_struct_spec", "any_identifier", "base_optional", "fields_list",
  "field_type", "field", "attribute", "attribute_list",
  "attribute_list_optional", "func_declaration", "func_prototype_no_attrs",
  "func_prototype", "compound_statement", "scope_start",
  "loop_scope_start", "switch_scope_start", "annotations_scope_start",
  "var_identifier", "colon_attributes", "semantic", "register_reservation",
  "packoffset_reservation", "parameters", "param_list", "parameter",
  "parameter_decl", "texture_type", "texture_ms_type", "uav_type",
  "rov_type", "resource_format", "type_no_void", "type",
  "declaration_statement", "typedef_type", "typedef", "type_specs",
  "type_spec", "declaration", "variables_def", "variables_def_typed",
  "variable_decl", "state_block_start", "stateblock_lhs_identifier",
  "state_block_index_opt", "state_block", "state_block_list",
  "variable_def", "variable_def_typed", "array", "arrays", "var_modifiers",
  "complex_initializer", "complex_initializer_list", "initializer_expr",
  "initializer_expr_list", "boolean", "statement_list", "statement",
  "jump_statement", "selection_statement", "if_body", "loop_statement",
  "switch_statement", "switch_case", "switch_cases", "expr_optional",
  "expr_statement", "func_arguments", "primary_expr", "postfix_expr",
  "unary_expr", "mul_expr", "add_expr", "shift_expr", "relational_expr",
  "equality_expr", "bitand_expr", "bitxor_expr", "bitor_expr",
  "logicand_expr", "logicor_expr", "conditional_expr", "assignment_expr",
  "assign_op", "expr", YY_NULLPTR
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
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,    59,   123,   125,    60,    62,    58,
      91,    93,    40,    41,    44,    46,    61,    43,    45,   126,
      33,    42,    47,    37,    38,    94,   124,    63
};
# endif

#define YYPACT_NINF (-493)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-275)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -493,  2582,  -493,  1787,  1787,  1787,  1787,  1787,   129,  1787,
    1787,  1787,  1787,  1787,  1787,  1787,  1787,  1787,  1787,  1787,
    1787,   129,   129,   129,  1787,  1787,  1787,  1787,   -53,  -493,
    -493,  -493,   129,  -493,  -493,  -493,  -493,  -493,    25,  -493,
    -493,  -493,  3198,  -493,  -493,   -31,  1787,  -493,  -493,  -493,
     -14,  -493,  2701,  -493,  -493,  -493,  -493,  -493,  -493,  -493,
    -493,    48,  -493,  -493,  -493,  -493,  -493,  -493,  -493,  -493,
    -493,  -493,  -493,  -493,    48,  -493,    48,    48,  2803,  -493,
    -493,  -493,  -493,    78,  -493,  -493,  -493,  -493,  2905,  -493,
      76,  -493,  -493,  -493,   129,  -493,  -493,  -493,    96,  -493,
    -493,  -493,  -493,  -493,  -493,  -493,   113,  -493,  -493,  -493,
    -493,  -493,  -493,  -493,  -493,  -493,  -493,  -493,  -493,  -493,
    -493,  -493,  -493,  -493,  -493,  -493,  -493,  -493,  -493,  -493,
    -493,  -493,    34,  -493,  -493,  -493,  -493,  -493,  -493,  -493,
    -493,  -493,  -493,  -493,     1,   139,  -493,  -493,  -493,   129,
     115,  -493,  -493,   149,   153,   171,   177,  -493,   133,  -493,
     132,   166,   184,   196,  -493,  -493,   129,  -493,  2453,  3324,
     200,    62,  -493,  1377,   199,   -41,  -493,  2905,  2905,    -2,
    -493,   203,  -493,  2905,  -493,  -493,  -493,  1787,  1787,  1787,
    1787,   202,   204,   232,  -493,  3243,  -493,  -493,  -493,  -493,
     199,    -3,  -493,   129,   233,   234,  -493,  -493,   242,  -493,
    2453,  2453,  -493,  -493,  -493,  3447,  3499,  -493,  2453,  2453,
    2453,  2453,  2453,   525,  2905,  -493,   161,  -493,  -493,     3,
     357,    50,   163,    64,    -9,    39,   224,   226,   230,   280,
     -33,  -493,  -493,  -493,  -493,  2803,  -493,   256,   257,   258,
    1646,   253,    54,  -493,  -493,  2803,  1236,  -493,  -493,  -493,
    -493,  -493,   262,  -493,  -493,   254,  1774,   199,  -493,  -493,
    1915,   259,   260,  3401,   267,   270,   263,    88,   268,  2905,
      13,   271,   274,  -493,  -493,  3279,     6,  2803,   228,   272,
     368,   278,   283,  -493,  -493,   129,    62,   129,  2043,  -493,
    -493,  -493,  2905,   173,  -493,  -493,  -493,  -493,  2043,   279,
     281,  2453,  -493,  -493,  2453,   129,  -493,  -493,  -493,  -493,
    -493,  -493,  -493,  -493,  -493,  -493,  -493,  2453,  2453,  2453,
    2453,  2453,  2453,  2453,  2453,  2453,  2453,  2453,  2453,  2453,
    2453,  2453,  2453,  2453,  2453,  2453,  2453,   129,  -493,  -493,
    -493,  -493,    11,   282,    53,   339,   -17,  -493,  -493,  -493,
    2453,  -493,    99,  -493,    88,   288,  1915,  -493,  -493,  -493,
     298,  -493,  -493,  3007,  -493,  -493,   299,    51,  -493,  -493,
    -493,  -493,  -493,  -493,  -493,  2453,  -493,  -493,  2595,   285,
    -493,    18,  -493,   129,  -493,  -493,  -493,     0,  -493,   129,
    -493,   368,  -493,  -493,  -493,   287,   289,   290,   293,  -493,
     147,  -493,   294,  2453,  -493,  -493,   124,   301,  -493,  -493,
    -493,  -493,  -493,    50,    50,   163,   163,    64,    64,    64,
      64,    -9,    -9,    39,   224,   226,   230,   280,    32,  -493,
    2453,  1377,   305,   310,   311,  -493,  -493,  -493,  -493,    93,
     -25,  -493,    10,   312,   158,  -493,   129,  -493,  3446,   316,
     313,   317,  -493,   -21,  -493,   314,  -493,   286,  3109,  -493,
    -493,  -493,  -493,    48,  -493,  2043,    62,  -493,   167,   306,
    -493,   180,  -493,  2043,  2453,   182,   326,  2184,  2453,  2453,
     249,  -493,  -493,  -493,  -493,  -493,   318,   322,  -493,   328,
    -493,  1518,   337,    19,  -493,  -493,  -493,   129,   129,  -493,
    1787,  1915,   129,   331,   330,   324,   341,  -493,  2453,  -493,
     329,  -493,  1377,   342,  2184,  2803,  2184,   185,   193,  -493,
    2043,   346,   343,  -493,  -493,  -493,   347,  -493,   129,   151,
      67,  -493,  -493,   199,  -493,  -493,  2043,  -493,  -493,   463,
    -493,  2453,  2312,  2312,  1377,   353,   348,   351,  1915,   308,
    -493,  -493,  -493,   129,  2453,  -493,   129,  -493,  -493,   350,
    1377,   205,   352,   354,  -493,    94,   360,  -493,   362,  -493,
     355,   130,   114,   331,   315,   356,  -493,   366,  1377,  1377,
    2453,   365,  -493,    73,  -493,  -493,  -493,   209,  2453,  -493,
     129,  -493,  -493,  -493,  -493,  -493,    52,   653,  -493,  -493,
    -493,   129,   134,   358,   801,   948,   363,   214,  -493,  1095,
    -493,  -493,   129,   364,  -493
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,   196,     1,   196,   196,   196,   196,   196,     0,   196,
     196,   196,   196,   196,   196,   196,   196,   196,   196,   196,
     196,    10,    10,    10,   196,   196,   196,   196,     0,    72,
      73,     9,     0,    25,    26,    27,     7,     8,     0,     6,
     159,    57,   196,     3,    64,     0,   196,     5,   160,   158,
       0,   170,     0,   199,   209,   207,   214,   197,    45,    47,
      46,    15,   203,   210,   213,   212,   200,   198,   201,   211,
     208,   202,   216,   204,    15,    11,    15,    15,     0,   205,
     215,   206,    39,     0,    37,     4,    58,    65,     0,    62,
      68,    61,   217,   167,     0,   155,    96,   136,     0,    35,
     149,   145,   146,   150,   152,   151,   124,   148,   113,   114,
     115,   116,   117,   118,   119,   154,   144,   106,   139,   107,
     108,   109,   110,   111,   112,   125,   127,   128,   129,   130,
     126,   140,     0,    36,   131,    97,   101,    98,   102,   104,
     105,    99,   100,   103,     0,   122,   147,   157,   141,     0,
       0,    41,    42,   132,     0,     0,     0,   156,     0,    71,
       0,     0,     0,     0,   162,   161,     0,    55,   196,   196,
       0,     0,    66,   196,   194,   186,   171,     0,     0,   143,
      50,    48,   142,     0,    74,    40,   190,   196,   196,   196,
     196,    45,    47,     0,   191,   196,    68,    68,    68,    68,
     194,     0,   164,     0,     0,     0,   227,   266,     0,   226,
     196,   196,   261,   262,   263,    72,    73,   265,   196,   196,
     196,   196,   196,   196,     0,   224,     0,   264,   275,   282,
     290,   294,   297,   300,   305,   308,   310,   312,   314,   316,
     318,   320,   223,    34,    38,     0,   143,     0,     0,     0,
     196,    60,    69,   232,   230,     0,   196,   228,   233,   234,
     235,   236,     0,   231,   333,   257,   196,   194,    74,   173,
     196,     0,     0,   196,     0,     0,     0,    15,     0,     0,
       0,     0,     0,    68,    16,   196,     0,     0,     0,     0,
      20,     0,     0,   166,   163,     0,     0,     0,   196,   173,
     283,   284,     0,     0,   285,   286,   287,   288,   196,     0,
       0,   196,   276,   277,   196,     0,   328,   329,   323,   324,
     325,   326,   327,   330,   331,   332,   322,   196,   196,   196,
     196,   196,   196,   196,   196,   196,   196,   196,   196,   196,
     196,   196,   196,   196,   196,   196,   196,     0,   237,   238,
     241,   240,     0,     0,     0,     0,     0,    67,   229,   258,
     196,   192,     0,   195,    15,   181,   196,   187,   218,   153,
       0,    44,    51,     0,    49,    50,     0,     0,    33,    75,
      76,    77,   133,   120,   134,   196,   137,   138,   196,     0,
      17,     0,    13,     0,    28,    29,    30,     0,    22,    10,
      18,    21,    23,    24,   165,     0,     0,   260,     0,   181,
     194,   268,     0,   196,    56,   225,     0,   278,   321,   291,
     292,   293,   290,   295,   296,   298,   299,   303,   304,   301,
     302,   306,   307,   309,   311,   313,   315,   317,     0,   239,
     196,   196,     0,     0,     0,   334,   193,   172,   181,     0,
       0,   221,     0,     0,     0,    53,     0,    52,   196,     0,
       0,     0,    78,     0,    89,    90,    91,    93,     0,    74,
      14,    32,    31,    15,    19,   196,     0,   271,     0,     0,
     272,     0,   279,   196,   196,     0,     0,   196,   196,   196,
       0,   178,   175,   176,   177,   188,   174,   179,   189,     0,
     219,   196,     0,     0,   168,    43,   121,     0,     0,   135,
     196,   196,     0,    63,     0,     0,     0,   273,   196,   280,
       0,   319,   196,     0,   196,     0,   196,     0,     0,   184,
     196,     0,     0,   181,   220,   222,     0,    54,     0,     0,
       0,    92,    94,   194,   173,   269,   196,   289,   281,   243,
     242,   196,   196,   196,   196,     0,     0,     0,   196,     0,
     123,   169,    86,     0,   196,    79,     0,    74,   181,     0,
     196,     0,     0,     0,   245,     0,     0,   180,     0,   185,
       0,     0,     0,    95,     0,     0,   244,     0,   196,   196,
     196,     0,   254,     0,   183,   182,    87,     0,   196,    81,
       0,    12,   270,   246,   248,   247,     0,   196,   249,   255,
      80,     0,     0,     0,   196,   196,     0,     0,    84,   196,
      82,    83,     0,     0,    85
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -493,  -493,    16,   107,  -493,   -46,  -493,   164,  -493,   509,
     510,  -493,   116,  -493,  -493,  -493,  -493,  -493,  -493,  -493,
    -493,   -57,  -493,   141,    12,  -493,   137,  -493,  -493,     5,
     514,  -493,  -493,   474,  -493,   477,   -22,  -493,  -493,  -493,
      47,  -261,  -493,  -493,  -493,  -493,  -493,    15,  -493,  -493,
    -493,  -493,  -493,    92,    63,   -51,    45,  -493,  -493,  -493,
     235,    46,  -493,  -126,  -493,  -267,  -493,  -493,  -327,  -493,
     -72,  -493,  -493,  -171,    -1,  -311,  -493,  -228,  -125,  -493,
    -492,  -220,  -493,  -493,  -493,  -493,  -493,   -61,  -493,  -193,
    -438,  -231,  -493,  -493,   307,    33,    37,  -238,    27,   197,
     192,   194,   195,   198,  -493,  -493,  -128,  -493,  -148
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    74,   400,   285,   160,   401,   289,    33,   394,
     395,    36,   396,   397,    37,    38,    85,   149,   169,    39,
      40,   150,   151,   152,   174,   275,   273,   456,   372,    41,
     251,   252,    43,    44,    45,   253,   290,   354,   355,   195,
     223,   277,   379,   380,   381,   389,   465,   466,   467,   153,
     154,   155,   156,   278,   157,   347,   254,   166,    48,   201,
     202,    49,   503,    50,   175,   365,   497,   532,   449,   450,
     194,    51,   267,   268,   224,   367,   452,   225,   407,   227,
     256,   257,   258,   259,   550,   260,   261,   592,   593,   262,
     263,   408,   228,   229,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   240,   241,   264,   327,   265
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      52,   158,    53,    54,    55,    56,    57,   364,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      61,   164,   176,    78,    79,    80,    81,   165,   161,   293,
     162,   163,   409,    75,    75,    75,   358,   171,    76,    77,
     242,    88,   368,   226,    83,    92,    47,    86,    46,   526,
      46,    46,    46,    46,    46,   451,    46,    46,    46,    46,
      46,    46,    46,    46,    46,    46,    46,    46,   173,   286,
     303,    46,    46,    46,    46,   441,   345,   412,   186,    82,
     590,   442,   478,   415,    22,    23,   552,   353,   553,    46,
     333,   591,   334,    46,   269,   460,   363,   427,   428,   429,
     430,   590,   352,    89,    90,   270,   335,   336,   461,   312,
     313,   498,   591,    58,    59,   615,    60,   509,   362,   499,
      93,   490,   619,   491,   346,   413,   271,   272,   337,   338,
      94,   294,   276,   -46,   182,   -70,   471,   -46,   368,   492,
     392,   295,   242,   314,   181,   439,   500,   463,   315,   339,
      94,   384,   470,   537,   501,   360,   340,   385,   443,   391,
      84,   184,    94,   538,    58,    59,   416,   179,   245,   180,
     242,   484,   255,   309,   288,   333,   360,   334,   200,   493,
     242,    58,    59,   242,    60,   159,   279,   279,   279,   279,
     535,   614,    29,    30,   287,   494,   360,   491,   438,   418,
     542,   328,   329,   330,   356,   193,   559,   564,   513,   608,
     565,   566,   172,   492,   244,   296,    46,   302,   193,   167,
     168,   486,    92,    58,    59,   159,    60,   377,   383,   495,
     393,   378,   445,   177,    46,    46,    46,    46,   242,   479,
     446,   584,    46,   360,   515,    58,    59,   578,    60,   185,
     178,   410,   520,   493,   598,   255,    86,   599,   600,    58,
      59,   388,    60,   191,   192,   482,    60,   196,   360,   494,
      46,   597,   373,   368,   360,   617,   183,   568,   360,   491,
     280,   281,   282,   368,   287,   242,   187,   266,   481,   413,
     188,   246,   485,   180,   562,   492,   563,    58,    59,   556,
      60,   197,   549,   517,   310,   311,   583,   200,   189,   406,
     331,   332,    22,    23,   190,   569,   411,   360,   447,   198,
      46,   186,   457,   519,   311,   522,   360,   417,   554,   360,
     368,   199,    46,   246,   574,   493,   555,   360,   491,   266,
     527,   528,   274,   405,   -72,   491,   -73,   242,   587,   360,
     586,   494,   610,   611,   492,   242,   521,   621,   622,   572,
     573,   492,   291,   292,   423,   424,   431,   432,   604,   605,
     425,   426,   567,   242,   283,   297,   298,   299,   341,    58,
      59,   342,    60,   242,   504,   529,   343,   468,   344,   462,
     348,   349,   350,    32,   493,   358,   359,   369,   360,   358,
     374,   493,   242,   571,   370,   375,   382,   376,   398,   386,
     494,    75,   387,   399,   402,   473,   581,   494,   242,   403,
     444,   413,   414,   448,   440,   453,   459,   514,   469,   475,
     242,   523,   511,   476,   311,    46,   477,   480,    58,    59,
     255,    60,   606,   483,   579,    58,    59,   487,    60,   518,
     612,   601,   488,   489,   506,   507,   502,   373,   510,   508,
     530,   496,   531,   533,   536,   544,   561,   545,   393,   316,
     377,   317,   548,   557,   356,   318,   319,   320,   321,   322,
     323,   324,   325,   546,   551,   560,   525,   570,   575,   558,
     496,   576,   577,   585,   594,   588,   595,   589,   596,   602,
     603,   618,   496,   326,   607,    46,   620,   624,   474,   468,
      34,    35,   458,   472,   455,    42,    87,   300,   301,   539,
     540,   255,    91,   516,   543,   541,   304,   305,   306,   307,
     404,   512,   609,   524,   434,     3,     4,   435,   433,   436,
       5,     0,     0,   437,     0,     0,     0,     0,     0,     0,
       6,     7,     0,   255,     0,     0,     9,    46,     0,    10,
      11,    12,    13,     0,     0,    14,    15,     0,    16,   255,
       0,   496,     0,     0,     0,   580,     0,     0,   582,     0,
       0,     0,     0,    17,     0,     0,     0,   255,   255,     0,
       0,     0,     0,     0,     0,     0,   496,     0,     0,    18,
      19,     0,     0,    20,     0,     0,   255,     0,     0,     0,
       0,     0,   613,   255,   255,     0,     0,     0,   255,     0,
       0,     0,     0,   616,    25,    26,     0,     0,     0,    27,
       0,     0,     0,     0,   623,   419,   420,   421,   422,   422,
     422,   422,   422,   422,   422,   422,   422,   422,   422,   422,
     422,   422,   422,     0,     0,    29,    30,   247,     0,     0,
    -253,     0,     0,     3,     4,   203,   204,   308,     5,   205,
     248,  -253,     0,     0,   249,   -59,     0,     0,     6,     7,
     206,   -59,     0,     0,     9,     0,   -59,    10,    11,    12,
      13,     0,   422,    14,    15,   207,    16,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   250,
       0,    17,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   208,     0,    18,    19,     0,
       0,    20,     0,     0,   -59,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   209,
      24,     0,    25,    26,     0,     0,     0,    27,   -59,   210,
     211,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   212,
     213,   214,     0,   215,   216,   217,     0,  -256,    90,  -253,
       0,     0,     0,    32,     0,   218,     0,     0,     0,     0,
     219,   220,   221,   222,     0,   247,     0,     0,  -251,     0,
       0,     3,     4,   203,   204,     0,     5,   205,   248,  -251,
       0,     0,   249,   -59,     0,   547,     6,     7,   206,   -59,
       0,     0,     9,     0,   -59,    10,    11,    12,    13,     0,
       0,    14,    15,   207,    16,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   250,     0,    17,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   208,     0,    18,    19,     0,     0,    20,
       0,     0,   -59,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   209,    24,     0,
      25,    26,     0,     0,     0,    27,   -59,   210,   211,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   212,   213,   214,
       0,   215,   216,   217,     0,  -256,    90,  -251,     0,     0,
       0,    32,     0,   218,     0,     0,     0,     0,   219,   220,
     221,   222,   247,     0,     0,  -252,     0,     0,     3,     4,
     203,   204,     0,     5,   205,   248,  -252,     0,     0,   249,
     -59,     0,     0,     6,     7,   206,   -59,     0,     0,     9,
       0,   -59,    10,    11,    12,    13,     0,     0,    14,    15,
     207,    16,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   250,     0,    17,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     208,     0,    18,    19,     0,     0,    20,     0,     0,   -59,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   209,    24,     0,    25,    26,     0,
       0,     0,    27,   -59,   210,   211,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   212,   213,   214,     0,   215,   216,
     217,     0,  -256,    90,  -252,     0,     0,     0,    32,     0,
     218,     0,     0,     0,     0,   219,   220,   221,   222,   247,
       0,     0,  -250,     0,     0,     3,     4,   203,   204,     0,
       5,   205,   248,  -250,     0,     0,   249,   -59,     0,     0,
       6,     7,   206,   -59,     0,     0,     9,     0,   -59,    10,
      11,    12,    13,     0,     0,    14,    15,   207,    16,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   250,     0,    17,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   208,     0,    18,
      19,     0,     0,    20,     0,     0,   -59,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   209,    24,     0,    25,    26,     0,     0,     0,    27,
     -59,   210,   211,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   212,   213,   214,     0,   215,   216,   217,     0,  -256,
      90,  -250,     0,     0,     0,    32,     0,   218,     0,     0,
     247,     0,   219,   220,   221,   222,     3,     4,   203,   204,
       0,     5,   205,   248,     0,     0,     0,   249,   -59,     0,
       0,     6,     7,   206,   -59,     0,     0,     9,     0,   -59,
      10,    11,    12,    13,     0,     0,    14,    15,   207,    16,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   250,     0,    17,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   208,     0,
      18,    19,     0,     0,    20,     0,     0,   -59,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   209,    24,     0,    25,    26,     0,     0,     0,
      27,   -59,   210,   211,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   212,   213,   214,     0,   215,   216,   217,     0,
    -256,    90,   357,     0,     0,     0,    32,     0,   218,     0,
       0,   247,     0,   219,   220,   221,   222,     3,     4,   203,
     204,     0,     5,   205,   248,     0,     0,     0,   249,   -59,
       0,     0,     6,     7,   206,   -59,     0,     0,     9,     0,
     -59,    10,    11,    12,    13,     0,     0,    14,    15,   207,
      16,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   250,     0,    17,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   208,
       0,    18,    19,     0,     0,    20,     0,     0,   -59,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   209,    24,     0,    25,    26,     0,     0,
       0,    27,   -59,   210,   211,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   212,   213,   214,     0,   215,   216,   217,
       0,  -256,    90,     0,     0,     0,     0,    32,     0,   218,
       0,     0,     0,     0,   219,   220,   221,   222,     3,     4,
     203,   204,     0,     5,   205,     0,     0,     0,     0,     0,
       0,     0,     0,     6,     7,   206,     0,     0,     0,     9,
       0,     0,    10,    11,    12,    13,     0,     0,    14,    15,
     207,    16,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    17,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     208,     0,    18,    19,     0,     0,    20,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   209,     0,     0,    25,    26,     0,
       0,     0,    27,     0,   210,   211,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   212,   213,   214,     0,   215,   216,
     217,     0,     0,   366,   534,     0,     3,     4,   203,   204,
     218,     5,   205,     0,     0,   219,   220,   221,   222,     0,
       0,     6,     7,   206,     0,     0,     0,     9,     0,     0,
      10,    11,    12,    13,     0,     0,    14,    15,   207,    16,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    17,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   208,     0,
      18,    19,     0,     0,    20,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   209,     0,     0,    25,    26,     0,     0,     0,
      27,     0,   210,   211,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   212,   213,   214,     0,   215,   216,   217,     0,
     351,     0,     0,     0,     3,     4,   203,   204,   218,     5,
     205,     0,     0,   219,   220,   221,   222,     3,     4,     6,
       7,   206,     5,     0,     0,     9,     0,     0,    10,    11,
      12,    13,     6,     7,    14,    15,   207,    16,     9,     0,
       0,    10,    11,    12,    13,     0,     0,    14,    15,     0,
      16,     0,    17,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    17,   208,     0,    18,    19,
       0,     0,    20,     0,     0,     0,     0,     0,     0,     0,
       0,    18,    19,     0,     0,    20,     0,     0,     0,     0,
     209,     0,     0,    25,    26,     0,     0,     0,    27,     0,
     210,   211,     0,     0,     0,     0,    25,    26,     0,     0,
       0,    27,     0,     0,     0,     0,     0,     0,     0,     0,
     212,   213,   214,     0,   215,   216,   217,     0,     0,     0,
       0,     0,     0,     0,     0,   361,   218,    29,    30,     0,
       0,   219,   220,   221,   222,     3,     4,   203,   204,     0,
       5,   205,     0,     0,     0,     0,     0,     0,     0,     0,
       6,     7,   206,     0,     0,     0,     9,     0,     0,    10,
      11,    12,    13,     0,     0,    14,    15,   207,    16,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    17,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   208,     0,    18,
      19,     0,     0,    20,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   209,     0,     0,    25,    26,     0,     0,     0,    27,
       0,   210,   211,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   212,   213,   214,     0,   215,   216,   217,     0,     0,
     366,     0,     0,     3,     4,   203,   204,   218,     5,   205,
       0,     0,   219,   220,   221,   222,     0,     0,     6,     7,
     206,     0,     0,     0,     9,     0,     0,    10,    11,    12,
      13,     0,     0,    14,    15,   207,    16,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    17,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   208,     0,    18,    19,     0,
       0,    20,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   209,
       0,     0,    25,    26,     0,     0,     0,    27,     0,   210,
     211,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   212,
     213,   214,     0,   215,   216,   217,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   218,  -259,     0,     0,     0,
     219,   220,   221,   222,     3,     4,   203,   204,     0,     5,
     205,     0,     0,     0,     0,     0,     0,     0,     0,     6,
       7,   206,     0,     0,     0,     9,     0,     0,    10,    11,
      12,    13,     0,     0,    14,    15,   207,    16,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    17,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   208,     0,    18,    19,
       0,     0,    20,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     209,     0,     0,    25,    26,     0,     0,     0,    27,     0,
     210,   211,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     212,   213,   214,     0,   215,   216,   217,     0,  -256,     0,
       0,     0,     3,     4,   203,   204,   218,     5,   205,     0,
       0,   219,   220,   221,   222,     0,     0,     6,     7,   206,
       0,     0,     0,     9,     0,     0,    10,    11,    12,    13,
       0,     0,    14,    15,   207,    16,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      17,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   208,     0,    18,    19,     0,     0,
      20,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   209,     0,
       0,    25,    26,     0,     0,     0,    27,     0,   210,   211,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   212,   213,
     214,     0,   215,   216,   217,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   218,  -256,     0,     0,     0,   219,
     220,   221,   222,     3,     4,   203,   204,     0,     5,   205,
       0,     0,     0,     0,     0,     0,     0,     0,     6,     7,
     206,     0,     0,     0,     9,     0,     0,    10,    11,    12,
      13,     0,     0,    14,    15,   207,    16,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    17,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   208,     0,    18,    19,     0,
       0,    20,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   209,
       0,     0,    25,    26,     0,     0,     0,    27,     0,   210,
     211,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   212,
     213,   214,     2,   215,   216,   217,     0,     0,     0,     0,
       0,     0,     3,     4,     0,   218,     0,     5,     0,     0,
     219,   220,   221,   222,     0,     3,     4,     6,     7,     0,
       5,     8,     0,     9,     0,     0,    10,    11,    12,    13,
       6,     7,    14,    15,     0,    16,     9,     0,     0,    10,
      11,    12,    13,     0,     0,    14,    15,     0,    16,     0,
      17,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    17,     0,     0,    18,    19,     0,     0,
      20,     0,     0,     0,     0,    21,    22,    23,     0,    18,
      19,     0,     0,    20,     0,     0,     0,     0,     0,    24,
       0,    25,    26,     0,     0,     0,    27,     0,     0,     0,
       0,     0,     0,     0,    25,    26,     0,     0,   464,    27,
       0,     0,     0,     0,    95,     0,    96,    97,     0,    98,
      99,    28,    29,    30,     0,   100,    31,     0,     0,     0,
     101,   102,    32,     0,   103,    29,    30,     0,     0,     0,
       0,   104,     0,   105,     0,     0,     0,     0,   -88,   106,
       0,     0,     0,     0,     0,     0,     0,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,     0,     0,     0,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,     0,   130,     0,     0,     0,     0,     0,
     131,   132,     0,   133,     0,     0,     0,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,     0,     0,   144,
       0,     0,   145,   146,   147,     0,    95,     0,    96,    97,
       0,    98,     0,     0,     0,     0,     0,   100,     0,     0,
       0,     0,   101,   102,     0,     0,   103,     0,     0,     0,
       0,     0,     0,   104,   148,   105,     0,     0,     0,     0,
       0,   106,     0,     0,     0,     0,     0,     0,     0,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,     0,
       0,     0,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,     0,   130,     0,     0,     0,
       0,     0,   131,   132,     0,     0,     0,     0,     0,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,     0,
       0,   144,     0,     0,   145,   146,   147,     0,    95,     0,
      96,    97,     0,    98,     0,     0,     0,     0,     0,   100,
       0,     0,     0,     0,   101,   102,     0,     0,   103,     0,
       0,     0,     0,     0,     0,   104,   148,   105,     0,     0,
       0,     0,     0,   106,     0,     0,     0,     0,     0,     0,
       0,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,     0,     0,     0,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,     0,   130,     0,
       0,     0,     0,     0,   131,   170,     0,     0,     0,     0,
       0,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,     0,     0,   144,     0,     0,   145,   146,   147,     0,
      95,     0,    96,    97,     0,    98,     0,     0,     0,     0,
       0,   100,     0,     0,     0,     0,   101,   102,     0,     0,
     103,     0,     0,     0,     0,     0,     0,   104,   148,   105,
       0,     0,     0,     0,     0,   106,     0,     0,     0,     0,
       0,     0,     0,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,     0,     0,     0,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,     0,
     130,     0,     0,     0,     0,     0,   131,   454,     0,     0,
       0,     0,     0,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,     0,     0,   144,     0,     0,   145,   146,
     147,     0,    95,     0,    96,    97,     0,    98,     0,     0,
       0,     0,     0,   100,     0,     0,     0,     0,   101,   102,
       0,     0,   103,     0,     0,     0,     0,     0,     0,   104,
     148,   105,     0,     0,     0,     0,     0,   106,     0,     0,
       0,     0,     0,     0,     0,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,     0,     0,     0,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,     0,   130,     0,     0,     0,     0,     0,   131,   170,
       0,     0,     0,     0,     0,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,     0,     0,   144,     3,     4,
     145,   146,     0,     5,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     6,     7,     0,     0,     0,     0,     9,
       0,     0,    10,    11,    12,    13,     0,     0,    14,    15,
       0,    16,   148,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     3,     4,     0,    17,     0,     5,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     6,     7,
       0,     0,    18,    19,     9,     0,    20,    10,    11,    12,
      13,     0,     0,    14,    15,     0,    16,     0,     0,     3,
       4,     0,     0,     0,     5,     0,     0,    25,    26,     0,
       0,    17,    27,     0,     6,     7,     0,     0,     0,     0,
       9,     0,     0,    10,    11,    12,    13,    18,    19,    14,
      15,    20,    16,     0,     0,     0,     0,     0,    29,    30,
       0,     0,     0,     0,     3,     4,     0,    17,    32,     5,
       0,     0,    25,    26,     0,     0,     0,    27,     0,     6,
       7,     0,     0,    18,    19,     9,     0,    20,    10,    11,
      12,    13,     0,     0,    14,    15,     0,    16,     0,     0,
       0,     0,     0,    29,    30,     0,     0,     0,    25,    26,
       0,   284,    17,    27,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    18,    19,
       0,     0,    20,     0,     0,     0,     0,     0,     0,    29,
      30,     3,     4,     0,     0,     0,     5,   390,     0,     0,
       0,    24,     0,    25,    26,     0,     6,     7,    27,     0,
       0,     0,     9,     0,     0,    10,    11,    12,    13,     0,
       0,    14,    15,     0,    16,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    29,    30,     3,     4,     0,    17,
     243,     5,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     6,     7,     0,     0,    18,    19,     9,     0,    20,
      10,    11,    12,    13,     0,     0,    14,    15,     0,    16,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      25,    26,     0,     0,    17,    27,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      18,    19,     0,     0,    20,     0,     0,     0,     0,     0,
       0,    29,    30,     0,     0,     0,     0,   371,     0,     0,
       0,     0,     0,     0,     0,    25,    26,     0,     0,     0,
      27,     0,     0,  -267,  -267,  -267,  -267,  -267,  -267,  -267,
    -267,  -267,  -267,  -267,  -267,  -267,  -267,  -267,  -267,  -267,
    -267,  -267,  -267,     0,     0,     0,    29,    30,     0,     0,
       0,  -267,   505,  -267,  -267,  -267,  -267,  -267,  -267,     0,
    -267,  -267,  -267,  -267,  -267,  -267,     0,     0,  -267,  -267,
    -267,  -267,  -267,  -267,  -267,  -274,  -274,  -274,  -274,  -274,
    -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,  -274,
    -274,  -274,  -274,  -274,  -274,     0,     0,     0,     0,     0,
       0,     0,     0,  -274,     0,  -274,  -274,  -274,  -274,  -274,
    -274,     0,  -274,  -274,  -274,  -274,  -274,  -274,     0,     0,
    -274,  -274,  -274,  -274,  -274,  -274,  -274
};

static const yytype_int16 yycheck[] =
{
       1,    52,     3,     4,     5,     6,     7,   268,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
       8,    78,    94,    24,    25,    26,    27,    78,    74,   200,
      76,    77,   299,    21,    22,    23,   256,    88,    22,    23,
     168,    42,   270,   168,    32,    46,     1,    42,     1,   487,
       3,     4,     5,     6,     7,   366,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    90,   195,
     218,    24,    25,    26,    27,    22,   109,   308,   150,   132,
       7,    28,   409,   311,    84,    85,   524,    33,   526,    42,
     111,    18,   113,    46,   135,    44,   267,   335,   336,   337,
     338,     7,   250,   134,   135,   146,   115,   116,    57,   106,
     107,   136,    18,   130,   131,   607,   133,   138,   266,   144,
     134,   448,   614,    30,   157,   142,   177,   178,   137,   138,
     144,   134,   183,   135,   133,    81,   136,   139,   366,    46,
     134,   144,   270,   140,   132,   134,   136,   385,   145,   110,
     144,   138,   134,   134,   144,   144,   117,   144,   105,   285,
     135,   149,   144,   144,   130,   131,   314,   133,   169,   135,
     298,   139,   173,   224,   196,   111,   144,   113,   166,    86,
     308,   130,   131,   311,   133,   137,   187,   188,   189,   190,
     501,   139,   130,   131,   195,   102,   144,    30,   346,   327,
     511,   151,   152,   153,   255,   158,   533,   140,   469,   136,
     143,   144,   136,    46,   169,   203,   169,   218,   171,   141,
     142,   441,   223,   130,   131,   137,   133,   139,   279,   136,
     287,   277,   360,   137,   187,   188,   189,   190,   366,   410,
     141,   568,   195,   144,   475,   130,   131,   558,   133,   134,
     137,   302,   483,    86,   140,   256,   251,   143,   144,   130,
     131,   283,   133,   130,   131,   141,   133,   135,   144,   102,
     223,   141,   273,   501,   144,   141,   137,   544,   144,    30,
     188,   189,   190,   511,   285,   413,   137,   140,   413,   142,
     137,   133,   440,   135,   143,    46,   145,   130,   131,   530,
     133,   135,   522,   136,   143,   144,   567,   295,   137,   297,
     147,   148,    84,    85,   137,   546,   143,   144,   364,   135,
     273,   393,   373,   143,   144,   143,   144,   315,   143,   144,
     558,   135,   285,   133,   554,    86,   143,   144,    30,   140,
     488,   489,   139,   296,   142,    30,   142,   475,   143,   144,
     570,   102,   143,   144,    46,   483,   484,   143,   144,   552,
     553,    46,   198,   199,   331,   332,   339,   340,   588,   589,
     333,   334,   543,   501,   142,   142,   142,   135,   154,   130,
     131,   155,   133,   511,   456,   136,   156,   388,   108,   377,
     134,   134,   134,   140,    86,   615,   134,   138,   144,   619,
     133,    86,   530,   551,   144,   135,   138,   144,   136,   138,
     102,   399,   138,    45,   136,   399,   564,   102,   546,   136,
      81,   142,   141,   135,   142,   127,   127,   473,   143,   142,
     558,   105,   146,   144,   144,   388,   143,   143,   130,   131,
     441,   133,   590,   142,   136,   130,   131,   142,   133,   143,
     598,   136,   142,   142,   138,   142,   144,   458,   144,   142,
     142,   449,   140,   135,   127,   135,   538,   143,   525,   112,
     139,   114,   143,   127,   525,   118,   119,   120,   121,   122,
     123,   124,   125,   142,   142,   138,   487,    24,   135,   146,
     478,   143,   141,   143,   134,   143,   134,   143,   143,   143,
     134,   143,   490,   146,   139,   458,   143,   143,   401,   510,
       1,     1,   375,   397,   373,     1,    42,   210,   211,   507,
     508,   522,    45,   476,   512,   510,   219,   220,   221,   222,
     295,   468,   593,   487,   342,    10,    11,   343,   341,   344,
      15,    -1,    -1,   345,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    26,    -1,   554,    -1,    -1,    31,   510,    -1,    34,
      35,    36,    37,    -1,    -1,    40,    41,    -1,    43,   570,
      -1,   559,    -1,    -1,    -1,   563,    -1,    -1,   566,    -1,
      -1,    -1,    -1,    58,    -1,    -1,    -1,   588,   589,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   584,    -1,    -1,    74,
      75,    -1,    -1,    78,    -1,    -1,   607,    -1,    -1,    -1,
      -1,    -1,   600,   614,   615,    -1,    -1,    -1,   619,    -1,
      -1,    -1,    -1,   611,    99,   100,    -1,    -1,    -1,   104,
      -1,    -1,    -1,    -1,   622,   328,   329,   330,   331,   332,
     333,   334,   335,   336,   337,   338,   339,   340,   341,   342,
     343,   344,   345,    -1,    -1,   130,   131,     4,    -1,    -1,
       7,    -1,    -1,    10,    11,    12,    13,   142,    15,    16,
      17,    18,    -1,    -1,    21,    22,    -1,    -1,    25,    26,
      27,    28,    -1,    -1,    31,    -1,    33,    34,    35,    36,
      37,    -1,   385,    40,    41,    42,    43,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    56,
      -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    72,    -1,    74,    75,    -1,
      -1,    78,    -1,    -1,    81,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      97,    -1,    99,   100,    -1,    -1,    -1,   104,   105,   106,
     107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,
     127,   128,    -1,   130,   131,   132,    -1,   134,   135,   136,
      -1,    -1,    -1,   140,    -1,   142,    -1,    -1,    -1,    -1,
     147,   148,   149,   150,    -1,     4,    -1,    -1,     7,    -1,
      -1,    10,    11,    12,    13,    -1,    15,    16,    17,    18,
      -1,    -1,    21,    22,    -1,   518,    25,    26,    27,    28,
      -1,    -1,    31,    -1,    33,    34,    35,    36,    37,    -1,
      -1,    40,    41,    42,    43,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,    58,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    72,    -1,    74,    75,    -1,    -1,    78,
      -1,    -1,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    -1,
      99,   100,    -1,    -1,    -1,   104,   105,   106,   107,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,   127,   128,
      -1,   130,   131,   132,    -1,   134,   135,   136,    -1,    -1,
      -1,   140,    -1,   142,    -1,    -1,    -1,    -1,   147,   148,
     149,   150,     4,    -1,    -1,     7,    -1,    -1,    10,    11,
      12,    13,    -1,    15,    16,    17,    18,    -1,    -1,    21,
      22,    -1,    -1,    25,    26,    27,    28,    -1,    -1,    31,
      -1,    33,    34,    35,    36,    37,    -1,    -1,    40,    41,
      42,    43,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    56,    -1,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      72,    -1,    74,    75,    -1,    -1,    78,    -1,    -1,    81,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    -1,    99,   100,    -1,
      -1,    -1,   104,   105,   106,   107,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   126,   127,   128,    -1,   130,   131,
     132,    -1,   134,   135,   136,    -1,    -1,    -1,   140,    -1,
     142,    -1,    -1,    -1,    -1,   147,   148,   149,   150,     4,
      -1,    -1,     7,    -1,    -1,    10,    11,    12,    13,    -1,
      15,    16,    17,    18,    -1,    -1,    21,    22,    -1,    -1,
      25,    26,    27,    28,    -1,    -1,    31,    -1,    33,    34,
      35,    36,    37,    -1,    -1,    40,    41,    42,    43,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    56,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    72,    -1,    74,
      75,    -1,    -1,    78,    -1,    -1,    81,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    -1,    99,   100,    -1,    -1,    -1,   104,
     105,   106,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   126,   127,   128,    -1,   130,   131,   132,    -1,   134,
     135,   136,    -1,    -1,    -1,   140,    -1,   142,    -1,    -1,
       4,    -1,   147,   148,   149,   150,    10,    11,    12,    13,
      -1,    15,    16,    17,    -1,    -1,    -1,    21,    22,    -1,
      -1,    25,    26,    27,    28,    -1,    -1,    31,    -1,    33,
      34,    35,    36,    37,    -1,    -1,    40,    41,    42,    43,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    56,    -1,    58,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    72,    -1,
      74,    75,    -1,    -1,    78,    -1,    -1,    81,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    97,    -1,    99,   100,    -1,    -1,    -1,
     104,   105,   106,   107,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   126,   127,   128,    -1,   130,   131,   132,    -1,
     134,   135,   136,    -1,    -1,    -1,   140,    -1,   142,    -1,
      -1,     4,    -1,   147,   148,   149,   150,    10,    11,    12,
      13,    -1,    15,    16,    17,    -1,    -1,    -1,    21,    22,
      -1,    -1,    25,    26,    27,    28,    -1,    -1,    31,    -1,
      33,    34,    35,    36,    37,    -1,    -1,    40,    41,    42,
      43,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    56,    -1,    58,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    72,
      -1,    74,    75,    -1,    -1,    78,    -1,    -1,    81,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    97,    -1,    99,   100,    -1,    -1,
      -1,   104,   105,   106,   107,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   126,   127,   128,    -1,   130,   131,   132,
      -1,   134,   135,    -1,    -1,    -1,    -1,   140,    -1,   142,
      -1,    -1,    -1,    -1,   147,   148,   149,   150,    10,    11,
      12,    13,    -1,    15,    16,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    26,    27,    -1,    -1,    -1,    31,
      -1,    -1,    34,    35,    36,    37,    -1,    -1,    40,    41,
      42,    43,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    58,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      72,    -1,    74,    75,    -1,    -1,    78,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    -1,    -1,    99,   100,    -1,
      -1,    -1,   104,    -1,   106,   107,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   126,   127,   128,    -1,   130,   131,
     132,    -1,    -1,   135,   136,    -1,    10,    11,    12,    13,
     142,    15,    16,    -1,    -1,   147,   148,   149,   150,    -1,
      -1,    25,    26,    27,    -1,    -1,    -1,    31,    -1,    -1,
      34,    35,    36,    37,    -1,    -1,    40,    41,    42,    43,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    58,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    72,    -1,
      74,    75,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    -1,    -1,    99,   100,    -1,    -1,    -1,
     104,    -1,   106,   107,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   126,   127,   128,    -1,   130,   131,   132,    -1,
     134,    -1,    -1,    -1,    10,    11,    12,    13,   142,    15,
      16,    -1,    -1,   147,   148,   149,   150,    10,    11,    25,
      26,    27,    15,    -1,    -1,    31,    -1,    -1,    34,    35,
      36,    37,    25,    26,    40,    41,    42,    43,    31,    -1,
      -1,    34,    35,    36,    37,    -1,    -1,    40,    41,    -1,
      43,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    58,    72,    -1,    74,    75,
      -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    74,    75,    -1,    -1,    78,    -1,    -1,    -1,    -1,
      96,    -1,    -1,    99,   100,    -1,    -1,    -1,   104,    -1,
     106,   107,    -1,    -1,    -1,    -1,    99,   100,    -1,    -1,
      -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     126,   127,   128,    -1,   130,   131,   132,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,   130,   131,    -1,
      -1,   147,   148,   149,   150,    10,    11,    12,    13,    -1,
      15,    16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    26,    27,    -1,    -1,    -1,    31,    -1,    -1,    34,
      35,    36,    37,    -1,    -1,    40,    41,    42,    43,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    72,    -1,    74,
      75,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    -1,    -1,    99,   100,    -1,    -1,    -1,   104,
      -1,   106,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   126,   127,   128,    -1,   130,   131,   132,    -1,    -1,
     135,    -1,    -1,    10,    11,    12,    13,   142,    15,    16,
      -1,    -1,   147,   148,   149,   150,    -1,    -1,    25,    26,
      27,    -1,    -1,    -1,    31,    -1,    -1,    34,    35,    36,
      37,    -1,    -1,    40,    41,    42,    43,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    72,    -1,    74,    75,    -1,
      -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      -1,    -1,    99,   100,    -1,    -1,    -1,   104,    -1,   106,
     107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,
     127,   128,    -1,   130,   131,   132,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,   143,    -1,    -1,    -1,
     147,   148,   149,   150,    10,    11,    12,    13,    -1,    15,
      16,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      26,    27,    -1,    -1,    -1,    31,    -1,    -1,    34,    35,
      36,    37,    -1,    -1,    40,    41,    42,    43,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    72,    -1,    74,    75,
      -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    -1,    -1,    99,   100,    -1,    -1,    -1,   104,    -1,
     106,   107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     126,   127,   128,    -1,   130,   131,   132,    -1,   134,    -1,
      -1,    -1,    10,    11,    12,    13,   142,    15,    16,    -1,
      -1,   147,   148,   149,   150,    -1,    -1,    25,    26,    27,
      -1,    -1,    -1,    31,    -1,    -1,    34,    35,    36,    37,
      -1,    -1,    40,    41,    42,    43,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    72,    -1,    74,    75,    -1,    -1,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    -1,
      -1,    99,   100,    -1,    -1,    -1,   104,    -1,   106,   107,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,   127,
     128,    -1,   130,   131,   132,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,   143,    -1,    -1,    -1,   147,
     148,   149,   150,    10,    11,    12,    13,    -1,    15,    16,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    26,
      27,    -1,    -1,    -1,    31,    -1,    -1,    34,    35,    36,
      37,    -1,    -1,    40,    41,    42,    43,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    72,    -1,    74,    75,    -1,
      -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      -1,    -1,    99,   100,    -1,    -1,    -1,   104,    -1,   106,
     107,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,
     127,   128,     0,   130,   131,   132,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    11,    -1,   142,    -1,    15,    -1,    -1,
     147,   148,   149,   150,    -1,    10,    11,    25,    26,    -1,
      15,    29,    -1,    31,    -1,    -1,    34,    35,    36,    37,
      25,    26,    40,    41,    -1,    43,    31,    -1,    -1,    34,
      35,    36,    37,    -1,    -1,    40,    41,    -1,    43,    -1,
      58,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    58,    -1,    -1,    74,    75,    -1,    -1,
      78,    -1,    -1,    -1,    -1,    83,    84,    85,    -1,    74,
      75,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    97,
      -1,    99,   100,    -1,    -1,    -1,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    99,   100,    -1,    -1,   103,   104,
      -1,    -1,    -1,    -1,     3,    -1,     5,     6,    -1,     8,
       9,   129,   130,   131,    -1,    14,   134,    -1,    -1,    -1,
      19,    20,   140,    -1,    23,   130,   131,    -1,    -1,    -1,
      -1,    30,    -1,    32,    -1,    -1,    -1,    -1,   143,    38,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    -1,    -1,    -1,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    -1,    73,    -1,    -1,    -1,    -1,    -1,
      79,    80,    -1,    82,    -1,    -1,    -1,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    -1,    -1,    98,
      -1,    -1,   101,   102,   103,    -1,     3,    -1,     5,     6,
      -1,     8,    -1,    -1,    -1,    -1,    -1,    14,    -1,    -1,
      -1,    -1,    19,    20,    -1,    -1,    23,    -1,    -1,    -1,
      -1,    -1,    -1,    30,   133,    32,    -1,    -1,    -1,    -1,
      -1,    38,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    -1,
      -1,    -1,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    -1,    73,    -1,    -1,    -1,
      -1,    -1,    79,    80,    -1,    -1,    -1,    -1,    -1,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    -1,
      -1,    98,    -1,    -1,   101,   102,   103,    -1,     3,    -1,
       5,     6,    -1,     8,    -1,    -1,    -1,    -1,    -1,    14,
      -1,    -1,    -1,    -1,    19,    20,    -1,    -1,    23,    -1,
      -1,    -1,    -1,    -1,    -1,    30,   133,    32,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    -1,    -1,    -1,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    -1,    73,    -1,
      -1,    -1,    -1,    -1,    79,    80,    -1,    -1,    -1,    -1,
      -1,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    -1,    -1,    98,    -1,    -1,   101,   102,   103,    -1,
       3,    -1,     5,     6,    -1,     8,    -1,    -1,    -1,    -1,
      -1,    14,    -1,    -1,    -1,    -1,    19,    20,    -1,    -1,
      23,    -1,    -1,    -1,    -1,    -1,    -1,    30,   133,    32,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    -1,    -1,    -1,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    -1,
      73,    -1,    -1,    -1,    -1,    -1,    79,    80,    -1,    -1,
      -1,    -1,    -1,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    -1,    -1,    98,    -1,    -1,   101,   102,
     103,    -1,     3,    -1,     5,     6,    -1,     8,    -1,    -1,
      -1,    -1,    -1,    14,    -1,    -1,    -1,    -1,    19,    20,
      -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    30,
     133,    32,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    -1,    -1,    -1,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    -1,    73,    -1,    -1,    -1,    -1,    -1,    79,    80,
      -1,    -1,    -1,    -1,    -1,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    -1,    -1,    98,    10,    11,
     101,   102,    -1,    15,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    25,    26,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    34,    35,    36,    37,    -1,    -1,    40,    41,
      -1,    43,   133,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    10,    11,    -1,    58,    -1,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    26,
      -1,    -1,    74,    75,    31,    -1,    78,    34,    35,    36,
      37,    -1,    -1,    40,    41,    -1,    43,    -1,    -1,    10,
      11,    -1,    -1,    -1,    15,    -1,    -1,    99,   100,    -1,
      -1,    58,   104,    -1,    25,    26,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    34,    35,    36,    37,    74,    75,    40,
      41,    78,    43,    -1,    -1,    -1,    -1,    -1,   130,   131,
      -1,    -1,    -1,    -1,    10,    11,    -1,    58,   140,    15,
      -1,    -1,    99,   100,    -1,    -1,    -1,   104,    -1,    25,
      26,    -1,    -1,    74,    75,    31,    -1,    78,    34,    35,
      36,    37,    -1,    -1,    40,    41,    -1,    43,    -1,    -1,
      -1,    -1,    -1,   130,   131,    -1,    -1,    -1,    99,   100,
      -1,   138,    58,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    74,    75,
      -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,   130,
     131,    10,    11,    -1,    -1,    -1,    15,   138,    -1,    -1,
      -1,    97,    -1,    99,   100,    -1,    25,    26,   104,    -1,
      -1,    -1,    31,    -1,    -1,    34,    35,    36,    37,    -1,
      -1,    40,    41,    -1,    43,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   130,   131,    10,    11,    -1,    58,
     136,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    26,    -1,    -1,    74,    75,    31,    -1,    78,
      34,    35,    36,    37,    -1,    -1,    40,    41,    -1,    43,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      99,   100,    -1,    -1,    58,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      74,    75,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,
      -1,   130,   131,    -1,    -1,    -1,    -1,   136,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    99,   100,    -1,    -1,    -1,
     104,    -1,    -1,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,    -1,    -1,    -1,   130,   131,    -1,    -1,
      -1,   134,   136,   136,   137,   138,   139,   140,   141,    -1,
     143,   144,   145,   146,   147,   148,    -1,    -1,   151,   152,
     153,   154,   155,   156,   157,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   134,    -1,   136,   137,   138,   139,   140,
     141,    -1,   143,   144,   145,   146,   147,   148,    -1,    -1,
     151,   152,   153,   154,   155,   156,   157
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   159,     0,    10,    11,    15,    25,    26,    29,    31,
      34,    35,    36,    37,    40,    41,    43,    58,    74,    75,
      78,    83,    84,    85,    97,    99,   100,   104,   129,   130,
     131,   134,   140,   166,   167,   168,   169,   172,   173,   177,
     178,   187,   188,   190,   191,   192,   198,   214,   216,   219,
     221,   229,   232,   232,   232,   232,   232,   232,   130,   131,
     133,   182,   232,   232,   232,   232,   232,   232,   232,   232,
     232,   232,   232,   232,   160,   182,   160,   160,   232,   232,
     232,   232,   132,   182,   135,   174,   187,   191,   232,   134,
     135,   193,   232,   134,   144,     3,     5,     6,     8,     9,
      14,    19,    20,    23,    30,    32,    38,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      73,    79,    80,    82,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    98,   101,   102,   103,   133,   175,
     179,   180,   181,   207,   208,   209,   210,   212,   213,   137,
     163,   163,   163,   163,   179,   213,   215,   141,   142,   176,
      80,   213,   136,   194,   182,   222,   228,   137,   137,   133,
     135,   182,   133,   137,   182,   134,   228,   137,   137,   137,
     137,   130,   131,   198,   228,   197,   135,   135,   135,   135,
     182,   217,   218,    12,    13,    16,    27,    42,    72,    96,
     106,   107,   126,   127,   128,   130,   131,   132,   142,   147,
     148,   149,   150,   198,   232,   235,   236,   237,   250,   251,
     252,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     262,   263,   264,   136,   214,   232,   133,     4,    17,    21,
      56,   188,   189,   193,   214,   232,   238,   239,   240,   241,
     243,   244,   247,   248,   264,   266,   140,   230,   231,   135,
     146,   213,   213,   184,   139,   183,   213,   199,   211,   232,
     211,   211,   211,   142,   138,   162,   221,   232,   194,   165,
     194,   165,   165,   231,   134,   144,   182,   142,   142,   135,
     252,   252,   232,   266,   252,   252,   252,   252,   142,   213,
     143,   144,   106,   107,   140,   145,   112,   114,   118,   119,
     120,   121,   122,   123,   124,   125,   146,   265,   151,   152,
     153,   147,   148,   111,   113,   115,   116,   137,   138,   110,
     117,   154,   155,   156,   108,   109,   157,   213,   134,   134,
     134,   134,   266,    33,   195,   196,   213,   136,   239,   134,
     144,   141,   266,   231,   199,   223,   135,   233,   235,   138,
     144,   136,   186,   232,   133,   135,   144,   139,   163,   200,
     201,   202,   138,   213,   138,   144,   138,   138,   194,   203,
     138,   221,   134,   179,   167,   168,   170,   171,   136,    45,
     161,   164,   136,   136,   218,   198,   182,   236,   249,   223,
     213,   143,   249,   142,   141,   235,   266,   182,   264,   252,
     252,   252,   252,   253,   253,   254,   254,   255,   255,   255,
     255,   256,   256,   257,   258,   259,   260,   261,   266,   134,
     142,    22,    28,   105,    81,   264,   141,   163,   135,   226,
     227,   233,   234,   127,    80,   181,   185,   213,   184,   127,
      44,    57,   182,   255,   103,   204,   205,   206,   232,   143,
     134,   136,   170,   160,   161,   142,   144,   143,   226,   231,
     143,   236,   141,   142,   139,   266,   239,   142,   142,   142,
     226,    30,    46,    86,   102,   136,   182,   224,   136,   144,
     136,   144,   144,   220,   228,   136,   138,   142,   142,   138,
     144,   146,   212,   199,   163,   249,   198,   136,   143,   143,
     249,   264,   143,   105,   219,   232,   248,   266,   266,   136,
     142,   140,   225,   135,   136,   233,   127,   134,   144,   182,
     182,   205,   233,   182,   135,   143,   142,   252,   143,   239,
     242,   142,   248,   248,   143,   143,   249,   127,   146,   226,
     138,   228,   143,   145,   140,   143,   144,   231,   223,   249,
      24,   266,   247,   247,   239,   135,   143,   141,   233,   136,
     182,   266,   182,   199,   226,   143,   239,   143,   143,   143,
       7,    18,   245,   246,   134,   134,   143,   141,   140,   143,
     144,   136,   143,   134,   239,   239,   266,   139,   136,   245,
     143,   144,   266,   182,   139,   238,   182,   141,   143,   238,
     143,   143,   144,   182,   143
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int16 yyr1[] =
{
       0,   158,   159,   159,   159,   159,   159,   159,   159,   159,
     160,   160,   161,   162,   162,   163,   163,   163,   164,   164,
     165,   165,   166,   167,   168,   169,   169,   169,   170,   170,
     171,   171,   172,   173,   174,   175,   175,   176,   176,   177,
     178,   179,   179,   180,   181,   182,   182,   182,   183,   183,
     184,   184,   185,   185,   186,   187,   187,   188,   188,   189,
     189,   190,   190,   191,   192,   192,   193,   193,   194,   195,
     196,   197,   198,   198,   199,   199,   199,   199,   200,   201,
     201,   201,   201,   201,   201,   201,   202,   202,   203,   203,
     203,   204,   204,   205,   205,   206,   207,   207,   207,   207,
     207,   207,   207,   207,   208,   208,   209,   209,   209,   209,
     209,   209,   209,   210,   210,   210,   210,   210,   210,   210,
     211,   212,   212,   212,   212,   212,   212,   212,   212,   212,
     212,   212,   212,   212,   212,   212,   212,   212,   212,   212,
     212,   212,   212,   212,   212,   212,   212,   212,   212,   212,
     212,   212,   212,   212,   212,   212,   213,   213,   214,   214,
     214,   215,   215,   216,   217,   217,   218,   219,   220,   220,
     221,   221,   222,   223,   224,   224,   224,   224,   224,   225,
     225,   226,   226,   226,   227,   227,   228,   228,   228,   228,
     229,   229,   230,   230,   231,   231,   232,   232,   232,   232,
     232,   232,   232,   232,   232,   232,   232,   232,   232,   232,
     232,   232,   232,   232,   232,   232,   232,   232,   233,   233,
     233,   234,   234,   235,   236,   236,   237,   237,   238,   238,
     239,   239,   239,   239,   239,   239,   239,   240,   240,   240,
     240,   240,   241,   242,   242,   243,   243,   243,   243,   244,
     245,   245,   245,   245,   246,   246,   247,   247,   248,   249,
     249,   250,   250,   250,   250,   250,   250,   250,   250,   250,
     250,   250,   250,   250,   250,   251,   251,   251,   251,   251,
     251,   251,   252,   252,   252,   252,   252,   252,   252,   252,
     253,   253,   253,   253,   254,   254,   254,   255,   255,   255,
     256,   256,   256,   256,   256,   257,   257,   257,   258,   258,
     259,   259,   260,   260,   261,   261,   262,   262,   263,   263,
     264,   264,   265,   265,   265,   265,   265,   265,   265,   265,
     265,   265,   265,   266,   266
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     3,     2,     2,     2,     2,     2,
       0,     1,     7,     2,     3,     0,     3,     4,     1,     2,
       1,     2,     6,     6,     6,     1,     1,     1,     1,     1,
       1,     2,     7,     5,     3,     1,     1,     0,     2,     2,
       3,     1,     1,     6,     4,     1,     1,     1,     0,     2,
       0,     2,     1,     1,     4,     3,     6,     1,     2,     0,
       1,     2,     2,     7,     1,     2,     2,     4,     0,     0,
       0,     0,     1,     1,     0,     2,     2,     2,     2,     5,
       8,     7,    10,    10,     9,    12,     5,     7,     1,     2,
       2,     1,     3,     1,     3,     5,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     6,     1,     8,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     4,     4,     6,     1,     4,     4,     1,
       1,     1,     2,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     4,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     5,     1,     3,     2,     2,     1,     3,
       1,     3,     4,     0,     1,     1,     1,     1,     1,     0,
       3,     0,     6,     6,     3,     5,     1,     3,     5,     5,
       3,     3,     2,     3,     0,     2,     0,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     1,     3,
       4,     1,     3,     1,     1,     3,     1,     1,     1,     2,
       1,     1,     1,     1,     1,     1,     1,     2,     2,     3,
       2,     2,     6,     1,     3,     7,     9,     9,     9,     9,
       4,     3,     3,     2,     1,     2,     0,     1,     2,     0,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     6,
       9,     4,     4,     5,     1,     1,     2,     2,     3,     4,
       5,     6,     1,     2,     2,     2,     2,     2,     2,     6,
       1,     3,     3,     3,     1,     3,     3,     1,     3,     3,
       1,     3,     3,     3,     3,     1,     3,     3,     1,     3,
       1,     3,     1,     3,     1,     3,     1,     3,     1,     5,
       1,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     3
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
        yyerror (&yylloc, scanner, ctx, YY_("syntax error: cannot back up")); \
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
#if HLSL_YYDEBUG

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
# if defined HLSL_YYLTYPE_IS_TRIVIAL && HLSL_YYLTYPE_IS_TRIVIAL

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
                  Type, Value, Location, scanner, ctx); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, void *scanner, struct hlsl_ctx *ctx)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  YYUSE (yylocationp);
  YYUSE (scanner);
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
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, void *scanner, struct hlsl_ctx *ctx)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  YY_LOCATION_PRINT (yyo, *yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yytype, yyvaluep, yylocationp, scanner, ctx);
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule, void *scanner, struct hlsl_ctx *ctx)
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
                       , &(yylsp[(yyi + 1) - (yynrhs)])                       , scanner, ctx);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule, scanner, ctx); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !HLSL_YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !HLSL_YYDEBUG */


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
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, void *scanner, struct hlsl_ctx *ctx)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  YYUSE (scanner);
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
yyparse (void *scanner, struct hlsl_ctx *ctx)
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
# if defined HLSL_YYLTYPE_IS_TRIVIAL && HLSL_YYLTYPE_IS_TRIVIAL
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
      yychar = yylex (&yylval, &yylloc, scanner);
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
  case 5:
#line 6816 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            hlsl_block_add_block(&ctx->static_initializers, (yyvsp[0].block));
            destroy_block((yyvsp[0].block));
        }
#line 9216 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 10:
#line 6827 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.name) = NULL;
        }
#line 9224 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 12:
#line 6834 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            if (!add_pass(ctx, (yyvsp[-5].name), (yyvsp[-4].scope), (yyvsp[-1].state_block), &(yylsp[-6])))
                YYABORT;
        }
#line 9233 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 13:
#line 6841 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            struct hlsl_block *block;

            block = initialize_vars(ctx, (yyvsp[-1].list));
            destroy_block(block);
        }
#line 9244 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 14:
#line 6848 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            struct hlsl_block *block;

            block = initialize_vars(ctx, (yyvsp[-1].list));
            destroy_block(block);
        }
#line 9255 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 15:
#line 6857 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.scope) = NULL;
        }
#line 9263 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 16:
#line 6861 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            hlsl_pop_scope(ctx);
            (yyval.scope) = NULL;
        }
#line 9272 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 17:
#line 6866 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            struct hlsl_scope *scope = ctx->cur_scope;

            hlsl_pop_scope(ctx);
            (yyval.scope) = scope;
        }
#line 9283 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 22:
#line 6883 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            struct hlsl_scope *scope = ctx->cur_scope;
            hlsl_pop_scope(ctx);

            if (!add_technique(ctx, (yyvsp[-4].name), scope, (yyvsp[-3].scope), "technique", &(yylsp[-5])))
                YYABORT;
        }
#line 9295 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 23:
#line 6893 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            struct hlsl_scope *scope = ctx->cur_scope;
            hlsl_pop_scope(ctx);

            if (!add_technique(ctx, (yyvsp[-4].name), scope, (yyvsp[-3].scope), "technique10", &(yylsp[-5])))
                YYABORT;
        }
#line 9307 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 24:
#line 6903 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            struct hlsl_scope *scope = ctx->cur_scope;
            hlsl_pop_scope(ctx);

            if (ctx->profile->type == VKD3D_SHADER_TYPE_EFFECT && ctx->profile->major_version == 2)
                hlsl_error(ctx, &(yylsp[-5]), VKD3D_SHADER_ERROR_HLSL_INVALID_SYNTAX,
                        "The 'technique11' keyword is invalid for this profile.");

            if (!add_technique(ctx, (yyvsp[-4].name), scope, (yyvsp[-3].scope), "technique11", &(yylsp[-5])))
                YYABORT;
        }
#line 9323 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 32:
#line 6930 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            struct hlsl_scope *scope = ctx->cur_scope;
            hlsl_pop_scope(ctx);
            if (!(add_effect_group(ctx, (yyvsp[-5].name), scope, (yyvsp[-4].scope), &(yylsp[-5]))))
                YYABORT;
        }
#line 9334 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 33:
#line 6939 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            if ((yyvsp[-1].colon_attributes).semantic.name)
                hlsl_error(ctx, &(yylsp[-1]), VKD3D_SHADER_ERROR_HLSL_INVALID_SEMANTIC, "Semantics are not allowed on buffers.");

            if (!(ctx->cur_buffer = hlsl_new_buffer(ctx, (yyvsp[-3].buffer_type), (yyvsp[-2].name), (yyvsp[-4].modifiers), &(yyvsp[-1].colon_attributes).reg_reservation, (yyvsp[0].scope), &(yylsp[-2]))))
                YYABORT;
        }
#line 9346 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 34:
#line 6949 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            ctx->cur_buffer = ctx->globals_buffer;
        }
#line 9354 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 35:
#line 6955 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.buffer_type) = HLSL_BUFFER_CONSTANT;
        }
#line 9362 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 36:
#line 6959 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.buffer_type) = HLSL_BUFFER_TEXTURE;
        }
#line 9370 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 39:
#line 6969 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            const char **new_array = NULL;

            ctx->location.line = (yyvsp[-1].intval);
            if (strcmp((yyvsp[0].name), ctx->location.source_name))
                new_array = hlsl_realloc(ctx, ctx->source_files,
                        sizeof(*ctx->source_files) * (ctx->source_files_count + 1));

            if (new_array)
            {
                ctx->source_files = new_array;
                ctx->source_files[ctx->source_files_count++] = (yyvsp[0].name);
                ctx->location.source_name = (yyvsp[0].name);
            }
            else
            {
                vkd3d_free((yyvsp[0].name));
            }
        }
#line 9394 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 40:
#line 6991 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            if (!(yyvsp[-1].type)->name)
                hlsl_error(ctx, &(yylsp[-1]), VKD3D_SHADER_ERROR_HLSL_INVALID_SYNTAX,
                    "Anonymous struct type must declare a variable.");

            if ((yyvsp[-2].modifiers))
                hlsl_error(ctx, &(yylsp[-2]), VKD3D_SHADER_ERROR_HLSL_INVALID_MODIFIER,
                        "Modifiers are not allowed on struct type declarations.");

            if (!((yyval.block) = make_empty_block(ctx)))
                YYABORT;
        }
#line 9411 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 43:
#line 7010 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            bool ret;

            if ((yyvsp[-3].type))
            {
                char *name;

                if (!(name = hlsl_strdup(ctx, "$super")))
                    YYABORT;
                if (!hlsl_array_reserve(ctx, (void **)&(yyvsp[-1].fields).fields, &(yyvsp[-1].fields).capacity, 1 + (yyvsp[-1].fields).count, sizeof(*(yyvsp[-1].fields).fields)))
                    YYABORT;
                memmove(&(yyvsp[-1].fields).fields[1], (yyvsp[-1].fields).fields, (yyvsp[-1].fields).count * sizeof(*(yyvsp[-1].fields).fields));
                ++(yyvsp[-1].fields).count;

                memset(&(yyvsp[-1].fields).fields[0], 0, sizeof((yyvsp[-1].fields).fields[0]));
                (yyvsp[-1].fields).fields[0].type = (yyvsp[-3].type);
                (yyvsp[-1].fields).fields[0].loc = (yylsp[-3]);
                (yyvsp[-1].fields).fields[0].name = name;
            }

            (yyval.type) = hlsl_new_struct_type(ctx, (yyvsp[-4].name), (yyvsp[-1].fields).fields, (yyvsp[-1].fields).count);

            if (hlsl_get_var(ctx->cur_scope, (yyvsp[-4].name)))
            {
                hlsl_error(ctx, &(yylsp[-4]), VKD3D_SHADER_ERROR_HLSL_REDEFINED, "\"%s\" is already declared as a variable.", (yyvsp[-4].name));
                YYABORT;
            }

            ret = hlsl_scope_add_type(ctx->cur_scope, (yyval.type));
            if (!ret)
            {
                hlsl_error(ctx, &(yylsp[-4]), VKD3D_SHADER_ERROR_HLSL_REDEFINED, "Struct \"%s\" is already defined.", (yyvsp[-4].name));
                YYABORT;
            }
        }
#line 9451 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 44:
#line 7048 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.type) = hlsl_new_struct_type(ctx, NULL, (yyvsp[-1].fields).fields, (yyvsp[-1].fields).count);
        }
#line 9459 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 48:
#line 7060 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.type) = NULL;
        }
#line 9467 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 49:
#line 7064 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.type) = hlsl_get_type(ctx->cur_scope, (yyvsp[0].name), true, true);
            if ((yyval.type)->class != HLSL_CLASS_STRUCT)
            {
                hlsl_error(ctx, &(yylsp[0]), VKD3D_SHADER_ERROR_HLSL_INVALID_TYPE, "Base type \"%s\" is not a struct.", (yyvsp[0].name));
                YYABORT;
            }
            vkd3d_free((yyvsp[0].name));
        }
#line 9481 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 50:
#line 7076 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.fields).fields = NULL;
            (yyval.fields).count = 0;
            (yyval.fields).capacity = 0;
        }
#line 9491 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 51:
#line 7082 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            size_t i;

            for (i = 0; i < (yyvsp[0].fields).count; ++i)
            {
                const struct hlsl_struct_field *field = &(yyvsp[0].fields).fields[i];
                const struct hlsl_struct_field *existing;

                if ((existing = get_struct_field((yyvsp[-1].fields).fields, (yyvsp[-1].fields).count, field->name)))
                {
                    hlsl_error(ctx, &field->loc, VKD3D_SHADER_ERROR_HLSL_REDEFINED,
                            "Field \"%s\" is already defined.", field->name);
                    hlsl_note(ctx, &existing->loc, VKD3D_SHADER_LOG_ERROR,
                            "'%s' was previously defined here.", field->name);
                }
            }

            if (!hlsl_array_reserve(ctx, (void **)&(yyvsp[-1].fields).fields, &(yyvsp[-1].fields).capacity, (yyvsp[-1].fields).count + (yyvsp[0].fields).count, sizeof(*(yyvsp[-1].fields).fields)))
                YYABORT;
            memcpy((yyvsp[-1].fields).fields + (yyvsp[-1].fields).count, (yyvsp[0].fields).fields, (yyvsp[0].fields).count * sizeof(*(yyvsp[0].fields).fields));
            (yyvsp[-1].fields).count += (yyvsp[0].fields).count;
            vkd3d_free((yyvsp[0].fields).fields);

            (yyval.fields) = (yyvsp[-1].fields);
        }
#line 9521 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 54:
#line 7114 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            struct hlsl_type *type;
            uint32_t modifiers = (yyvsp[-3].modifiers);

            if (!(type = apply_type_modifiers(ctx, (yyvsp[-2].type), &modifiers, true, &(yylsp[-3]))))
                YYABORT;
            if (modifiers & ~HLSL_INTERPOLATION_MODIFIERS_MASK)
            {
                struct vkd3d_string_buffer *string;

                if ((string = hlsl_modifiers_to_string(ctx, modifiers)))
                    hlsl_error(ctx, &(yylsp[-3]), VKD3D_SHADER_ERROR_HLSL_INVALID_MODIFIER,
                            "Modifiers '%s' are not allowed on struct fields.", string->buffer);
                hlsl_release_string_buffer(ctx, string);
            }
            if (!gen_struct_fields(ctx, &(yyval.fields), type, modifiers, (yyvsp[-1].list)))
                YYABORT;
        }
#line 9544 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 55:
#line 7135 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            if (!((yyval.attr) = hlsl_alloc(ctx, offsetof(struct hlsl_attribute, args[0]))))
            {
                vkd3d_free((yyvsp[-1].name));
                YYABORT;
            }
            (yyval.attr)->name = (yyvsp[-1].name);
            hlsl_block_init(&(yyval.attr)->instrs);
            (yyval.attr)->loc = (yyloc);
            (yyval.attr)->args_count = 0;
        }
#line 9560 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 56:
#line 7147 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            unsigned int i;

            if (!((yyval.attr) = hlsl_alloc(ctx, offsetof(struct hlsl_attribute, args[(yyvsp[-2].initializer).args_count]))))
            {
                vkd3d_free((yyvsp[-4].name));
                free_parse_initializer(&(yyvsp[-2].initializer));
                YYABORT;
            }
            (yyval.attr)->name = (yyvsp[-4].name);
            hlsl_block_init(&(yyval.attr)->instrs);
            hlsl_block_add_block(&(yyval.attr)->instrs, (yyvsp[-2].initializer).instrs);
            (yyval.attr)->loc = (yyloc);
            (yyval.attr)->args_count = (yyvsp[-2].initializer).args_count;
            for (i = 0; i < (yyvsp[-2].initializer).args_count; ++i)
                hlsl_src_from_node(&(yyval.attr)->args[i], (yyvsp[-2].initializer).args[i]);
            free_parse_initializer(&(yyvsp[-2].initializer));
        }
#line 9583 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 57:
#line 7168 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.attr_list).count = 1;
            if (!((yyval.attr_list).attrs = hlsl_alloc(ctx, sizeof(*(yyval.attr_list).attrs))))
            {
                hlsl_free_attribute((yyvsp[0].attr));
                YYABORT;
            }
            (yyval.attr_list).attrs[0] = (yyvsp[0].attr);
        }
#line 9597 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 58:
#line 7178 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            const struct hlsl_attribute **new_array;

            (yyval.attr_list) = (yyvsp[-1].attr_list);
            if (!(new_array = vkd3d_realloc((yyval.attr_list).attrs, ((yyval.attr_list).count + 1) * sizeof(*(yyval.attr_list).attrs))))
            {
                cleanup_parse_attribute_list(&(yyval.attr_list));
                YYABORT;
            }
            (yyval.attr_list).attrs = new_array;
            (yyval.attr_list).attrs[(yyval.attr_list).count++] = (yyvsp[0].attr);
        }
#line 9614 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 59:
#line 7193 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.attr_list).count = 0;
            (yyval.attr_list).attrs = NULL;
        }
#line 9623 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 61:
#line 7201 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            struct hlsl_ir_function_decl *decl = (yyvsp[-1].function).decl;

            if (decl->has_body)
            {
                hlsl_error(ctx, &decl->loc, VKD3D_SHADER_ERROR_HLSL_REDEFINED,
                         "Function \"%s\" is already defined.", decl->func->name);
                hlsl_note(ctx, &decl->loc, VKD3D_SHADER_LOG_ERROR,
                         "\"%s\" was previously defined here.", decl->func->name);
                destroy_block((yyvsp[0].block));
            }
            else
            {
                size_t i;

                decl->has_body = true;
                hlsl_block_add_block(&decl->body, (yyvsp[0].block));
                destroy_block((yyvsp[0].block));

                /* Semantics are taken from whichever definition has a body.
                 * We can't just replace the hlsl_ir_var pointers, though: if
                 * the function was already declared but not defined, the
                 * callers would have used the old declaration's parameters to
                 * transfer arguments. */

                if (!(yyvsp[-1].function).first)
                {
                    VKD3D_ASSERT(decl->parameters.count == (yyvsp[-1].function).parameters.count);

                    for (i = 0; i < (yyvsp[-1].function).parameters.count; ++i)
                    {
                        struct hlsl_ir_var *dst = decl->parameters.vars[i];
                        struct hlsl_ir_var *src = (yyvsp[-1].function).parameters.vars[i];

                        hlsl_cleanup_semantic(&dst->semantic);
                        dst->semantic = src->semantic;
                        memset(&src->semantic, 0, sizeof(src->semantic));
                    }

                    if (decl->return_var)
                    {
                        hlsl_cleanup_semantic(&decl->return_var->semantic);
                        decl->return_var->semantic = (yyvsp[-1].function).return_semantic;
                        memset(&(yyvsp[-1].function).return_semantic, 0, sizeof((yyvsp[-1].function).return_semantic));
                    }
                }
            }
            hlsl_pop_scope(ctx);

            if (!(yyvsp[-1].function).first)
            {
                vkd3d_free((yyvsp[-1].function).parameters.vars);
                hlsl_cleanup_semantic(&(yyvsp[-1].function).return_semantic);
            }
        }
#line 9683 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 62:
#line 7257 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            hlsl_pop_scope(ctx);
        }
#line 9691 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 63:
#line 7264 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            uint32_t modifiers = (yyvsp[-6].modifiers);
            struct hlsl_ir_var *var;
            struct hlsl_type *type;

            /* Functions are unconditionally inlined. */
            modifiers &= ~HLSL_MODIFIER_INLINE;

            if (modifiers & ~(HLSL_MODIFIERS_MAJORITY_MASK | HLSL_MODIFIER_EXPORT))
                hlsl_error(ctx, &(yylsp[-6]), VKD3D_SHADER_ERROR_HLSL_INVALID_MODIFIER,
                        "Unexpected modifier used on a function.");
            if (!(type = apply_type_modifiers(ctx, (yyvsp[-5].type), &modifiers, true, &(yylsp[-6]))))
                YYABORT;
            if ((var = hlsl_get_var(ctx->globals, (yyvsp[-4].name))))
            {
                hlsl_error(ctx, &(yylsp[-4]), VKD3D_SHADER_ERROR_HLSL_REDEFINED,
                        "\"%s\" is already declared as a variable.", (yyvsp[-4].name));
                hlsl_note(ctx, &var->loc, VKD3D_SHADER_LOG_ERROR,
                        "\"%s\" was previously declared here.", (yyvsp[-4].name));
            }
            if (hlsl_types_are_equal(type, ctx->builtin_types.Void) && (yyvsp[0].colon_attributes).semantic.name)
            {
                hlsl_error(ctx, &(yylsp[0]), VKD3D_SHADER_ERROR_HLSL_INVALID_SEMANTIC,
                        "Semantics are not allowed on void functions.");
            }

            if ((yyvsp[0].colon_attributes).reg_reservation.reg_type)
                FIXME("Unexpected register reservation for a function.\n");
            if ((yyvsp[0].colon_attributes).reg_reservation.offset_type)
                hlsl_error(ctx, &(yylsp[-2]), VKD3D_SHADER_ERROR_HLSL_INVALID_RESERVATION,
                        "packoffset() is not allowed on functions.");

            if (((yyval.function).decl = hlsl_get_func_decl(ctx, (yyvsp[-4].name), &(yyvsp[-2].parameters))))
            {
                const struct hlsl_func_parameters *params = &(yyval.function).decl->parameters;
                size_t i;

                if (!hlsl_types_are_equal((yyvsp[-5].type), (yyval.function).decl->return_type))
                {
                    hlsl_error(ctx, &(yylsp[-4]), VKD3D_SHADER_ERROR_HLSL_REDEFINED,
                            "\"%s\" was already declared with a different return type.", (yyvsp[-4].name));
                    hlsl_note(ctx, &(yyval.function).decl->loc, VKD3D_SHADER_LOG_ERROR, "\"%s\" was previously declared here.", (yyvsp[-4].name));
                }

                vkd3d_free((yyvsp[-4].name));

                /* We implement function invocation by copying to input
                 * parameters, emitting a HLSL_IR_CALL instruction, then copying
                 * from output parameters. As a result, we need to use the same
                 * parameter variables for every invocation of this function,
                 * which means we use the parameters created by the first
                 * declaration. If we're not the first declaration, the
                 * parameter variables that just got created will end up being
                 * mostly ignored—in particular, they won't be used in actual
                 * IR.
                 *
                 * There is a hitch: if this is the actual definition, the
                 * function body will look up parameter variables by name. We
                 * must return the original parameters, and not the ones we just
                 * created, but we're in the wrong scope, and the parameters
                 * might not even have the same names.
                 *
                 * Therefore we need to shuffle the parameters we just created
                 * into a dummy scope where they'll never be looked up, and
                 * rename the original parameters so they have the expected
                 * names. We actually do this for every prototype: we don't know
                 * whether this is the function definition yet, but it doesn't
                 * really matter. The variables can only be used in the
                 * actual definition, and don't do anything in a declaration.
                 *
                 * This is complex, and it seems tempting to avoid this logic by
                 * putting arguments into the HLSL_IR_CALL instruction, letting
                 * the canonical variables be the ones attached to the function
                 * definition, and resolving the copies when inlining. The
                 * problem with this is output parameters. We would have to use
                 * a lot of parsing logic on already lowered IR, which is
                 * brittle and ugly.
                 */

                VKD3D_ASSERT((yyvsp[-2].parameters).count == params->count);
                for (i = 0; i < params->count; ++i)
                {
                    struct hlsl_ir_var *orig_param = params->vars[i];
                    struct hlsl_ir_var *new_param = (yyvsp[-2].parameters).vars[i];
                    char *new_name;

                    list_remove(&orig_param->scope_entry);
                    list_add_tail(&ctx->cur_scope->vars, &orig_param->scope_entry);

                    list_remove(&new_param->scope_entry);
                    list_add_tail(&ctx->dummy_scope->vars, &new_param->scope_entry);

                    if (!(new_name = hlsl_strdup(ctx, new_param->name)))
                        YYABORT;
                    vkd3d_free((void *)orig_param->name);
                    orig_param->name = new_name;
                }

                (yyval.function).first = false;
                (yyval.function).parameters = (yyvsp[-2].parameters);
                (yyval.function).return_semantic = (yyvsp[0].colon_attributes).semantic;
            }
            else
            {
                if (!((yyval.function).decl = hlsl_new_func_decl(ctx, type, &(yyvsp[-2].parameters), &(yyvsp[0].colon_attributes).semantic, &(yylsp[-4]))))
                    YYABORT;

                hlsl_add_function(ctx, (yyvsp[-4].name), (yyval.function).decl);

                (yyval.function).first = true;
            }

            ctx->cur_function = (yyval.function).decl;
        }
#line 9810 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 65:
#line 7382 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            check_attribute_list_for_duplicates(ctx, &(yyvsp[-1].attr_list));

            if ((yyvsp[0].function).first)
            {
                (yyvsp[0].function).decl->attr_count = (yyvsp[-1].attr_list).count;
                (yyvsp[0].function).decl->attrs = (yyvsp[-1].attr_list).attrs;
            }
            else
            {
                cleanup_parse_attribute_list(&(yyvsp[-1].attr_list));
            }
            (yyval.function) = (yyvsp[0].function);
        }
#line 9829 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 66:
#line 7399 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            if (!((yyval.block) = make_empty_block(ctx)))
                YYABORT;
        }
#line 9838 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 67:
#line 7404 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            hlsl_pop_scope(ctx);
            (yyval.block) = (yyvsp[-1].block);
        }
#line 9847 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 68:
#line 7411 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            hlsl_push_scope(ctx);
        }
#line 9855 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 69:
#line 7417 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            hlsl_push_scope(ctx);
            ctx->cur_scope->loop = true;
        }
#line 9864 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 70:
#line 7424 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            hlsl_push_scope(ctx);
            ctx->cur_scope->_switch = true;
        }
#line 9873 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 71:
#line 7431 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            hlsl_push_scope(ctx);
            ctx->cur_scope->annotations = true;
        }
#line 9882 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 74:
#line 7442 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.colon_attributes).semantic = (struct hlsl_semantic){0};
            (yyval.colon_attributes).reg_reservation.reg_type = 0;
            (yyval.colon_attributes).reg_reservation.offset_type = 0;
        }
#line 9892 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 75:
#line 7448 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            hlsl_cleanup_semantic(&(yyval.colon_attributes).semantic);
            (yyval.colon_attributes).semantic = (yyvsp[0].semantic);
        }
#line 9901 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 76:
#line 7453 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            if ((yyval.colon_attributes).reg_reservation.reg_type)
                hlsl_fixme(ctx, &(yylsp[0]), "Multiple register() reservations.");

            (yyval.colon_attributes).reg_reservation.reg_type = (yyvsp[0].reg_reservation).reg_type;
            (yyval.colon_attributes).reg_reservation.reg_index = (yyvsp[0].reg_reservation).reg_index;
            (yyval.colon_attributes).reg_reservation.reg_space = (yyvsp[0].reg_reservation).reg_space;
        }
#line 9914 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 77:
#line 7462 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            if (ctx->cur_buffer == ctx->globals_buffer)
            {
                hlsl_error(ctx, &(yylsp[0]), VKD3D_SHADER_ERROR_HLSL_INVALID_RESERVATION,
                        "The packoffset() reservation is only allowed within 'cbuffer' blocks.");
            }
            else
            {
                (yyval.colon_attributes).reg_reservation.offset_type = (yyvsp[0].reg_reservation).offset_type;
                (yyval.colon_attributes).reg_reservation.offset_index = (yyvsp[0].reg_reservation).offset_index;
            }
        }
#line 9931 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 78:
#line 7477 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            char *p;

            if (!((yyval.semantic).raw_name = hlsl_strdup(ctx, (yyvsp[0].name))))
                YYABORT;

            for (p = (yyvsp[0].name) + strlen((yyvsp[0].name)); p > (yyvsp[0].name) && isdigit(p[-1]); --p)
                ;
            (yyval.semantic).name = (yyvsp[0].name);
            (yyval.semantic).index = atoi(p);
            (yyval.semantic).reported_missing = false;
            (yyval.semantic).reported_duplicated_output_next_index = 0;
            (yyval.semantic).reported_duplicated_input_incompatible_next_index = 0;
            *p = 0;
        }
#line 9951 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 79:
#line 7496 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            memset(&(yyval.reg_reservation), 0, sizeof((yyval.reg_reservation)));
            if (!parse_reservation_index(ctx, (yyvsp[-1].name), 0, &(yyval.reg_reservation)))
                hlsl_error(ctx, &(yylsp[-1]), VKD3D_SHADER_ERROR_HLSL_INVALID_RESERVATION,
                        "Invalid register reservation '%s'.", (yyvsp[-1].name));

            vkd3d_free((yyvsp[-1].name));
        }
#line 9964 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 80:
#line 7505 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            memset(&(yyval.reg_reservation), 0, sizeof((yyval.reg_reservation)));
            if (!parse_reservation_index(ctx, (yyvsp[-4].name), evaluate_static_expression_as_uint(ctx, (yyvsp[-2].block), &(yylsp[-2])), &(yyval.reg_reservation)))
            {
                hlsl_error(ctx, &(yylsp[-4]), VKD3D_SHADER_ERROR_HLSL_INVALID_RESERVATION,
                        "Invalid register reservation '%s'.", (yyvsp[-4].name));
            }

            vkd3d_free((yyvsp[-4].name));
            vkd3d_free((yyvsp[-2].block));
        }
#line 9980 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 81:
#line 7517 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            memset(&(yyval.reg_reservation), 0, sizeof((yyval.reg_reservation)));
            if (parse_reservation_index(ctx, (yyvsp[-1].name), 0, &(yyval.reg_reservation)))
            {
                hlsl_fixme(ctx, &(yylsp[-3]), "Reservation shader target %s.", (yyvsp[-3].name));
            }
            else if (parse_reservation_space((yyvsp[-1].name), &(yyval.reg_reservation).reg_space))
            {
                if (!parse_reservation_index(ctx, (yyvsp[-3].name), 0, &(yyval.reg_reservation)))
                    hlsl_error(ctx, &(yylsp[-3]), VKD3D_SHADER_ERROR_HLSL_INVALID_RESERVATION,
                            "Invalid register reservation '%s'.", (yyvsp[-3].name));
            }
            else
            {
                hlsl_error(ctx, &(yylsp[-1]), VKD3D_SHADER_ERROR_HLSL_INVALID_RESERVATION,
                        "Invalid register or space reservation '%s'.", (yyvsp[-1].name));
            }

            vkd3d_free((yyvsp[-3].name));
            vkd3d_free((yyvsp[-1].name));
        }
#line 10006 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 82:
#line 7539 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            memset(&(yyval.reg_reservation), 0, sizeof((yyval.reg_reservation)));

            if (!parse_reservation_space((yyvsp[-1].name), &(yyval.reg_reservation).reg_space))
                hlsl_error(ctx, &(yylsp[-1]), VKD3D_SHADER_ERROR_HLSL_INVALID_RESERVATION,
                        "Invalid register space reservation '%s'.", (yyvsp[-1].name));

            if (!parse_reservation_index(ctx, (yyvsp[-6].name), evaluate_static_expression_as_uint(ctx, (yyvsp[-4].block), &(yylsp[-4])), &(yyval.reg_reservation)))
            {
                hlsl_error(ctx, &(yylsp[-6]), VKD3D_SHADER_ERROR_HLSL_INVALID_RESERVATION,
                        "Invalid register reservation '%s'.", (yyvsp[-6].name));
            }

            vkd3d_free((yyvsp[-6].name));
            vkd3d_free((yyvsp[-4].block));
            vkd3d_free((yyvsp[-1].name));
        }
#line 10028 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 83:
#line 7557 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            hlsl_fixme(ctx, &(yylsp[-6]), "Reservation shader target %s.", (yyvsp[-6].name));

            memset(&(yyval.reg_reservation), 0, sizeof((yyval.reg_reservation)));
            if (!parse_reservation_index(ctx, (yyvsp[-4].name), evaluate_static_expression_as_uint(ctx, (yyvsp[-2].block), &(yylsp[-2])), &(yyval.reg_reservation)))
            {
                hlsl_error(ctx, &(yylsp[-4]), VKD3D_SHADER_ERROR_HLSL_INVALID_RESERVATION,
                        "Invalid register reservation '%s'.", (yyvsp[-4].name));
            }

            vkd3d_free((yyvsp[-6].name));
            vkd3d_free((yyvsp[-4].name));
            vkd3d_free((yyvsp[-2].block));
        }
#line 10047 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 84:
#line 7572 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            hlsl_fixme(ctx, &(yylsp[-5]), "Reservation shader target %s.", (yyvsp[-5].name));

            memset(&(yyval.reg_reservation), 0, sizeof((yyval.reg_reservation)));
            if (!parse_reservation_index(ctx, (yyvsp[-3].name), 0, &(yyval.reg_reservation)))
                hlsl_error(ctx, &(yylsp[-3]), VKD3D_SHADER_ERROR_HLSL_INVALID_RESERVATION,
                        "Invalid register reservation '%s'.", (yyvsp[-3].name));

            if (!parse_reservation_space((yyvsp[-1].name), &(yyval.reg_reservation).reg_space))
                hlsl_error(ctx, &(yylsp[-1]), VKD3D_SHADER_ERROR_HLSL_INVALID_RESERVATION,
                        "Invalid register space reservation '%s'.", (yyvsp[-1].name));

            vkd3d_free((yyvsp[-5].name));
            vkd3d_free((yyvsp[-3].name));
            vkd3d_free((yyvsp[-1].name));
        }
#line 10068 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 85:
#line 7589 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            hlsl_fixme(ctx, &(yylsp[-8]), "Reservation shader target %s.", (yyvsp[-8].name));

            memset(&(yyval.reg_reservation), 0, sizeof((yyval.reg_reservation)));
            if (!parse_reservation_index(ctx, (yyvsp[-6].name), evaluate_static_expression_as_uint(ctx, (yyvsp[-4].block), &(yylsp[-4])), &(yyval.reg_reservation)))
            {
                hlsl_error(ctx, &(yylsp[-6]), VKD3D_SHADER_ERROR_HLSL_INVALID_RESERVATION,
                        "Invalid register reservation '%s'.", (yyvsp[-6].name));
            }

            if (!parse_reservation_space((yyvsp[-1].name), &(yyval.reg_reservation).reg_space))
                hlsl_error(ctx, &(yylsp[-1]), VKD3D_SHADER_ERROR_HLSL_INVALID_RESERVATION,
                        "Invalid register space reservation '%s'.", (yyvsp[-1].name));

            vkd3d_free((yyvsp[-8].name));
            vkd3d_free((yyvsp[-6].name));
            vkd3d_free((yyvsp[-4].block));
            vkd3d_free((yyvsp[-1].name));
        }
#line 10092 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 86:
#line 7611 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.reg_reservation) = parse_packoffset(ctx, (yyvsp[-1].name), NULL, &(yyloc));

            vkd3d_free((yyvsp[-1].name));
        }
#line 10102 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 87:
#line 7617 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.reg_reservation) = parse_packoffset(ctx, (yyvsp[-3].name), (yyvsp[-1].name), &(yyloc));

            vkd3d_free((yyvsp[-3].name));
            vkd3d_free((yyvsp[-1].name));
        }
#line 10113 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 88:
#line 7626 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            memset(&(yyval.parameters), 0, sizeof((yyval.parameters)));
        }
#line 10121 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 89:
#line 7630 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            memset(&(yyval.parameters), 0, sizeof((yyval.parameters)));
        }
#line 10129 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 90:
#line 7634 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.parameters) = (yyvsp[0].parameters);
        }
#line 10137 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 91:
#line 7640 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            memset(&(yyval.parameters), 0, sizeof((yyval.parameters)));
            if (!add_func_parameter(ctx, &(yyval.parameters), &(yyvsp[0].parameter), &(yylsp[0])))
            {
                ERR("Error adding function parameter %s.\n", (yyvsp[0].parameter).name);
                YYABORT;
            }
        }
#line 10150 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 92:
#line 7649 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.parameters) = (yyvsp[-2].parameters);
            if (!add_func_parameter(ctx, &(yyval.parameters), &(yyvsp[0].parameter), &(yylsp[0])))
            {
                hlsl_error(ctx, &(yylsp[0]), VKD3D_SHADER_ERROR_HLSL_REDEFINED,
                        "Parameter \"%s\" is already declared.", (yyvsp[0].parameter).name);
                YYABORT;
            }
        }
#line 10164 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 94:
#line 7662 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.parameter) = (yyvsp[-2].parameter);
            (yyval.parameter).initializer = (yyvsp[0].initializer);
        }
#line 10173 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 95:
#line 7669 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            uint32_t modifiers = (yyvsp[-4].modifiers);
            struct hlsl_type *type;
            unsigned int i;

            if (!(type = apply_type_modifiers(ctx, (yyvsp[-3].type), &modifiers, true, &(yylsp[-4]))))
                YYABORT;

            (yyval.parameter).modifiers = modifiers;
            if (!((yyval.parameter).modifiers & (HLSL_STORAGE_IN | HLSL_STORAGE_OUT)))
                (yyval.parameter).modifiers |= HLSL_STORAGE_IN;

            for (i = 0; i < (yyvsp[-1].arrays).count; ++i)
            {
                if ((yyvsp[-1].arrays).sizes[i] == HLSL_ARRAY_ELEMENTS_COUNT_IMPLICIT)
                {
                    hlsl_error(ctx, &(yylsp[-2]), VKD3D_SHADER_ERROR_HLSL_INVALID_TYPE,
                            "Implicit size arrays not allowed in function parameters.");
                }
                type = hlsl_new_array_type(ctx, type, (yyvsp[-1].arrays).sizes[i]);
            }
            vkd3d_free((yyvsp[-1].arrays).sizes);

            (yyval.parameter).type = type;

            if (hlsl_version_ge(ctx, 5, 1) && type->class == HLSL_CLASS_ARRAY && hlsl_type_is_resource(type))
                hlsl_fixme(ctx, &(yylsp[-3]), "Shader model 5.1+ resource array.");

            (yyval.parameter).name = (yyvsp[-2].name);
            (yyval.parameter).semantic = (yyvsp[0].colon_attributes).semantic;
            (yyval.parameter).reg_reservation = (yyvsp[0].colon_attributes).reg_reservation;

            memset(&(yyval.parameter).initializer, 0, sizeof((yyval.parameter).initializer));
        }
#line 10212 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 96:
#line 7706 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.sampler_dim) = HLSL_SAMPLER_DIM_BUFFER;
        }
#line 10220 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 97:
#line 7710 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.sampler_dim) = HLSL_SAMPLER_DIM_1D;
        }
#line 10228 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 98:
#line 7714 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.sampler_dim) = HLSL_SAMPLER_DIM_2D;
        }
#line 10236 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 99:
#line 7718 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.sampler_dim) = HLSL_SAMPLER_DIM_3D;
        }
#line 10244 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 100:
#line 7722 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.sampler_dim) = HLSL_SAMPLER_DIM_CUBE;
        }
#line 10252 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 101:
#line 7726 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.sampler_dim) = HLSL_SAMPLER_DIM_1DARRAY;
        }
#line 10260 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 102:
#line 7730 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.sampler_dim) = HLSL_SAMPLER_DIM_2DARRAY;
        }
#line 10268 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 103:
#line 7734 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.sampler_dim) = HLSL_SAMPLER_DIM_CUBEARRAY;
        }
#line 10276 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 104:
#line 7740 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.sampler_dim) = HLSL_SAMPLER_DIM_2DMS;
        }
#line 10284 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 105:
#line 7744 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.sampler_dim) = HLSL_SAMPLER_DIM_2DMSARRAY;
        }
#line 10292 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 106:
#line 7750 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.sampler_dim) = HLSL_SAMPLER_DIM_BUFFER;
        }
#line 10300 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 107:
#line 7754 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.sampler_dim) = HLSL_SAMPLER_DIM_STRUCTURED_BUFFER;
        }
#line 10308 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 108:
#line 7758 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.sampler_dim) = HLSL_SAMPLER_DIM_1D;
        }
#line 10316 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 109:
#line 7762 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.sampler_dim) = HLSL_SAMPLER_DIM_1DARRAY;
        }
#line 10324 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 110:
#line 7766 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.sampler_dim) = HLSL_SAMPLER_DIM_2D;
        }
#line 10332 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 111:
#line 7770 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.sampler_dim) = HLSL_SAMPLER_DIM_2DARRAY;
        }
#line 10340 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 112:
#line 7774 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.sampler_dim) = HLSL_SAMPLER_DIM_3D;
        }
#line 10348 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 113:
#line 7780 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.sampler_dim) = HLSL_SAMPLER_DIM_BUFFER;
        }
#line 10356 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 114:
#line 7784 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.sampler_dim) = HLSL_SAMPLER_DIM_STRUCTURED_BUFFER;
        }
#line 10364 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 115:
#line 7788 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.sampler_dim) = HLSL_SAMPLER_DIM_1D;
        }
#line 10372 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 116:
#line 7792 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.sampler_dim) = HLSL_SAMPLER_DIM_1DARRAY;
        }
#line 10380 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 117:
#line 7796 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.sampler_dim) = HLSL_SAMPLER_DIM_2D;
        }
#line 10388 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 118:
#line 7800 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.sampler_dim) = HLSL_SAMPLER_DIM_2DARRAY;
        }
#line 10396 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 119:
#line 7804 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.sampler_dim) = HLSL_SAMPLER_DIM_3D;
        }
#line 10404 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 120:
#line 7810 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            uint32_t modifiers = (yyvsp[-1].modifiers);

            if (!((yyval.type) = apply_type_modifiers(ctx, (yyvsp[0].type), &modifiers, false, &(yylsp[-1]))))
                YYABORT;
        }
#line 10415 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 121:
#line 7819 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            if ((yyvsp[-3].type)->class != HLSL_CLASS_SCALAR)
            {
                struct vkd3d_string_buffer *string;

                string = hlsl_type_to_string(ctx, (yyvsp[-3].type));
                if (string)
                    hlsl_error(ctx, &(yylsp[-3]), VKD3D_SHADER_ERROR_HLSL_INVALID_TYPE,
                            "Vector base type %s is not scalar.", string->buffer);
                hlsl_release_string_buffer(ctx, string);
                YYABORT;
            }
            if ((yyvsp[-1].intval) < 1 || (yyvsp[-1].intval) > 4)
            {
                hlsl_error(ctx, &(yylsp[-1]), VKD3D_SHADER_ERROR_HLSL_INVALID_SIZE,
                        "Vector size %d is not between 1 and 4.", (yyvsp[-1].intval));
                YYABORT;
            }

            (yyval.type) = hlsl_type_clone(ctx, hlsl_get_vector_type(ctx, (yyvsp[-3].type)->e.numeric.type, (yyvsp[-1].intval)), 0, 0);
            (yyval.type)->is_minimum_precision = (yyvsp[-3].type)->is_minimum_precision;
        }
#line 10442 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 122:
#line 7842 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.type) = hlsl_get_vector_type(ctx, HLSL_TYPE_FLOAT, 4);
        }
#line 10450 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 123:
#line 7846 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            if ((yyvsp[-5].type)->class != HLSL_CLASS_SCALAR)
            {
                struct vkd3d_string_buffer *string;

                string = hlsl_type_to_string(ctx, (yyvsp[-5].type));
                if (string)
                    hlsl_error(ctx, &(yylsp[-5]), VKD3D_SHADER_ERROR_HLSL_INVALID_TYPE,
                            "Matrix base type %s is not scalar.", string->buffer);
                hlsl_release_string_buffer(ctx, string);
                YYABORT;
            }
            if ((yyvsp[-3].intval) < 1 || (yyvsp[-3].intval) > 4)
            {
                hlsl_error(ctx, &(yylsp[-3]), VKD3D_SHADER_ERROR_HLSL_INVALID_SIZE,
                        "Matrix row count %d is not between 1 and 4.", (yyvsp[-3].intval));
                YYABORT;
            }
            if ((yyvsp[-1].intval) < 1 || (yyvsp[-1].intval) > 4)
            {
                hlsl_error(ctx, &(yylsp[-1]), VKD3D_SHADER_ERROR_HLSL_INVALID_SIZE,
                        "Matrix column count %d is not between 1 and 4.", (yyvsp[-1].intval));
                YYABORT;
            }

            (yyval.type) = hlsl_type_clone(ctx, hlsl_get_matrix_type(ctx, (yyvsp[-5].type)->e.numeric.type, (yyvsp[-1].intval), (yyvsp[-3].intval)), 0, 0);
            (yyval.type)->is_minimum_precision = (yyvsp[-5].type)->is_minimum_precision;
        }
#line 10483 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 124:
#line 7875 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.type) = hlsl_get_matrix_type(ctx, HLSL_TYPE_FLOAT, 4, 4);
        }
#line 10491 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 125:
#line 7879 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.type) = ctx->builtin_types.sampler[HLSL_SAMPLER_DIM_GENERIC];
        }
#line 10499 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 126:
#line 7883 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.type) = ctx->builtin_types.sampler[HLSL_SAMPLER_DIM_COMPARISON];
        }
#line 10507 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 127:
#line 7887 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.type) = ctx->builtin_types.sampler[HLSL_SAMPLER_DIM_1D];
        }
#line 10515 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 128:
#line 7891 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.type) = ctx->builtin_types.sampler[HLSL_SAMPLER_DIM_2D];
        }
#line 10523 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 129:
#line 7895 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.type) = ctx->builtin_types.sampler[HLSL_SAMPLER_DIM_3D];
        }
#line 10531 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 130:
#line 7899 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.type) = ctx->builtin_types.sampler[HLSL_SAMPLER_DIM_CUBE];
        }
#line 10539 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 131:
#line 7903 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.type) = hlsl_new_texture_type(ctx, HLSL_SAMPLER_DIM_GENERIC, NULL, 0);
        }
#line 10547 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 132:
#line 7907 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.type) = hlsl_new_texture_type(ctx, (yyvsp[0].sampler_dim), hlsl_get_vector_type(ctx, HLSL_TYPE_FLOAT, 4), 0);
        }
#line 10555 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 133:
#line 7911 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            validate_texture_format_type(ctx, (yyvsp[-1].type), &(yylsp[-1]));
            (yyval.type) = hlsl_new_texture_type(ctx, (yyvsp[-3].sampler_dim), (yyvsp[-1].type), 0);
        }
#line 10564 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 134:
#line 7916 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            validate_texture_format_type(ctx, (yyvsp[-1].type), &(yylsp[-1]));

            (yyval.type) = hlsl_new_texture_type(ctx, (yyvsp[-3].sampler_dim), (yyvsp[-1].type), 0);
        }
#line 10574 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 135:
#line 7922 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            unsigned int sample_count;
            struct hlsl_block block;

            hlsl_block_init(&block);
            hlsl_block_add_block(&block, (yyvsp[-1].block));

            sample_count = evaluate_static_expression_as_uint(ctx, &block, &(yylsp[-1]));

            hlsl_block_cleanup(&block);

            vkd3d_free((yyvsp[-1].block));

            (yyval.type) = hlsl_new_texture_type(ctx, (yyvsp[-5].sampler_dim), (yyvsp[-3].type), sample_count);
        }
#line 10594 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 136:
#line 7938 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.type) = hlsl_new_texture_type(ctx, HLSL_SAMPLER_DIM_RAW_BUFFER, hlsl_get_scalar_type(ctx, HLSL_TYPE_UINT), 0);
        }
#line 10602 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 137:
#line 7942 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            validate_uav_type(ctx, (yyvsp[-3].sampler_dim), (yyvsp[-1].type), &(yylsp[-1]));
            (yyval.type) = hlsl_new_uav_type(ctx, (yyvsp[-3].sampler_dim), (yyvsp[-1].type), false);
        }
#line 10611 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 138:
#line 7947 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            validate_uav_type(ctx, (yyvsp[-3].sampler_dim), (yyvsp[-1].type), &(yylsp[0]));
            (yyval.type) = hlsl_new_uav_type(ctx, (yyvsp[-3].sampler_dim), (yyvsp[-1].type), true);
        }
#line 10620 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 139:
#line 7952 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.type) = hlsl_new_uav_type(ctx, HLSL_SAMPLER_DIM_RAW_BUFFER, hlsl_get_scalar_type(ctx, HLSL_TYPE_UINT), false);
        }
#line 10628 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 140:
#line 7956 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.type) = ctx->builtin_types.string;
        }
#line 10636 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 141:
#line 7960 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.type) = hlsl_get_type(ctx->cur_scope, (yyvsp[0].name), true, true);
            if ((yyval.type)->is_minimum_precision)
            {
                if (hlsl_version_lt(ctx, 4, 0))
                {
                    hlsl_error(ctx, &(yylsp[0]), VKD3D_SHADER_ERROR_HLSL_INVALID_TYPE,
                            "Target profile doesn't support minimum-precision types.");
                }
                else
                {
                    FIXME("Reinterpreting type %s.\n", (yyval.type)->name);
                }
            }
            vkd3d_free((yyvsp[0].name));
        }
#line 10657 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 142:
#line 7977 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            struct hlsl_type *type = hlsl_get_type(ctx->cur_scope, (yyvsp[0].name), true, true);

            if (hlsl_is_numeric_type(type) && type->e.numeric.type == HLSL_TYPE_INT)
            {
                if (!(type = hlsl_type_clone(ctx, type, 0, 0)))
                    YYABORT;
                vkd3d_free((void *)type->name);
                type->name = NULL;
                type->e.numeric.type = HLSL_TYPE_UINT;
            }
            else
            {
                hlsl_error(ctx, &(yylsp[0]), VKD3D_SHADER_ERROR_HLSL_INVALID_TYPE,
                        "The 'unsigned' keyword can't be used with type %s.", (yyvsp[0].name));
            }

            (yyval.type) = type;
        }
#line 10681 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 143:
#line 7997 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.type) = hlsl_get_type(ctx->cur_scope, (yyvsp[0].name), true, true);
            if ((yyval.type)->class != HLSL_CLASS_STRUCT)
                hlsl_error(ctx, &(yylsp[-1]), VKD3D_SHADER_ERROR_HLSL_REDEFINED, "\"%s\" redefined as a structure.", (yyvsp[0].name));
            vkd3d_free((yyvsp[0].name));
        }
#line 10692 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 144:
#line 8004 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.type) = hlsl_get_type(ctx->cur_scope, "RenderTargetView", true, true);
        }
#line 10700 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 145:
#line 8008 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.type) = hlsl_get_type(ctx->cur_scope, "DepthStencilState", true, true);
        }
#line 10708 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 146:
#line 8012 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.type) = hlsl_get_type(ctx->cur_scope, "DepthStencilView", true, true);
        }
#line 10716 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 147:
#line 8016 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.type) = hlsl_get_type(ctx->cur_scope, "VertexShader", true, true);
        }
#line 10724 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 148:
#line 8020 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.type) = hlsl_get_type(ctx->cur_scope, "PixelShader", true, true);
        }
#line 10732 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 149:
#line 8024 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.type) = hlsl_get_type(ctx->cur_scope, "ComputeShader", true, true);
        }
#line 10740 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 150:
#line 8028 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.type) = hlsl_get_type(ctx->cur_scope, "DomainShader", true, true);
        }
#line 10748 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 151:
#line 8032 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.type) = hlsl_get_type(ctx->cur_scope, "HullShader", true, true);
        }
#line 10756 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 152:
#line 8036 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.type) = hlsl_get_type(ctx->cur_scope, "GeometryShader", true, true);
        }
#line 10764 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 153:
#line 8040 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            if ((yyvsp[-1].type)->class != HLSL_CLASS_STRUCT)
                hlsl_error(ctx, &(yylsp[-3]), VKD3D_SHADER_ERROR_HLSL_INVALID_TYPE,
                        "ConstantBuffer<...> requires user-defined structure type.");
            (yyval.type) = hlsl_new_cb_type(ctx, (yyvsp[-1].type));
        }
#line 10775 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 154:
#line 8047 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.type) = hlsl_get_type(ctx->cur_scope, "RasterizerState", true, true);
        }
#line 10783 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 155:
#line 8051 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.type) = hlsl_get_type(ctx->cur_scope, "BlendState", true, true);
        }
#line 10791 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 157:
#line 8058 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.type) = ctx->builtin_types.Void;
        }
#line 10799 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 160:
#line 8066 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            if (!((yyval.block) = make_empty_block(ctx)))
                YYABORT;
        }
#line 10808 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 163:
#line 8077 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            struct parse_variable_def *v, *v_next;
            uint32_t modifiers = (yyvsp[-3].modifiers);
            struct hlsl_type *type;

            if (!(type = apply_type_modifiers(ctx, (yyvsp[-2].type), &modifiers, false, &(yylsp[-3]))))
            {
                LIST_FOR_EACH_ENTRY_SAFE(v, v_next, (yyvsp[-1].list), struct parse_variable_def, entry)
                    free_parse_variable_def(v);
                vkd3d_free((yyvsp[-1].list));
                YYABORT;
            }

            if (modifiers)
            {
                hlsl_error(ctx, &(yylsp[-4]), VKD3D_SHADER_ERROR_HLSL_INVALID_MODIFIER,
                        "Storage modifiers are not allowed on typedefs.");
                LIST_FOR_EACH_ENTRY_SAFE(v, v_next, (yyvsp[-1].list), struct parse_variable_def, entry)
                    vkd3d_free(v);
                vkd3d_free((yyvsp[-1].list));
                YYABORT;
            }
            if (!add_typedef(ctx, type, (yyvsp[-1].list)))
                YYABORT;
        }
#line 10838 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 164:
#line 8105 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            if (!((yyval.list) = make_empty_list(ctx)))
                YYABORT;
            list_add_head((yyval.list), &(yyvsp[0].variable_def)->entry);
        }
#line 10848 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 165:
#line 8111 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.list) = (yyvsp[-2].list);
            list_add_tail((yyval.list), &(yyvsp[0].variable_def)->entry);
        }
#line 10857 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 166:
#line 8118 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.variable_def) = hlsl_alloc(ctx, sizeof(*(yyval.variable_def)));
            (yyval.variable_def)->loc = (yylsp[-1]);
            (yyval.variable_def)->name = (yyvsp[-1].name);
            (yyval.variable_def)->arrays = (yyvsp[0].arrays);
        }
#line 10868 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 167:
#line 8127 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            if (!((yyval.block) = initialize_vars(ctx, (yyvsp[-1].list))))
                YYABORT;
        }
#line 10877 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 168:
#line 8134 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            if (!((yyval.list) = make_empty_list(ctx)))
                YYABORT;
            list_add_head((yyval.list), &(yyvsp[0].variable_def)->entry);
        }
#line 10887 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 169:
#line 8140 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.list) = (yyvsp[-2].list);
            list_add_tail((yyval.list), &(yyvsp[0].variable_def)->entry);
        }
#line 10896 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 170:
#line 8147 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            if (!((yyval.list) = make_empty_list(ctx)))
                YYABORT;
            list_add_head((yyval.list), &(yyvsp[0].variable_def)->entry);

            declare_var(ctx, (yyvsp[0].variable_def));
        }
#line 10908 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 171:
#line 8155 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            struct parse_variable_def *head_def;

            VKD3D_ASSERT(!list_empty((yyvsp[-2].list)));
            head_def = LIST_ENTRY(list_head((yyvsp[-2].list)), struct parse_variable_def, entry);

            VKD3D_ASSERT(head_def->basic_type);
            (yyvsp[0].variable_def)->basic_type = head_def->basic_type;
            (yyvsp[0].variable_def)->modifiers = head_def->modifiers;
            (yyvsp[0].variable_def)->modifiers_loc = head_def->modifiers_loc;

            declare_var(ctx, (yyvsp[0].variable_def));

            (yyval.list) = (yyvsp[-2].list);
            list_add_tail((yyval.list), &(yyvsp[0].variable_def)->entry);
        }
#line 10929 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 172:
#line 8174 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.variable_def) = hlsl_alloc(ctx, sizeof(*(yyval.variable_def)));
            (yyval.variable_def)->loc = (yylsp[-3]);
            (yyval.variable_def)->name = (yyvsp[-3].name);
            (yyval.variable_def)->arrays = (yyvsp[-2].arrays);
            (yyval.variable_def)->semantic = (yyvsp[-1].colon_attributes).semantic;
            (yyval.variable_def)->reg_reservation = (yyvsp[-1].colon_attributes).reg_reservation;
            (yyval.variable_def)->annotations = (yyvsp[0].scope);
        }
#line 10943 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 173:
#line 8186 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            ctx->in_state_block = 1;
        }
#line 10951 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 174:
#line 8192 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.name) = (yyvsp[0].name);
        }
#line 10959 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 175:
#line 8196 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            if (!((yyval.name) = hlsl_strdup(ctx, "pixelshader")))
                YYABORT;
        }
#line 10968 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 176:
#line 8201 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            if (!((yyval.name) = hlsl_strdup(ctx, "texture")))
                YYABORT;
        }
#line 10977 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 177:
#line 8206 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            if (!((yyval.name) = hlsl_strdup(ctx, "vertexshader")))
                YYABORT;
        }
#line 10986 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 178:
#line 8211 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            if (!((yyval.name) = hlsl_strdup(ctx, "geometryshader")))
                YYABORT;
        }
#line 10995 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 179:
#line 8218 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.state_block_index).has_index = false;
            (yyval.state_block_index).index = 0;
        }
#line 11004 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 180:
#line 8223 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
       {
            if ((yyvsp[-1].intval) < 0)
            {
                hlsl_error(ctx, &(yylsp[-1]), VKD3D_SHADER_ERROR_HLSL_INVALID_INDEX,
                        "State block array index is not a positive integer constant.");
                YYABORT;
            }
            (yyval.state_block_index).has_index = true;
            (yyval.state_block_index).index = (yyvsp[-1].intval);
       }
#line 11019 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 181:
#line 8236 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            if (!((yyval.state_block) = hlsl_alloc(ctx, sizeof(*(yyval.state_block)))))
                YYABORT;
        }
#line 11028 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 182:
#line 8241 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            struct hlsl_state_block_entry *entry;
            unsigned int i;

            if (!(entry = hlsl_alloc(ctx, sizeof(*entry))))
                YYABORT;

            entry->name = (yyvsp[-4].name);
            entry->lhs_has_index = (yyvsp[-3].state_block_index).has_index;
            entry->lhs_index = (yyvsp[-3].state_block_index).index;

            entry->instrs = (yyvsp[-1].initializer).instrs;

            entry->args_count = (yyvsp[-1].initializer).args_count;
            if (!(entry->args = hlsl_alloc(ctx, sizeof(*entry->args) * entry->args_count)))
                YYABORT;
            for (i = 0; i < entry->args_count; ++i)
                hlsl_src_from_node(&entry->args[i], (yyvsp[-1].initializer).args[i]);
            vkd3d_free((yyvsp[-1].initializer).args);

            (yyval.state_block) = (yyvsp[-5].state_block);
            hlsl_state_block_add_entry((yyval.state_block), entry);
        }
#line 11056 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 183:
#line 8265 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            struct hlsl_state_block_entry *entry;
            unsigned int i;

            if (!(entry = hlsl_alloc(ctx, sizeof(*entry))))
                YYABORT;

            entry->is_function_call = true;

            entry->name = (yyvsp[-4].name);
            entry->lhs_has_index = false;
            entry->lhs_index = 0;

            entry->instrs = (yyvsp[-2].initializer).instrs;

            entry->args_count = (yyvsp[-2].initializer).args_count;
            if (!(entry->args = hlsl_alloc(ctx, sizeof(*entry->args) * entry->args_count)))
                YYABORT;
            for (i = 0; i < entry->args_count; ++i)
                hlsl_src_from_node(&entry->args[i], (yyvsp[-2].initializer).args[i]);
            vkd3d_free((yyvsp[-2].initializer).args);

            hlsl_validate_state_block_entry(ctx, entry, &(yylsp[-2]));

            (yyval.state_block) = (yyvsp[-5].state_block);
            hlsl_state_block_add_entry((yyval.state_block), entry);
        }
#line 11088 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 184:
#line 8295 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            if (!((yyval.variable_def) = hlsl_alloc(ctx, sizeof(*(yyval.variable_def)))))
                YYABORT;

            if(!(vkd3d_array_reserve((void **)&(yyval.variable_def)->state_blocks, &(yyval.variable_def)->state_block_capacity,
                    (yyval.variable_def)->state_block_count + 1, sizeof(*(yyval.variable_def)->state_blocks))))
                YYABORT;
            (yyval.variable_def)->state_blocks[(yyval.variable_def)->state_block_count++] = (yyvsp[-1].state_block);
        }
#line 11102 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 185:
#line 8305 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.variable_def) = (yyvsp[-4].variable_def);

            if(!(vkd3d_array_reserve((void **)&(yyval.variable_def)->state_blocks, &(yyval.variable_def)->state_block_capacity,
                    (yyval.variable_def)->state_block_count + 1, sizeof(*(yyval.variable_def)->state_blocks))))
                YYABORT;
            (yyval.variable_def)->state_blocks[(yyval.variable_def)->state_block_count++] = (yyvsp[-1].state_block);
        }
#line 11115 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 187:
#line 8317 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.variable_def) = (yyvsp[-2].variable_def);
            (yyval.variable_def)->initializer = (yyvsp[0].initializer);
        }
#line 11124 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 188:
#line 8322 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.variable_def) = (yyvsp[-4].variable_def);
            ctx->in_state_block = 0;

            if(!(vkd3d_array_reserve((void **)&(yyval.variable_def)->state_blocks, &(yyval.variable_def)->state_block_capacity,
                    (yyval.variable_def)->state_block_count + 1, sizeof(*(yyval.variable_def)->state_blocks))))
                YYABORT;
            (yyval.variable_def)->state_blocks[(yyval.variable_def)->state_block_count++] = (yyvsp[-1].state_block);
        }
#line 11138 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 189:
#line 8332 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.variable_def) = (yyvsp[-4].variable_def);
            ctx->in_state_block = 0;

            (yyval.variable_def)->state_blocks = (yyvsp[-1].variable_def)->state_blocks;
            (yyval.variable_def)->state_block_count = (yyvsp[-1].variable_def)->state_block_count;
            (yyval.variable_def)->state_block_capacity = (yyvsp[-1].variable_def)->state_block_capacity;
            (yyvsp[-1].variable_def)->state_blocks = NULL;
            (yyvsp[-1].variable_def)->state_block_count = 0;
            (yyvsp[-1].variable_def)->state_block_capacity = 0;
            free_parse_variable_def((yyvsp[-1].variable_def));
        }
#line 11155 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 190:
#line 8347 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            uint32_t modifiers = (yyvsp[-2].modifiers);
            struct hlsl_type *type;

            if (!(type = apply_type_modifiers(ctx, (yyvsp[-1].type), &modifiers, true, &(yylsp[-2]))))
                YYABORT;

            check_invalid_in_out_modifiers(ctx, modifiers, &(yylsp[-2]));

            (yyval.variable_def) = (yyvsp[0].variable_def);
            (yyval.variable_def)->basic_type = type;
            (yyval.variable_def)->modifiers = modifiers;
            (yyval.variable_def)->modifiers_loc = (yylsp[-2]);
        }
#line 11174 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 191:
#line 8362 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            uint32_t modifiers = (yyvsp[-2].modifiers);
            struct hlsl_type *type;

            if (!(type = apply_type_modifiers(ctx, (yyvsp[-1].type), &modifiers, true, &(yylsp[-2]))))
                YYABORT;

            check_invalid_in_out_modifiers(ctx, modifiers, &(yylsp[-2]));

            (yyval.variable_def) = (yyvsp[0].variable_def);
            (yyval.variable_def)->basic_type = type;
            (yyval.variable_def)->modifiers = modifiers;
            (yyval.variable_def)->modifiers_loc = (yylsp[-2]);
        }
#line 11193 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 192:
#line 8379 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.intval) = HLSL_ARRAY_ELEMENTS_COUNT_IMPLICIT;
        }
#line 11201 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 193:
#line 8383 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.intval) = evaluate_static_expression_as_uint(ctx, (yyvsp[-1].block), &(yylsp[-1]));

            if (!(yyval.intval))
            {
                hlsl_error(ctx, &(yylsp[-1]), VKD3D_SHADER_ERROR_HLSL_INVALID_SIZE,
                        "Array size is not a positive integer constant.");
                YYABORT;
            }

            if ((yyval.intval) > 65536)
            {
                hlsl_error(ctx, &(yylsp[-1]), VKD3D_SHADER_ERROR_HLSL_INVALID_SIZE,
                        "Array size %u is not between 1 and 65536.", (yyval.intval));
                YYABORT;
            }

            destroy_block((yyvsp[-1].block));
        }
#line 11225 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 194:
#line 8405 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.arrays).sizes = NULL;
            (yyval.arrays).count = 0;
        }
#line 11234 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 195:
#line 8410 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            uint32_t *new_array;

            (yyval.arrays) = (yyvsp[0].arrays);

            if (!(new_array = hlsl_realloc(ctx, (yyval.arrays).sizes, ((yyval.arrays).count + 1) * sizeof(*new_array))))
            {
                vkd3d_free((yyval.arrays).sizes);
                YYABORT;
            }

            (yyval.arrays).sizes = new_array;
            (yyval.arrays).sizes[(yyval.arrays).count++] = (yyvsp[-1].intval);
        }
#line 11253 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 196:
#line 8427 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.modifiers) = 0;
        }
#line 11261 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 197:
#line 8431 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.modifiers) = add_modifiers(ctx, (yyvsp[0].modifiers), HLSL_STORAGE_EXTERN, &(yylsp[-1]));
        }
#line 11269 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 198:
#line 8435 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.modifiers) = add_modifiers(ctx, (yyvsp[0].modifiers), HLSL_STORAGE_NOINTERPOLATION, &(yylsp[-1]));
        }
#line 11277 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 199:
#line 8439 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.modifiers) = add_modifiers(ctx, (yyvsp[0].modifiers), HLSL_STORAGE_CENTROID, &(yylsp[-1]));
        }
#line 11285 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 200:
#line 8443 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.modifiers) = add_modifiers(ctx, (yyvsp[0].modifiers), HLSL_STORAGE_LINEAR, &(yylsp[-1]));
        }
#line 11293 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 201:
#line 8447 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.modifiers) = add_modifiers(ctx, (yyvsp[0].modifiers), HLSL_STORAGE_NOPERSPECTIVE, &(yylsp[-1]));
        }
#line 11301 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 202:
#line 8451 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.modifiers) = add_modifiers(ctx, (yyvsp[0].modifiers), HLSL_STORAGE_SHARED, &(yylsp[-1]));
        }
#line 11309 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 203:
#line 8455 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.modifiers) = add_modifiers(ctx, (yyvsp[0].modifiers), HLSL_STORAGE_GROUPSHARED, &(yylsp[-1]));
        }
#line 11317 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 204:
#line 8459 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.modifiers) = add_modifiers(ctx, (yyvsp[0].modifiers), HLSL_STORAGE_STATIC, &(yylsp[-1]));
        }
#line 11325 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 205:
#line 8463 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.modifiers) = add_modifiers(ctx, (yyvsp[0].modifiers), HLSL_STORAGE_UNIFORM, &(yylsp[-1]));
        }
#line 11333 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 206:
#line 8467 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.modifiers) = add_modifiers(ctx, (yyvsp[0].modifiers), HLSL_MODIFIER_VOLATILE, &(yylsp[-1]));
        }
#line 11341 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 207:
#line 8471 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.modifiers) = add_modifiers(ctx, (yyvsp[0].modifiers), HLSL_MODIFIER_CONST, &(yylsp[-1]));
        }
#line 11349 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 208:
#line 8475 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.modifiers) = add_modifiers(ctx, (yyvsp[0].modifiers), HLSL_MODIFIER_ROW_MAJOR, &(yylsp[-1]));
        }
#line 11357 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 209:
#line 8479 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.modifiers) = add_modifiers(ctx, (yyvsp[0].modifiers), HLSL_MODIFIER_COLUMN_MAJOR, &(yylsp[-1]));
        }
#line 11365 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 210:
#line 8483 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.modifiers) = add_modifiers(ctx, (yyvsp[0].modifiers), HLSL_STORAGE_IN, &(yylsp[-1]));
        }
#line 11373 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 211:
#line 8487 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.modifiers) = add_modifiers(ctx, (yyvsp[0].modifiers), HLSL_STORAGE_OUT, &(yylsp[-1]));
        }
#line 11381 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 212:
#line 8491 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.modifiers) = add_modifiers(ctx, (yyvsp[0].modifiers), HLSL_STORAGE_IN | HLSL_STORAGE_OUT, &(yylsp[-1]));
        }
#line 11389 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 213:
#line 8495 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.modifiers) = add_modifiers(ctx, (yyvsp[0].modifiers), HLSL_MODIFIER_INLINE, &(yylsp[-1]));
        }
#line 11397 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 214:
#line 8499 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.modifiers) = add_modifiers(ctx, (yyvsp[0].modifiers), HLSL_MODIFIER_EXPORT, &(yylsp[-1]));
        }
#line 11405 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 215:
#line 8503 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.modifiers) = add_modifiers(ctx, (yyvsp[0].modifiers), HLSL_MODIFIER_UNORM, &(yylsp[-1]));
        }
#line 11413 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 216:
#line 8507 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.modifiers) = add_modifiers(ctx, (yyvsp[0].modifiers), HLSL_MODIFIER_SNORM, &(yylsp[-1]));
        }
#line 11421 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 217:
#line 8511 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.modifiers) = (yyvsp[0].modifiers);

            if (!strcmp((yyvsp[-1].name), "precise"))
                (yyval.modifiers) = add_modifiers(ctx, (yyvsp[0].modifiers), HLSL_MODIFIER_PRECISE, &(yylsp[-1]));
            else if (!strcmp((yyvsp[-1].name), "single"))
                (yyval.modifiers) = add_modifiers(ctx, (yyvsp[0].modifiers), HLSL_MODIFIER_SINGLE, &(yylsp[-1]));
            else
                hlsl_error(ctx, &(yylsp[-1]), VKD3D_SHADER_ERROR_HLSL_UNKNOWN_MODIFIER,
                        "Unknown modifier %s.", debugstr_a((yyvsp[-1].name)));
        }
#line 11437 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 218:
#line 8525 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.initializer).args_count = 1;
            if (!((yyval.initializer).args = hlsl_alloc(ctx, sizeof(*(yyval.initializer).args))))
            {
                destroy_block((yyvsp[0].block));
                YYABORT;
            }
            (yyval.initializer).args[0] = node_from_block((yyvsp[0].block));
            (yyval.initializer).instrs = (yyvsp[0].block);
            (yyval.initializer).braces = false;
            (yyval.initializer).loc = (yyloc);
        }
#line 11454 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 219:
#line 8538 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.initializer) = (yyvsp[-1].initializer);
            (yyval.initializer).braces = true;
        }
#line 11463 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 220:
#line 8543 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.initializer) = (yyvsp[-2].initializer);
            (yyval.initializer).braces = true;
        }
#line 11472 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 222:
#line 8551 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            struct hlsl_ir_node **new_args;
            unsigned int i;

            (yyval.initializer) = (yyvsp[-2].initializer);
            if (!(new_args = hlsl_realloc(ctx, (yyval.initializer).args, ((yyval.initializer).args_count + (yyvsp[0].initializer).args_count) * sizeof(*(yyval.initializer).args))))
            {
                free_parse_initializer(&(yyval.initializer));
                free_parse_initializer(&(yyvsp[0].initializer));
                YYABORT;
            }
            (yyval.initializer).args = new_args;
            for (i = 0; i < (yyvsp[0].initializer).args_count; ++i)
                (yyval.initializer).args[(yyval.initializer).args_count++] = (yyvsp[0].initializer).args[i];
            hlsl_block_add_block((yyval.initializer).instrs, (yyvsp[0].initializer).instrs);
            free_parse_initializer(&(yyvsp[0].initializer));
            (yyval.initializer).loc = (yyloc);
        }
#line 11495 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 224:
#line 8575 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.initializer).args_count = 1;
            if (!((yyval.initializer).args = hlsl_alloc(ctx, sizeof(*(yyval.initializer).args))))
            {
                destroy_block((yyvsp[0].block));
                YYABORT;
            }
            (yyval.initializer).args[0] = node_from_block((yyvsp[0].block));
            (yyval.initializer).instrs = (yyvsp[0].block);
            (yyval.initializer).braces = false;
            (yyval.initializer).loc = (yyloc);
        }
#line 11512 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 225:
#line 8588 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            struct hlsl_ir_node **new_args;

            (yyval.initializer) = (yyvsp[-2].initializer);
            if (!(new_args = hlsl_realloc(ctx, (yyval.initializer).args, ((yyval.initializer).args_count + 1) * sizeof(*(yyval.initializer).args))))
            {
                free_parse_initializer(&(yyval.initializer));
                destroy_block((yyvsp[0].block));
                YYABORT;
            }
            (yyval.initializer).args = new_args;
            (yyval.initializer).args[(yyval.initializer).args_count++] = node_from_block((yyvsp[0].block));
            hlsl_block_add_block((yyval.initializer).instrs, (yyvsp[0].block));
            destroy_block((yyvsp[0].block));
        }
#line 11532 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 226:
#line 8606 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.boolval) = true;
        }
#line 11540 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 227:
#line 8610 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.boolval) = false;
        }
#line 11548 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 229:
#line 8617 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.block) = (yyvsp[-1].block);
            hlsl_block_add_block((yyval.block), (yyvsp[0].block));
            destroy_block((yyvsp[0].block));
        }
#line 11558 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 237:
#line 8634 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            struct hlsl_ir_node *jump;

            if (!is_break_allowed(ctx->cur_scope))
            {
                hlsl_error(ctx, &(yylsp[-1]), VKD3D_SHADER_ERROR_HLSL_INVALID_SYNTAX,
                        "The 'break' statement must be used inside of a loop or a switch.");
            }

            if (!((yyval.block) = make_empty_block(ctx)))
                YYABORT;
            if (!(jump = hlsl_new_jump(ctx, HLSL_IR_JUMP_BREAK, NULL, &(yylsp[-1]))))
                YYABORT;
            hlsl_block_add_instr((yyval.block), jump);
        }
#line 11578 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 238:
#line 8650 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            struct hlsl_ir_node *jump;

            check_continue(ctx, ctx->cur_scope, &(yylsp[-1]));

            if (!((yyval.block) = make_empty_block(ctx)))
                YYABORT;

            if (!(jump = hlsl_new_jump(ctx, HLSL_IR_JUMP_UNRESOLVED_CONTINUE, NULL, &(yylsp[-1]))))
                YYABORT;
            hlsl_block_add_instr((yyval.block), jump);
        }
#line 11595 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 239:
#line 8663 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.block) = (yyvsp[-1].block);
            if (!add_return(ctx, (yyval.block), node_from_block((yyval.block)), &(yylsp[-2])))
                YYABORT;
        }
#line 11605 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 240:
#line 8669 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            if (!((yyval.block) = make_empty_block(ctx)))
                YYABORT;
            if (!add_return(ctx, (yyval.block), NULL, &(yylsp[-1])))
                YYABORT;
        }
#line 11616 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 241:
#line 8676 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            struct hlsl_ir_node *discard, *c;

            if (!((yyval.block) = make_empty_block(ctx)))
                YYABORT;

            if (!(c = hlsl_new_uint_constant(ctx, ~0u, &(yylsp[-1]))))
                return false;
            hlsl_block_add_instr((yyval.block), c);

            if (!(discard = hlsl_new_jump(ctx, HLSL_IR_JUMP_DISCARD_NZ, c, &(yylsp[-1]))))
                return false;
            hlsl_block_add_instr((yyval.block), discard);
        }
#line 11635 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 242:
#line 8693 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            struct hlsl_ir_node *condition = node_from_block((yyvsp[-2].block));
            const struct parse_attribute_list *attributes = &(yyvsp[-5].attr_list);
            struct hlsl_ir_node *instr;
            unsigned int i;

            check_attribute_list_for_duplicates(ctx, attributes);

            for (i = 0; i < attributes->count; ++i)
            {
                const struct hlsl_attribute *attr = attributes->attrs[i];

                if (!strcmp(attr->name, "branch")
                        || !strcmp(attr->name, "flatten"))
                {
                    hlsl_warning(ctx, &(yylsp[-5]), VKD3D_SHADER_WARNING_HLSL_IGNORED_ATTRIBUTE, "Unhandled attribute '%s'.", attr->name);
                }
                else
                {
                    hlsl_warning(ctx, &(yylsp[-5]), VKD3D_SHADER_WARNING_HLSL_UNKNOWN_ATTRIBUTE, "Unrecognized attribute '%s'.", attr->name);
                }
            }

            check_condition_type(ctx, condition);

            if (!(condition = add_cast(ctx, (yyvsp[-2].block), condition, hlsl_get_scalar_type(ctx, HLSL_TYPE_BOOL), &(yylsp[-2]))))
            {
                destroy_block((yyvsp[0].if_body).then_block);
                destroy_block((yyvsp[0].if_body).else_block);
                cleanup_parse_attribute_list(&(yyvsp[-5].attr_list));
                YYABORT;
            }

            if (!(instr = hlsl_new_if(ctx, condition, (yyvsp[0].if_body).then_block, (yyvsp[0].if_body).else_block, &(yylsp[-4]))))
            {
                destroy_block((yyvsp[0].if_body).then_block);
                destroy_block((yyvsp[0].if_body).else_block);
                cleanup_parse_attribute_list(&(yyvsp[-5].attr_list));
                YYABORT;
            }
            destroy_block((yyvsp[0].if_body).then_block);
            destroy_block((yyvsp[0].if_body).else_block);
            cleanup_parse_attribute_list(&(yyvsp[-5].attr_list));

            (yyval.block) = (yyvsp[-2].block);
            hlsl_block_add_instr((yyval.block), instr);
        }
#line 11687 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 243:
#line 8743 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.if_body).then_block = (yyvsp[0].block);
            (yyval.if_body).else_block = NULL;
        }
#line 11696 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 244:
#line 8748 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.if_body).then_block = (yyvsp[-2].block);
            (yyval.if_body).else_block = (yyvsp[0].block);
        }
#line 11705 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 245:
#line 8755 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.block) = create_loop(ctx, LOOP_WHILE, &(yyvsp[-6].attr_list), NULL, (yyvsp[-2].block), NULL, (yyvsp[0].block), &(yylsp[-4]));
            hlsl_pop_scope(ctx);
            cleanup_parse_attribute_list(&(yyvsp[-6].attr_list));
        }
#line 11715 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 246:
#line 8761 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.block) = create_loop(ctx, LOOP_DO_WHILE, &(yyvsp[-8].attr_list), NULL, (yyvsp[-2].block), NULL, (yyvsp[-5].block), &(yylsp[-6]));
            hlsl_pop_scope(ctx);
            cleanup_parse_attribute_list(&(yyvsp[-8].attr_list));
        }
#line 11725 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 247:
#line 8767 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.block) = create_loop(ctx, LOOP_FOR, &(yyvsp[-8].attr_list), (yyvsp[-4].block), (yyvsp[-3].block), (yyvsp[-2].block), (yyvsp[0].block), &(yylsp[-6]));
            hlsl_pop_scope(ctx);
            cleanup_parse_attribute_list(&(yyvsp[-8].attr_list));
        }
#line 11735 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 248:
#line 8773 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.block) = create_loop(ctx, LOOP_FOR, &(yyvsp[-8].attr_list), (yyvsp[-4].block), (yyvsp[-3].block), (yyvsp[-2].block), (yyvsp[0].block), &(yylsp[-6]));
            hlsl_pop_scope(ctx);
            cleanup_parse_attribute_list(&(yyvsp[-8].attr_list));
        }
#line 11745 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 249:
#line 8781 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            struct hlsl_ir_node *selector = node_from_block((yyvsp[-4].block));
            struct hlsl_ir_node *s;

            if (!(selector = add_implicit_conversion(ctx, (yyvsp[-4].block), selector, hlsl_get_scalar_type(ctx, HLSL_TYPE_UINT), &(yylsp[-4]))))
            {
                destroy_switch_cases((yyvsp[-1].list));
                destroy_block((yyvsp[-4].block));
                cleanup_parse_attribute_list(&(yyvsp[-8].attr_list));
                YYABORT;
            }

            s = hlsl_new_switch(ctx, selector, (yyvsp[-1].list), &(yylsp[-6]));

            destroy_switch_cases((yyvsp[-1].list));

            if (!s)
            {
                destroy_block((yyvsp[-4].block));
                cleanup_parse_attribute_list(&(yyvsp[-8].attr_list));
                YYABORT;
            }

            (yyval.block) = (yyvsp[-4].block);
            hlsl_block_add_instr((yyval.block), s);

            hlsl_pop_scope(ctx);
            cleanup_parse_attribute_list(&(yyvsp[-8].attr_list));
        }
#line 11779 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 250:
#line 8813 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            struct hlsl_ir_switch_case *c;
            unsigned int value;

            value = evaluate_static_expression_as_uint(ctx, (yyvsp[-2].block), &(yylsp[-2]));

            c = hlsl_new_switch_case(ctx, value, false, (yyvsp[0].block), &(yylsp[-2]));

            destroy_block((yyvsp[-2].block));
            destroy_block((yyvsp[0].block));

            if (!c)
                YYABORT;
            (yyval.switch_case) = c;
        }
#line 11799 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 251:
#line 8829 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            struct hlsl_ir_switch_case *c;
            unsigned int value;

            value = evaluate_static_expression_as_uint(ctx, (yyvsp[-1].block), &(yylsp[-1]));

            c = hlsl_new_switch_case(ctx, value, false, NULL, &(yylsp[-1]));

            destroy_block((yyvsp[-1].block));

            if (!c)
                YYABORT;
            (yyval.switch_case) = c;
        }
#line 11818 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 252:
#line 8844 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            struct hlsl_ir_switch_case *c;

            c = hlsl_new_switch_case(ctx, 0, true, (yyvsp[0].block), &(yylsp[-2]));

            destroy_block((yyvsp[0].block));

            if (!c)
                YYABORT;
            (yyval.switch_case) = c;
        }
#line 11834 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 253:
#line 8856 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            struct hlsl_ir_switch_case *c;

            if (!(c = hlsl_new_switch_case(ctx, 0, true, NULL, &(yylsp[-1]))))
                YYABORT;
            (yyval.switch_case) = c;
        }
#line 11846 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 254:
#line 8866 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            struct hlsl_ir_switch_case *c = LIST_ENTRY((yyvsp[0].switch_case), struct hlsl_ir_switch_case, entry);
            if (!((yyval.list) = make_empty_list(ctx)))
            {
                hlsl_free_ir_switch_case(c);
                YYABORT;
            }
            list_add_head((yyval.list), &(yyvsp[0].switch_case)->entry);
        }
#line 11860 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 255:
#line 8876 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.list) = (yyvsp[-1].list);
            check_duplicated_switch_cases(ctx, (yyvsp[0].switch_case), (yyval.list));
            list_add_tail((yyval.list), &(yyvsp[0].switch_case)->entry);
        }
#line 11870 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 256:
#line 8884 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            if (!((yyval.block) = make_empty_block(ctx)))
                YYABORT;
        }
#line 11879 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 258:
#line 8892 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.block) = (yyvsp[-1].block);
        }
#line 11887 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 259:
#line 8898 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.initializer).args = NULL;
            (yyval.initializer).args_count = 0;
            if (!((yyval.initializer).instrs = make_empty_block(ctx)))
                YYABORT;
            (yyval.initializer).braces = false;
            (yyval.initializer).loc = (yyloc);
        }
#line 11900 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 261:
#line 8910 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            struct hlsl_ir_node *c;

            if (!(c = hlsl_new_float_constant(ctx, (yyvsp[0].floatval), &(yylsp[0]))))
                YYABORT;
            if (!((yyval.block) = make_block(ctx, c)))
                YYABORT;
        }
#line 11913 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 262:
#line 8919 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            struct hlsl_ir_node *c;

            if (!(c = hlsl_new_int_constant(ctx, (yyvsp[0].intval), &(yylsp[0]))))
                YYABORT;
            if (!((yyval.block) = make_block(ctx, c)))
                YYABORT;
        }
#line 11926 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 263:
#line 8928 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            struct hlsl_ir_node *c;

            if (!(c = hlsl_new_uint_constant(ctx, (yyvsp[0].intval), &(yylsp[0]))))
                YYABORT;
            if (!((yyval.block) = make_block(ctx, c)))
                YYABORT;
        }
#line 11939 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 264:
#line 8937 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            struct hlsl_ir_node *c;

            if (!(c = hlsl_new_bool_constant(ctx, (yyvsp[0].boolval), &(yylsp[0]))))
                YYABORT;
            if (!((yyval.block) = make_block(ctx, c)))
            {
                hlsl_free_instr(c);
                YYABORT;
            }
        }
#line 11955 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 265:
#line 8949 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            struct hlsl_ir_node *c;

            if (!(c = hlsl_new_string_constant(ctx, (yyvsp[0].name), &(yylsp[0]))))
            {
                vkd3d_free((yyvsp[0].name));
                YYABORT;
            }
            vkd3d_free((yyvsp[0].name));

            if (!((yyval.block) = make_block(ctx, c)))
            {
                hlsl_free_instr(c);
                YYABORT;
            }
        }
#line 11976 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 266:
#line 8966 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            struct hlsl_ir_node *c;

            if (!(c = hlsl_new_null_constant(ctx, &(yylsp[0]))))
                YYABORT;
            if (!((yyval.block) = make_block(ctx, c)))
            {
                hlsl_free_instr(c);
                YYABORT;
            }
        }
#line 11992 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 267:
#line 8978 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            struct hlsl_ir_load *load;
            struct hlsl_ir_var *var;

            if (!(var = hlsl_get_var(ctx->cur_scope, (yyvsp[0].name))))
            {
                hlsl_error(ctx, &(yylsp[0]), VKD3D_SHADER_ERROR_HLSL_NOT_DEFINED, "Variable \"%s\" is not defined.", (yyvsp[0].name));
                vkd3d_free((yyvsp[0].name));
                YYABORT;
            }
            vkd3d_free((yyvsp[0].name));
            if (!(load = hlsl_new_var_load(ctx, var, &(yylsp[0]))))
                YYABORT;
            if (!((yyval.block) = make_block(ctx, &load->node)))
                YYABORT;
        }
#line 12013 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 268:
#line 8995 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.block) = (yyvsp[-1].block);
        }
#line 12021 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 269:
#line 9000 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            if (!((yyval.block) = add_shader_compilation(ctx, (yyvsp[-4].name), (yyvsp[-3].name), &(yyvsp[-1].initializer), &(yylsp[-5]))))
            {
                vkd3d_free((yyvsp[-4].name));
                vkd3d_free((yyvsp[-3].name));
                YYABORT;
            }
            vkd3d_free((yyvsp[-4].name));
            vkd3d_free((yyvsp[-3].name));
        }
#line 12036 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 270:
#line 9011 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            if (!((yyval.block) = add_shader_compilation(ctx, (yyvsp[-6].name), (yyvsp[-4].name), &(yyvsp[-2].initializer), &(yylsp[-8]))))
            {
                vkd3d_free((yyvsp[-6].name));
                vkd3d_free((yyvsp[-4].name));
                YYABORT;
            }
            vkd3d_free((yyvsp[-6].name));
            vkd3d_free((yyvsp[-4].name));
        }
#line 12051 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 271:
#line 9022 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            if (!((yyval.block) = add_compile_variant(ctx, HLSL_COMPILE_TYPE_CONSTRUCTGSWITHSO, &(yyvsp[-1].initializer), &(yylsp[-3]))))
                YYABORT;
        }
#line 12060 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 272:
#line 9027 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            if (!((yyval.block) = add_call(ctx, (yyvsp[-3].name), &(yyvsp[-1].initializer), &(yylsp[-3]))))
            {
                vkd3d_free((yyvsp[-3].name));
                YYABORT;
            }
            vkd3d_free((yyvsp[-3].name));
        }
#line 12073 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 273:
#line 9036 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            struct hlsl_ir_node *sampler_state;
            ctx->in_state_block = 0;

            if (!ctx->in_state_block && ctx->cur_scope != ctx->globals)
                hlsl_error(ctx, &(yylsp[-4]), VKD3D_SHADER_ERROR_HLSL_MISPLACED_SAMPLER_STATE,
                        "sampler_state must be in global scope or a state block.");

            if (!(sampler_state = hlsl_new_sampler_state(ctx, (yyvsp[-1].state_block), &(yylsp[-4]))))
            {
                hlsl_free_state_block((yyvsp[-1].state_block));
                YYABORT;
            }
            hlsl_free_state_block((yyvsp[-1].state_block));

            if (!((yyval.block) = make_block(ctx, sampler_state)))
                YYABORT;
        }
#line 12096 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 274:
#line 9055 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            if (ctx->in_state_block)
            {
                struct hlsl_ir_node *constant;

                if (!(constant = hlsl_new_stateblock_constant(ctx, (yyvsp[0].name), &(yylsp[0]))))
                    YYABORT;
                vkd3d_free((yyvsp[0].name));

                if (!((yyval.block) = make_block(ctx, constant)))
                    YYABORT;
            }
            else
            {
                hlsl_error(ctx, &(yylsp[0]), VKD3D_SHADER_ERROR_HLSL_NOT_DEFINED, "Identifier \"%s\" is not declared.", (yyvsp[0].name));
                vkd3d_free((yyvsp[0].name));

                if (!((yyval.block) = make_empty_block(ctx)))
                    YYABORT;
                (yyval.block)->value = ctx->error_instr;
            }
        }
#line 12123 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 276:
#line 9081 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            if (!add_increment(ctx, (yyvsp[-1].block), false, true, &(yylsp[0])))
            {
                destroy_block((yyvsp[-1].block));
                YYABORT;
            }
            (yyval.block) = (yyvsp[-1].block);
        }
#line 12136 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 277:
#line 9090 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            if (!add_increment(ctx, (yyvsp[-1].block), true, true, &(yylsp[0])))
            {
                destroy_block((yyvsp[-1].block));
                YYABORT;
            }
            (yyval.block) = (yyvsp[-1].block);
        }
#line 12149 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 278:
#line 9099 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            struct hlsl_ir_node *node = node_from_block((yyvsp[-2].block));

            if (node->data_type->class == HLSL_CLASS_STRUCT)
            {
                if (!add_record_access_recurse(ctx, (yyvsp[-2].block), (yyvsp[0].name), &(yylsp[-1])))
                {
                    destroy_block((yyvsp[-2].block));
                    vkd3d_free((yyvsp[0].name));
                    YYABORT;
                }
            }
            else if (hlsl_is_numeric_type(node->data_type))
            {
                struct hlsl_ir_node *swizzle;

                if ((swizzle = get_swizzle(ctx, node, (yyvsp[0].name), &(yylsp[0]))))
                {
                    hlsl_block_add_instr((yyvsp[-2].block), swizzle);
                }
                else
                {
                    hlsl_error(ctx, &(yylsp[0]), VKD3D_SHADER_ERROR_HLSL_INVALID_SYNTAX, "Invalid swizzle \"%s\".", (yyvsp[0].name));
                    (yyvsp[-2].block)->value = ctx->error_instr;
                }
            }
            else if (node->data_type->class != HLSL_CLASS_ERROR)
            {
                hlsl_error(ctx, &(yylsp[0]), VKD3D_SHADER_ERROR_HLSL_INVALID_SYNTAX, "Invalid subscript \"%s\".", (yyvsp[0].name));
                (yyvsp[-2].block)->value = ctx->error_instr;
            }
            vkd3d_free((yyvsp[0].name));
            (yyval.block) = (yyvsp[-2].block);
        }
#line 12188 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 279:
#line 9134 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            struct hlsl_ir_node *array = node_from_block((yyvsp[-3].block)), *index = node_from_block((yyvsp[-1].block));

            hlsl_block_add_block((yyvsp[-1].block), (yyvsp[-3].block));
            destroy_block((yyvsp[-3].block));

            if (!add_array_access(ctx, (yyvsp[-1].block), array, index, &(yylsp[-2])))
            {
                destroy_block((yyvsp[-1].block));
                YYABORT;
            }
            (yyval.block) = (yyvsp[-1].block);
        }
#line 12206 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 280:
#line 9150 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            if ((yyvsp[-4].modifiers))
            {
                hlsl_error(ctx, &(yylsp[-4]), VKD3D_SHADER_ERROR_HLSL_INVALID_MODIFIER,
                        "Modifiers are not allowed on constructors.");
                free_parse_initializer(&(yyvsp[-1].initializer));
                YYABORT;
            }
            if (!hlsl_is_numeric_type((yyvsp[-3].type)))
            {
                struct vkd3d_string_buffer *string;

                if ((string = hlsl_type_to_string(ctx, (yyvsp[-3].type))))
                    hlsl_error(ctx, &(yylsp[-3]), VKD3D_SHADER_ERROR_HLSL_INVALID_TYPE,
                            "Constructor data type %s is not numeric.", string->buffer);
                hlsl_release_string_buffer(ctx, string);
                free_parse_initializer(&(yyvsp[-1].initializer));
                YYABORT;
            }

            if (!((yyval.block) = add_constructor(ctx, (yyvsp[-3].type), &(yyvsp[-1].initializer), &(yylsp[-3]))))
            {
                free_parse_initializer(&(yyvsp[-1].initializer));
                YYABORT;
            }
        }
#line 12237 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 281:
#line 9177 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            struct hlsl_ir_node *object = node_from_block((yyvsp[-5].block));

            hlsl_block_add_block((yyvsp[-5].block), (yyvsp[-1].initializer).instrs);
            vkd3d_free((yyvsp[-1].initializer).instrs);

            if (!add_method_call(ctx, (yyvsp[-5].block), object, (yyvsp[-3].name), &(yyvsp[-1].initializer), &(yylsp[-3])))
            {
                destroy_block((yyvsp[-5].block));
                vkd3d_free((yyvsp[-1].initializer).args);
                YYABORT;
            }
            vkd3d_free((yyvsp[-1].initializer).args);
            (yyval.block) = (yyvsp[-5].block);
        }
#line 12257 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 283:
#line 9196 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            if (!add_increment(ctx, (yyvsp[0].block), false, false, &(yylsp[-1])))
            {
                destroy_block((yyvsp[0].block));
                YYABORT;
            }
            (yyval.block) = (yyvsp[0].block);
        }
#line 12270 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 284:
#line 9205 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            if (!add_increment(ctx, (yyvsp[0].block), true, false, &(yylsp[-1])))
            {
                destroy_block((yyvsp[0].block));
                YYABORT;
            }
            (yyval.block) = (yyvsp[0].block);
        }
#line 12283 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 285:
#line 9214 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.block) = (yyvsp[0].block);
        }
#line 12291 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 286:
#line 9218 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            add_unary_arithmetic_expr(ctx, (yyvsp[0].block), HLSL_OP1_NEG, node_from_block((yyvsp[0].block)), &(yylsp[-1]));
            (yyval.block) = (yyvsp[0].block);
        }
#line 12300 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 287:
#line 9223 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            add_unary_bitwise_expr(ctx, (yyvsp[0].block), HLSL_OP1_BIT_NOT, node_from_block((yyvsp[0].block)), &(yylsp[-1]));
            (yyval.block) = (yyvsp[0].block);
        }
#line 12309 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 288:
#line 9228 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            add_unary_logical_expr(ctx, (yyvsp[0].block), HLSL_OP1_LOGIC_NOT, node_from_block((yyvsp[0].block)), &(yylsp[-1]));
            (yyval.block) = (yyvsp[0].block);
        }
#line 12318 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 289:
#line 9234 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            if ((yyvsp[-4].modifiers))
            {
                hlsl_error(ctx, &(yylsp[-4]), VKD3D_SHADER_ERROR_HLSL_INVALID_MODIFIER,
                        "Modifiers are not allowed on casts.");
                YYABORT;
            }

            if (!add_explicit_conversion(ctx, (yyvsp[0].block), (yyvsp[-3].type), &(yyvsp[-2].arrays), &(yylsp[-3])))
            {
                destroy_block((yyvsp[0].block));
                vkd3d_free((yyvsp[-2].arrays).sizes);
                YYABORT;
            }
            vkd3d_free((yyvsp[-2].arrays).sizes);
            (yyval.block) = (yyvsp[0].block);
        }
#line 12340 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 291:
#line 9255 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.block) = add_binary_expr_merge(ctx, (yyvsp[-2].block), (yyvsp[0].block), HLSL_OP2_MUL, &(yylsp[-1]));
        }
#line 12348 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 292:
#line 9259 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.block) = add_binary_expr_merge(ctx, (yyvsp[-2].block), (yyvsp[0].block), HLSL_OP2_DIV, &(yylsp[-1]));
        }
#line 12356 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 293:
#line 9263 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.block) = add_binary_expr_merge(ctx, (yyvsp[-2].block), (yyvsp[0].block), HLSL_OP2_MOD, &(yylsp[-1]));
        }
#line 12364 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 295:
#line 9270 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.block) = add_binary_expr_merge(ctx, (yyvsp[-2].block), (yyvsp[0].block), HLSL_OP2_ADD, &(yylsp[-1]));
        }
#line 12372 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 296:
#line 9274 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            struct hlsl_ir_node *neg;

            if (!(neg = add_unary_arithmetic_expr(ctx, (yyvsp[0].block), HLSL_OP1_NEG, node_from_block((yyvsp[0].block)), &(yylsp[-1]))))
                YYABORT;
            (yyval.block) = add_binary_expr_merge(ctx, (yyvsp[-2].block), (yyvsp[0].block), HLSL_OP2_ADD, &(yylsp[-1]));
        }
#line 12384 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 298:
#line 9285 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.block) = add_binary_expr_merge(ctx, (yyvsp[-2].block), (yyvsp[0].block), HLSL_OP2_LSHIFT, &(yylsp[-1]));
        }
#line 12392 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 299:
#line 9289 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.block) = add_binary_expr_merge(ctx, (yyvsp[-2].block), (yyvsp[0].block), HLSL_OP2_RSHIFT, &(yylsp[-1]));
        }
#line 12400 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 301:
#line 9296 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.block) = add_binary_expr_merge(ctx, (yyvsp[-2].block), (yyvsp[0].block), HLSL_OP2_LESS, &(yylsp[-1]));
        }
#line 12408 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 302:
#line 9300 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.block) = add_binary_expr_merge(ctx, (yyvsp[0].block), (yyvsp[-2].block), HLSL_OP2_LESS, &(yylsp[-1]));
        }
#line 12416 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 303:
#line 9304 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.block) = add_binary_expr_merge(ctx, (yyvsp[0].block), (yyvsp[-2].block), HLSL_OP2_GEQUAL, &(yylsp[-1]));
        }
#line 12424 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 304:
#line 9308 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.block) = add_binary_expr_merge(ctx, (yyvsp[-2].block), (yyvsp[0].block), HLSL_OP2_GEQUAL, &(yylsp[-1]));
        }
#line 12432 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 306:
#line 9315 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.block) = add_binary_expr_merge(ctx, (yyvsp[-2].block), (yyvsp[0].block), HLSL_OP2_EQUAL, &(yylsp[-1]));
        }
#line 12440 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 307:
#line 9319 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.block) = add_binary_expr_merge(ctx, (yyvsp[-2].block), (yyvsp[0].block), HLSL_OP2_NEQUAL, &(yylsp[-1]));
        }
#line 12448 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 309:
#line 9326 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.block) = add_binary_expr_merge(ctx, (yyvsp[-2].block), (yyvsp[0].block), HLSL_OP2_BIT_AND, &(yylsp[-1]));
        }
#line 12456 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 311:
#line 9333 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.block) = add_binary_expr_merge(ctx, (yyvsp[-2].block), (yyvsp[0].block), HLSL_OP2_BIT_XOR, &(yylsp[-1]));
        }
#line 12464 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 313:
#line 9340 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.block) = add_binary_expr_merge(ctx, (yyvsp[-2].block), (yyvsp[0].block), HLSL_OP2_BIT_OR, &(yylsp[-1]));
        }
#line 12472 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 315:
#line 9347 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.block) = add_binary_expr_merge(ctx, (yyvsp[-2].block), (yyvsp[0].block), HLSL_OP2_LOGIC_AND, &(yylsp[-1]));
        }
#line 12480 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 317:
#line 9354 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.block) = add_binary_expr_merge(ctx, (yyvsp[-2].block), (yyvsp[0].block), HLSL_OP2_LOGIC_OR, &(yylsp[-1]));
        }
#line 12488 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 319:
#line 9361 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            struct hlsl_ir_node *cond = node_from_block((yyvsp[-4].block));
            struct hlsl_ir_node *first = node_from_block((yyvsp[-2].block));
            struct hlsl_ir_node *second = node_from_block((yyvsp[0].block));

            hlsl_block_add_block((yyvsp[-4].block), (yyvsp[-2].block));
            hlsl_block_add_block((yyvsp[-4].block), (yyvsp[0].block));
            destroy_block((yyvsp[-2].block));
            destroy_block((yyvsp[0].block));

            if (!add_ternary(ctx, (yyvsp[-4].block), cond, first, second))
                YYABORT;
            (yyval.block) = (yyvsp[-4].block);
        }
#line 12507 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 321:
#line 9380 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            struct hlsl_ir_node *lhs = node_from_block((yyvsp[-2].block)), *rhs = node_from_block((yyvsp[0].block));

            if (lhs->data_type->modifiers & HLSL_MODIFIER_CONST)
            {
                hlsl_error(ctx, &(yylsp[-1]), VKD3D_SHADER_ERROR_HLSL_MODIFIES_CONST, "Statement modifies a const expression.");
                YYABORT;
            }
            hlsl_block_add_block((yyvsp[0].block), (yyvsp[-2].block));
            destroy_block((yyvsp[-2].block));
            if (!add_assignment(ctx, (yyvsp[0].block), lhs, (yyvsp[-1].assign_op), rhs))
                YYABORT;
            (yyval.block) = (yyvsp[0].block);
        }
#line 12526 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 322:
#line 9397 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.assign_op) = ASSIGN_OP_ASSIGN;
        }
#line 12534 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 323:
#line 9401 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.assign_op) = ASSIGN_OP_ADD;
        }
#line 12542 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 324:
#line 9405 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.assign_op) = ASSIGN_OP_SUB;
        }
#line 12550 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 325:
#line 9409 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.assign_op) = ASSIGN_OP_MUL;
        }
#line 12558 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 326:
#line 9413 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.assign_op) = ASSIGN_OP_DIV;
        }
#line 12566 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 327:
#line 9417 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.assign_op) = ASSIGN_OP_MOD;
        }
#line 12574 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 328:
#line 9421 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.assign_op) = ASSIGN_OP_LSHIFT;
        }
#line 12582 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 329:
#line 9425 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.assign_op) = ASSIGN_OP_RSHIFT;
        }
#line 12590 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 330:
#line 9429 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.assign_op) = ASSIGN_OP_AND;
        }
#line 12598 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 331:
#line 9433 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.assign_op) = ASSIGN_OP_OR;
        }
#line 12606 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 332:
#line 9437 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.assign_op) = ASSIGN_OP_XOR;
        }
#line 12614 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;

  case 334:
#line 9444 "E:\\KNSoft\\Kundows\\Upstream\\ReactOS\\Source\\sdk\\lib\\vkd3d\\libs\\vkd3d-shader\\hlsl.y"
        {
            (yyval.block) = (yyvsp[-2].block);
            hlsl_block_add_block((yyval.block), (yyvsp[0].block));
            destroy_block((yyvsp[0].block));
        }
#line 12624 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"
    break;


#line 12628 "E:\\KNSoft\\Kundows\\Build\\ReactOSParserSources\\Vkd3d\\hlsl.tab.c"

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
      yyerror (&yylloc, scanner, ctx, YY_("syntax error"));
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
        yyerror (&yylloc, scanner, ctx, yymsgp);
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
                      yytoken, &yylval, &yylloc, scanner, ctx);
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
                  yystos[yystate], yyvsp, yylsp, scanner, ctx);
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
  yyerror (&yylloc, scanner, ctx, YY_("memory exhausted"));
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
                  yytoken, &yylval, &yylloc, scanner, ctx);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[+*yyssp], yyvsp, yylsp, scanner, ctx);
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
