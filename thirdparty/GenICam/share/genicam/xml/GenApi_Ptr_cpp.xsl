<?xml version="1.0" encoding="UTF-8"?>
<!-- ***************************************************************************
*  (c) 2006 by Basler Vision Technologies
*  Section: Vision Components
*  Project: GenApi
*	 Author:  Fritz Dierks
*  $Header: /cvs/genicam/genicam/xml/GenApi/GenApi_Ptr_cpp.xsl,v 1.9.2.1 2008/07/14 09:22:21 hartmut_nebelung Exp $
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

<!-- Parameter to override the default include statement of the class definition include file -->
<xsl:param name="Include"/>
  
<xsl:template match="/">
//-----------------------------------------------------------------------------
//  (c) 2004, 2005 by Basler Vision Technologies
//  Section: Vision Components
//  Project: GenApi
//	Author:  Fritz Dierks
//-----------------------------------------------------------------------------
/*!
	\file  
*/
//-----------------------------------------------------------------------------
//  This file is generated automatically
//  Do not modify!
//-----------------------------------------------------------------------------
<xsl:choose>
<xsl:when test="$Include=''">
#if defined (_MSC_VER) &amp;&amp; defined (_WIN32)
#   include &lt;GenApi/<xsl:value-of select="/my:RegisterDescription/@VendorName"/>/<xsl:value-of select="/my:RegisterDescription/@ModelName"/>.h&gt;
#elif defined (__GNUC__) &amp;&amp; defined (__linux__)
#   include "<xsl:value-of select="/my:RegisterDescription/@ModelName"/>.h"
#else
#   error Unknown/unsupported platform
#endif
</xsl:when>

<xsl:otherwise>
#include <xsl:value-of select="$Include"/>
</xsl:otherwise>  
</xsl:choose>

#include &lt;GenApi/DLLLoad.h&gt;

using namespace GenICam;
using namespace GenApi;

namespace <xsl:value-of select="/my:RegisterDescription/@VendorName"/>_<xsl:value-of select="/my:RegisterDescription/@ModelName"/>
{
	
	C<xsl:value-of select="/my:RegisterDescription/@ModelName"/>::C<xsl:value-of select="/my:RegisterDescription/@ModelName"/>(void) :
		<xsl:apply-templates select="//my:Category/my:pFeature" mode="ReferenceCreation"/>CNodeMapRef()
	{
	}

	C<xsl:value-of select="/my:RegisterDescription/@ModelName"/>::C<xsl:value-of select="/my:RegisterDescription/@ModelName"/>(GenICam::gcstring DeviceName) :
		<xsl:apply-templates select="//my:Category/my:pFeature" mode="ReferenceCreation"/>CNodeMapRef(DeviceName)
	{
	}

	C<xsl:value-of select="/my:RegisterDescription/@ModelName"/>::~C<xsl:value-of select="/my:RegisterDescription/@ModelName"/>(void)
	{
	<xsl:apply-templates select="//my:Category/my:pFeature" mode="ReferenceDeletion"/>
	}     

   	void C<xsl:value-of select="/my:RegisterDescription/@ModelName"/>::_Initialize()
	{
	<xsl:apply-templates select="//my:Category/my:pFeature" mode="ReferenceInitialization"/>
	}

	void C<xsl:value-of select="/my:RegisterDescription/@ModelName"/>::_LoadDLL(void)
	{
		CNodeMapRef::_LoadDLL("<xsl:value-of select="/my:RegisterDescription/@VendorName"/>", "<xsl:value-of select="/my:RegisterDescription/@ModelName"/>");
	}
	          
}<xsl:text>&#x0a;</xsl:text>
</xsl:template>

<!-- =========================================================================== -->
<!-- ReferenceCreation                                                                                                                                -->
<!-- =========================================================================== -->

	<xsl:template match="my:*" mode="ReferenceCreation">
		<xsl:variable name="Feature" select="string()"/>
		<xsl:variable name="NodeType" select="name(//*[@Name=$Feature])"/>
		<xsl:choose>
			<xsl:when test="$NodeType = 'Enumeration'">
				<xsl:value-of select="$Feature"/>( *new GenApi::<xsl:value-of select="document('NodeTypes.xml')//CppReference[../@Name=$NodeType]"/>&lt;<xsl:value-of select="$Feature"/>Enums&gt;() ),
			</xsl:when>
			<xsl:when test="$NodeType != 'Category'">
				<xsl:value-of select="$Feature"/>( *new GenApi::<xsl:value-of select="document('NodeTypes.xml')//CppReference[../@Name=$NodeType]"/>() ),
			</xsl:when>
		</xsl:choose>
	</xsl:template>

