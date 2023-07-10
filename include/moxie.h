#ifndef SRC_INCLUDE_MOXIE_H_
#define SRC_INCLUDE_MOXIE_H_

#include "CppUTestExt/MockSupport_c.h"

/*
 * Types:
 */

/**
 * @brief A MoxieState describes the state of a mocked function.
 */
typedef struct MoxieState
{
    int mockFlag;
    char *scope;
    void *callFunc;
    void *stubFunc;
} MoxieState;

/*
 * API Functions:
 */

/**
 * @brief Resets the CppUMock integration for the specified function.
 *
 * @example Moxie_reset(sqrt)();
 */
#define Moxie_reset(FUNC) __mReset_#FUNC

/**
 * @brief Enables the CppUMock integration for the specified function.
 *
 * @example Moxie_enable(sqrt)();
 */
#define Moxie_enable(FUNC) __mEnable_#FUNC

/**
 * @brief Sets the CppUMock scope in which the mock expectations are tracked.
 *
 * @example Moxie_setScope(sqrt)("MATH");
 *
 * @param scope the CppUMock scope
 */
#define Moxie_setScope(FUNC) __mSetScope_#FUNC

/**
 * @brief Overrides the callback function executed upon call.
 *
 * @code
 * void
 * my_sqrt_callFunc(MockSupport_c *mockSupport, MockActualCall_c *actualCall, double arg)
 * {
 *     actualCall->withDoubleParameters("arg", arg);
 * }
 *
 * Moxie_setCallFunc(sqrt)(my_sqrt_callFunc);
 * @endcode
 *
 * @param callFunc a MoxieCallFunc_##FUNC callback function
 */
#define Moxie_setCallFunc(FUNC) __mSetCallFunc_#FUNC

/**
 * @brief Overrides the callback function executed upon return.
 *
 * @code
 * double
 * my_sqrt_returnFunc(MockSupport_c *mockSupport, MockActualCall_c *actualCall, double arg)
 * {
 *     return arg * arg;
 * }
 *
 * Moxie_setStubFunc(sqrt)(my_sqrt_returnFunc);
 * @endcode
 *
 * @param callFunc a MoxieStubFunc_##FUNC callback function
 */
#define Moxie_setStubFunc(FUNC) __mSetStubFunc_#FUNC

/*
 * API Macros:
 */

/**
 * @brief Specifies for andReturnBoolValue() that a void function should exit
 * instead of invoking a stubFunc or a realFunc.
 *
 * The necessity of this flag is exemplified by unconditional void functions
 * that set output parameters. Thus, if a developer configures an expectation
 * for an output parameter, but the underlying realFunc is executed, then the
 * underlying realFunc will override the configured expectation.
 */
#define M_VOID_EXIT 0

/**
 * @brief Notifies a GCC/Clang C compiler that the attributed function should
 * be emitted as a weak symbol rather than a global symbol.
 *
 * @code
 * M_EXPORT_MOCK
 * extern double sqrt(double);
 * @endcode
 */
#define M_EXPORT_MOCK __attribute__((weak))

/**
 * @brief Returns the function identifier of the real implementation of FUNC.
 *
 * @example M_REAL_FUNC(sqrt)(1.0);
 */
#define M_REAL_FUNC(FUNC) __real_##FUNC

/**
 * @brief Returns the function identifier of the mocked implementation of FUNC.
 *
 * @example M_MOCK_FUNC(sqrt)(1.0);
 */
#define M_MOCK_FUNC(FUNC) __wrap_##FUNC

/**
 * @brief Declares the specified function is to be mocked.
 *
 * @note It is recommended to organize all M_DECLARE_MOCK macro calls into a
 * centralized '.h' file.
 * @note M_RETURN_VOID and/or M_PARAM_VOID can be used as needed; if the
 * specified function does not accept any parameters, then M_PARAM_VOID must be
 * specified.
 *
 * @code
 * M_DECLARE_MOCK(
 *     M_RETURN_DOUBLE(double),
 *     pow,
 *     M_PARAM_DOUBLE(double,x),
 *     M_PARAM_DOUBLE(double,y)
 * );
 * @endcode
 *
 * @see "Type Macros" below for an overview of the supported Type Macros.
 *
 * @param RET a M_RETURN_* macro call that describes the return type of the
 *            specified function
 * @param FUNC a C identifier that names the specified function
 * @param ... a sequence of M_PARAM_* macro calls that describes the parameter
 *            types of the specified function
 */
#define M_DECLARE_MOCK(RET,FUNC,...) _M_DECLARE_MOCK(RET,FUNC,__VA_ARGS__)

/**
 * @brief Implements the specified function as a mock.
 *
 * @note It is recommended to organize all M_IMPLEMENT_MOCK macro calls into a
 * centralized '.c' file.
 * @note M_RETURN_VOID and/or M_PARAM_VOID can be used as needed; if the
 * specified function does not accept any parameters, then M_PARAM_VOID must be
 * specified.
 *
 * @code
 * M_IMPLEMENT_MOCK(
 *     M_RETURN_DOUBLE(double),
 *     pow,
 *     M_PARAM_DOUBLE(double,x),
 *     M_PARAM_DOUBLE(double,y)
 * );
 * @endcode
 *
 * @see "Type Macros" below for an overview of the supported Type Macros.
 *
 * @param RET a M_RETURN_* macro call that describes the return type of the
 *            specified function
 * @param FUNC a C identifier that names the specified function
 * @param ... a sequence of M_PARAM_* macro calls that describes the parameter
 *            types of the specified function
 */
