<?xml version="1.0"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">

<xsl:template match="/">
<html>
<body bgcolor="#ffdead">
        <xsl:apply-templates select="//net"/>
</body>
</html>
</xsl:template>

<xsl:template match="net">
        <h1>Net #<xsl:value-of select="@id"/></h1><br/>
        <xsl:apply-templates select="name"/>
        <xsl:apply-templates select="description"/>
	[<a href="net.php?action=edit&id=<xsl:value-of select="@id"/>">edit</a>]
<!--
        <center><table width="50%" bgcolor="#FFFFFF" cellpadding="7">
	<tr><td>Footrope</td><td>Headrope</td></tr>
        <xsl:apply-templates select="specs"/>
        </table></center>
-->
</xsl:template>

<xsl:template match="name">
        <i>Name: <xsl:value-of select="."/></i>
</xsl:template>

<xsl:template match="description">
        <i>Description: <xsl:value-of select="."/></i>
</xsl:template>

<xsl:template match="specs">
                <tr>
                        <td>
        <xsl:apply-templates select="./footrope_length"/>
                        </td>
                        <td>
        <xsl:apply-templates select="./headrope_length"/>
                        </td>
                </tr>
</xsl:template>

<xsl:template match="footrope_length">
        <xsl:value-of select="."/>
</xsl:template>

<xsl:template match="headrope_length">
        <xsl:value-of select="."/>
</xsl:template>
</xsl:stylesheet>

