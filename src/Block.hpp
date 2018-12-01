/**
 *		@file 		Block.hpp
 *		@date 		01/03/2018
 *		@author 	Filip Kocica <xkocic01@fit.vutbr.cz>
 *    @author   Ondřej Vašíček <xvasic25@stud.fit.vutbr.cz>
 *		@brief 		Implementation of Block used in blockedior
 */

#include <iostream>
#include <math.h>

#include "BlockType.hpp"
#include "Port.hpp"
#include "TypeName.hpp"
#include "BlockEditorException.hpp"

///
/// Namespace with implementation of logic of an application
///
namespace BlockEditorLogic
{
	///
	/// Interface which declares actions & values that all the blocks have same
	///
	class CBlock
	{
	public:
		CBlock() = delete;
		CBlock(ID, EBlockType, TypeName);
		CBlock(ID, EBlockType, int, int, PortValue,
									CPort*, CPort*,  CPort*, TypeName);
		virtual ~CBlock();

		friend
		std::ostream& 	operator<<(std::ostream&, const CBlock&);

		CPort*   addOutputPort(ID portID);
		void     addInputPort(Ports whichPort, CPort* port);
		void     removePort(Ports whichPort);
		void     setPort(Ports whichPort, void *ptr = nullptr);
		bool     hasPort(Ports whichPort) const;
		CPort*   getPort(Ports whichPort) const;

		virtual
		PortValue   performOperation(PortValue&& pv1, PortValue&& pv2);

		void        setInputValue(PortValue);
		PortValue   getValue() const;
		TypeName    getTypeName() const;
		ID          getPortID(Ports whichPort) const;
		ID          getID() const;
		EBlockType  getType() const;
		std::pair<int, int> getPosition() const;
		void        setPosition(std::pair<int, int>);

	protected:
		ID          m_blockID;         /**< Unique ID */
		EBlockType  m_bt;              /**< Type of block {ADD|SUB|etc.} */
		int         m_x, m_y;          /**< Position of block in scheme */
		PortValue   m_value = .0;      /**< Value of input port */

		CPort       *m_inputPort1;
		CPort       *m_inputPort2;     /**< Input ports */
		CPort       *m_outputPort;     /**< Output port */

		TypeName    m_name;            /**< Name of type of this block */
	private:

	};
}