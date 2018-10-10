/**
 *		@file 		TypeName.cpp
 *		@date 		12/04/2018
 *		@author 	Filip Kocica <xkocic01@fit.vutbr.cz>
 *    @author   Ondřej Vašíček <xvasic25@stud.fit.vutbr.cz>
 *		@brief    Type of data on port/block
 */

#include "TypeName.hpp"

///
/// Namespace with implementation of logic of an application
///
namespace BlockEditorLogic
{
  /**
   * @brief Converts index in comboBox to type name string
   * @param Current index in comboBox
   * @retun Typename string
   */
  std::string index2typename(int currIndex)
  {
    std::string tn;

    switch (currIndex)
    {
        case 0:
          tn = TN_FLOAT;
        break;
        case 1:
          tn = TN_INTEGER;
        break;
        case 2:
          tn = TN_HEXA;
        break;
        default:
          tn = TN_FLOAT;
        break;
    }

    return tn;
  }
}