#define M_IMPLEMENT_MOCK(RET,FUNC,...) _M_IMPLEMENT_MOCK(RET,FUNC,__VA_ARGS__)

/*
 * Type Macros:
 * These Type Macros are associated with the generalized types supported by
 * CppUMock.
 */

// Undefine Type Suffixes to prevent early expansion such that the Type Suffixes
// can be appended after *_TYPE, *_NAME, and *_CALLBACK macros and expanded with
// the caller specified arguments.
#undef _VOID
#undef _MS
#undef _MAC
#undef _BOOL
#undef _INT
#undef _UINT
#undef _LONG
#undef _ULONG
#undef _DOUBLE
#undef _CHAR_PTR
#undef _PTR
#undef _IN_PTR
#undef _OUT_PTR
#undef _IN_TYPE_PTR
#undef _OUT_TYPE_PTR
#undef _IGNORE
#undef _CUSTOM

// Returns:
// - M_RETURN_* macros are the public macros that capture the developer's
//   description of the return type.
// - For each M_RETURN_* macro, corresponding private _M_RETURN_TYPE_* and
//   _M_RETURN_CALLBACK_* macros must be defined with identical arity between
//   all 3 macros.
// - M_RETURN_VOID does not need any arguments to be specified.
// - M_RETURN_CUSTOM can be utilized to specify custom code generation for the
//   return.
#define M_RETURN_VOID _M_DEFER(_VOID)
#define M_RETURN_BOOL(PTYPE) _M_DEFER(_BOOL)(PTYPE)
#define M_RETURN_INT(PTYPE) _M_DEFER(_INT)(PTYPE)
#define M_RETURN_UINT(PTYPE) _M_DEFER(_UINT)(PTYPE)
#define M_RETURN_LONG(PTYPE) _M_DEFER(_LONG)(PTYPE)
#define M_RETURN_ULONG(PTYPE) _M_DEFER(_ULONG)(PTYPE)
#define M_RETURN_DOUBLE(PTYPE) _M_DEFER(_DOUBLE)(PTYPE)
#define M_RETURN_CHAR_PTR(PTYPE) _M_DEFER(_CHAR_PTR)(PTYPE)
#define M_RETURN_PTR(PTYPE) _M_DEFER(_PTR)(PTYPE)
#define M_RETURN_CUSTOM(PTYPE,...) _M_DEFER(_CUSTOM)(PTYPE,__VA_ARGS__)

#define _M_RETURN_TYPE(RET) _M_RETURN_TYPE_##RET
#define _M_RETURN_TYPE_VOID void
#define _M_RETURN_TYPE_BOOL(PTYPE) PTYPE
#define _M_RETURN_TYPE_INT(PTYPE) PTYPE
#define _M_RETURN_TYPE_UINT(PTYPE) PTYPE
#define _M_RETURN_TYPE_LONG(PTYPE) PTYPE
#define _M_RETURN_TYPE_ULONG(PTYPE) PTYPE
#define _M_RETURN_TYPE_DOUBLE(PTYPE) PTYPE
#define _M_RETURN_TYPE_CHAR_PTR(PTYPE) PTYPE
#define _M_RETURN_TYPE_PTR(PTYPE) PTYPE
#define _M_RETURN_TYPE_CUSTOM(PTYPE,...) PTYPE

#define _M_RETURN_CALLBACK(RET) _M_RETURN_CALLBACK_##RET
#define _M_RETURN_CALLBACK_VOID return;
#define _M_RETURN_CALLBACK_BOOL(PTYPE) return (PTYPE) actualCall->boolReturnValue();
#define _M_RETURN_CALLBACK_INT(PTYPE) return (PTYPE) actualCall->intReturnValue();
#define _M_RETURN_CALLBACK_UINT(PTYPE) return (PTYPE) actualCall->unsignedIntReturnValue();
#define _M_RETURN_CALLBACK_LONG(PTYPE) return (PTYPE) actualCall->longIntReturnValue();
#define _M_RETURN_CALLBACK_ULONG(PTYPE) return (PTYPE) actualCall->unsignedLongIntReturnValue();
#define _M_RETURN_CALLBACK_DOUBLE(PTYPE) return (PTYPE) actualCall->doubleReturnValue();
#define _M_RETURN_CALLBACK_CHAR_PTR(PTYPE) return (PTYPE) actualCall->stringReturnValue();
#define _M_RETURN_CALLBACK_PTR(PTYPE) return (PTYPE) actualCall->pointerReturnValue();
#define _M_RETURN_CALLBACK_CUSTOM(PTYPE,...) __VA_ARGS__

