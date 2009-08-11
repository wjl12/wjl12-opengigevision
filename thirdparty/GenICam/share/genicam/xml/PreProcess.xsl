<?xml version="1.0" encoding="UTF-8"?>
<!-- edited with XMLSPY v2004 rel. 4 U (http://www.xmlspy.com) by Heike Skrzynski-Fox (Basler AG) -->
<!-- edited with XMLSpy v2005 U (http://www.xmlspy.com) by Heike Skrzynski-Fox (Basler AG) -->
<!--.................................................................................................................................
     (c) 2006 Basler Vision Technologies
		 Section: Vision Components
		 Project: GenApi
		 $Header: /cvs/genicam/genicam/xml/GenApi/PreProcess.xsl,v 1.13 2008/01/11 12:33:19 hartmut_nebelung Exp $
		 Author: Hartmut Nebelung
		 
		 Convert nested xml register spec into the appropriate format for GenApi.xsl.

     License: This file is published under the license of the EMVA GenICam  Standard Group. 
     A text file describing the legal terms is included in  your installation as 'GenICam_license.pdf'. 
     If for some reason you are missing  this file please contact the EMVA or visit the website
     (http://www.genicam.org) for a full copy.
 
     THIS SOFTWARE IS PROVIDED BY THE EMVA GENICAM STANDARD GROUP "AS IS"
     AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,  
     THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR  
     PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE EMVA GENICAM STANDARD  GROUP 
     OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,  SPECIAL, 
     EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT  LIMITED TO, 
     PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,  DATA, OR PROFITS; 
     OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY  THEORY OF LIABILITY, 
     WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT  (INCLUDING NEGLIGENCE OR OTHERWISE) 
     ARISING IN ANY WAY OUT OF THE USE  OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
     POSSIBILITY OF SUCH DAMAGE.
     .................................................................................................................................-->
<xsl:stylesheet version="1.0" xmlns:gen="http://www.genicam.org/GenApi/Version_1_0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output method="xml" version="1.0" encoding="utf-8" indent="yes"/>
	<!--Main template: looks for the RegisterDescription-->
	<xsl:template match="/">
		<xsl:apply-templates select="gen:RegisterDescription"/>
	</xsl:template>
	<!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->
	<!--Replace the Group nodes by it's content.-->
	<!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->
	<xsl:template match="gen:Group">
		<xsl:apply-templates select="node()"/>
	</xsl:template>
	<!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->
	<!--Strip the Extension node -->
	<!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->
	<xsl:template match="gen:Extension">
  </xsl:template>
	<!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->
	<!--Replace Bit entry by MSB and LSB -->
	<!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->
	<!-- Example: <Bit>12</Bit>   is converted to <MSB>12</MSB><LSB>12</LSB> -->
	<xsl:template match="gen:Bit">
		<xsl:element name="LSB" namespace="http://www.genicam.org/GenApi/Version_1_0">
			<xsl:value-of select="text()"/>
		</xsl:element>
		<xsl:element name="MSB" namespace="http://www.genicam.org/GenApi/Version_1_0">
			<xsl:value-of select="text()"/>
		</xsl:element>
	</xsl:template>
	<!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->
	<!--Extract embedded SwissKnifes -->
	<!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->
	<xsl:template match="gen:Register|gen:IntReg|gen:MaskedIntReg|gen:FloatReg|gen:StringReg">
	   <!-- Create SwissKnife elements -->
	   <xsl:for-each select="gen:IntSwissKnife">
	       <xsl:apply-templates select="." mode="GenerateElement">
			   <xsl:with-param name="Name" select="generate-id()"/>
	       </xsl:apply-templates>
	   </xsl:for-each>
		<!-- Create references to the SwissKnife elements -->
		<xsl:copy>
			<xsl:for-each select="node() | @*">
				<xsl:choose>
					<xsl:when test="name()='IntSwissKnife'">
						<xsl:apply-templates select="." mode="GenerateReference">
							<xsl:with-param name="Name" select="generate-id()"/>
						</xsl:apply-templates>
					</xsl:when>
					<xsl:otherwise>
						<xsl:apply-templates select="."/>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:for-each>
		</xsl:copy>
	</xsl:template>
	<!--Create a SwissKnife element -->
	<xsl:template match="gen:SwissKnife | gen:IntSwissKnife" mode="GenerateElement">
		<xsl:param name="Name"/>
		<xsl:copy>
			<xsl:attribute name="Name"><xsl:value-of select="$Name"/></xsl:attribute>
			<xsl:attribute name="NameSpace">Custom</xsl:attribute>
			<xsl:apply-templates select="node()"/>
		</xsl:copy>
	</xsl:template>
	<!--Create a reference to SwissKnife element uns the Name parameter-->
	<xsl:template match="gen:SwissKnife | gen:IntSwissKnife" mode="GenerateReference">
		<xsl:param name="Name"/>
		<xsl:element name="pAddress" namespace="http://www.genicam.org/GenApi/Version_1_0">
			<xsl:value-of select="$Name"/>
		</xsl:element>
	</xsl:template>
	<!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->
	<!--Extract embedded EnumEntries -->
	<!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->
	<xsl:template match="gen:Enumeration">
		<!-- Create the EnumEntry elements -->
		<xsl:for-each select="node() | @*">
			<xsl:choose>
				<xsl:when test="name()='EnumEntry'">
					<xsl:apply-templates select="." mode="GenerateElement">
						<xsl:with-param name="EnumerationName" select="../@Name"/>
						<xsl:with-param name="EntryName" select="@Name"/>
						<xsl:with-param name="NameSpace" select="@NameSpace"/>
					</xsl:apply-templates>
				</xsl:when>
			</xsl:choose>
		</xsl:for-each>
		<!-- Create references to the EnumEntry elements -->
		<xsl:copy>
			<xsl:for-each select="node() | @*">
				<xsl:choose>
					<xsl:when test="name()='EnumEntry'">
						<xsl:apply-templates select="." mode="GenerateReference">
							<xsl:with-param name="EnumerationName" select="../@Name"/>
							<xsl:with-param name="EntryName" select="@Name"/>
						</xsl:apply-templates>
					</xsl:when>
					<xsl:otherwise>
						<xsl:apply-templates select="."/>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:for-each>
		</xsl:copy>
	</xsl:template>
	<!--Create an EnumEntry element -->
	<xsl:template match="gen:EnumEntry" mode="GenerateElement">
		<xsl:param name="EntryName"/>
		<xsl:param name="EnumerationName"/>
		<xsl:param name="NameSpace"/>
	    <xsl:copy>
	        <xsl:attribute name="Name">EnumEntry_<xsl:value-of select="$EnumerationName"/>_<xsl:value-of select="$EntryName"/></xsl:attribute>
	        <xsl:attribute name="NameSpace"><xsl:value-of select="$NameSpace"/></xsl:attribute>
	        <xsl:apply-templates select="node()"/>
	        <xsl:if test="0=count(*[name()='Symbolic'])">
	            <xsl:element name="Symbolic" namespace="http://www.genicam.org/GenApi/Version_1_0">
	                <xsl:value-of select="$EntryName"/>
	            </xsl:element>
	        </xsl:if>
	        <!--<xsl:choose>
	            <xsl:when test="Symbolic"></xsl:when>
	            <xsl:otherwise>
	                <xsl:element name="Symbolic" namespace="http://www.genicam.org/GenApi/Version_1_0">
	                    <xsl:value-of select="$EntryName"/>
	                </xsl:element>
	            </xsl:otherwise>
	        </xsl:choose>-->
	    </xsl:copy>
	</xsl:template>
	<!--Create a reference to EnumEntry element -->
	<xsl:template match="gen:EnumEntry" mode="GenerateReference">
		<xsl:param name="EntryName"/>
		<xsl:param name="EnumerationName"/>
		<xsl:element name="pEnumEntry" namespace="http://www.genicam.org/GenApi/Version_1_0">EnumEntry_<xsl:value-of select="$EnumerationName"/>_<xsl:value-of select="$EntryName"/>
		</xsl:element>
	</xsl:template>
	<!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->
	<!--Extract the ConfRom Key entries.-->
	<!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->
	<xsl:template match="gen:ConfRom">
		<!-- create int key and text desc elements-->
		<xsl:apply-templates select="gen:IntKey|gen:TextDesc" mode="GenerateElement"/>
		<!-- shallow copy of ConfRom -->
		<xsl:copy>
			<xsl:apply-templates select="@*|gen:Language|gen:Unit|gen:Address|gen:Length|gen:pPort|gen:IntSwissKnife|gen:pAddress"/>
		</xsl:copy>
	</xsl:template>
	<!--Copy an Integer Key or a Textual Descriptor-->
	<xsl:template match="gen:IntKey|gen:TextDesc" mode="GenerateElement">
		<xsl:copy>
			<xsl:attribute name="Name"><xsl:value-of select="@Name"/></xsl:attribute>
			<xsl:attribute name="NameSpace"><xsl:value-of select="@NameSpace"/></xsl:attribute>
			<xsl:element name="p1212Parser" namespace="http://www.genicam.org/GenApi/Version_1_0">
				<xsl:value-of select="../@Name"/>
			</xsl:element>
			<xsl:element name="Key" namespace="http://www.genicam.org/GenApi/Version_1_0">
				<xsl:value-of select="."/>
			</xsl:element>
		</xsl:copy>
	</xsl:template>
	<!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->
	<!--Extract the StructReg entries.-->
	<!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->
	<!-- Walk the StructEntry elements inside the StructReg elements -->
	<xsl:template match="gen:StructReg">
		<xsl:for-each select="node()">
			<xsl:if test="name()='StructEntry'">
				<xsl:call-template name="StructReg_GenerateMaskedIntElement">
					<xsl:with-param name="NodeName" select="@Name"/>
					<xsl:with-param name="SubNodes" select="node()"/>
				</xsl:call-template>
			</xsl:if>
		</xsl:for-each>
	</xsl:template>
	<!--Create a MaskedInt element -->
	<xsl:template name="StructReg_GenerateMaskedIntElement">
		<xsl:param name="NodeName"/>
		<xsl:param name="SubNodes"/>
		<xsl:element name="MaskedIntReg" namespace="http://www.genicam.org/GenApi/Version_1_0">
			<!--Copy all attributes name and namespace-->
			<xsl:copy-of select="@*"/>
			<!-- Copy all elements which are not StructEntry elements -->
			<xsl:for-each select="../*[name()!='StructEntry'] ">
				<xsl:variable name="CurrentNodeName" select="name()"/>
				<xsl:if test="0=count($SubNodes[name()=$CurrentNodeName])">
					<xsl:copy>
						<xsl:apply-templates select="node()"/>
					</xsl:copy>
				</xsl:if>
			</xsl:for-each>
			<!-- Copy the sub-elements from the StructEntry elements -->
			<xsl:for-each select="$SubNodes">
				<xsl:choose>
					<!-- Make an LSB/MSB pair from a Bit element -->
					<xsl:when test="name()='Bit'">
						<xsl:element name="LSB" namespace="http://www.genicam.org/GenApi/Version_1_0">
							<xsl:value-of select="text()"/>
						</xsl:element>
						<xsl:element name="MSB" namespace="http://www.genicam.org/GenApi/Version_1_0">
							<xsl:value-of select="text()"/>
						</xsl:element>
					</xsl:when>
					<!-- Copy all other sub-elements -->
					<xsl:otherwise>
						<xsl:copy>
							<xsl:apply-templates select="node()"/>
						</xsl:copy>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:for-each>
		</xsl:element>
	</xsl:template>
	<!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->
	<!--Copy any other attribute or element which are not matched by the above rules.-->
	<!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->
	<xsl:template match="@*|node()">
		<xsl:copy>
			<xsl:apply-templates select="@*|node()"/>
		</xsl:copy>
	</xsl:template>
</xsl:stylesheet>