<!-- =========================================================================== -->
<!-- ReferenceDeletion                                                                                                                                -->
<!-- =========================================================================== -->

<xsl:template match="my:*" mode="ReferenceDeletion">
	<xsl:variable name="Feature" select="string()"/>
	<xsl:variable name="NodeType" select="name(//*[@Name=$Feature])"/>
  <xsl:choose>
    <xsl:when test="$NodeType = 'Enumeration'">
      delete static_cast &lt; GenApi::<xsl:value-of select="document('NodeTypes.xml')//CppReference[../@Name=$NodeType]"/>&lt;<xsl:value-of select="$Feature"/>Enums&gt; *&gt; (&amp;<xsl:value-of select="string()"/> );
    </xsl:when>
    <xsl:when test="$NodeType != 'Category'">
      delete static_cast &lt; GenApi::<xsl:value-of select="document('NodeTypes.xml')//CppReference[../@Name=$NodeType]"/>*&gt; (&amp;<xsl:value-of select="string()"/> );
    </xsl:when>
  </xsl:choose>
</xsl:template>

<!-- =========================================================================== -->
<!-- ReferenceInitialization                                                                                                                            -->
<!-- =========================================================================== -->

<xsl:template match="my:*" mode="ReferenceInitialization">
	<xsl:variable name="FeatureName" select="string()"/>
	<xsl:variable name="NodeType" select="name(//*[@Name=$FeatureName])"/>
	<xsl:if test="$NodeType != 'Category'">
		<xsl:text>	dynamic_cast&lt;GenApi::IReference*&gt;(&amp;</xsl:text><xsl:value-of select="string()"/><xsl:text>)-&gt;SetReference(_Ptr-&gt;GetNode("</xsl:text><xsl:value-of select="string()"/>"));
	</xsl:if>
	<xsl:if test="$NodeType = 'Enumeration'">
		<xsl:text>	dynamic_cast&lt;GenApi::IEnumReference*&gt;(&amp;</xsl:text><xsl:value-of select="$FeatureName"/><xsl:text>)-&gt;SetNumEnums(</xsl:text><xsl:value-of select="count(//my:*[@Name=$FeatureName]/my:pEnumEntry)"/>)<xsl:text>;&#10;	</xsl:text>
		<xsl:apply-templates select="//my:*[@Name=$FeatureName]/my:pEnumEntry" mode="EnumInitialization">
			<xsl:with-param name="EnumName" select="$FeatureName"/>
		</xsl:apply-templates>
	</xsl:if>
</xsl:template>
	
	<!-- =========================================================================== -->
	<!-- EnumInitialization                                                                                                                            -->
	<!-- =========================================================================== -->
	
	<xsl:template match="my:*" mode="EnumInitialization">
		<xsl:param name="EnumName"/>
		<xsl:variable name="FeatureName" select="string()"/>
		<xsl:apply-templates select="//my:*[@Name=$FeatureName]" mode="EnumEntryInitialization">
			<xsl:with-param name="EnumName" select="$EnumName"/>
		</xsl:apply-templates>
	</xsl:template>

	<!-- =========================================================================== -->
	<!-- EnumEntryInitialization                                                                                                                            -->
	<!-- =========================================================================== -->
	
	<xsl:template match="my:*" mode="EnumEntryInitialization">
		<xsl:param name="EnumName"/>
		<xsl:text>	dynamic_cast&lt;GenApi::IEnumReference*&gt;(&amp;</xsl:text><xsl:value-of select="$EnumName"/><xsl:text>)-&gt;SetEnumReference( </xsl:text><xsl:value-of select="$EnumName"/>_<xsl:value-of select="./my:Symbolic"/>, "<xsl:value-of select="./my:Symbolic"/>" );
	</xsl:template>


</xsl:stylesheet>