// Parameters:
// - M_PARAM_* macros are the public macros that capture the developer's
// description of the parameter types.
// - For each M_PARAM_* macro, corresponding private _M_PARAM_TYPE_*,
//   _M_PARAM_NAME_*, and _M_PARAM_CALLBACK_* macros must be defined with
//   identical arity between all 4 macros.
// - M_PARAM_VOID does not need any arguments to be specified.
// - M_PARAM_IGNORE can be utilized to ignore code generation for the specified
//   parameter.
// - M_PARAM_CUSTOM can be utilized to specify custom code generation for the
//   specified parameter.
#define M_PARAM_VOID _M_DEFER(_VOID)(void,)
#define M_PARAM_MS _M_DEFER(_MS)(MockSupport_c*,mockSupport)
#define M_PARAM_MAC _M_DEFER(_MAC)(MockActualCall_c*,actualCall)
#define M_PARAM_BOOL(PTYPE,PNAME) _M_DEFER(_BOOL)(PTYPE,PNAME)
#define M_PARAM_INT(PTYPE,PNAME) _M_DEFER(_INT)(PTYPE,PNAME)
#define M_PARAM_UINT(PTYPE,PNAME) _M_DEFER(_UINT)(PTYPE,PNAME)
#define M_PARAM_LONG(PTYPE,PNAME) _M_DEFER(_LONG)(PTYPE,PNAME)
#define M_PARAM_ULONG(PTYPE,PNAME) _M_DEFER(_ULONG)(PTYPE,PNAME)
#define M_PARAM_DOUBLE(PTYPE,PNAME) _M_DEFER(_DOUBLE)(PTYPE,PNAME)
#define M_PARAM_CHAR_PTR(PTYPE,PNAME) _M_DEFER(_CHAR_PTR)(PTYPE,PNAME)
#define M_PARAM_IN_PTR(PTYPE,PNAME) _M_DEFER(_IN_PTR)(PTYPE,PNAME)
#define M_PARAM_OUT_PTR(PTYPE,PNAME) _M_DEFER(_OUT_PTR)(PTYPE,PNAME)
#define M_PARAM_IN_TYPE_PTR(PTYPE,PNAME) _M_DEFER(_IN_TYPE_PTR)(PTYPE,PNAME)
#define M_PARAM_OUT_TYPE_PTR(PTYPE,PNAME) _M_DEFER(_OUT_TYPE_PTR)(PTYPE,PNAME)
#define M_PARAM_IGNORE(PTYPE,PNAME) _M_DEFER(_IGNORE)(PTYPE,PNAME)
#define M_PARAM_CUSTOM(PTYPE,PNAME,...) _M_DEFER(_CUSTOM)(PTYPE,PNAME,__VA_ARGS__)

#define _M_PARAM_TYPE(P) _M_PARAM_TYPE##P
#define _M_PARAM_TYPE_VOID(PTYPE,PNAME) PTYPE
#define _M_PARAM_TYPE_MS(PTYPE,PNAME) PTYPE
#define _M_PARAM_TYPE_MAC(PTYPE,PNAME) PTYPE
#define _M_PARAM_TYPE_BOOL(PTYPE,PNAME) PTYPE
#define _M_PARAM_TYPE_INT(PTYPE,PNAME) PTYPE
#define _M_PARAM_TYPE_UINT(PTYPE,PNAME) PTYPE
#define _M_PARAM_TYPE_LONG(PTYPE,PNAME) PTYPE
#define _M_PARAM_TYPE_ULONG(PTYPE,PNAME) PTYPE
#define _M_PARAM_TYPE_DOUBLE(PTYPE,PNAME) PTYPE
#define _M_PARAM_TYPE_CHAR_PTR(PTYPE,PNAME) PTYPE
#define _M_PARAM_TYPE_IN_PTR(PTYPE,PNAME) PTYPE
#define _M_PARAM_TYPE_OUT_PTR(PTYPE,PNAME) PTYPE
#define _M_PARAM_TYPE_IN_TYPE_PTR(PTYPE,PNAME) PTYPE
#define _M_PARAM_TYPE_OUT_TYPE_PTR(PTYPE,PNAME) PTYPE
#define _M_PARAM_TYPE_IGNORE(PTYPE,PNAME) PTYPE
#define _M_PARAM_TYPE_CUSTOM(PTYPE,PNAME,...) PTYPE

#define _M_PARAM_NAME(P) _M_PARAM_NAME##P
#define _M_PARAM_NAME_VOID(PTYPE,PNAME) /* N/A. */
#define _M_PARAM_NAME_MS(PTYPE,PNAME) PNAME
#define _M_PARAM_NAME_MAC(PTYPE,PNAME) PNAME
#define _M_PARAM_NAME_BOOL(PTYPE,PNAME) PNAME
#define _M_PARAM_NAME_INT(PTYPE,PNAME) PNAME
#define _M_PARAM_NAME_UINT(PTYPE,PNAME) PNAME
#define _M_PARAM_NAME_LONG(PTYPE,PNAME) PNAME
#define _M_PARAM_NAME_ULONG(PTYPE,PNAME) PNAME
#define _M_PARAM_NAME_DOUBLE(PTYPE,PNAME) PNAME
#define _M_PARAM_NAME_CHAR_PTR(PTYPE,PNAME) PNAME
#define _M_PARAM_NAME_IN_PTR(PTYPE,PNAME) PNAME
#define _M_PARAM_NAME_OUT_PTR(PTYPE,PNAME) PNAME
#define _M_PARAM_NAME_IN_TYPE_PTR(PTYPE,PNAME) PNAME
#define _M_PARAM_NAME_OUT_TYPE_PTR(PTYPE,PNAME) PNAME
#define _M_PARAM_NAME_IGNORE(PTYPE,PNAME) PNAME
#define _M_PARAM_NAME_CUSTOM(PTYPE,PNAME,...) PNAME

