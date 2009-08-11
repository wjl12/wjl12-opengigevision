<?xml version="1.0" encoding="UTF-8"?>
<!-- ***************************************************************************
*  (c) 2006 by Basler Vision Technologies
*  Section: Vision Components
*  Project: GenApi
*	 Author:  Fritz Dierks
*  $Header: /cvs/genicam/genicam/xml/GenApi/GenApi_Ptr_h.xsl,v 1.7 2007/12/17 13:37:18 hartmut_nebelung Exp $
*
*  License: This file is published under the license of the EMVA GenICam  Standard Group. 
*  A text file describing the legal terms is included in  your installation as 'GenICam_license.pdf'. 
*  If for some reason you are missing  this file please contact the EMVA or visit the website
*  (http://www.genicam.org) for a full copy.
* 
*  THIS SOFTWARE IS PROVIDED BY THE EMVA GENICAM STANDARD GROUP "AS IS"
*  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,  
*  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR  
*  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE EMVA GENICAM STANDARD  GROUP 
*  OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,  SPECIAL, 
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT  LIMITED TO, 
*  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,  DATA, OR PROFITS; 
*  OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY  THEORY OF LIABILITY, 
*  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT  (INCLUDING NEGLIGENCE OR OTHERWISE) 
*  ARISING IN ANY WAY OUT OF THE USE  OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
*  POSSIBILITY OF SUCH DAMAGE.
******************************************************************************** -->
<xsl:stylesheet version="1.0" xmlns:my="http://www.genicam.org/GenApi/Version_1_0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:output method="text" encoding="UTF-8"/>
<xsl:template match="/">
<!-- =========================================================================== -->
<!-- Main body                                                                                                                                       -->
<!-- =========================================================================== -->
//-----------------------------------------------------------------------------
//  (c) 2004, 2005 by Basler Vision Technologies
//  Section: Vision Components
//  Project: GenApi
//-----------------------------------------------------------------------------
/*!
	\file  
	\brief <xsl:value-of select="/my:RegisterDescription/@ToolTip"/>
*/
//-----------------------------------------------------------------------------
//  This file is generated automatically
//  Do not modify!
//-----------------------------------------------------------------------------

#ifndef <xsl:value-of select="/my:RegisterDescription/@VendorName"/>_<xsl:value-of select="/my:RegisterDescription/@ModelName"/>_H
#define <xsl:value-of select="/my:RegisterDescription/@VendorName"/>_<xsl:value-of select="/my:RegisterDescription/@ModelName"/>_H

#include &lt;GenApi/IEnumerationT.h&gt;
#include &lt;GenApi/NodeMapRef.h&gt;

//! The namespace containing the device's control interface and related enumeration types
namespace <xsl:value-of select="/my:RegisterDescription/@VendorName"/>_<xsl:value-of select="/my:RegisterDescription/@ModelName"/>
{
	//**************************************************************************************************
	// Enumerations
	//**************************************************************************************************
	<xsl:apply-templates select="/my:RegisterDescription/my:Enumeration" mode="DefiningEnums"/>

	//**************************************************************************************************
	// Access class
	//**************************************************************************************************

	//! <xsl:value-of select="/my:RegisterDescription/@ToolTip"/>
	class C<xsl:value-of select="/my:RegisterDescription/@ModelName"/> : public GenApi::CNodeMapRef
	{
		//----------------------------------------------------------------------------------------------------------------
		// Implementation
		//----------------------------------------------------------------------------------------------------------------
	public:
		//! Constructor
		C<xsl:value-of select="/my:RegisterDescription/@ModelName"/>(void);

		//! Constructor
		C<xsl:value-of select="/my:RegisterDescription/@ModelName"/>(GenICam::gcstring DeviceName);

		//! Destructor
		virtual ~C<xsl:value-of select="/my:RegisterDescription/@ModelName"/>(void);

		//! Creates the object from the default DLL
		void _LoadDLL(void);
		
	protected:
		//! Initializes the references
		virtual void _Initialize(void);

	public:		
		//----------------------------------------------------------------------------------------------------------------
		// References to features
		//----------------------------------------------------------------------------------------------------------------
		<xsl:apply-templates select="//my:Category/my:pFeature" mode="ReferenceDeclaration"/>

	private:
		//! not implemented copy constructor
		C<xsl:value-of select="/my:RegisterDescription/@ModelName"/>(C<xsl:value-of select="/my:RegisterDescription/@ModelName"/>&amp;);
		
		//! not implemented assignment operator 
		C<xsl:value-of select="/my:RegisterDescription/@ModelName"/>&amp; operator=(C<xsl:value-of select="/my:RegisterDescription/@ModelName"/>&amp;);
	
	};

}

	//----------------------------------------------------------------------------------------------------------------
	// Definition of Category node groups
	//----------------------------------------------------------------------------------------------------------------
	<xsl:apply-templates select="//my:Category" mode="DefiningGroups"/>

#endif // <xsl:value-of select="/my:RegisterDescription/@VendorName"/>_<xsl:value-of select="/my:RegisterDescription/@ModelName"/>_H

</xsl:template>

<!-- =========================================================================== -->
<!-- DefiningGroups                                                                                                                                -->
<!-- =========================================================================== -->
	<xsl:template match="my:Category" mode="DefiningGroups">
    <!-- 
		<xsl:variable name="CategoryName" select="@Name"/>
		/*! \defgroup <xsl:value-of select="$CategoryName"/><xsl:text> </xsl:text><xsl:value-of select="$CategoryName"/> - <xsl:value-of select="my:ToolTip"/>
			\ingroup <xsl:value-of select="//my:Category[my:pFeature=$CategoryName]/@Name"/> */
-->
    </xsl:template>
<!-- =========================================================================== -->
<!-- DefiningEnums                                                                                                                                -->
<!-- =========================================================================== -->

	<xsl:template match="my:Enumeration" mode="DefiningEnums">
	//! Valid values for <xsl:value-of select="@Name"/>
	enum <xsl:value-of select="@Name"/>Enums 
	{
		<xsl:apply-templates select="./my:*" mode="InsideDefiningEnums"/>
	};
	</xsl:template>

		<!-- =========================================================================== -->
		<!-- InsideDefiningEnums                                                                                                                                -->
		<!-- =========================================================================== -->

		<xsl:template match="my:*" mode="InsideDefiningEnums"/>

		<xsl:template match="my:pEnumEntry" mode="InsideDefiningEnums">
			<xsl:variable name="NodeName" select="string()"/>
			<!--<xsl:value-of select="../@Name"/>_<xsl:value-of select="../../my:EnumEntry[@Name=$NodeName]/my:Symbolic"/> = <xsl:value-of select="../../my:EnumEntry[@Name=$NodeName]/my:Value"/>,	//!&lt;<xsl:value-of select="../../my:EnumEntry[@Name=$NodeName]/my:ToolTip"/>.-->
			<xsl:value-of select="../@Name"/>_<xsl:value-of select="../../my:EnumEntry[@Name=$NodeName]/my:Symbolic"/>,	//!&lt;<xsl:value-of select="../../my:EnumEntry[@Name=$NodeName]/my:ToolTip"/>.
		</xsl:template>

<!-- =========================================================================== -->
<!-- ReferenceDeclaration                                                                                                                          -->
<!-- =========================================================================== -->

<xsl:template match="my:*" mode="ReferenceDeclaration">
	<xsl:variable name="Feature" select="string()"/>
	<xsl:variable name="NodeType" select="name(//*[@Name=$Feature])"/>
	<xsl:if test="$NodeType = 'Category'">
		<xsl:apply-templates select="./my:pFeature" mode="ReferenceDeclaration"/>
	</xsl:if>
	<xsl:if test="$NodeType != 'Category'">
		//! <xsl:value-of select="//*[@Name=$Feature]/my:ToolTip"/> 
		/*! 
    <!--\ingroup <xsl:value-of select="//my:Category[my:pFeature=$Feature]/@Name"/> -->
			 <xsl:value-of select="//*[@Name=$Feature]/my:Description"/> */
		<xsl:if test="$NodeType = 'Enumeration'">
			<xsl:text>		GenApi::</xsl:text>
			<xsl:value-of select="document('NodeTypes.xml')//CppInterface[../@Name=$NodeType]"/><xsl:text>T&lt;</xsl:text>
			<xsl:value-of select="string()"/>Enums <xsl:text>&gt;</xsl:text>
		</xsl:if>
		<xsl:if test="$NodeType != 'Enumeration'">
			<xsl:text>		GenApi::</xsl:text>
			<xsl:value-of select="document('NodeTypes.xml')//CppInterface[../@Name=$NodeType]"/>
		</xsl:if>
		<xsl:text> &amp;</xsl:text>
		<xsl:value-of select="string()"/>;
	</xsl:if>
</xsl:template>

</xsl:stylesheet>
