
/*
 * ruby-clangc ruby bindings for the C interface of Clang
 * Copyright (C) 2015  cedlemo <cedlemo@gmx.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "constants.h"
void init_clang_enums_to_constants(VALUE m_clang){
VALUE m_AvailabilityKind = rb_define_module_under(m_clang,"AvailabilityKind");
rb_define_const(m_AvailabilityKind,"AVAILABLE", INT2NUM(0));
rb_define_const(m_AvailabilityKind,"DEPRECATED", INT2NUM(1));
rb_define_const(m_AvailabilityKind,"NOT_AVAILABLE", INT2NUM(2));
rb_define_const(m_AvailabilityKind,"NOT_ACCESSIBLE", INT2NUM(3));

VALUE m_GlobalOptFlags = rb_define_module_under(m_clang,"GlobalOptFlags");
rb_define_const(m_GlobalOptFlags,"NONE", INT2NUM(0));
rb_define_const(m_GlobalOptFlags,"THREAD_BACKGROUND_PRIORITY_FOR_INDEXING", INT2NUM(1));
rb_define_const(m_GlobalOptFlags,"THREAD_BACKGROUND_PRIORITY_FOR_EDITING", INT2NUM(2));
rb_define_const(m_GlobalOptFlags,"THREAD_BACKGROUND_PRIORITY_FOR_ALL", INT2NUM(3));

VALUE m_DiagnosticSeverity = rb_define_module_under(m_clang,"DiagnosticSeverity");
rb_define_const(m_DiagnosticSeverity,"IGNORED", INT2NUM(0));
rb_define_const(m_DiagnosticSeverity,"NOTE", INT2NUM(1));
rb_define_const(m_DiagnosticSeverity,"WARNING", INT2NUM(2));
rb_define_const(m_DiagnosticSeverity,"ERROR", INT2NUM(3));
rb_define_const(m_DiagnosticSeverity,"FATAL", INT2NUM(4));

VALUE m_LoadDiag_Error = rb_define_module_under(m_clang,"LoadDiag_Error");
rb_define_const(m_LoadDiag_Error,"NONE", INT2NUM(0));
rb_define_const(m_LoadDiag_Error,"UNKNOWN", INT2NUM(1));
rb_define_const(m_LoadDiag_Error,"CANNOT_LOAD", INT2NUM(2));
rb_define_const(m_LoadDiag_Error,"INVALID_FILE", INT2NUM(3));

VALUE m_DiagnosticDisplayOptions = rb_define_module_under(m_clang,"DiagnosticDisplayOptions");
rb_define_const(m_DiagnosticDisplayOptions,"DISPLAY_SOURCE_LOCATION", INT2NUM(1));
rb_define_const(m_DiagnosticDisplayOptions,"DISPLAY_COLUMN", INT2NUM(2));
rb_define_const(m_DiagnosticDisplayOptions,"DISPLAY_SOURCE_RANGES", INT2NUM(4));
rb_define_const(m_DiagnosticDisplayOptions,"DISPLAY_OPTION", INT2NUM(8));
rb_define_const(m_DiagnosticDisplayOptions,"DISPLAY_CATEGORY_ID", INT2NUM(16));
rb_define_const(m_DiagnosticDisplayOptions,"DISPLAY_CATEGORY_NAME", INT2NUM(32));

VALUE m_TranslationUnit_Flags = rb_define_module_under(m_clang,"TranslationUnit_Flags");
rb_define_const(m_TranslationUnit_Flags,"NONE", INT2NUM(0));
rb_define_const(m_TranslationUnit_Flags,"DETAILED_PREPROCESSING_RECORD", INT2NUM(1));
rb_define_const(m_TranslationUnit_Flags,"INCOMPLETE", INT2NUM(2));
rb_define_const(m_TranslationUnit_Flags,"PRECOMPILED_PREAMBLE", INT2NUM(4));
rb_define_const(m_TranslationUnit_Flags,"CACHE_COMPLETION_RESULTS", INT2NUM(8));
rb_define_const(m_TranslationUnit_Flags,"FOR_SERIALIZATION", INT2NUM(16));
rb_define_const(m_TranslationUnit_Flags,"CXX_CHAINED_PCH", INT2NUM(32));
rb_define_const(m_TranslationUnit_Flags,"SKIP_FUNCTION_BODIES", INT2NUM(64));
rb_define_const(m_TranslationUnit_Flags,"INCLUDE_BRIEF_COMMENTS_IN_CODE_COMPLETION", INT2NUM(128));

VALUE m_SaveTranslationUnit_Flags = rb_define_module_under(m_clang,"SaveTranslationUnit_Flags");
rb_define_const(m_SaveTranslationUnit_Flags,"NONE", INT2NUM(0));

VALUE m_SaveError = rb_define_module_under(m_clang,"SaveError");
rb_define_const(m_SaveError,"NONE", INT2NUM(0));
rb_define_const(m_SaveError,"UNKNOWN", INT2NUM(1));
rb_define_const(m_SaveError,"TRANSLATION_ERRORS", INT2NUM(2));
rb_define_const(m_SaveError,"INVALID_TU", INT2NUM(3));

VALUE m_Reparse_Flags = rb_define_module_under(m_clang,"Reparse_Flags");
rb_define_const(m_Reparse_Flags,"NONE", INT2NUM(0));

VALUE m_TUResourceUsageKind = rb_define_module_under(m_clang,"TUResourceUsageKind");
rb_define_const(m_TUResourceUsageKind,"AST", INT2NUM(1));
rb_define_const(m_TUResourceUsageKind,"IDENTIFIERS", INT2NUM(2));
rb_define_const(m_TUResourceUsageKind,"SELECTORS", INT2NUM(3));
rb_define_const(m_TUResourceUsageKind,"GLOBAL_COMPLETION_RESULTS", INT2NUM(4));
rb_define_const(m_TUResourceUsageKind,"SOURCE_MANAGER_CONTENT_CACHE", INT2NUM(5));
rb_define_const(m_TUResourceUsageKind,"AST_SIDE_TABLES", INT2NUM(6));
rb_define_const(m_TUResourceUsageKind,"SOURCE_MANAGER_MEMBUFFER_MALLOC", INT2NUM(7));
rb_define_const(m_TUResourceUsageKind,"SOURCE_MANAGER_MEMBUFFER_M_MAP", INT2NUM(8));
rb_define_const(m_TUResourceUsageKind,"EXTERNAL_AST_SOURCE_MEMBUFFER_MALLOC", INT2NUM(9));
rb_define_const(m_TUResourceUsageKind,"EXTERNAL_AST_SOURCE_MEMBUFFER_M_MAP", INT2NUM(10));
rb_define_const(m_TUResourceUsageKind,"PREPROCESSOR", INT2NUM(11));
rb_define_const(m_TUResourceUsageKind,"PREPROCESSING_RECORD", INT2NUM(12));
rb_define_const(m_TUResourceUsageKind,"SOURCE_MANAGER_DATA_STRUCTURES", INT2NUM(13));
rb_define_const(m_TUResourceUsageKind,"PREPROCESSOR_HEADER_SEARCH", INT2NUM(14));
rb_define_const(m_TUResourceUsageKind,"MEMORY_IN_BYTES_BEGIN", INT2NUM(1));
rb_define_const(m_TUResourceUsageKind,"MEMORY_IN_BYTES_END", INT2NUM(14));
rb_define_const(m_TUResourceUsageKind,"FIRST", INT2NUM(1));
rb_define_const(m_TUResourceUsageKind,"LAST", INT2NUM(14));

VALUE m_CursorKind = rb_define_module_under(m_clang,"CursorKind");
rb_define_const(m_CursorKind,"UNEXPOSED_DECL", INT2NUM(1));
rb_define_const(m_CursorKind,"STRUCT_DECL", INT2NUM(2));
rb_define_const(m_CursorKind,"UNION_DECL", INT2NUM(3));
rb_define_const(m_CursorKind,"CLASS_DECL", INT2NUM(4));
rb_define_const(m_CursorKind,"ENUM_DECL", INT2NUM(5));
rb_define_const(m_CursorKind,"FIELD_DECL", INT2NUM(6));
rb_define_const(m_CursorKind,"ENUM_CONSTANT_DECL", INT2NUM(7));
rb_define_const(m_CursorKind,"FUNCTION_DECL", INT2NUM(8));
rb_define_const(m_CursorKind,"VAR_DECL", INT2NUM(9));
rb_define_const(m_CursorKind,"PARM_DECL", INT2NUM(10));
rb_define_const(m_CursorKind,"OBJ_C_INTERFACE_DECL", INT2NUM(11));
rb_define_const(m_CursorKind,"OBJ_C_CATEGORY_DECL", INT2NUM(12));
rb_define_const(m_CursorKind,"OBJ_C_PROTOCOL_DECL", INT2NUM(13));
rb_define_const(m_CursorKind,"OBJ_C_PROPERTY_DECL", INT2NUM(14));
rb_define_const(m_CursorKind,"OBJ_C_IVAR_DECL", INT2NUM(15));
rb_define_const(m_CursorKind,"OBJ_C_INSTANCE_METHOD_DECL", INT2NUM(16));
rb_define_const(m_CursorKind,"OBJ_C_CLASS_METHOD_DECL", INT2NUM(17));
rb_define_const(m_CursorKind,"OBJ_C_IMPLEMENTATION_DECL", INT2NUM(18));
rb_define_const(m_CursorKind,"OBJ_C_CATEGORY_IMPL_DECL", INT2NUM(19));
rb_define_const(m_CursorKind,"TYPEDEF_DECL", INT2NUM(20));
rb_define_const(m_CursorKind,"CXX_METHOD", INT2NUM(21));
rb_define_const(m_CursorKind,"NAMESPACE", INT2NUM(22));
rb_define_const(m_CursorKind,"LINKAGE_SPEC", INT2NUM(23));
rb_define_const(m_CursorKind,"CONSTRUCTOR", INT2NUM(24));
rb_define_const(m_CursorKind,"DESTRUCTOR", INT2NUM(25));
rb_define_const(m_CursorKind,"CONVERSION_FUNCTION", INT2NUM(26));
rb_define_const(m_CursorKind,"TEMPLATE_TYPE_PARAMETER", INT2NUM(27));
rb_define_const(m_CursorKind,"NON_TYPE_TEMPLATE_PARAMETER", INT2NUM(28));
rb_define_const(m_CursorKind,"TEMPLATE_TEMPLATE_PARAMETER", INT2NUM(29));
rb_define_const(m_CursorKind,"FUNCTION_TEMPLATE", INT2NUM(30));
rb_define_const(m_CursorKind,"CLASS_TEMPLATE", INT2NUM(31));
rb_define_const(m_CursorKind,"CLASS_TEMPLATE_PARTIAL_SPECIALIZATION", INT2NUM(32));
rb_define_const(m_CursorKind,"NAMESPACE_ALIAS", INT2NUM(33));
rb_define_const(m_CursorKind,"USING_DIRECTIVE", INT2NUM(34));
rb_define_const(m_CursorKind,"USING_DECLARATION", INT2NUM(35));
rb_define_const(m_CursorKind,"TYPE_ALIAS_DECL", INT2NUM(36));
rb_define_const(m_CursorKind,"OBJ_C_SYNTHESIZE_DECL", INT2NUM(37));
rb_define_const(m_CursorKind,"OBJ_C_DYNAMIC_DECL", INT2NUM(38));
rb_define_const(m_CursorKind,"CXX_ACCESS_SPECIFIER", INT2NUM(39));
rb_define_const(m_CursorKind,"FIRST_DECL", INT2NUM(1));
rb_define_const(m_CursorKind,"LAST_DECL", INT2NUM(39));
rb_define_const(m_CursorKind,"FIRST_REF", INT2NUM(40));
rb_define_const(m_CursorKind,"OBJ_C_SUPER_CLASS_REF", INT2NUM(40));
rb_define_const(m_CursorKind,"OBJ_C_PROTOCOL_REF", INT2NUM(41));
rb_define_const(m_CursorKind,"OBJ_C_CLASS_REF", INT2NUM(42));
rb_define_const(m_CursorKind,"TYPE_REF", INT2NUM(43));
rb_define_const(m_CursorKind,"CXX_BASE_SPECIFIER", INT2NUM(44));
rb_define_const(m_CursorKind,"TEMPLATE_REF", INT2NUM(45));
rb_define_const(m_CursorKind,"NAMESPACE_REF", INT2NUM(46));
rb_define_const(m_CursorKind,"MEMBER_REF", INT2NUM(47));
rb_define_const(m_CursorKind,"LABEL_REF", INT2NUM(48));
rb_define_const(m_CursorKind,"OVERLOADED_DECL_REF", INT2NUM(49));
rb_define_const(m_CursorKind,"VARIABLE_REF", INT2NUM(50));
rb_define_const(m_CursorKind,"LAST_REF", INT2NUM(50));
rb_define_const(m_CursorKind,"FIRST_INVALID", INT2NUM(70));
rb_define_const(m_CursorKind,"INVALID_FILE", INT2NUM(70));
rb_define_const(m_CursorKind,"NO_DECL_FOUND", INT2NUM(71));
rb_define_const(m_CursorKind,"NOT_IMPLEMENTED", INT2NUM(72));
rb_define_const(m_CursorKind,"INVALID_CODE", INT2NUM(73));
rb_define_const(m_CursorKind,"LAST_INVALID", INT2NUM(73));
rb_define_const(m_CursorKind,"FIRST_EXPR", INT2NUM(100));
rb_define_const(m_CursorKind,"UNEXPOSED_EXPR", INT2NUM(100));
rb_define_const(m_CursorKind,"DECL_REF_EXPR", INT2NUM(101));
rb_define_const(m_CursorKind,"MEMBER_REF_EXPR", INT2NUM(102));
rb_define_const(m_CursorKind,"CALL_EXPR", INT2NUM(103));
rb_define_const(m_CursorKind,"OBJ_C_MESSAGE_EXPR", INT2NUM(104));
rb_define_const(m_CursorKind,"BLOCK_EXPR", INT2NUM(105));
rb_define_const(m_CursorKind,"INTEGER_LITERAL", INT2NUM(106));
rb_define_const(m_CursorKind,"FLOATING_LITERAL", INT2NUM(107));
rb_define_const(m_CursorKind,"IMAGINARY_LITERAL", INT2NUM(108));
rb_define_const(m_CursorKind,"STRING_LITERAL", INT2NUM(109));
rb_define_const(m_CursorKind,"CHARACTER_LITERAL", INT2NUM(110));
rb_define_const(m_CursorKind,"PAREN_EXPR", INT2NUM(111));
rb_define_const(m_CursorKind,"UNARY_OPERATOR", INT2NUM(112));
rb_define_const(m_CursorKind,"ARRAY_SUBSCRIPT_EXPR", INT2NUM(113));
rb_define_const(m_CursorKind,"BINARY_OPERATOR", INT2NUM(114));
rb_define_const(m_CursorKind,"COMPOUND_ASSIGN_OPERATOR", INT2NUM(115));
rb_define_const(m_CursorKind,"CONDITIONAL_OPERATOR", INT2NUM(116));
rb_define_const(m_CursorKind,"C_STYLE_CAST_EXPR", INT2NUM(117));
rb_define_const(m_CursorKind,"COMPOUND_LITERAL_EXPR", INT2NUM(118));
rb_define_const(m_CursorKind,"INIT_LIST_EXPR", INT2NUM(119));
rb_define_const(m_CursorKind,"ADDR_LABEL_EXPR", INT2NUM(120));
rb_define_const(m_CursorKind,"STMT_EXPR", INT2NUM(121));
rb_define_const(m_CursorKind,"GENERIC_SELECTION_EXPR", INT2NUM(122));
rb_define_const(m_CursorKind,"GNU_NULL_EXPR", INT2NUM(123));
rb_define_const(m_CursorKind,"CXX_STATIC_CAST_EXPR", INT2NUM(124));
rb_define_const(m_CursorKind,"CXX_DYNAMIC_CAST_EXPR", INT2NUM(125));
rb_define_const(m_CursorKind,"CXX_REINTERPRET_CAST_EXPR", INT2NUM(126));
rb_define_const(m_CursorKind,"CXX_CONST_CAST_EXPR", INT2NUM(127));
rb_define_const(m_CursorKind,"CXX_FUNCTIONAL_CAST_EXPR", INT2NUM(128));
rb_define_const(m_CursorKind,"CXX_TYPEID_EXPR", INT2NUM(129));
rb_define_const(m_CursorKind,"CXX_BOOL_LITERAL_EXPR", INT2NUM(130));
rb_define_const(m_CursorKind,"CXX_NULL_PTR_LITERAL_EXPR", INT2NUM(131));
rb_define_const(m_CursorKind,"CXX_THIS_EXPR", INT2NUM(132));
rb_define_const(m_CursorKind,"CXX_THROW_EXPR", INT2NUM(133));
rb_define_const(m_CursorKind,"CXX_NEW_EXPR", INT2NUM(134));
rb_define_const(m_CursorKind,"CXX_DELETE_EXPR", INT2NUM(135));
rb_define_const(m_CursorKind,"UNARY_EXPR", INT2NUM(136));
rb_define_const(m_CursorKind,"OBJ_C_STRING_LITERAL", INT2NUM(137));
rb_define_const(m_CursorKind,"OBJ_C_ENCODE_EXPR", INT2NUM(138));
rb_define_const(m_CursorKind,"OBJ_C_SELECTOR_EXPR", INT2NUM(139));
rb_define_const(m_CursorKind,"OBJ_C_PROTOCOL_EXPR", INT2NUM(140));
rb_define_const(m_CursorKind,"OBJ_C_BRIDGED_CAST_EXPR", INT2NUM(141));
rb_define_const(m_CursorKind,"PACK_EXPANSION_EXPR", INT2NUM(142));
rb_define_const(m_CursorKind,"SIZE_OF_PACK_EXPR", INT2NUM(143));
rb_define_const(m_CursorKind,"LAMBDA_EXPR", INT2NUM(144));
rb_define_const(m_CursorKind,"OBJ_C_BOOL_LITERAL_EXPR", INT2NUM(145));
rb_define_const(m_CursorKind,"OBJ_C_SELF_EXPR", INT2NUM(146));
rb_define_const(m_CursorKind,"LAST_EXPR", INT2NUM(146));
rb_define_const(m_CursorKind,"FIRST_STMT", INT2NUM(200));
rb_define_const(m_CursorKind,"UNEXPOSED_STMT", INT2NUM(200));
rb_define_const(m_CursorKind,"LABEL_STMT", INT2NUM(201));
rb_define_const(m_CursorKind,"COMPOUND_STMT", INT2NUM(202));
rb_define_const(m_CursorKind,"CASE_STMT", INT2NUM(203));
rb_define_const(m_CursorKind,"DEFAULT_STMT", INT2NUM(204));
rb_define_const(m_CursorKind,"IF_STMT", INT2NUM(205));
rb_define_const(m_CursorKind,"SWITCH_STMT", INT2NUM(206));
rb_define_const(m_CursorKind,"WHILE_STMT", INT2NUM(207));
rb_define_const(m_CursorKind,"DO_STMT", INT2NUM(208));
rb_define_const(m_CursorKind,"FOR_STMT", INT2NUM(209));
rb_define_const(m_CursorKind,"GOTO_STMT", INT2NUM(210));
rb_define_const(m_CursorKind,"INDIRECT_GOTO_STMT", INT2NUM(211));
rb_define_const(m_CursorKind,"CONTINUE_STMT", INT2NUM(212));
rb_define_const(m_CursorKind,"BREAK_STMT", INT2NUM(213));
rb_define_const(m_CursorKind,"RETURN_STMT", INT2NUM(214));
rb_define_const(m_CursorKind,"GCC_ASM_STMT", INT2NUM(215));
rb_define_const(m_CursorKind,"ASM_STMT", INT2NUM(215));
rb_define_const(m_CursorKind,"OBJ_C_AT_TRY_STMT", INT2NUM(216));
rb_define_const(m_CursorKind,"OBJ_C_AT_CATCH_STMT", INT2NUM(217));
rb_define_const(m_CursorKind,"OBJ_C_AT_FINALLY_STMT", INT2NUM(218));
rb_define_const(m_CursorKind,"OBJ_C_AT_THROW_STMT", INT2NUM(219));
rb_define_const(m_CursorKind,"OBJ_C_AT_SYNCHRONIZED_STMT", INT2NUM(220));
rb_define_const(m_CursorKind,"OBJ_C_AUTORELEASE_POOL_STMT", INT2NUM(221));
rb_define_const(m_CursorKind,"OBJ_C_FOR_COLLECTION_STMT", INT2NUM(222));
rb_define_const(m_CursorKind,"CXX_CATCH_STMT", INT2NUM(223));
rb_define_const(m_CursorKind,"CXX_TRY_STMT", INT2NUM(224));
rb_define_const(m_CursorKind,"CXX_FOR_RANGE_STMT", INT2NUM(225));
rb_define_const(m_CursorKind,"SEH_TRY_STMT", INT2NUM(226));
rb_define_const(m_CursorKind,"SEH_EXCEPT_STMT", INT2NUM(227));
rb_define_const(m_CursorKind,"SEH_FINALLY_STMT", INT2NUM(228));
rb_define_const(m_CursorKind,"MS_ASM_STMT", INT2NUM(229));
rb_define_const(m_CursorKind,"NULL_STMT", INT2NUM(230));
rb_define_const(m_CursorKind,"DECL_STMT", INT2NUM(231));
rb_define_const(m_CursorKind,"OMP_PARALLEL_DIRECTIVE", INT2NUM(232));
rb_define_const(m_CursorKind,"OMP_SIMD_DIRECTIVE", INT2NUM(233));
rb_define_const(m_CursorKind,"OMP_FOR_DIRECTIVE", INT2NUM(234));
rb_define_const(m_CursorKind,"OMP_SECTIONS_DIRECTIVE", INT2NUM(235));
rb_define_const(m_CursorKind,"OMP_SECTION_DIRECTIVE", INT2NUM(236));
rb_define_const(m_CursorKind,"OMP_SINGLE_DIRECTIVE", INT2NUM(237));
rb_define_const(m_CursorKind,"OMP_PARALLEL_FOR_DIRECTIVE", INT2NUM(238));
rb_define_const(m_CursorKind,"OMP_PARALLEL_SECTIONS_DIRECTIVE", INT2NUM(239));
rb_define_const(m_CursorKind,"OMP_TASK_DIRECTIVE", INT2NUM(240));
rb_define_const(m_CursorKind,"OMP_MASTER_DIRECTIVE", INT2NUM(241));
rb_define_const(m_CursorKind,"OMP_CRITICAL_DIRECTIVE", INT2NUM(242));
rb_define_const(m_CursorKind,"OMP_TASKYIELD_DIRECTIVE", INT2NUM(243));
rb_define_const(m_CursorKind,"OMP_BARRIER_DIRECTIVE", INT2NUM(244));
rb_define_const(m_CursorKind,"OMP_TASKWAIT_DIRECTIVE", INT2NUM(245));
rb_define_const(m_CursorKind,"OMP_FLUSH_DIRECTIVE", INT2NUM(246));
rb_define_const(m_CursorKind,"SEH_LEAVE_STMT", INT2NUM(247));
rb_define_const(m_CursorKind,"OMP_ORDERED_DIRECTIVE", INT2NUM(248));
rb_define_const(m_CursorKind,"OMP_ATOMIC_DIRECTIVE", INT2NUM(249));
rb_define_const(m_CursorKind,"OMP_FOR_SIMD_DIRECTIVE", INT2NUM(250));
rb_define_const(m_CursorKind,"OMP_PARALLEL_FOR_SIMD_DIRECTIVE", INT2NUM(251));
rb_define_const(m_CursorKind,"OMP_TARGET_DIRECTIVE", INT2NUM(252));
rb_define_const(m_CursorKind,"OMP_TEAMS_DIRECTIVE", INT2NUM(253));
rb_define_const(m_CursorKind,"OMP_TASKGROUP_DIRECTIVE", INT2NUM(254));
rb_define_const(m_CursorKind,"OMP_CANCELLATION_POINT_DIRECTIVE", INT2NUM(255));
rb_define_const(m_CursorKind,"OMP_CANCEL_DIRECTIVE", INT2NUM(256));
rb_define_const(m_CursorKind,"LAST_STMT", INT2NUM(256));
rb_define_const(m_CursorKind,"TRANSLATION_UNIT", INT2NUM(300));
rb_define_const(m_CursorKind,"FIRST_ATTR", INT2NUM(400));
rb_define_const(m_CursorKind,"UNEXPOSED_ATTR", INT2NUM(400));
rb_define_const(m_CursorKind,"IB_ACTION_ATTR", INT2NUM(401));
rb_define_const(m_CursorKind,"IB_OUTLET_ATTR", INT2NUM(402));
rb_define_const(m_CursorKind,"IB_OUTLET_COLLECTION_ATTR", INT2NUM(403));
rb_define_const(m_CursorKind,"CXX_FINAL_ATTR", INT2NUM(404));
rb_define_const(m_CursorKind,"CXX_OVERRIDE_ATTR", INT2NUM(405));
rb_define_const(m_CursorKind,"ANNOTATE_ATTR", INT2NUM(406));
rb_define_const(m_CursorKind,"ASM_LABEL_ATTR", INT2NUM(407));
rb_define_const(m_CursorKind,"PACKED_ATTR", INT2NUM(408));
rb_define_const(m_CursorKind,"PURE_ATTR", INT2NUM(409));
rb_define_const(m_CursorKind,"CONST_ATTR", INT2NUM(410));
rb_define_const(m_CursorKind,"NO_DUPLICATE_ATTR", INT2NUM(411));
rb_define_const(m_CursorKind,"CUDA_CONSTANT_ATTR", INT2NUM(412));
rb_define_const(m_CursorKind,"CUDA_DEVICE_ATTR", INT2NUM(413));
rb_define_const(m_CursorKind,"CUDA_GLOBAL_ATTR", INT2NUM(414));
rb_define_const(m_CursorKind,"CUDA_HOST_ATTR", INT2NUM(415));
rb_define_const(m_CursorKind,"CUDA_SHARED_ATTR", INT2NUM(416));
rb_define_const(m_CursorKind,"LAST_ATTR", INT2NUM(416));
rb_define_const(m_CursorKind,"PREPROCESSING_DIRECTIVE", INT2NUM(500));
rb_define_const(m_CursorKind,"MACRO_DEFINITION", INT2NUM(501));
rb_define_const(m_CursorKind,"MACRO_EXPANSION", INT2NUM(502));
rb_define_const(m_CursorKind,"MACRO_INSTANTIATION", INT2NUM(502));
rb_define_const(m_CursorKind,"INCLUSION_DIRECTIVE", INT2NUM(503));
rb_define_const(m_CursorKind,"FIRST_PREPROCESSING", INT2NUM(500));
rb_define_const(m_CursorKind,"LAST_PREPROCESSING", INT2NUM(503));
rb_define_const(m_CursorKind,"MODULE_IMPORT_DECL", INT2NUM(600));
rb_define_const(m_CursorKind,"FIRST_EXTRA_DECL", INT2NUM(600));
rb_define_const(m_CursorKind,"LAST_EXTRA_DECL", INT2NUM(600));
rb_define_const(m_CursorKind,"OVERLOAD_CANDIDATE", INT2NUM(700));

VALUE m_LinkageKind = rb_define_module_under(m_clang,"LinkageKind");
rb_define_const(m_LinkageKind,"INVALID", INT2NUM(0));
rb_define_const(m_LinkageKind,"NO_LINKAGE", INT2NUM(1));
rb_define_const(m_LinkageKind,"INTERNAL", INT2NUM(2));
rb_define_const(m_LinkageKind,"UNIQUE_EXTERNAL", INT2NUM(3));
rb_define_const(m_LinkageKind,"EXTERNAL", INT2NUM(4));

VALUE m_LanguageKind = rb_define_module_under(m_clang,"LanguageKind");
rb_define_const(m_LanguageKind,"INVALID", INT2NUM(0));
rb_define_const(m_LanguageKind,"C", INT2NUM(1));
rb_define_const(m_LanguageKind,"OBJ_C", INT2NUM(2));
rb_define_const(m_LanguageKind,"C_PLUS_PLUS", INT2NUM(3));

VALUE m_TypeKind = rb_define_module_under(m_clang,"TypeKind");
rb_define_const(m_TypeKind,"INVALID", INT2NUM(0));
rb_define_const(m_TypeKind,"UNEXPOSED", INT2NUM(1));
rb_define_const(m_TypeKind,"VOID", INT2NUM(2));
rb_define_const(m_TypeKind,"BOOL", INT2NUM(3));
rb_define_const(m_TypeKind,"CHAR_U", INT2NUM(4));
rb_define_const(m_TypeKind,"U_CHAR", INT2NUM(5));
rb_define_const(m_TypeKind,"CHAR16", INT2NUM(6));
rb_define_const(m_TypeKind,"CHAR32", INT2NUM(7));
rb_define_const(m_TypeKind,"U_SHORT", INT2NUM(8));
rb_define_const(m_TypeKind,"U_INT", INT2NUM(9));
rb_define_const(m_TypeKind,"U_LONG", INT2NUM(10));
rb_define_const(m_TypeKind,"U_LONG_LONG", INT2NUM(11));
rb_define_const(m_TypeKind,"U_INT128", INT2NUM(12));
rb_define_const(m_TypeKind,"CHAR_S", INT2NUM(13));
rb_define_const(m_TypeKind,"S_CHAR", INT2NUM(14));
rb_define_const(m_TypeKind,"W_CHAR", INT2NUM(15));
rb_define_const(m_TypeKind,"SHORT", INT2NUM(16));
rb_define_const(m_TypeKind,"INT", INT2NUM(17));
rb_define_const(m_TypeKind,"LONG", INT2NUM(18));
rb_define_const(m_TypeKind,"LONG_LONG", INT2NUM(19));
rb_define_const(m_TypeKind,"INT128", INT2NUM(20));
rb_define_const(m_TypeKind,"FLOAT", INT2NUM(21));
rb_define_const(m_TypeKind,"DOUBLE", INT2NUM(22));
rb_define_const(m_TypeKind,"LONG_DOUBLE", INT2NUM(23));
rb_define_const(m_TypeKind,"NULL_PTR", INT2NUM(24));
rb_define_const(m_TypeKind,"OVERLOAD", INT2NUM(25));
rb_define_const(m_TypeKind,"DEPENDENT", INT2NUM(26));
rb_define_const(m_TypeKind,"OBJ_C_ID", INT2NUM(27));
rb_define_const(m_TypeKind,"OBJ_C_CLASS", INT2NUM(28));
rb_define_const(m_TypeKind,"OBJ_C_SEL", INT2NUM(29));
rb_define_const(m_TypeKind,"FIRST_BUILTIN", INT2NUM(2));
rb_define_const(m_TypeKind,"LAST_BUILTIN", INT2NUM(29));
rb_define_const(m_TypeKind,"COMPLEX", INT2NUM(100));
rb_define_const(m_TypeKind,"POINTER", INT2NUM(101));
rb_define_const(m_TypeKind,"BLOCK_POINTER", INT2NUM(102));
rb_define_const(m_TypeKind,"L_VALUE_REFERENCE", INT2NUM(103));
rb_define_const(m_TypeKind,"R_VALUE_REFERENCE", INT2NUM(104));
rb_define_const(m_TypeKind,"RECORD", INT2NUM(105));
rb_define_const(m_TypeKind,"ENUM", INT2NUM(106));
rb_define_const(m_TypeKind,"TYPEDEF", INT2NUM(107));
rb_define_const(m_TypeKind,"OBJ_C_INTERFACE", INT2NUM(108));
rb_define_const(m_TypeKind,"OBJ_C_OBJECT_POINTER", INT2NUM(109));
rb_define_const(m_TypeKind,"FUNCTION_NO_PROTO", INT2NUM(110));
rb_define_const(m_TypeKind,"FUNCTION_PROTO", INT2NUM(111));
rb_define_const(m_TypeKind,"CONSTANT_ARRAY", INT2NUM(112));
rb_define_const(m_TypeKind,"VECTOR", INT2NUM(113));
rb_define_const(m_TypeKind,"INCOMPLETE_ARRAY", INT2NUM(114));
rb_define_const(m_TypeKind,"VARIABLE_ARRAY", INT2NUM(115));
rb_define_const(m_TypeKind,"DEPENDENT_SIZED_ARRAY", INT2NUM(116));
rb_define_const(m_TypeKind,"MEMBER_POINTER", INT2NUM(117));

VALUE m_CallingConv = rb_define_module_under(m_clang,"CallingConv");
rb_define_const(m_CallingConv,"DEFAULT", INT2NUM(0));
rb_define_const(m_CallingConv,"C", INT2NUM(1));
rb_define_const(m_CallingConv,"X86_STD_CALL", INT2NUM(2));
rb_define_const(m_CallingConv,"X86_FAST_CALL", INT2NUM(3));
rb_define_const(m_CallingConv,"X86_THIS_CALL", INT2NUM(4));
rb_define_const(m_CallingConv,"X86_PASCAL", INT2NUM(5));
rb_define_const(m_CallingConv,"AAPCS", INT2NUM(6));
rb_define_const(m_CallingConv,"AAPCS_VFP", INT2NUM(7));
rb_define_const(m_CallingConv,"INTEL_OCL_BICC", INT2NUM(9));
rb_define_const(m_CallingConv,"X86_64_WIN64", INT2NUM(10));
rb_define_const(m_CallingConv,"X86_64_SYS_V", INT2NUM(11));
rb_define_const(m_CallingConv,"X86_VECTOR_CALL", INT2NUM(12));
rb_define_const(m_CallingConv,"INVALID", INT2NUM(100));
rb_define_const(m_CallingConv,"UNEXPOSED", INT2NUM(200));

VALUE m_TemplateArgumentKind = rb_define_module_under(m_clang,"TemplateArgumentKind");
rb_define_const(m_TemplateArgumentKind,"NULL", INT2NUM(0));
rb_define_const(m_TemplateArgumentKind,"TYPE", INT2NUM(1));
rb_define_const(m_TemplateArgumentKind,"DECLARATION", INT2NUM(2));
rb_define_const(m_TemplateArgumentKind,"NULL_PTR", INT2NUM(3));
rb_define_const(m_TemplateArgumentKind,"INTEGRAL", INT2NUM(4));
rb_define_const(m_TemplateArgumentKind,"TEMPLATE", INT2NUM(5));
rb_define_const(m_TemplateArgumentKind,"TEMPLATE_EXPANSION", INT2NUM(6));
rb_define_const(m_TemplateArgumentKind,"EXPRESSION", INT2NUM(7));
rb_define_const(m_TemplateArgumentKind,"PACK", INT2NUM(8));
rb_define_const(m_TemplateArgumentKind,"INVALID", INT2NUM(9));

VALUE m_TypeLayoutError = rb_define_module_under(m_clang,"TypeLayoutError");
rb_define_const(m_TypeLayoutError,"INVALID", INT2NUM(-1));
rb_define_const(m_TypeLayoutError,"INCOMPLETE", INT2NUM(-2));
rb_define_const(m_TypeLayoutError,"DEPENDENT", INT2NUM(-3));
rb_define_const(m_TypeLayoutError,"NOT_CONSTANT_SIZE", INT2NUM(-4));
rb_define_const(m_TypeLayoutError,"INVALID_FIELD_NAME", INT2NUM(-5));

VALUE m_RefQualifierKind = rb_define_module_under(m_clang,"RefQualifierKind");
rb_define_const(m_RefQualifierKind,"NONE", INT2NUM(0));
rb_define_const(m_RefQualifierKind,"L_VALUE", INT2NUM(1));
rb_define_const(m_RefQualifierKind,"R_VALUE", INT2NUM(2));

VALUE m__XAccessSpecifier = rb_define_module_under(m_clang,"_XAccessSpecifier");
rb_define_const(m__XAccessSpecifier,"CXX_INVALID_ACCESS_SPECIFIER", INT2NUM(0));
rb_define_const(m__XAccessSpecifier,"CXX_PUBLIC", INT2NUM(1));
rb_define_const(m__XAccessSpecifier,"CXX_PROTECTED", INT2NUM(2));
rb_define_const(m__XAccessSpecifier,"CXX_PRIVATE", INT2NUM(3));

VALUE m__StorageClass = rb_define_module_under(m_clang,"_StorageClass");
rb_define_const(m__StorageClass,"SC_INVALID", INT2NUM(0));
rb_define_const(m__StorageClass,"SC_NONE", INT2NUM(1));
rb_define_const(m__StorageClass,"SC_EXTERN", INT2NUM(2));
rb_define_const(m__StorageClass,"SC_STATIC", INT2NUM(3));
rb_define_const(m__StorageClass,"SC_PRIVATE_EXTERN", INT2NUM(4));
rb_define_const(m__StorageClass,"SC_OPEN_CL_WORK_GROUP_LOCAL", INT2NUM(5));
rb_define_const(m__StorageClass,"SC_AUTO", INT2NUM(6));
rb_define_const(m__StorageClass,"SC_REGISTER", INT2NUM(7));

VALUE m_ChildVisitResult = rb_define_module_under(m_clang,"ChildVisitResult");
rb_define_const(m_ChildVisitResult,"BREAK", INT2NUM(0));
rb_define_const(m_ChildVisitResult,"CONTINUE", INT2NUM(1));
rb_define_const(m_ChildVisitResult,"RECURSE", INT2NUM(2));

VALUE m_ObjCPropertyAttrKind = rb_define_module_under(m_clang,"ObjCPropertyAttrKind");
rb_define_const(m_ObjCPropertyAttrKind,"NOATTR", INT2NUM(0));
rb_define_const(m_ObjCPropertyAttrKind,"READONLY", INT2NUM(1));
rb_define_const(m_ObjCPropertyAttrKind,"GETTER", INT2NUM(2));
rb_define_const(m_ObjCPropertyAttrKind,"ASSIGN", INT2NUM(4));
rb_define_const(m_ObjCPropertyAttrKind,"READWRITE", INT2NUM(8));
rb_define_const(m_ObjCPropertyAttrKind,"RETAIN", INT2NUM(16));
rb_define_const(m_ObjCPropertyAttrKind,"COPY", INT2NUM(32));
rb_define_const(m_ObjCPropertyAttrKind,"NONATOMIC", INT2NUM(64));
rb_define_const(m_ObjCPropertyAttrKind,"SETTER", INT2NUM(128));
rb_define_const(m_ObjCPropertyAttrKind,"ATOMIC", INT2NUM(256));
rb_define_const(m_ObjCPropertyAttrKind,"WEAK", INT2NUM(512));
rb_define_const(m_ObjCPropertyAttrKind,"STRONG", INT2NUM(1024));
rb_define_const(m_ObjCPropertyAttrKind,"UNSAFE_UNRETAINED", INT2NUM(2048));

VALUE m_ObjCDeclQualifierKind = rb_define_module_under(m_clang,"ObjCDeclQualifierKind");
rb_define_const(m_ObjCDeclQualifierKind,"NONE", INT2NUM(0));
rb_define_const(m_ObjCDeclQualifierKind,"IN", INT2NUM(1));
rb_define_const(m_ObjCDeclQualifierKind,"INOUT", INT2NUM(2));
rb_define_const(m_ObjCDeclQualifierKind,"OUT", INT2NUM(4));
rb_define_const(m_ObjCDeclQualifierKind,"BYCOPY", INT2NUM(8));
rb_define_const(m_ObjCDeclQualifierKind,"BYREF", INT2NUM(16));
rb_define_const(m_ObjCDeclQualifierKind,"ONEWAY", INT2NUM(32));

VALUE m_NameRefFlags = rb_define_module_under(m_clang,"NameRefFlags");
rb_define_const(m_NameRefFlags,"WANT_QUALIFIER", INT2NUM(1));
rb_define_const(m_NameRefFlags,"WANT_TEMPLATE_ARGS", INT2NUM(2));
rb_define_const(m_NameRefFlags,"WANT_SINGLE_PIECE", INT2NUM(4));

VALUE m_TokenKind = rb_define_module_under(m_clang,"TokenKind");
rb_define_const(m_TokenKind,"PUNCTUATION", INT2NUM(0));
rb_define_const(m_TokenKind,"KEYWORD", INT2NUM(1));
rb_define_const(m_TokenKind,"IDENTIFIER", INT2NUM(2));
rb_define_const(m_TokenKind,"LITERAL", INT2NUM(3));
rb_define_const(m_TokenKind,"COMMENT", INT2NUM(4));

VALUE m_CompletionChunkKind = rb_define_module_under(m_clang,"CompletionChunkKind");
rb_define_const(m_CompletionChunkKind,"OPTIONAL", INT2NUM(0));
rb_define_const(m_CompletionChunkKind,"TYPED_TEXT", INT2NUM(1));
rb_define_const(m_CompletionChunkKind,"TEXT", INT2NUM(2));
rb_define_const(m_CompletionChunkKind,"PLACEHOLDER", INT2NUM(3));
rb_define_const(m_CompletionChunkKind,"INFORMATIVE", INT2NUM(4));
rb_define_const(m_CompletionChunkKind,"CURRENT_PARAMETER", INT2NUM(5));
rb_define_const(m_CompletionChunkKind,"LEFT_PAREN", INT2NUM(6));
rb_define_const(m_CompletionChunkKind,"RIGHT_PAREN", INT2NUM(7));
rb_define_const(m_CompletionChunkKind,"LEFT_BRACKET", INT2NUM(8));
rb_define_const(m_CompletionChunkKind,"RIGHT_BRACKET", INT2NUM(9));
rb_define_const(m_CompletionChunkKind,"LEFT_BRACE", INT2NUM(10));
rb_define_const(m_CompletionChunkKind,"RIGHT_BRACE", INT2NUM(11));
rb_define_const(m_CompletionChunkKind,"LEFT_ANGLE", INT2NUM(12));
rb_define_const(m_CompletionChunkKind,"RIGHT_ANGLE", INT2NUM(13));
rb_define_const(m_CompletionChunkKind,"COMMA", INT2NUM(14));
rb_define_const(m_CompletionChunkKind,"RESULT_TYPE", INT2NUM(15));
rb_define_const(m_CompletionChunkKind,"COLON", INT2NUM(16));
rb_define_const(m_CompletionChunkKind,"SEMI_COLON", INT2NUM(17));
rb_define_const(m_CompletionChunkKind,"EQUAL", INT2NUM(18));
rb_define_const(m_CompletionChunkKind,"HORIZONTAL_SPACE", INT2NUM(19));
rb_define_const(m_CompletionChunkKind,"VERTICAL_SPACE", INT2NUM(20));

VALUE m_CodeComplete_Flags = rb_define_module_under(m_clang,"CodeComplete_Flags");
rb_define_const(m_CodeComplete_Flags,"INCLUDE_MACROS", INT2NUM(1));
rb_define_const(m_CodeComplete_Flags,"INCLUDE_CODE_PATTERNS", INT2NUM(2));
rb_define_const(m_CodeComplete_Flags,"INCLUDE_BRIEF_COMMENTS", INT2NUM(4));

VALUE m_CompletionContext = rb_define_module_under(m_clang,"CompletionContext");
rb_define_const(m_CompletionContext,"UNEXPOSED", INT2NUM(0));
rb_define_const(m_CompletionContext,"ANY_TYPE", INT2NUM(1));
rb_define_const(m_CompletionContext,"ANY_VALUE", INT2NUM(2));
rb_define_const(m_CompletionContext,"OBJ_C_OBJECT_VALUE", INT2NUM(4));
rb_define_const(m_CompletionContext,"OBJ_C_SELECTOR_VALUE", INT2NUM(8));
rb_define_const(m_CompletionContext,"CXX_CLASS_TYPE_VALUE", INT2NUM(16));
rb_define_const(m_CompletionContext,"DOT_MEMBER_ACCESS", INT2NUM(32));
rb_define_const(m_CompletionContext,"ARROW_MEMBER_ACCESS", INT2NUM(64));
rb_define_const(m_CompletionContext,"OBJ_C_PROPERTY_ACCESS", INT2NUM(128));
rb_define_const(m_CompletionContext,"ENUM_TAG", INT2NUM(256));
rb_define_const(m_CompletionContext,"UNION_TAG", INT2NUM(512));
rb_define_const(m_CompletionContext,"STRUCT_TAG", INT2NUM(1024));
rb_define_const(m_CompletionContext,"CLASS_TAG", INT2NUM(2048));
rb_define_const(m_CompletionContext,"NAMESPACE", INT2NUM(4096));
rb_define_const(m_CompletionContext,"NESTED_NAME_SPECIFIER", INT2NUM(8192));
rb_define_const(m_CompletionContext,"OBJ_C_INTERFACE", INT2NUM(16384));
rb_define_const(m_CompletionContext,"OBJ_C_PROTOCOL", INT2NUM(32768));
rb_define_const(m_CompletionContext,"OBJ_C_CATEGORY", INT2NUM(65536));
rb_define_const(m_CompletionContext,"OBJ_C_INSTANCE_MESSAGE", INT2NUM(131072));
rb_define_const(m_CompletionContext,"OBJ_C_CLASS_MESSAGE", INT2NUM(262144));
rb_define_const(m_CompletionContext,"OBJ_C_SELECTOR_NAME", INT2NUM(524288));
rb_define_const(m_CompletionContext,"MACRO_NAME", INT2NUM(1048576));
rb_define_const(m_CompletionContext,"NATURAL_LANGUAGE", INT2NUM(2097152));
rb_define_const(m_CompletionContext,"UNKNOWN", INT2NUM(4194303));

VALUE m_VisitorResult = rb_define_module_under(m_clang,"VisitorResult");
rb_define_const(m_VisitorResult,"BREAK", INT2NUM(0));
rb_define_const(m_VisitorResult,"CONTINUE", INT2NUM(1));

VALUE m_Result = rb_define_module_under(m_clang,"Result");
rb_define_const(m_Result,"SUCCESS", INT2NUM(0));
rb_define_const(m_Result,"INVALID", INT2NUM(1));
rb_define_const(m_Result,"VISIT_BREAK", INT2NUM(2));

VALUE m_IdxEntityKind = rb_define_module_under(m_clang,"IdxEntityKind");
rb_define_const(m_IdxEntityKind,"UNEXPOSED", INT2NUM(0));
rb_define_const(m_IdxEntityKind,"TYPEDEF", INT2NUM(1));
rb_define_const(m_IdxEntityKind,"FUNCTION", INT2NUM(2));
rb_define_const(m_IdxEntityKind,"VARIABLE", INT2NUM(3));
rb_define_const(m_IdxEntityKind,"FIELD", INT2NUM(4));
rb_define_const(m_IdxEntityKind,"ENUM_CONSTANT", INT2NUM(5));
rb_define_const(m_IdxEntityKind,"OBJ_C_CLASS", INT2NUM(6));
rb_define_const(m_IdxEntityKind,"OBJ_C_PROTOCOL", INT2NUM(7));
rb_define_const(m_IdxEntityKind,"OBJ_C_CATEGORY", INT2NUM(8));
rb_define_const(m_IdxEntityKind,"OBJ_C_INSTANCE_METHOD", INT2NUM(9));
rb_define_const(m_IdxEntityKind,"OBJ_C_CLASS_METHOD", INT2NUM(10));
rb_define_const(m_IdxEntityKind,"OBJ_C_PROPERTY", INT2NUM(11));
rb_define_const(m_IdxEntityKind,"OBJ_C_IVAR", INT2NUM(12));
rb_define_const(m_IdxEntityKind,"ENUM", INT2NUM(13));
rb_define_const(m_IdxEntityKind,"STRUCT", INT2NUM(14));
rb_define_const(m_IdxEntityKind,"UNION", INT2NUM(15));
rb_define_const(m_IdxEntityKind,"CXX_CLASS", INT2NUM(16));
rb_define_const(m_IdxEntityKind,"CXX_NAMESPACE", INT2NUM(17));
rb_define_const(m_IdxEntityKind,"CXX_NAMESPACE_ALIAS", INT2NUM(18));
rb_define_const(m_IdxEntityKind,"CXX_STATIC_VARIABLE", INT2NUM(19));
rb_define_const(m_IdxEntityKind,"CXX_STATIC_METHOD", INT2NUM(20));
rb_define_const(m_IdxEntityKind,"CXX_INSTANCE_METHOD", INT2NUM(21));
rb_define_const(m_IdxEntityKind,"CXX_CONSTRUCTOR", INT2NUM(22));
rb_define_const(m_IdxEntityKind,"CXX_DESTRUCTOR", INT2NUM(23));
rb_define_const(m_IdxEntityKind,"CXX_CONVERSION_FUNCTION", INT2NUM(24));
rb_define_const(m_IdxEntityKind,"CXX_TYPE_ALIAS", INT2NUM(25));
rb_define_const(m_IdxEntityKind,"CXX_INTERFACE", INT2NUM(26));

VALUE m_IdxEntityLanguage = rb_define_module_under(m_clang,"IdxEntityLanguage");
rb_define_const(m_IdxEntityLanguage,"NONE", INT2NUM(0));
rb_define_const(m_IdxEntityLanguage,"C", INT2NUM(1));
rb_define_const(m_IdxEntityLanguage,"OBJ_C", INT2NUM(2));
rb_define_const(m_IdxEntityLanguage,"CXX", INT2NUM(3));

VALUE m_IdxEntityXTemplateKind = rb_define_module_under(m_clang,"IdxEntityXTemplateKind");
rb_define_const(m_IdxEntityXTemplateKind,"NON_TEMPLATE", INT2NUM(0));
rb_define_const(m_IdxEntityXTemplateKind,"TEMPLATE", INT2NUM(1));
rb_define_const(m_IdxEntityXTemplateKind,"TEMPLATE_PARTIAL_SPECIALIZATION", INT2NUM(2));
rb_define_const(m_IdxEntityXTemplateKind,"TEMPLATE_SPECIALIZATION", INT2NUM(3));

VALUE m_IdxAttrKind = rb_define_module_under(m_clang,"IdxAttrKind");
rb_define_const(m_IdxAttrKind,"UNEXPOSED", INT2NUM(0));
rb_define_const(m_IdxAttrKind,"IB_ACTION", INT2NUM(1));
rb_define_const(m_IdxAttrKind,"IB_OUTLET", INT2NUM(2));
rb_define_const(m_IdxAttrKind,"IB_OUTLET_COLLECTION", INT2NUM(3));

VALUE m_IdxDeclInfoFlags = rb_define_module_under(m_clang,"IdxDeclInfoFlags");
rb_define_const(m_IdxDeclInfoFlags,"SKIPPED", INT2NUM(1));

VALUE m_IdxObjCContainerKind = rb_define_module_under(m_clang,"IdxObjCContainerKind");
rb_define_const(m_IdxObjCContainerKind,"FORWARD_REF", INT2NUM(0));
rb_define_const(m_IdxObjCContainerKind,"INTERFACE", INT2NUM(1));
rb_define_const(m_IdxObjCContainerKind,"IMPLEMENTATION", INT2NUM(2));

VALUE m_IdxEntityRefKind = rb_define_module_under(m_clang,"IdxEntityRefKind");
rb_define_const(m_IdxEntityRefKind,"DIRECT", INT2NUM(1));
rb_define_const(m_IdxEntityRefKind,"IMPLICIT", INT2NUM(2));

VALUE m_IndexOptFlags = rb_define_module_under(m_clang,"IndexOptFlags");
rb_define_const(m_IndexOptFlags,"NONE", INT2NUM(0));
rb_define_const(m_IndexOptFlags,"SUPPRESS_REDUNDANT_REFS", INT2NUM(1));
rb_define_const(m_IndexOptFlags,"INDEX_FUNCTION_LOCAL_SYMBOLS", INT2NUM(2));
rb_define_const(m_IndexOptFlags,"INDEX_IMPLICIT_TEMPLATE_INSTANTIATIONS", INT2NUM(4));
rb_define_const(m_IndexOptFlags,"SUPPRESS_WARNINGS", INT2NUM(8));
rb_define_const(m_IndexOptFlags,"SKIP_PARSED_BODIES_IN_SESSION", INT2NUM(16));

}
void init_clang_errors_enums_to_constants(VALUE m_clang){
VALUE m_ErrorCode = rb_define_module_under(m_clang,"ErrorCode");
rb_define_const(m_ErrorCode,"SUCCESS", INT2NUM(0));
rb_define_const(m_ErrorCode,"FAILURE", INT2NUM(1));
rb_define_const(m_ErrorCode,"CRASHED", INT2NUM(2));
rb_define_const(m_ErrorCode,"INVALID_ARGUMENTS", INT2NUM(3));
rb_define_const(m_ErrorCode,"AST_READ_ERROR", INT2NUM(4));

}