#define _M_PARAM_CALLBACK(P) _M_PARAM_CALLBACK##P
#define _M_PARAM_CALLBACK_VOID(PTYPE,PNAME) /* N/A. */
#define _M_PARAM_CALLBACK_MS(PTYPE,PNAME) /* N/A. */
#define _M_PARAM_CALLBACK_MAC(PTYPE,PNAME) /* N/A. */
#define _M_PARAM_CALLBACK_BOOL(PTYPE,PNAME) actualCall->withBoolParameters(_M_STR(PNAME),PNAME);
#define _M_PARAM_CALLBACK_INT(PTYPE,PNAME) actualCall->withIntParameters(_M_STR(PNAME),PNAME);
#define _M_PARAM_CALLBACK_UINT(PTYPE,PNAME) actualCall->withUnsignedIntParameters(_M_STR(PNAME),PNAME);
#define _M_PARAM_CALLBACK_LONG(PTYPE,PNAME) actualCall->withLongIntParameters(_M_STR(PNAME),PNAME);
#define _M_PARAM_CALLBACK_ULONG(PTYPE,PNAME) actualCall->withUnsignedLongIntParameters(_M_STR(PNAME),PNAME);
#define _M_PARAM_CALLBACK_DOUBLE(PTYPE,PNAME) actualCall->withDoubleParameters(_M_STR(PNAME),PNAME);
#define _M_PARAM_CALLBACK_CHAR_PTR(PTYPE,PNAME) actualCall->withStringParameters(_M_STR(PNAME),PNAME);
#define _M_PARAM_CALLBACK_IN_PTR(PTYPE,PNAME) actualCall->withPointerParameters(_M_STR(PNAME),PNAME);
#define _M_PARAM_CALLBACK_OUT_PTR(PTYPE,PNAME) actualCall->withOutputParameter(_M_STR(PNAME),PNAME);
#define _M_PARAM_CALLBACK_IN_TYPE_PTR(PTYPE,PNAME) actualCall->withParameterOfType(_M_STR(PTYPE),_M_STR(PNAME),PNAME);
#define _M_PARAM_CALLBACK_OUT_TYPE_PTR(PTYPE,PNAME) actualCall->withOutputParameterOfType(_M_STR(PTYPE),_M_STR(PNAME),PNAME);
#define _M_PARAM_CALLBACK_IGNORE(PTYPE,PNAME) /* N/A. */
#define _M_PARAM_CALLBACK_CUSTOM(PTYPE,PNAME,...) __VA_ARGS__

/*
 * Implementation Macros:
 */

#ifndef Assert
#define Assert(c,...)
#endif

