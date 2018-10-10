/**
 *		@file 		BlockType.hpp
 *		@date 		01/03/2018
 *		@author 	Filip Kocica <xkocic01@fit.vutbr.cz>
 *    @author   Ondřej Vašíček <xvasic25@stud.fit.vutbr.cz>
 *		@brief    Types of block used in scheme
 */

#pragma once

#include <iostream>

///
/// Namespace with implementation of logic of an application
///
namespace BlockEditorLogic
{
	///
	/// Type of block enumator
	///
	enum EBlockType
	{
		BT_INPUT = 0,   /**< Input block -- DONT USE IN GUI ! */
		BT_ADD,					/**< Type of block which performs suma operator on inputs */
		BT_SUB,					/**< Type of block which performs sub operator on inputs */
		BT_MUL,     		/**< Type of block which performs mul operation on inputs */
		BT_DIV,     		/**< Type of block which performs div operation on inputs */
		BT_POW,					/**< Type of block which performs pow operation first^second input */
		// TODO: BT_ADD_STRING,      /**< Type of block which performs concatenation on two strings */
	};
}