#define _M_DECLARE_MOCK(RET,FUNC,...) \
typedef void (*MoxieCallFunc_##FUNC)(_M_PARAMS_MOCK_PROTOTYPE(FUNC,__VA_ARGS__)); \
typedef _M_RETURN_TYPE(RET) (*MoxieStubFunc_##FUNC)(_M_PARAMS_MOCK_PROTOTYPE(FUNC,__VA_ARGS__)); \
extern void __mReset_##FUNC(void); \
extern void __mEnable_##FUNC(void); \
extern void __mSetScope_##FUNC(const char *); \
extern void __mSetCallFunc_##FUNC(MoxieCallFunc_##FUNC); \
extern void __mSetStubFunc_##FUNC(MoxieStubFunc_##FUNC); \
extern _M_RETURN_TYPE(RET) __real_##FUNC(_M_PARAMS_PROTOTYPE(FUNC,__VA_ARGS__)); \
extern _M_RETURN_TYPE(RET) __wrap_##FUNC(_M_PARAMS_PROTOTYPE(FUNC,__VA_ARGS__)); \
extern void __mCallFunc_##FUNC(_M_PARAMS_MOCK_PROTOTYPE(FUNC,__VA_ARGS__)); \
extern _M_RETURN_TYPE(RET) __mStubFunc_##FUNC(_M_PARAMS_MOCK_PROTOTYPE(FUNC,__VA_ARGS__));

#define _M_IMPLEMENT_MOCK(RET,FUNC,...) \
_M_IMPLEMENT_MOCK_PREFACE(RET,FUNC,__VA_ARGS__) \
_M_IMPLEMENT_MOCK_REAL(RET,FUNC,__VA_ARGS__) \
_M_IMPLEMENT_MOCK_WRAP(RET,FUNC,__VA_ARGS__) \
_M_IMPLEMENT_MOCK_CALL(RET,FUNC,__VA_ARGS__) \
_M_IMPLEMENT_MOCK_STUB(RET,FUNC,__VA_ARGS__) \
_M_DYLD_INTERPOSE(FUNC)

#define _M_IMPLEMENT_MOCK_PREFACE(RET,FUNC,...) \
static MoxieState __mState_##FUNC = { \
    .mockFlag = 0, \
    .scope = "", \
    .callFunc = &__mCallFunc_##FUNC, \
    .stubFunc = &__mStubFunc_##FUNC, \
}; \
void __mReset_##FUNC(void) \
{ \
    __mState_##FUNC.mockFlag = 0; \
    __mState_##FUNC.scope = ""; \
    __mState_##FUNC.callFunc = &__mCallFunc_##FUNC; \
    __mState_##FUNC.stubFunc = &__mStubFunc_##FUNC; \
} \
void __mEnable_##FUNC(void) \
{ \
    __mState_##FUNC.mockFlag = 1; \
} \
void __mSetScope_##FUNC(const char *scope) \
{ \
    Assert(__mState_##FUNC.mockFlag); \
    __mState_##FUNC.scope = (char *)scope; \
} \
__mSetCallFunc_##FUNC(MoxieCallFunc_##FUNC callFunc) \
{ \
    Assert(__mState_##FUNC.mockFlag); \
    __mState_##FUNC.callFunc = callFunc; \
} \
__mSetStubFunc_##FUNC(MoxieStubFunc_##FUNC stubFunc) \
{ \
    Assert(__mState_##FUNC.mockFlag); \
    __mState_##FUNC.stubFunc = stubFunc; \
}

#ifdef __darwin__
#define _M_IMPLEMENT_MOCK_REAL(RET,FUNC,...) \
_M_RETURN_TYPE(RET) __real_##FUNC(_M_PARAMS_DECLARATION(FUNC,__VA_ARGS__)) \
{ \
    _M_RETURN(RET) FUNC(_M_PARAMS_CALL(FUNC,__VA_ARGS__)); \
}
#else
#define _M_IMPLEMENT_MOCK_REAL(RET,FUNC,...)
#endif

#define _M_IMPLEMENT_MOCK_WRAP(RET,FUNC,...) \
_M_RETURN_TYPE(RET) __wrap_##FUNC(_M_PARAMS_DECLARATION(FUNC,__VA_ARGS__)) \
{ \
    int mockFlag = __mState_##FUNC.mockFlag; \
    /* Fast Path: */ \
    if (!mockFlag) \
    { \
        _M_RETURN(RET) __real_##FUNC(_M_PARAMS_CALL(FUNC,__VA_ARGS__)); \
    } \
    /* Slow Path: */ \
    else \
    { \
        /* Resolve Scope: */ \
        char *scope = __mState_##FUNC.scope; \
        MockSupport_c *mockSupport = (scope[0] == '\0') ? mock_c() : mock_scope_c(scope); \
        /* Process Call: */ \
        MockActualCall_c *actualCall = mockSupport->actualCall(_M_STR(FUNC)); \
        /* Defer to callFunc: */ \
        MoxieCallFunc_##FUNC callFunc = __mState_##FUNC.callFunc; \
        if (callFunc != NULL) \
        { \
            (*callFunc)(_M_PARAMS_MOCK_CALL(FUNC,__VA_ARGS__)); \
        } \
        /* Process Return: */ \
        /* Defer to stubFunc: */ \
        MoxieStubFunc_##FUNC stubFunc = __mState_##FUNC.stubFunc; \
        if (stubFunc != NULL) \
        { \
            _M_RETURN(RET) (*stubFunc)(_M_PARAMS_MOCK_CALL(FUNC,__VA_ARGS__)); \
        } \
        /* Defer to realFunc: */ \
        else \
        { \
            _M_RETURN(RET) __real_##FUNC(_M_PARAMS_CALL(FUNC,__VA_ARGS__)); \ 
        } \
    } \
}

#define _M_IMPLEMENT_MOCK_CALL(RET,FUNC,...) \
void __mCallFunc_##FUNC(_M_PARAMS_MOCK_DECLARATION(FUNC,__VA_ARGS__)) \
{ \
    _M_PARAMS_FUNC_IMPL(FUNC,__VA_ARGS__); \
}

#define _M_IMPLEMENT_MOCK_STUB(RET,FUNC,...) \
_M_RETURN_TYPE(RET) __mStubFunc_##FUNC(_M_PARAMS_MOCK_DECLARATION(FUNC,__VA_ARGS__)) \
{ \
    if (actualCall->hasReturnValue()) \
    { \
        _M_RETURN_FUNC_IMPL(RET); \
    } \
    else \
    { \
        _M_RETURN(RET) __real_##FUNC(_M_PARAMS_CALL(FUNC,__VA_ARGS__));
    } \
}

#define _M_COMPARE_void(x) x

#define _M_RETURN(RET) _M_WHEN(_M_NOT_EQUAL(_M_RETURN_TYPE(RET),void))(return)

#define _M_RETURN_FUNC_IMPL(RET) _M_RETURN_CALLBACK(RET)

#define _M_PARAMS_MOCK_DECLARATION(FUNC,P,...) \
_M_IF(_M_EQUAL(_M_PARAM_TYPE(P),void))( \
_M_PARAMS(_M_PARAM_DECLARATION,FUNC,M_PARAM_MS,M_PARAM_MAC), \
_M_PARAMS(_M_PARAM_DECLARATION,FUNC,M_PARAM_MS,M_PARAM_MAC,P,##__VA_ARGS__))

#define _M_PARAMS_MOCK_PROTOTYPE(FUNC,P,...) \
_M_IF(_M_EQUAL(_M_PARAM_TYPE(P),void))( \
_M_PARAMS(_M_PARAM_PROTOTYPE,FUNC,M_PARAM_MS,M_PARAM_MAC), \
_M_PARAMS(_M_PARAM_PROTOTYPE,FUNC,M_PARAM_MS,M_PARAM_MAC,P,##__VA_ARGS__))

#define _M_PARAMS_MOCK_CALL(FUNC,P,...) \
_M_IF(_M_EQUAL(_M_PARAM_TYPE(P),void))( \
_M_PARAMS(_M_PARAM_CALL,FUNC,M_PARAM_MS,M_PARAM_MAC), \
_M_PARAMS(_M_PARAM_CALL,FUNC,M_PARAM_MS,M_PARAM_MAC,P,##__VA_ARGS__))

#define _M_PARAMS_DECLARATION(FUNC,...) _M_PARAMS(_M_PARAM_DECLARATION,FUNC,__VA_ARGS__)
#define _M_PARAM_DECLARATION_TAIL(FUNC,INDEX,P) _M_PARAM_TYPE(P) _M_PARAM_NAME(P)
#define _M_PARAM_DECLARATION(FUNC,INDEX,P) _M_PARAM_TYPE(P) _M_PARAM_NAME(P),

#define _M_PARAMS_PROTOTYPE(FUNC,...) _M_PARAMS(_M_PARAM_PROTOTYPE,FUNC,__VA_ARGS__)
#define _M_PARAM_PROTOTYPE_TAIL(FUNC,INDEX,P) _M_PARAM_TYPE(P)
#define _M_PARAM_PROTOTYPE(FUNC,INDEX,P) _M_PARAM_TYPE(P),

#define _M_PARAMS_CALL(FUNC,...) _M_PARAMS(_M_PARAM_CALL,FUNC,__VA_ARGS__)
#define _M_PARAM_CALL_TAIL(FUNC,INDEX,P) _M_PARAM_NAME(P)
#define _M_PARAM_CALL(FUNC,INDEX,P) _M_PARAM_NAME(P),

#define _M_PARAMS_FUNC_IMPL(FUNC,...) _M_PARAMS(_M_PARAM_FUNC_IMPL,FUNC,__VA_ARGS__)
#define _M_PARAM_FUNC_IMPL_TAIL(FUNC,INDEX,P) _M_PARAM_CALLBACK(P)
#define _M_PARAM_FUNC_IMPL(FUNC,INDEX,P) _M_PARAM_CALLBACK(P)

#define _M_PARAMS(CALLBACK,FUNC,...) _M_DCAT(_M_PARAMS_,_M_N_VA_ARGS(__VA_ARGS__))(CALLBACK,FUNC,0,__VA_ARGS__)
#define _M_PARAMS_0(CALLBACK,FUNC,INDEX,P,...) _M_PCAT(CALLBACK,_TAIL)(FUNC,INDEX,P)
#define _M_PARAMS_1(CALLBACK,FUNC,INDEX,P,...) CALLBACK(FUNC,INDEX,P)_M_PARAMS_0(CALLBACK,FUNC,_M_INC(INDEX),__VA_ARGS__)
#define _M_PARAMS_2(CALLBACK,FUNC,INDEX,P,...) CALLBACK(FUNC,INDEX,P)_M_PARAMS_1(CALLBACK,FUNC,_M_INC(INDEX),__VA_ARGS__)
#define _M_PARAMS_3(CALLBACK,FUNC,INDEX,P,...) CALLBACK(FUNC,INDEX,P)_M_PARAMS_2(CALLBACK,FUNC,_M_INC(INDEX),__VA_ARGS__)
#define _M_PARAMS_4(CALLBACK,FUNC,INDEX,P,...) CALLBACK(FUNC,INDEX,P)_M_PARAMS_3(CALLBACK,FUNC,_M_INC(INDEX),__VA_ARGS__)
#define _M_PARAMS_5(CALLBACK,FUNC,INDEX,P,...) CALLBACK(FUNC,INDEX,P)_M_PARAMS_4(CALLBACK,FUNC,_M_INC(INDEX),__VA_ARGS__)
#define _M_PARAMS_6(CALLBACK,FUNC,INDEX,P,...) CALLBACK(FUNC,INDEX,P)_M_PARAMS_5(CALLBACK,FUNC,_M_INC(INDEX),__VA_ARGS__)
#define _M_PARAMS_7(CALLBACK,FUNC,INDEX,P,...) CALLBACK(FUNC,INDEX,P)_M_PARAMS_6(CALLBACK,FUNC,_M_INC(INDEX),__VA_ARGS__)
#define _M_PARAMS_8(CALLBACK,FUNC,INDEX,P,...) CALLBACK(FUNC,INDEX,P)_M_PARAMS_7CALLBACK,FUNC,_M_INC(INDEX),__VA_ARGS__)
#define _M_PARAMS_9(CALLBACK,FUNC,INDEX,P,...) CALLBACK(FUNC,INDEX,P)_M_PARAMS_8(CALLBACK,FUNC,_M_INC(INDEX),__VA_ARGS__)
#define _M_PARAMS_10(CALLBACK,FUNC,INDEX,P,...) CALLBACK(FUNC,INDEX,P)_M_PARAMS_9(CALLBACK,FUNC,_M_INC(INDEX),__VA_ARGS__)
#define _M_PARAMS_11(CALLBACK,FUNC,INDEX,P,...) CALLBACK(FUNC,INDEX,P)_M_PARAMS_10(CALLBACK,FUNC,_M_INC(INDEX),__VA_ARGS__)
#define _M_PARAMS_12(CALLBACK,FUNC,INDEX,P,...) CALLBACK(FUNC,INDEX,P)_M_PARAMS_11(CALLBACK,FUNC,_M_INC(INDEX),__VA_ARGS__)
#define _M_PARAMS_13(CALLBACK,FUNC,INDEX,P,...) CALLBACK(FUNC,INDEX,P)_M_PARAMS_12(CALLBACK,FUNC,_M_INC(INDEX),__VA_ARGS__)
#define _M_PARAMS_14(CALLBACK,FUNC,INDEX,P,...) CALLBACK(FUNC,INDEX,P)_M_PARAMS_13(CALLBACK,FUNC,_M_INC(INDEX),__VA_ARGS__)
#define _M_PARAMS_15(CALLBACK,FUNC,INDEX,P,...) CALLBACK(FUNC,INDEX,P)_M_PARAMS_14(CALLBACK,FUNC,_M_INC(INDEX),__VA_ARGS__)
#define _M_PARAMS_16(CALLBACK,FUNC,INDEX,P,...) CALLBACK(FUNC,INDEX,P)_M_PARAMS_15(CALLBACK,FUNC,_M_INC(INDEX),__VA_ARGS__)
#define _M_PARAMS_17(CALLBACK,FUNC,INDEX,P,...) CALLBACK(FUNC,INDEX,P)_M_PARAMS_16(CALLBACK,FUNC,_M_INC(INDEX),__VA_ARGS__)
#define _M_PARAMS_18(CALLBACK,FUNC,INDEX,P,...) CALLBACK(FUNC,INDEX,P)_M_PARAMS_17(CALLBACK,FUNC,_M_INC(INDEX),__VA_ARGS__)
#define _M_PARAMS_19(CALLBACK,FUNC,INDEX,P,...) CALLBACK(FUNC,INDEX,P)_M_PARAMS_18(CALLBACK,FUNC,_M_INC(INDEX),__VA_ARGS__)
#define _M_PARAMS_20(CALLBACK,FUNC,INDEX,P,...) CALLBACK(FUNC,INDEX,P)_M_PARAMS_19(CALLBACK,FUNC,_M_INC(INDEX),__VA_ARGS__)
#define _M_PARAMS_21(CALLBACK,FUNC,INDEX,P,...) CALLBACK(FUNC,INDEX,P)_M_PARAMS_20(CALLBACK,FUNC,_M_INC(INDEX),__VA_ARGS__)
#define _M_PARAMS_22(CALLBACK,FUNC,INDEX,P,...) CALLBACK(FUNC,INDEX,P)_M_PARAMS_21(CALLBACK,FUNC,_M_INC(INDEX),__VA_ARGS__)
#define _M_PARAMS_23(CALLBACK,FUNC,INDEX,P,...) CALLBACK(FUNC,INDEX,P)_M_PARAMS_22(CALLBACK,FUNC,_M_INC(INDEX),__VA_ARGS__)
#define _M_PARAMS_24(CALLBACK,FUNC,INDEX,P,...) CALLBACK(FUNC,INDEX,P)_M_PARAMS_23(CALLBACK,FUNC,_M_INC(INDEX),__VA_ARGS__)
#define _M_PARAMS_25(CALLBACK,FUNC,INDEX,P,...) CALLBACK(FUNC,INDEX,P)_M_PARAMS_24(CALLBACK,FUNC,_M_INC(INDEX),__VA_ARGS__)
#define _M_PARAMS_26(CALLBACK,FUNC,INDEX,P,...) CALLBACK(FUNC,INDEX,P)_M_PARAMS_25(CALLBACK,FUNC,_M_INC(INDEX),__VA_ARGS__)
#define _M_PARAMS_27(CALLBACK,FUNC,INDEX,P,...) CALLBACK(FUNC,INDEX,P)_M_PARAMS_26(CALLBACK,FUNC,_M_INC(INDEX),__VA_ARGS__)
#define _M_PARAMS_28(CALLBACK,FUNC,INDEX,P,...) CALLBACK(FUNC,INDEX,P)_M_PARAMS_27(CALLBACK,FUNC,_M_INC(INDEX),__VA_ARGS__)
#define _M_PARAMS_29(CALLBACK,FUNC,INDEX,P,...) CALLBACK(FUNC,INDEX,P)_M_PARAMS_28(CALLBACK,FUNC,_M_INC(INDEX),__VA_ARGS__)
#define _M_PARAMS_30(CALLBACK,FUNC,INDEX,P,...) CALLBACK(FUNC,INDEX,P)_M_PARAMS_29(CALLBACK,FUNC,_M_INC(INDEX),__VA_ARGS__)
#define _M_PARAMS_31(CALLBACK,FUNC,INDEX,P,...) CALLBACK(FUNC,INDEX,P)_M_PARAMS_30(CALLBACK,FUNC,_M_INC(INDEX),__VA_ARGS__)

/*
 * macOS Macros:
 * macOS does not support `-Wl,--wrap=...` LDFLAGS, so DYLD Interposing is
 * required to declaratively have macOS replace the REAL with the WRAP.
 * @see https://opensource.apple.com/source/dyld/dyld-97.1/include/mach-o/dyld-interposing.h.auto.html
 */

#ifdef __darwin__
#include <stdint.h>

typedef struct __interpose {
    const void *__wrapFunc;
    const void *__realFunc;
} __interpose;

#define _M_DYLD_INTERPOSE(FUNC) \
static const __interpose __interpose_##FUNC \
__attribute__((user,section("__DATA,__interpose"))) = \
{ \
    .__wrapFunc = (const void *)((uintptr_t)(&(__wrap_##FUNC))), \
    .__realFunc = (const void *)((uintptr_t)(&(FUNC))), \
};
#else
#define _M_DYLD_INTERPOSE(FUNC)
#endif

/*
 * Meta Macros:
 * @see https://github.com/Hirrolot/awesome-c-preprocessor
 */

#define _M_STR(x) #x

#define _M_PCAT(x,...) x ## __VA_ARGS__
#define _M_DCAT(x,...) _M_PCAT(x,__VA_ARGS__)

#define _M_INC(x) _M_PCAT(_M_INC_,x)
#define _M_INC_0 1
#define _M_INC_1 2
#define _M_INC_2 3
#define _M_INC_3 4
#define _M_INC_4 5
#define _M_INC_5 6
#define _M_INC_6 7
#define _M_INC_7 8
#define _M_INC_8 9
#define _M_INC_9 10
#define _M_INC_10 11
#define _M_INC_11 12
#define _M_INC_12 13
#define _M_INC_13 14
#define _M_INC_14 15
#define _M_INC_15 16
#define _M_INC_16 17
#define _M_INC_17 18
#define _M_INC_18 19
#define _M_INC_19 20
#define _M_INC_20 21
#define _M_INC_21 22
#define _M_INC_22 23
#define _M_INC_23 24
#define _M_INC_24 25
#define _M_INC_25 26
#define _M_INC_26 27
#define _M_INC_27 28
#define _M_INC_28 29
#define _M_INC_29 30
#define _M_INC_30 31
#define _M_INC_31 31

#define _M_DEC(x) _M_PCAT(_M_DEC_,x)
#define _M_DEC_0 0
#define _M_DEC_1 0
#define _M_DEC_2 1
#define _M_DEC_3 2
#define _M_DEC_4 3
#define _M_DEC_5 4
#define _M_DEC_6 5
#define _M_DEC_7 6
#define _M_DEC_8 7
#define _M_DEC_9 8
#define _M_DEC_10 9
#define _M_DEC_11 10
#define _M_DEC_12 11
#define _M_DEC_13 12
#define _M_DEC_14 13
#define _M_DEC_15 14
#define _M_DEC_16 15
#define _M_DEC_17 16
#define _M_DEC_18 17
#define _M_DEC_19 18
#define _M_DEC_20 19
#define _M_DEC_21 20
#define _M_DEC_22 21
#define _M_DEC_23 22
#define _M_DEC_24 23
#define _M_DEC_25 24
#define _M_DEC_26 25
#define _M_DEC_27 26
#define _M_DEC_28 27
#define _M_DEC_29 28
#define _M_DEC_30 29
#define _M_DEC_31 30

#define _M_IGNORE(...)
#define _M_EMPTY()
#define _M_BLOCK(x) x _M_EMPTY()
#define _M_EXPAND(...) __VA_ARGS__
#define _M_DEFER(...) __VA_ARGS__ _M_BLOCK(_M_EMPTY)()

#define _M_EVAL(...) _M_EVAL1(_M_EVAL1(_M_EVAL1(__VA_ARGS__)))
#define _M_EVAL1(...) _M_EVAL2(_M_EVAL2(_M_EVAL2(__VA_ARGS__)))
#define _M_EVAL2(...) _M_EVAL3(_M_EVAL3(_M_EVAL3(__VA_ARGS__)))
#define _M_EVAL3(...) _M_EVAL4(_M_EVAL4(_M_EVAL4(__VA_ARGS__)))
#define _M_EVAL4(...) _M_EVAL5(_M_EVAL5(_M_EVAL5(__VA_ARGS__)))
#define _M_EVAL5(...) _M_EVAL6(_M_EVAL6(_M_EVAL6(__VA_ARGS__)))
#define _M_EVAL6(...) _M_EVAL7(_M_EVAL7(_M_EVAL7(__VA_ARGS__)))
#define _M_EVAL7(...) _M_EVAL8(_M_EVAL8(_M_EVAL8(__VA_ARGS__)))
#define _M_EVAL8(...) _M_EXPAND(_M_EXPAND(_M_EXPAND(__VA_ARGS__)))

#define _M_CHECK_N(x,n,...) n
#define _M_CHECK(...) _M_CHECK_N(__VA_ARGS__,0,)
#define _M_PROBE(x) x, 1,
#define _M_IS_PAREN_PROBE(...) _M_PROBE(~)
#define _M_IS_PAREN(x) _M_CHECK(_M_IS_PAREN_PROBE x)

#define _M_IF_(x) _M_PCAT(_M_IF_,x)
#define _M_IF_0(t,...) __VA_ARGS__
#define _M_IF_1(t,...) t

#define _M_NEGATE(x) _M_PCAT(_M_NEGATE,x)
#define _M_NEGATE_0 1
#define _M_NEGATE_1 0

#define _M_NOT(x) _M_CHECK(_M_PCAT(_M_NOT_,x))
#define _M_NOT_0 _M_PROBE(~)
#define _M_AND(x) _M_PCAT(_M_AND_,x)
#define _M_AND_0(y) 0
#define _M_AND_1(y) y
#define _M_OR(x) _M_PCAT(_M_OR_,x)
#define _M_OR_0(y) y
#define _M_OR_1(y) 1

#define _M_IS_TRUE(x) _M_NEGATE(_M_NOT(x))
#define _M_IF(p) _M_IF_(_M_IS_TRUE(p))

#define _M_WHEN(p) _M_IF(p)(_M_EXPAND,_M_IGNORE)
#define _M_UNLESS(p) _M_IF(p)(_M_IGNORE,_M_EXPAND)

#define _M_NEQ_COMPARABLE(x,y) _M_IS_PAREN(_M_COMPARE_##x(_M_COMPARE_##y)(()))
#define _M_IS_COMPARABLE(x) _M_IS_PAREN(_M_DCAT(_M_COMPARE_,x)(()))
#define _M_NOT_EQUAL(x,y) _M_IF_(_M_AND(_M_IS_COMPARABLE(x))(_M_IS_COMPARABLE(y)))(_M_NEQ_COMPARABLE,1 _M_IGNORE)(x,y)
#define _M_EQUAL(x,y) _M_NEGATE(_M_NOT_EQUAL(x,y))

#define _M_N_VA_ARGS_(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,N,...) N
#define _M_N_VA_ARGS(...) _M_N_VA_ARGS_(__VA_ARGS__,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0)

#endif // SRC_INCLUDE_MOXIE_H_